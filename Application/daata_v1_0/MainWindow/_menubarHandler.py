from PyQt5 import QtWidgets


class MenuAction:
    ## Make an action to create a tab for each imported widget
    for key in self.dict_layouts.keys():
        self.dict_layouts[key]['Menu Action'] = QtWidgets.QAction(self)
        self.dict_layouts[key]['Menu Action'].setCheckable(False)
        self.dict_layouts[key]['Menu Action'].setToolTip('Open a new tab for ' + key)
        self.dict_layouts[key]['Menu Action'].setText(key)
        self.menuWidget.addAction(self.dict_layouts[key]['Menu Action'])

    ## Make an action in the File menu to display current parent and children tree
    self.action_parentChildren = QtWidgets.QAction(self)
    self.action_parentChildren.setToolTip(
        'Display a tree of all parent objects and their respective children for the current UI layout')
    self.action_parentChildren.setText('Display parent/children tree')
    self.menuFile.addAction(self.action_parentChildren)
