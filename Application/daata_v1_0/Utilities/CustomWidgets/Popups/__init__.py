from PyQt5 import QtWidgets, QtGui, uic
import os
from Utilities.findAncestryTree import findAncestryTree




## Creates a popup window to display all objects and their parent/child relationships
Ui_Dialog , _ = uic.loadUiType(os.path.join(os.path.dirname(__file__), 'dialog_info.ui'))  # loads the .ui file from QT Desginer
class popup_ParentChildrenTree(QtWidgets.QDialog, Ui_Dialog):
    def __init__(self, parent):
        super().__init__()
        self.setupUi(self)
        self.setWindowIcon(QtGui.QIcon(os.path.join(os.path.dirname(__file__), 'infoIcon.png')))
        self.setWindowTitle("Parent/Children Tree")
        self.resize(750,500)

        self.ancestor_count = 0
        tree = findAncestryTree(ancestor_count=self.ancestor_count, parent = parent, tree = parent.objectName())
        self.label.setText(tree)

        returnValue = self.exec()



class popup_closeTabConfirmation:
    def __init__(self, parent, index):
        ans = QtWidgets.QMessageBox.warning(parent, "Warning", "Do you want to close this tab? Any unsaved progress will be lost", QtWidgets.QMessageBox.Close | QtWidgets.QMessageBox.Cancel)
        # self.setWindowIcon(QtGui.QIcon(os.path.join(os.path.dirname(__file__), 'warningIcon.png')))
        if ans == QtWidgets.QMessageBox.Close:
            widget = parent.tabWidget.widget(index)
            if widget is not None:
                widget.deleteLater()
            parent.tabWidget.removeTab(index)