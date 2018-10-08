import AvrPyJoy#подключаем модуль
import time

J = AvrPyJoy.Joystic()#создаём объект Joystic
J.connect(0,timeout=10)#подключаем (req: path;manually: baudrate=9600, timeout=5)
J.start()#запускаем поток
J.info()#выводим информацию
def helloWorld():
    print('helloWorld')

J.connectButton("lt", helloWorld)#заводим функцию на нажатие кнопки

try:
    time.sleep(2)
    while True:
        lt = J.getButton("lt")#считывание кнопки lt
        rt = J.getButton("rt")#считывание кнопки rt
        x = J.getAxis("x")#считывание оси x
        y = J.getAxis("y")#считывание оси y
        print("X: %d Y: %d Lt: %d Rt: %d" %(x, y, lt, rt))
        time.sleep(0.5)
except KeyboardInterrupt:
    print("Ctrl + C pressed")
    J.stop()



