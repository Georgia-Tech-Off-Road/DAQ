from PyQt5 import QtWidgets, uic, QtCore, QtGui
import os
from Layouts import DAATALayout


uiFile, _ = uic.loadUiType(os.path.join(os.path.dirname(__file__), 'testLayout.ui'))  # loads the .ui file from QT Desginer

class Widget_Test(DAATALayout, uiFile):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        self.hide()

    def update(self):
        pass
