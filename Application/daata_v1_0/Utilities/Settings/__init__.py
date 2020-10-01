# https://stackoverflow.com/questions/39023584/implementing-a-preferences-dialog-window-in-pyqt
from PyQt5.QtCore import QSettings
from PyQt5 import QtWidgets, uic
import os


settings = QSettings('DAATA', 'MainWindow')

def settings_load(self):
    try:
        self.resize(settings.value('Window size'))
        self.move(settings.value('Window position'))
    except:
        pass

def settings_save(self):
    settings.setValue('Window size', self.size())
    settings.setValue('Window position', self.pos())
    # settings.setValue('')

def settings_debug(self):
    for key in settings.allKeys():
        print(key + ":\t" + str(settings.value(key)))


def settings_restoreDefaults(self):
    settings.clear()


uiFile, _ = uic.loadUiType(os.path.join(os.path.dirname(__file__), 'settings.ui'))  # loads the .ui file from QT Desginer
class SettingsDialog(QtWidgets.QDialog, uiFile):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        

        returnValue = self.exec()