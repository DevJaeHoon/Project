# -*- coding: utf-8 -*-

################################################################################
## Form generated from reading UI file 'mainUI.ui'
##
## Created by: Qt User Interface Compiler version 6.3.0
##
## WARNING! All changes made in this file will be lost when recompiling UI file!
################################################################################

from PySide6.QtCore import (QCoreApplication, QDate, QDateTime, QLocale,
    QMetaObject, QObject, QPoint, QRect,
    QSize, QTime, QUrl, Qt)
from PySide6.QtGui import (QBrush, QColor, QConicalGradient, QCursor,
    QFont, QFontDatabase, QGradient, QIcon,
    QImage, QKeySequence, QLinearGradient, QPainter,
    QPalette, QPixmap, QRadialGradient, QTransform)
from PySide6.QtWidgets import (QApplication, QLabel, QLineEdit, QMainWindow,
    QMenuBar, QPlainTextEdit, QPushButton, QSizePolicy,
    QStatusBar, QWidget)

class Ui_MainWindow(object):
    def setupUi(self, MainWindow):
        if not MainWindow.objectName():
            MainWindow.setObjectName(u"MainWindow")
        MainWindow.resize(762, 514)
        self.centralwidget = QWidget(MainWindow)
        self.centralwidget.setObjectName(u"centralwidget")
        self.logText = QPlainTextEdit(self.centralwidget)
        self.logText.setObjectName(u"logText")
        self.logText.setGeometry(QRect(10, 30, 301, 201))
        font = QFont()
        font.setFamilies([u"\ub9d1\uc740 \uace0\ub515"])
        self.logText.setFont(font)
        self.startButton = QPushButton(self.centralwidget)
        self.startButton.setObjectName(u"startButton")
        self.startButton.setGeometry(QRect(330, 250, 101, 51))
        self.leftButton = QPushButton(self.centralwidget)
        self.leftButton.setObjectName(u"leftButton")
        self.leftButton.setGeometry(QRect(20, 310, 81, 51))
        self.goButton = QPushButton(self.centralwidget)
        self.goButton.setObjectName(u"goButton")
        self.goButton.setGeometry(QRect(120, 240, 81, 51))
        self.rightButton = QPushButton(self.centralwidget)
        self.rightButton.setObjectName(u"rightButton")
        self.rightButton.setGeometry(QRect(220, 310, 81, 51))
        self.backButton = QPushButton(self.centralwidget)
        self.backButton.setObjectName(u"backButton")
        self.backButton.setGeometry(QRect(120, 380, 81, 51))
        self.midButton = QPushButton(self.centralwidget)
        self.midButton.setObjectName(u"midButton")
        self.midButton.setGeometry(QRect(120, 310, 81, 51))
        self.stopButton = QPushButton(self.centralwidget)
        self.stopButton.setObjectName(u"stopButton")
        self.stopButton.setGeometry(QRect(330, 310, 101, 51))
        self.label = QLabel(self.centralwidget)
        self.label.setObjectName(u"label")
        self.label.setGeometry(QRect(10, 10, 91, 16))
        self.sensingText = QPlainTextEdit(self.centralwidget)
        self.sensingText.setObjectName(u"sensingText")
        self.sensingText.setGeometry(QRect(320, 30, 431, 201))
        self.sensingText.setFont(font)
        self.label_2 = QLabel(self.centralwidget)
        self.label_2.setObjectName(u"label_2")
        self.label_2.setGeometry(QRect(320, 10, 91, 16))
        self.smileButton = QPushButton(self.centralwidget)
        self.smileButton.setObjectName(u"smileButton")
        self.smileButton.setGeometry(QRect(480, 250, 111, 51))
        self.angryButton = QPushButton(self.centralwidget)
        self.angryButton.setObjectName(u"angryButton")
        self.angryButton.setGeometry(QRect(620, 250, 111, 51))
        self.sadButton = QPushButton(self.centralwidget)
        self.sadButton.setObjectName(u"sadButton")
        self.sadButton.setGeometry(QRect(480, 310, 111, 51))
        self.surpriseButton = QPushButton(self.centralwidget)
        self.surpriseButton.setObjectName(u"surpriseButton")
        self.surpriseButton.setGeometry(QRect(620, 310, 111, 51))
        self.sendButton = QPushButton(self.centralwidget)
        self.sendButton.setObjectName(u"sendButton")
        self.sendButton.setGeometry(QRect(630, 370, 101, 51))
        self.textBox = QLineEdit(self.centralwidget)
        self.textBox.setObjectName(u"textBox")
        self.textBox.setGeometry(QRect(290, 370, 311, 31))
        self.manButton = QPushButton(self.centralwidget)
        self.manButton.setObjectName(u"manButton")
        self.manButton.setGeometry(QRect(330, 410, 101, 51))
        self.autoButton = QPushButton(self.centralwidget)
        self.autoButton.setObjectName(u"autoButton")
        self.autoButton.setGeometry(QRect(470, 410, 101, 51))
        MainWindow.setCentralWidget(self.centralwidget)
        self.menubar = QMenuBar(MainWindow)
        self.menubar.setObjectName(u"menubar")
        self.menubar.setGeometry(QRect(0, 0, 762, 22))
        MainWindow.setMenuBar(self.menubar)
        self.statusbar = QStatusBar(MainWindow)
        self.statusbar.setObjectName(u"statusbar")
        MainWindow.setStatusBar(self.statusbar)

        self.retranslateUi(MainWindow)
        self.startButton.clicked.connect(MainWindow.start)
        self.stopButton.clicked.connect(MainWindow.stop)
        self.rightButton.clicked.connect(MainWindow.right)
        self.backButton.clicked.connect(MainWindow.back)
        self.leftButton.clicked.connect(MainWindow.left)
        self.midButton.clicked.connect(MainWindow.mid)
        self.goButton.clicked.connect(MainWindow.go)
        self.smileButton.clicked.connect(MainWindow.smile)
        self.angryButton.clicked.connect(MainWindow.angry)
        self.sadButton.clicked.connect(MainWindow.sad)
        self.surpriseButton.clicked.connect(MainWindow.surprise)
        self.sendButton.clicked.connect(MainWindow.send)
        self.manButton.clicked.connect(MainWindow.man)
        self.autoButton.clicked.connect(MainWindow.auto)

        QMetaObject.connectSlotsByName(MainWindow)
    # setupUi

    def retranslateUi(self, MainWindow):
        MainWindow.setWindowTitle(QCoreApplication.translate("MainWindow", u"MainWindow", None))
        self.startButton.setText(QCoreApplication.translate("MainWindow", u" START", None))
        self.leftButton.setText(QCoreApplication.translate("MainWindow", u"Left", None))
        self.goButton.setText(QCoreApplication.translate("MainWindow", u"Go", None))
        self.rightButton.setText(QCoreApplication.translate("MainWindow", u"Right", None))
        self.backButton.setText(QCoreApplication.translate("MainWindow", u"Back", None))
        self.midButton.setText(QCoreApplication.translate("MainWindow", u"Mid", None))
        self.stopButton.setText(QCoreApplication.translate("MainWindow", u"STOP", None))
        self.label.setText(QCoreApplication.translate("MainWindow", u"command Table", None))
        self.label_2.setText(QCoreApplication.translate("MainWindow", u"sensing Table", None))
        self.smileButton.setText(QCoreApplication.translate("MainWindow", u"SMILE", None))
        self.angryButton.setText(QCoreApplication.translate("MainWindow", u"ANGRY", None))
        self.sadButton.setText(QCoreApplication.translate("MainWindow", u"SAD", None))
        self.surpriseButton.setText(QCoreApplication.translate("MainWindow", u"SURPRISE", None))
        self.sendButton.setText(QCoreApplication.translate("MainWindow", u"SEND", None))
        self.textBox.setText("")
        self.manButton.setText(QCoreApplication.translate("MainWindow", u"MANUAL", None))
        self.autoButton.setText(QCoreApplication.translate("MainWindow", u"AUTO", None))
    # retranslateUi

