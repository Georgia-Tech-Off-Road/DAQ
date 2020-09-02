from PyQt5 import uic, QtWidgets
from PyQt5.QtCore import QSettings, QRect

import pyqtgraph as pg
import numpy as np
from functools import partial
import json
import threading

from Widgets.Homepage import Widget_Homepage
from Widgets.Data_Collection import Widget_DataCollection
from Widgets.Layout_Test import Widget_Test

from MainWindow.Popup_ParentChildrenTree import Popup_ParentChildrenTree

import DataAcquisition




Ui_MainWindow, _ = uic.loadUiType(r'MainWindow\MainWindow.ui')  # loads the .ui file from QT Desginer

is_data_collecting = threading.Event()  # Creates an event to know if the data collection has started
data_collection_thread = threading.Thread(target=DataAcquisition.collect_data)  # Creates thread for collecting data

tabInstances = 0    # a counter for the number of tabs created in current session

class MainWindow(QtWidgets.QMainWindow, Ui_MainWindow):


    def __init__(self):
        super(MainWindow, self).__init__()
        self.setupUi(self)
        data_collection_thread.start()
        self.dict_widgets = {}  # instantiates dictionary that holds objects for widgets
        self.create_TabWidget()


        self.import_Widgets()
        self.populate_menubar()


        self.connectSignalsSlots()

        self.settings = QSettings('DAATA', 'MainWindow')
        try:
            self.resize(self.settings.value('window size'))
            self.move(self.settings.value('window position'))
        except:
            pass

    def populate_menubar(self):
        ## Make an action to create a tab for each imported widget
        for key in self.dict_widgets.keys():
            self.dict_widgets[key]['Menu Action'] = QtWidgets.QAction(self)
            self.dict_widgets[key]['Menu Action'].setCheckable(False)
            self.dict_widgets[key]['Menu Action'].setToolTip('Open a new tab for ' + key)
            self.dict_widgets[key]['Menu Action'].setText(key)
            self.menuWidget.addAction(self.dict_widgets[key]['Menu Action'])

        ## Make an action in the File menu to display current parent and children tree
        self.action_parentChildren = QtWidgets.QAction(self)
        self.action_parentChildren.setToolTip('Display a tree of all parent objects and their respective children for the current UI layout')
        self.action_parentChildren.setText('Display parent/children tree')
        self.menuFile.addAction(self.action_parentChildren)


    def import_Widgets(self):
        self.dict_widgets = {
            'Homepage': {
                'Create Widget': Widget_Homepage
            },

            'Data Collection': {
                'Create Widget': partial(Widget_DataCollection, data_collection_thread, is_data_collecting)
                # 'Create Widget': Widget_DataCollection
            },

            'Layout Test': {
                'Create Widget': Widget_Test
            }
        }

        for key in self.dict_widgets.keys():
            self.dict_widgets[key]['Widget'] = self.dict_widgets[key]['Create Widget']


    ## Creates tab widget for apps
    def create_TabWidget(self):
        self.tabWidget = QtWidgets.QTabWidget(self.centralwidget)
        self.tabWidget.setTabsClosable(True)
        self.tabWidget.setMovable(True)
        self.tabWidget.setObjectName("tabWidget")
        self.centralLayout.addWidget(self.tabWidget)


    def create_Tab(self, key):
        global tabInstances
        widg = self.dict_widgets[key]['Create Widget']()
        widg.setObjectName(key + " (instance " + str(tabInstances) + ")")           # set object names for each tab's widget (allows duplicate widgets to have a functional parent/child relationship)
        self.tabWidget.addTab(widg, key)
        tabInstances += 1


    def close_Tab(self, index):
        ans = QtWidgets.QMessageBox.question(self, "Warning", "Do you want to close this tab? Any unsaved progress will be lost", QtWidgets.QMessageBox.Close | QtWidgets.QMessageBox.Cancel)

        if ans == QtWidgets.QMessageBox.Close:
            widget = self.tabWidget.widget(index)
            if widget is not None:
                widget.deleteLater()
            self.tabWidget.removeTab(index)

    def closeEvent(self, event):
        self.settings.setValue('window size', self.size())
        self.settings.setValue('window position', self.pos())

    def rename_object(self, index):
        text, okPressed = QtWidgets.QInputDialog.getText(self, "Rename Object", "New Name:", QtWidgets.QLineEdit.Normal, "")
        if okPressed and text != '':
            self.tabWidget.setTabText(index, text)


    def open_CloseConfirmation(self):
        buttonReply = QtWidgets.QMessageBox.question(self, 'PyQt5 message', "Do you like PyQt5?",
                                                     QtWidgets.QMessageBox.Yes | QtWidgets.QMessageBox.No, QtWidgets.QMessageBox.No)
        if buttonReply == QtWidgets.QMessageBox.Yes:
            print('Yes clicked.')
        else:
            print('No clicked.')

    def connectSignalsSlots(self):
        for key in self.dict_widgets.keys():
            self.dict_widgets[key]['Menu Action'].triggered.connect(partial(self.create_Tab, key))
        self.tabWidget.tabCloseRequested.connect(self.close_Tab)
        self.tabWidget.tabBarDoubleClicked.connect(self.rename_object)
        self.action_parentChildren.triggered.connect(partial(Popup_ParentChildrenTree, self))
        # self.action_parentChildren.triggered.connect(Popup_ParentChildrenTree(self))
