from PyQt5 import QtWidgets, QtGui, uic
import os
from Utilities.findAncestryTree import findAncestryTree



## Creates a popup window to display all objects and their parent/child relationships
uiFile , _ = uic.loadUiType(os.path.join(os.path.dirname(__file__), 'dialog_info.ui'))  # loads the .ui file from QT Designer
class popup_ParentChildrenTree(QtWidgets.QDialog, uiFile):
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



