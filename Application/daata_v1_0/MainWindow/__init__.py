from PyQt5 import uic
from PyQt5.QtWidgets import QMainWindow, QWidget, QAction, QTabWidget, QMessageBox, QInputDialog, QLineEdit
from PyQt5.QtCore import QSettings

import pyqtgraph as pg
import numpy as np
from functools import partial
import json
import threading

from Widgets.Homepage import widget_Homepage
from Widgets.Data_Collection import Layout_Data_Collection
from Widgets.Layout_Test import widget_Test

import DataAcquisition


Ui_MainWindow, _ = uic.loadUiType(r'MainWindow\MainWindow.ui')  # loads the .ui file from QT Desginer

is_data_collecting = threading.Event()  # Creates an event to know if the data collection has started
data_collection_thread = threading.Thread(target=DataAcquisition.collect_data)  # Creates thread for collecting data


class MainWindow(QMainWindow, Ui_MainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()
        self.setupUi(self)
        self.dict_sensors = {}  # instantiates sensor dictionary

        data_collection_thread.start()

        ## Creates tab widget for apps
        self.tabWidget = QTabWidget(self.centralwidget)
        self.tabWidget.setTabsClosable(True)
        self.tabWidget.setMovable(True)
        self.tabWidget.setObjectName("tabWidget")
        self.centralLayout.addWidget(self.tabWidget)

        self.importWidgets()
        self.populate_menuWidget()


        self.connectSignalsSlots()
        self.settings = QSettings('DAATA', 'MainWindow')
        try:
            self.resize(self.settings.value('window size'))
            self.move(self.settings.value('window position'))
        except:
            pass

    def importWidgets(self):

        self.dict_sensors = {
            'Homepage': {
                'Create Widget': widget_Homepage.widget_Homepage
            },

            'Data Collection': {
                'Create Widget': Layout_Data_Collection
            },

            'Layout Test': {
                'Create Widget': widget_Test.Layout_Test
            }
        }

        for key in self.dict_sensors.keys():
            self.dict_sensors[key]['Widget'] = self.dict_sensors[key]['Create Widget']

        # self.dict_sensors['Data Collection']['Widget'].show()
        # self.setCentralWidget(self.dict_sensors['Data Collection']['Widget'])

    def populate_menuWidget(self):

        for key in self.dict_sensors.keys():
            self.dict_sensors[key]['Menu Action'] = QAction(self)
            self.dict_sensors[key]['Menu Action'].setCheckable(False)
            self.dict_sensors[key]['Menu Action'].setToolTip('Open a new tab for ' + key)
            self.dict_sensors[key]['Menu Action'].setText(key)
            self.menuWidget.addAction(self.dict_sensors[key]['Menu Action'])

    def createTab(self, key):
        self.tabWidget.addTab(self.dict_sensors[key]['Create Widget'](), key)

    def closeTab(self, index):
        ans =  QMessageBox.question(self, "Warning", "Do you want to close this tab? Any unsaved progress will be lost", QMessageBox.Close | QMessageBox.Cancel)

        if ans == QMessageBox.Close:
            widget = self.tabWidget.widget(index)
            if widget is not None:
                widget.close()
            self.tabWidget.removeTab(index)

    def closeEvent(self, event):
        self.settings.setValue('window size', self.size())
        self.settings.setValue('window position', self.pos())

    def renameObject(self, index):

        text, okPressed = QInputDialog.getText(self, "Get text", "Your name:", QLineEdit.Normal, "")
        if okPressed and text != '':
            self.tabWidget.setTabText(index, text)

    def openCloseConfirmation(self):
        buttonReply = QMessageBox.question(self, 'PyQt5 message', "Do you like PyQt5?",
                                           QMessageBox.Yes | QMessageBox.No, QMessageBox.No)
        if buttonReply == QMessageBox.Yes:
            print('Yes clicked.')
        else:
            print('No clicked.')

    def connectSignalsSlots(self):
        for key in self.dict_sensors.keys():
            self.dict_sensors[key]['Menu Action'].triggered.connect(partial(self.createTab, key))
        self.tabWidget.tabCloseRequested.connect(self.closeTab)
        self.tabWidget.tabBarDoubleClicked.connect(self.renameObject)

