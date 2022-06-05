from Raspi_MotorHAT import Raspi_MotorHAT, Raspi_DCMotor
from Raspi_PWM_Servo_Driver import PWM
import mysql.connector
from threading import Timer, Lock
from time import sleep
import signal
import sys
import os
picdir = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'pic')
libdir = os.path.join(os.path.dirname(os.path.realpath(__file__)), 'lib')
if os.path.exists(libdir):
    sys.path.append(libdir)
import logging
import traceback
from waveshare_OLED import OLED_0in96
from PIL import Image,ImageDraw,ImageFont
logging.basicConfig(level=logging.DEBUG)
import Adafruit_DHT
from gpiozero import DistanceSensor
from gpiozero import TonalBuzzer
from gpiozero.tones import Tone
from time import sleep
import datetime

def closeDB(signal, frame):
    print("BYE")
    mh.getMotor(2).run(Raspi_MotorHAT.RELEASE)
    cur.close()
    db.close()
    timer.cancel()
    timer2.cancel()
    OLED_0in96.config.module_exit()
    sys.exit(0)

def polling():
    global cur, db, ready

    lock.acquire()
    # 0.1초마다 DB로 부터 새로운 명령어 받을 준비하기
    cur.execute("select * from command order by time desc limit 1")
    for (id, time, cmd_string, arg_string, is_finish) in cur:
        if is_finish == 1 : break
        ready = (cmd_string, arg_string)
        cur.execute("update command set is_finish=1 where is_finish=0")

    db.commit()
    lock.release()

    global timer
    timer = Timer(0.1, polling)
    timer.start()

def sensing():
    global cur, db, sense, num3

    # 시간, 온도, 습도, 장애물과 거리를 0 ~ 1 사이 값으로 DB에 전송한다.
    humidity, temperature = Adafruit_DHT.read_retry(sense, pin)


    time = datetime.datetime.now()
    num1 = round(temperature / 100, 2)
    num2 = round(humidity / 100, 2)


    meta_string = '0|0|0'
    is_finish = 0

    msg = "Temp : " + str(num1) + "  Humid : " + str(num2) + "  Dist : " + str(num3)
    print(msg)
    query = "insert into sensing(time, num1, num2, num3, meta_string, is_finish) values (%s, %s, %s, %s, %s, %s)"
    value = (time, num1, num2, num3, meta_string, is_finish)

    lock.acquire()
    cur.execute(query, value)
    db.commit()
    lock.release()

    global timer2
    timer2 = Timer(1, sensing)
    timer2.start()

def display(msg):
    # OLED 출력 설정
    disp.clear()
    image1 = Image.new('1', (disp.width, disp.height), 255)
    draw = ImageDraw.Draw(image1)
    font1 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 18)
    font2 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 24)

    # 텍스트 출력
    draw.text((30,20), msg , font=font2, fill=0)
    region = image1.transpose(Image.ROTATE_180)
    disp.ShowImage(disp.getbuffer(region))


def go():
    display("전진")
    myMotor.setSpeed(200)
    myMotor.run(Raspi_MotorHAT.FORWARD)


def back():
    display("후진")
    myMotor.setSpeed(200)
    myMotor.run(Raspi_MotorHAT.BACKWARD)


def stop():
    display("정지")
    myMotor.setSpeed(200)
    myMotor.run(Raspi_MotorHAT.RELEASE)


def left():
    display("좌회전")
    pwm.setPWM(0, 0, 340)


def mid():
    display("중앙")
    pwm.setPWM(0, 0, 420)


def right():
    display("우회전")
    pwm.setPWM(0, 0, 480)

def buz1():
    # buzzer 출력
    global buzzer1
    lst = [242.32, 352.34, 810.2]
    for i in range(3):
        buzzer1.play(lst[i])
        sleep(0.2)
    buzzer1.stop()

def smile():
    disp.clear()
    Himage2 = Image.new('1', (disp.width, disp.height), "WHITE")
    bmp = Image.open('smile.bmp')
    Himage2.paste(bmp, (0, 0))
    Himage2 = Himage2.rotate(180)
    disp.ShowImage(disp.getbuffer(Himage2))

def angry():
    disp.clear()
    Himage2 = Image.new('1', (disp.width, disp.height), "WHITE")
    bmp = Image.open('angry.bmp')
    Himage2.paste(bmp, (0, 0))
    Himage2 = Himage2.rotate(180)
    disp.ShowImage(disp.getbuffer(Himage2))

def sad():
    disp.clear()
    Himage2 = Image.new('1', (disp.width, disp.height), "WHITE")
    bmp = Image.open('sad.bmp')
    Himage2.paste(bmp, (0, 0))
    Himage2 = Himage2.rotate(180)
    disp.ShowImage(disp.getbuffer(Himage2))

def surprise():
    disp.clear()
    Himage2 = Image.new('1', (disp.width, disp.height), "WHITE")
    bmp = Image.open('shocked.bmp')
    Himage2.paste(bmp, (0, 0))
    Himage2 = Himage2.rotate(180)
    disp.ShowImage(disp.getbuffer(Himage2))

def send(msg):
    # UI에서 보내온 10자 이하 메세지 출력
    disp.clear()
    image1 = Image.new('1', (disp.width, disp.height), 255)
    draw = ImageDraw.Draw(image1)
    font2 = ImageFont.truetype(os.path.join(picdir, 'Font.ttc'), 24)

    if len(msg) <= 5 : draw.text((5, 20), msg, font=font2, fill=0)
    else :
        str1 = msg[0:5]
        str2 = msg[5:]
        draw.text((5, 5), str1, font=font2, fill=0)
        draw.text((5, 35), str2, font=font2, fill=0)
    region = image1.transpose(Image.ROTATE_180)
    disp.ShowImage(disp.getbuffer(region))

# 생성한 IP, 계정명, PW, 스키마명 입력
db = mysql.connector.connect(host='', user='', password='', database='',
                             auth_plugin='mysql_native_password')
cur = db.cursor()
ready = None
timer = None
num3 = None
distance = None

mh = Raspi_MotorHAT(addr=0x6f)
myMotor = mh.getMotor(2)
pwm = PWM(0x6F)
pwm.setPWMFreq(60)

disp = OLED_0in96.OLED_0in96()
disp.Init()
disp.clear()

sensor = DistanceSensor(21,20)

buzzer1 = TonalBuzzer(14)

sense = Adafruit_DHT.DHT11
pin = 4
timer2 = None
lock = Lock()

flag = 1
flag2 = 0

signal.signal(signal.SIGINT, closeDB)
polling()
sensing()

# main thread
while True:
    distance = sensor.distance
    num3 = round(distance, 3)
    if num3 < 0.2 :
        stop()
        if flag : buz1()
        flag = 0
    else :
        flag = 1

    if ready == None : continue
    cmd, arg = ready
    ready = None

    if cmd == "man": flag2 = 1
    if cmd == "auto": flag2 = 0
    if flag2 == 1 :
        if cmd == "go" : go()
        if cmd == "back" : back()
        if cmd == "stop" : stop()
        if cmd == "left" : left()
        if cmd == "mid" : mid()
        if cmd == "right" : right()
    if cmd == "smile": smile()
    if cmd == "angry": angry()
    if cmd == "sad": sad()
    if cmd == "surprise": surprise()
    if cmd == "send": send(arg)