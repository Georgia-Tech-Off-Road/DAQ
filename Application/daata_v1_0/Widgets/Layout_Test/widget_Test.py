from PyQt5 import QtWidgets, uic, QtCore, QtGui
import os


Ui_Widget_Test, _ = uic.loadUiType(os.path.join(os.path.dirname(__file__), 'widget_Test.ui'))  # loads the .ui file from QT Desginer


class Layout_Test(QtWidgets.QWidget, Ui_Widget_Test):
    def __init__(self, parent):
        super().__init__(parent)
        self.setupUi(self)
