from PyQt5 import QtWidgets, uic, QtCore, QtGui
import os



Ui_Dialog , _ = uic.loadUiType(os.path.join(os.path.dirname(__file__), 'dialog_info.ui'))  # loads the .ui file from QT Desginer


class Popup_ParentChildrenTree(QtWidgets.QDialog, Ui_Dialog):
    def __init__(self, parent):
        super().__init__()
        self.setupUi(self)
        self.setWindowTitle("Parent/Children Tree")
        self.resize(750,500)
        self.ancestor_count = 0
        tree = self.findKids(1, parent, parent.objectName())
        self.label.setText(tree)
        returnValue = self.exec()


    # recursive method to find all descendants of an QtWidgets object
    def findKids(self, ancestor_count, parent, tree):
        # If there is at least one child, continue building tree
        # If there are no children left, return the passed in tree
        if parent.findChild(QtWidgets.QWidget) != None:
            a = ""
            for n in range(ancestor_count):
                a += "\t"
            ancestor_count += 1
            for child in parent.findChildren(QtWidgets.QWidget):
                if (child.objectName() in tree) and (child.objectName() != "") or (str(child) in tree):
                    pass
                else:
                    if child.objectName() != "":
                        treeBranch = "\n" + a + child.objectName()
                    else:
                        treeBranch = "\n" + a + str(child)

                    tree = tree + self.findKids(ancestor_count, child, treeBranch)
        else:
            pass

        return tree

