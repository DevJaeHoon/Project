from PySide6.QtWidgets import *
from PySide6.QtCore import *
from mainUI import Ui_MainWindow
import mysql.connector

class MyApp(QMainWindow, Ui_MainWindow):
    def __init__(self):
        super().__init__()
        self.ui = Ui_MainWindow()
        self.ui.setupUi(self)
        self.init()

    def init(self):
        # 생성한 IP, 계정명, PW, 스키마명 입력
        self.db = mysql.connector.connect(host='', user='', password='', database='')
        self.cur = self.db.cursor()

        #timer setting
        self.timer = QTimer()
        self.timer.setInterval(100) #500ms
        self.timer.timeout.connect(self.pollingQuery)
        self.timer2 = QTimer()
        self.timer2.setInterval(100)  # 500ms
        self.timer2.timeout.connect(self.errorMessage)
        self.flag1 = 1
        self.flag2 = 1
        self.flag3 = 1

    def start(self):
        self.timer.start()
        #self.timer2.start()

    def errorMessage(self):
        self.cur.execute("select * from sensing order by time desc limit 1")
        for (id, time, num1, num2, num3, meta_string, is_finish) in self.cur:
            str = "%d | %s | %6s | %6s | %6s | %10s | %4d" % (
            id, time.strftime("%Y%m%d %H:%M:%S"), num1, num2, num3, meta_string, is_finish)
        if float(str[29:33]) > 0.32:
            if self.flag1 == 1:
                self.flag1 = 0
                msg = QMessageBox()
                msg.setWindowTitle("32도 이상")
                msg.setIcon(QMessageBox.Warning)
                msg.setText("에어컨을 가동하겠습니다!")
                msg.exec()
        if float(str[29:33]) <= 0.31: self.flag1 = 1
        if float(str[38:42]) < 0.4:
            if self.flag2 == 1:
                self.flag2 = 0
                msg = QMessageBox()
                msg.setWindowTitle("습도 낮음")
                msg.setIcon(QMessageBox.Warning)
                msg.setText("가습기를 가동하겠습니다!")
                msg.exec()
        if float(str[38:42]) >= 0.4: self.flag2 = 1
        if float(str[46:51]) <= 0.2:
            if self.flag3 == 1:
                self.flag3 = 0
                msg = QMessageBox()
                msg.setWindowTitle("장애물 발견")
                msg.setIcon(QMessageBox.Warning)
                msg.setText("긴급 제동하겠습니다!")
                msg.exec()
        if float(str[46:51]) >0.2: self.flag3 = 1

    def pollingQuery(self):
        self.cur.execute("select * from command order by time desc limit 5")
        self.ui.logText.clear()
        for (id, time, cmd_string, arg_string, is_finish) in self.cur:
            str = "%d | %s | %6s | %6s | %4d" % (id, time.strftime("%Y%m%d %H:%M:%S"), cmd_string, arg_string, is_finish)
            self.ui.logText.appendPlainText(str)

        self.cur.execute("select * from sensing order by time desc limit 5")
        self.ui.sensingText.clear()
        for (id, time, num1, num2, num3, meta_string, is_finish) in self.cur:
            str = "%d | %s | %6s | %6s | %6s | %10s | %4d" % (
            id, time.strftime("%Y%m%d %H:%M:%S"), num1, num2, num3, meta_string, is_finish)
            self.ui.sensingText.appendPlainText(str)
        self.db.commit()

    def closeEvent(self, event):
        self.cur.close()
        self.db.close()

    def insertCommand(self, cmd_string, arg_string):
        time = QDateTime().currentDateTime().toPython()
        is_finish = 0

        query = "insert into command(time, cmd_string, arg_string, is_finish) values (%s, %s, %s, %s)"
        value = (time, cmd_string, arg_string, is_finish)

        self.cur.execute(query, value)
        self.db.commit()

    def go(self):
        self.insertCommand("go", "5")


    def stop(self):
        self.insertCommand("stop", "0")


    def back(self):
        self.insertCommand("back", "0")


    def left(self):
        self.insertCommand("left", "0")


    def mid(self):
        self.insertCommand("mid", "0")


    def right(self):
        self.insertCommand("right", "0")


    def smile(self):
        self.insertCommand("smile", "0")


    def angry(self):
        self.insertCommand("angry", "0")


    def sad(self):
        self.insertCommand("sad", "0")


    def surprise(self):
        self.insertCommand("surprise", "0")


    def man(self):
        self.insertCommand("man", "0")


    def auto(self):
        self.insertCommand("auto", "0")


    def send(self):
        str = self.ui.textBox.text()
        if 0 < len(str) <= 10 : self.insertCommand("send", str)
        if len(str) == 0 :
            msg = QMessageBox()
            msg.setWindowTitle("에러 발생")
            msg.setIcon(QMessageBox.Warning)
            msg.setText("텍스트를 입력하십시오!")
            msg.exec()
        if len(str) > 10 :
            msg = QMessageBox()
            msg.setWindowTitle("에러 발생")
            msg.setIcon(QMessageBox.Warning)
            msg.setText("너무 내용이 많습니다.(글자수 10자 제한)")
            msg.exec()
        str = ''
        self.ui.textBox.clear()


app = QApplication()
win = MyApp()
win.show()
app.exec()