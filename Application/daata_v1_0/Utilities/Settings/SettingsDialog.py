from PyQt5 import QtWidgets, uic
import os


uiFile, _ = uic.loadUiType(os.path.join(os.path.dirname(__file__), 'settings.ui'))  # loads the .ui file from QT Desginer
class SettingsDialog(QtWidgets.QDialog, uiFile):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        self.debug_tree()

        # for item in self.treeWidget_categories.items():
        #     item.setSizeHint(1,1000)
        returnValue = self.exec()

    def debug_tree(self):
        for i in range(self.treeWidget_categories.topLevelItemCount()):
            item = self.treeWidget_categories.topLevelItem(i)
            print(item.text(0))
            self.debug_treeRecursive(item, 0)

    def debug_treeRecursive(self, item, numTabs):
        numTabs += 1
        tabspacing = ""
        for n in range(numTabs):
            tabspacing += "\t"

        for i in range(item.childCount()):
            child = item.child(i)
            print(child.text(0))
            self.debug_treeRecursive(child, numTabs)


