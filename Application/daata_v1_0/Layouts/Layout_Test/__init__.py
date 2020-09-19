from PyQt5 import QtWidgets, uic, QtCore, QtGui
import os



Ui_Widget_Test, _ = uic.loadUiType(os.path.join(os.path.dirname(__file__), 'widget_Test.ui'))  # loads the .ui file from QT Desginer

class Widget_Test(QtWidgets.QWidget, Ui_Widget_Test):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        self.hide()
