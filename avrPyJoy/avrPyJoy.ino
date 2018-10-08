#include <EEPROM.h>//импорт либ

#define magicNumberAddr  0//аддрес магического числа
#define baudrate 9600//скорость работы
#define infoLed 13//пин светодиода состояния
#define maxPin 15//максимальное число GPIO

int magicNumber;//глобальные переменные
byte pinConf[maxPin + 2];//конфиг всех осей и пинов
bool connection = false;//состояние подключения
char nameConf[maxPin][10];
String Name;

struct axis{//структура оси
  byte pin;
  char nameA[10];
  };

struct button{//структура кнопки
  byte pin;
  char nameB[10];
  };

void setup()
{
  pinMode(infoLed, OUTPUT);//светодиод на 13 пине(настраивается параметром infoLed)
  Serial.begin(baudrate);//открытиесериала
  while(!Serial){
    fastBlink();//ждём настройки сериала
    }
  slowBlink();  //одно длинное моргание - успешно
    
  EEPROM.get(magicNumberAddr, magicNumber);//вынимаем магическое число
  
  int addr = 0 + sizeof(int);//сдвиг аддреса
  char joyName[10];//локал. имя
  byte structSize = 11;//
  
  EEPROM.get(addr, joyName);//читаем имя
  Name = String(joyName);//записываем в глобал.
  addr += 10;//сдвиг аддреса
  pinConf[0] = EEPROM.read(addr);//читаем кол-во осей
  addr += 1;//сдвиг аддреса
  pinConf[1] = EEPROM.read(addr);//читаем кол-во кнопок
  addr += 1;//сдвиг аддреса
  for(int i = 0;i<pinConf[0];i++){//считывание осей
      axis readAxis ={};//пустой буффер
      EEPROM.get(addr, readAxis);//чтение в буфер
      addr += structSize;//сдвиг аддреса
      pinConf[2+i] = 14 + readAxis.pin;//запись пина в конфигурацию пинов
      pinMode(14+readAxis.pin, INPUT);//настройка пина
      for(int j=0; j < strlen(readAxis.nameA); j++){//запись имени в конфигурацию имён соответствует i+2 пину
        nameConf[i][j] = readAxis.nameA[j];
      }
    }

  for(int i = pinConf[0]; i < pinConf[1]+pinConf[0]; i++){//считывание кнопок
    button readButtons = {};//пустой буфер
    EEPROM.get(addr, readButtons);//чтение в буфер
    addr += structSize;//сдвигаддреса
    pinConf[2+i] = readButtons.pin;//запись пина в конфигурацию пинов
    pinMode(readButtons.pin, INPUT);//настройка пина
    for(int j=0; j < strlen(readButtons.nameB); j++){//запись имени в конфигурацию имён соответствует i+2 пину
      nameConf[i][j] = readButtons.nameB[j];
    }
  }
  
  slowBlink();//одно моргание - успешно
  
  waitForMN();//ожидаем квитирования от компа
}
 
void loop()//главный цикл
{
  if(!connection){//если нет соединения вызываем поиск соединения
    waitForMN();
    }
    if(Serial.available() > 0){
        if(Serial.read() == '9'){
            Serial.read();
            connection = false;
          }
      }

    sendData();//отправка данных
    delay(1);
      
}

void waitForMN() {//двустороннее квитирование
  String data="";//пустая строка для данных
  byte sizeofbuf=0;//кол-во полученных байтов
  while (Serial.available() <= 0) {//пока в сериале ничего нет
    fastBlink();//быстро моргаем
  }
  while(!connection){//пока не установили соединение
    if(Serial.available()){//если есть не прочитанные байты в сериале
      sizeofbuf += 1;//увеличиваем кол-во прочитанных байтов
      char s = Serial.read();//считываем байт данных
      data = String(data + s);//добавляем к данным
      if(sizeofbuf==3){//получили 3 байта(длина MN)
        if(data.toInt()==magicNumber){//если совпадает
          Serial.println(magicNumber);//возвращаем MN
            connection = true;//соединение установлено
            //отправляем информацию обустройстве
            Serial.println(Name);//имя
            Serial.println(pinConf[0]);//кол-во осей
            Serial.println(pinConf[1]);//кол-во кнопок
            for(int i = 0; i<pinConf[0]+pinConf[1];i++){//отправляем
              Serial.println(nameConf[i]);
              }
            break;//выхоодим
          }else{
            Serial.read();//не совпало, обнуляем последний байт
            Serial.println(data);
            waitForMN();//запускаем рекурсию
            }
        }
    }
  }  
} 

byte getAxis(byte pin){
  int val = analogRead(pin);
  val = map(val, 0, 1023, 0, 255);
  return byte(val);
  }
  
byte getButtons(byte pin){
  byte val = digitalRead(pin);
  return val;
  }
  
void sendData(){
  String data = "";
  for(int i = 0; i<pinConf[0];i++){
      byte axisData = getAxis(pinConf[i+2]);
      char axisName[10];
      for(int j = 0; j < 10; j++){
          axisName[j] = nameConf[i][j];
        }
      data += String(axisName);
      data += String(axisData);
    }
        

    for(int i = pinConf[0]; i<pinConf[1]+pinConf[0];i++){
      byte buttonData = getButtons(pinConf[i+2]);
      char buttonName[10];
      for(int j = 0; j < 10; j++){
          buttonName[j] = nameConf[i][j];
        }
      data += String(buttonName);
      data += String(buttonData);
    }
    Serial.println(data);

  }

void sendInfo(){//без данных просто отправка всех осей и кнопок и имени
  
  }

//информация о состоянии
void fastBlink(){//быстрое мигание
    digitalWrite(infoLed, 1);
    delay(250);
    digitalWrite(infoLed, 0);
    delay(250);
}
void slowBlink(){//медленноемигание
  digitalWrite(infoLed, 1);
    delay(1000);
    digitalWrite(infoLed, 0);
    delay(1000);
  }
void normalBlink(){//нормальное мигание
  digitalWrite(infoLed, 1);
    delay(500);
    digitalWrite(infoLed, 0);
    delay(500);
  }
