#include <EEPROM.h>//импорт либ

#define magicNumberAddr 0//аддрес магического числа

struct axis{//структуры
  byte pin;
  char nameA[10];
  };

struct button{
  byte pin;
  char nameB[10];
  };

void setup() {
  for(int i=0;i<EEPROM.length();i++){//очищаем епром
    EEPROM.write(i,0);
    }
    int mn = 387;//магическое число
    byte structSize = 11;//размер структуры
    EEPROM.put(magicNumberAddr, mn);//приправим магией
    int addr = magicNumberAddr + sizeof(int);//сдвигаем аддрес
    char joyName[10] = "myJoy";//создаём имя
    EEPROM.put(addr, joyName);//добавляем в епром
    addr += 10;//сдвигаем аддрес
    byte axisSize = 2;
    byte buttonSize = 2;
    EEPROM.write(addr,axisSize);//записываем кол-во осей и кнопок со сдвигами аддресов
    addr+=1;
    EEPROM.write(addr, buttonSize);
    addr+=1;
    axis x = {0,'x'};//создаём оси и кнопки пин(если аналоговый,то только номер A0 - 0, название)
    axis y = {1, 'y'};
    button lt = {2, "lt"};
    button rt = {4, "rt"};
    EEPROM.put(addr,x);//добавляем в епром и сдвигаем аддрес
    addr+=structSize;
    EEPROM.put(addr,y);
    addr+=structSize;
    EEPROM.put(addr,lt);
    addr+=structSize;
    EEPROM.put(addr,rt);
    addr+=structSize;
    pinMode(13, OUTPUT);//информируем о завершение коротким блинком
    digitalWrite(13, 1);
    delay(2000);
    digitalWrite(13,0);
}

void loop() {

}
