from PyQt5 import uic, QtWidgets, QtGui, QtCore


from functools import partial
import threading
import logging
import os

from Layouts import DAATALayout
from Layouts.Homepage import Homepage
from Layouts.DataCollection import DataCollection
from Layouts.Layout_Test import Widget_Test


from Utilities.Popups.popups import popup_ParentChildrenTree
from MainWindow._tabHandler import closeTab
import DataAcquisition


is_data_collecting = threading.Event()  # Creates an event to know if the data collection has started
data_collection_thread = threading.Thread(target=DataAcquisition.collect_data)  # Creates thread for collecting data
logger = logging.getLogger("MainWindow")


Ui_MainWindow, _ = uic.loadUiType(os.path.join(os.path.dirname(__file__), 'MainWindow.ui'))  # loads the .ui file from QT Desginer





class MainWindow(QtWidgets.QMainWindow, Ui_MainWindow):
    def __init__(self):
        super(MainWindow, self).__init__()
        data_collection_thread.start()

        self.setupUi(self)
        self.dict_layouts = {}  # instantiates dictionary that holds objects for widgets
        self.import_Layouts()

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

        self.set_appIcon()
        self.loadStylesheet()


    # set app icon on taskbar and titlebar
    def set_appIcon(self):
        import ctypes
        myappid = 'mycompany.myproduct.subproduct.version'  # arbitrary string
        ctypes.windll.shell32.SetCurrentProcessExplicitAppUserModelID(myappid)


        path = os.path.join(os.path.dirname(__file__), 'icon_GTORLogo.png')
        self.setWindowIcon(QtGui.QIcon(path))

        # app_icon = QtGui.QIcon()
        # app_icon.addFile('icon_GTORLogo/icon_GTORLogo16x16.png', QtCore.QSize(16, 16))
        # app_icon.addFile('icon_GTORLogo/icon_GTORLogo24x24.png', QtCore.QSize(24, 24))
        # app_icon.addFile('icon_GTORLogo/icon_GTORLogo32x32.png', QtCore.QSize(32, 32))
        # app_icon.addFile('icon_GTORLogo/icon_GTORLogo48x48.png', QtCore.QSize(48, 48))
        # app_icon.addFile('icon_GTORLogo/icon_GTORLogo256x256.png', QtCore.QSize(256, 256))
        # self.setWindowIcon(app_icon)

    def loadStylesheet(self):
        stylesheet = """        
        
        /*  General color scheme  */

        
        /*  MainWindow color scheme  */
        QMainWindow {{
            background: {bgColor};
            }}

        QMainWindow [objectName^="tab_layouts"] {{
            background: {bgColor};
            }}
        QMainWindow QTabWidget {{
            }}
        QStackedWidget {{
            background-color: {bgColor2};
        }}
        QMenuBar {{
            background: white;
            }}
        QMenuBar::item {{
            padding: 4px;
            background: transparent;
            border-right: 1px solid lightGray;

            }}
        QMenuBar::item:selected {{
            background: rgb(237,237,237,100);
            }}
        
        
        
        /*  DataCollection layout color scheme   */
        DataCollection QWidget {{
            background-color: {foreColor};
            }}
        DataCollection QPushButton {{
            background-color: white;
            }}
        DataCollection CustomPlotWidget {{
            border-radius: 7px;
            border: 1px solid gray;
            }}
        
        
        
        /*  Homepage layout color scheme */
        Homepage .QWidget {{
            background-color: {foreColor};
            }}
        Homepage .QFrame {{
            background-color: {foreColor};
            }}
            

            
        """



        stylesheet = stylesheet.format(
            testColor = "pink",
            bgColor = "#dbcc93",
            bgColor2 = "#white",
            foreColor = "#f4f4f4",
            # windowBorder = "#B3A369",
            windowBorder = "white",
            defaultText = "white"
            )
        self.setStyleSheet(stylesheet)

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
            },

            'Engine Dyno': {
                'create_layout': partial(DataCollection, data_collection_thread, is_data_collecting)
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

    ##################################
    #### Overridden event methods ####

    ## allow color scheme of class to be changed by CSS stylesheets
    def paintEvent(self, pe):
        opt = QtGui.QStyleOption()
        opt.initFrom(self)
        p = QtGui.QPainter(self)
        s = self.style()
        s.drawPrimitive(QtGui.QStyle.PE_Widget, opt, p, self)
