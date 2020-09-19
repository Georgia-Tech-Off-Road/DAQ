from PyQt5 import QtWidgets, uic, QtCore, QtGui
import os
from DataAcquisition import data


Ui_Widget_Test, _ = uic.loadUiType(os.path.join(os.path.dirname(__file__), 'widget_Homepage.ui'))  # loads the .ui file from QT Desginer

class Widget_Homepage(QtWidgets.QWidget, Ui_Widget_Test):
    def __init__(self, parent):
        super().__init__()
        self.setupUi(self)
        # self.hide()
        self.parent = parent
        self.dict_sensorStatus = {}
        self.export_data()
        self.update_sensorStatus()
        self.create_buttonTab()


    def export_data(self):
        dir_ = QtGui.QFileDialog.getExistingDirectory(None, 'Select a folder:', 'C:\\', QtGui.QFileDialog.ShowDirsOnly)       #select a folder in the C drive
        print(dir_)
        # file = QtWidgets.QFileDialog.getSaveFileName(self, 'Save File', "data_collection.mat")        # returns a QUrl object of what User wishes to save file as
        # f = open(file[0], 'w')
        # f.write("blahablahjaha")
        # f.close()
        # print(file)
        pass

    def update_sensorStatus(self):
        connected_sensors = data.get_sensors(is_connected=True)
        for sensor in connected_sensors:
            self.dict_sensorStatus['sensor'] = {}
            self.dict_sensorStatus['sensor']['indicator'] = QtWidgets.QRadioButton(data.get_display_name(sensor), self.scrollArea_sensorStatus, objectName=sensor, checkable  = False)
            self.verticalLayout_sensorStatus.addWidget(self.dict_sensorStatus['sensor']['indicator'])
            # Create a vertical spacer that forces checkboxes to the top
        spacerItem1 = QtWidgets.QSpacerItem(20, 1000000, QtWidgets.QSizePolicy.Minimum,
                                            QtWidgets.QSizePolicy.Expanding)
        self.gridLayout_2.addItem(spacerItem1)


    def create_buttonTab(self):
        self.button = QtWidgets.QPushButton("Homepage")
        self.button.clicked.connect(self.switch_Homepage)

    def switch_Homepage(self):
        if self.button.text() == "Switch to Homepage":
            # self.parent().parent().addTab(self,"Homepage")
            self.parent.addTab(self,"Homepage")
            self.button.setText("Switch to Widgets")



    def updateParent(self):
        # self.parent()
        pass