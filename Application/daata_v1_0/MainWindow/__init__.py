from PyQt5 import uic, QtWidgets, QtGui, QtCore


from functools import partial
import threading
import logging
import os

from Layouts import DAATALayout
from Layouts.Homepage import Homepage
from Layouts.DataCollection import DataCollection
from Layouts.Layout_Test import Widget_Test


from Utilities.CustomWidgets.Popups import popup_ParentChildrenTree
from MainWindow._tabHandler import closeTab
import DataAcquisition


is_data_collecting = threading.Event()  # Creates an event to know if the data collection has started
data_collection_thread = threading.Thread(target=DataAcquisition.collect_data)  # Creates thread for collecting data
logger = logging.getLogger("MainWindow")

Ui_MainWindow, _ = uic.loadUiType(r'MainWindow\MainWindow.ui')  # loads the .ui file from QT Desginer





class MainWindow(QtWidgets.QMainWindow, Ui_MainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()
        data_collection_thread.start()

        self.setupUi(self)
        self.dict_layouts = {}  # instantiates dictionary that holds objects for widgets
        self.import_Layouts()
        self.setWindowIcon(QtGui.QIcon(os.path.join(os.path.dirname(__file__), 'icon_application.svg')))

        self.resetTabs_tabWidget()
        self.populate_menu()



        self.create_homepage()
        #
        # self.settings_debug()
        #
        # self.settings_load()

        self.refreshFreq = 60
        self.updateLoops = 0
        self.timer = QtCore.QTimer()
        self.timer.timeout.connect(self.updateAll)
        self.timer.start(1000 / self.refreshFreq)
        self.timer.setInterval(1000 / self.refreshFreq)

        self.connectSignalsSlots()

    ## Imported methods
    from ._tabHandler import resetTabs_tabWidget, create_layoutTab, rename_tab, closeTab
    from ._menubarHandler import populate_menu
    from Utilities.Settings.SettingsDialog import SettingsDialog

    def updateAll(self):
        if self.updateLoops > self.refreshFreq:
            self.updateLoops = 0
        self.updateLoops = self.updateLoops + 1

        if self.tab_homepage.isVisible():
            if (self.updateLoops % (self.refreshFreq / self.homepage.updateFreq)) == 0:
                self.homepage.update()
                # logger.debug("updating " + self.homepage.objectName())

        if self.tab_layouts.isVisible():
            for tab in self.tabWidget.findChildren(DAATALayout):
                if tab.isVisible():
                    if (self.updateLoops % (self.refreshFreq/tab.updateFreq)) == 0:
                        tab.update()
                        # logger.debug("updating " + tab.objectName())


    def import_Layouts(self):
        self.dict_layouts = {
            'Data Collection': {
                'create_layout': partial(DataCollection, data_collection_thread, is_data_collecting)
            },

            'Layout Test': {
                'create_layout': Widget_Test
            }
        }

    def create_homepage(self):
        self.homepage = Homepage()
        self.homepage.setObjectName("Homepage")
        self.gridLayout_tab_homepage.addWidget(self.homepage)

        # self.tabWidget.setCornerWidget(self.homepage.button, corner = Qt.Corner.TopRightCorner)



    def closeEvent(self, event):
        pass
        ##  Close Confirmation Window
        # close = QtWidgets.QMessageBox()
        # close.setText("Close DAATA?")
        # close.setStandardButtons(QtWidgets.QMessageBox.Yes | QtWidgets.QMessageBox.Cancel)
        # close = close.exec()
        # if close == QtWidgets.QMessageBox.Yes:
        #     event.accept()
        # else:
        #     event.ignore()





    def connectSignalsSlots(self):
        for key in self.dict_layouts.keys():
            self.dict_layouts[key]['menu_action'].triggered.connect(partial(self.create_layoutTab, key))
        self.tabWidget.tabBarDoubleClicked.connect(self.rename_tab)
        self.tabWidget.tabCloseRequested.connect(partial(self.closeTab, self))
        self.action_parentChildrenTree.triggered.connect(partial(popup_ParentChildrenTree, self))
        self.action_Preferences.triggered.connect(self.SettingsDialog)
