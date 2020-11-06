from PyQt5 import QtWidgets, QtGui, uic, QtCore
import os
from Utilities.findAncestryTree import findAncestryTree
from PyQt5.QtWidgets import QDialog, QDialogButtonBox, QVBoxLayout, QApplication


## Creates a popup window to display all objects and their parent/child relationships
uiFile_parentChildrenTree , _ = uic.loadUiType(os.path.join(os.path.dirname(__file__), 'dialog_info.ui'))  # loads the .ui file from QT Designer
class popup_ParentChildrenTree(QtWidgets.QDialog, uiFile_parentChildrenTree):
    def __init__(self, parent):
        super().__init__()
        self.setupUi(self)
        self.setWindowFlags(QtCore.Qt.WindowCloseButtonHint)     # hide the question mark in title bar
        self.setWindowIcon(QtGui.QIcon('iconInfo.png'))
        self.setWindowTitle("Parent/Children Tree")
        self.resize(750,500)

        self.ancestor_count = 0
        tree = findAncestryTree(ancestor_count=self.ancestor_count, parent = parent, tree = parent.objectName())
        self.label.setText(tree)

        returnValue = self.exec()


# open confirmation dialog when attempting to stop data collection
uiFile_stopDataConfirmation , _ = uic.loadUiType(os.path.join(os.path.dirname(__file__), 'dialog_confirmation.ui'))  # loads the .ui file from QT Designer
class popup_stopDataConfirmation(QDialog, uiFile_stopDataConfirmation):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        self.setWindowFlags(QtCore.Qt.WindowCloseButtonHint)     # hide the question mark in title bar
        self.setWindowIcon(QtGui.QIcon('iconWarning.png'))
        self.setWindowTitle("Stop data collection and save data?")
        self.label.setText("WARNING: Data being collected in current run will be stopped and CANNOT be resumed")
        returnValue = self.exec()

if __name__ == "__main__":
    import sys
    app = QApplication(sys.argv)
    ui = popup_stopDataConfirmation()
    sys.exit(app.exec_())

