## This file handles all top-level tabs in DAATA

from PyQt5 import QtWidgets
from functools import partial


## Creates tab widget for apps
def resetTabs_tabWidget(self):
    for index in range(self.tabWidget.count()):
        self.tabWidget.removeTab(0)
    self.create_layoutTab('Data Collection')  # sets default tab that pops up in Layouts
    self.tabWidget_central.setCurrentIndex(
        self.tabWidget_central.indexOf(self.tab_homepage))  ## temporary measure to default to homepage on startup
    self.tabWidget.setStyleSheet("""

    """)

# def refreshLayouts(self):
#     for gridPlotLayout


tabInstances = 0    # a counter for the number of tabs created in current session
def create_layoutTab(self, key):
    # from Utilities.Settings import settings_load, settings_save
    global tabInstances


    tab = self.dict_layouts[key]['create_layout']()
    tab.setObjectName(key + " (instance " + str(tabInstances) + ")")           # set object names for each tab's widget (allows duplicate widgets to have a functional parent/child relationship)
    self.tabWidget.addTab(tab, key)
    self.tabWidget.setCurrentIndex(self.tabWidget.indexOf(tab))
    self.tabWidget_central.setCurrentIndex(self.tabWidget_central.indexOf(self.tab_layouts))

    if key == 'Data Collection':
        # tab.slot_graphDimension()
        tab.create_gridPlotLayout()
    tabInstances += 1


def rename_tab(self, index):
    text, okPressed = QtWidgets.QInputDialog.getText(self, "Rename Tab", "New Name:",
                                                     QtWidgets.QLineEdit.Normal, "")
    if okPressed and text != '':
        self.tabWidget.setTabText(index, text)

def closeTab(self, parent, index):
    ans = QtWidgets.QMessageBox.warning(parent, "Warning",
                                        "Do you want to close this tab? Any unsaved progress will be lost",
                                        QtWidgets.QMessageBox.Close | QtWidgets.QMessageBox.Cancel)
    # self.setWindowIcon(QtGui.QIcon(os.path.join(os.path.dirname(__file__), 'warningIcon.png')))
    if ans == QtWidgets.QMessageBox.Close:
        widget = parent.tabWidget.widget(index)
        if widget is not None:
            widget.deleteLater()
        parent.tabWidget.removeTab(index)

    widget.closeEvent()
