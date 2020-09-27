from PyQt5 import QtWidgets, uic, QtCore, QtGui
import os
from DataAcquisition import data
from Layouts import DAATALayout
import logging

logger = logging.getLogger("Homepage")

uiFile, _ = uic.loadUiType(os.path.join(os.path.dirname(__file__), 'homepage.ui'))  # loads the .ui file from QT Desginer

class Layout_Homepage(DAATALayout, uiFile):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        # self.hide()
        self.dict_sensorStatus = {}
        self.connected_sensors = data.get_sensors(is_connected=True)

        self.create_sensorStatusCheckboxes()
        self.create_connectionStatusCheckboxes()
        # self.export_data()

        self.updateFreq =  2    # how often the layout checks for new sensors (Hz)


    def export_data(self):
        dir_ = QtGui.QFileDialog.getExistingDirectory(None, 'Select GTOR Network Drive', os.path.expanduser('~'), QtGui.QFileDialog.ShowDirsOnly)       #select a folder in the C drive
        print(dir_)
        # file = QtWidgets.QFileDialog.getSaveFileName(self, 'Save File', "data_collection.mat")        # returns a QUrl object of what User wishes to save file as
        # f = open(file[0], 'w')
        # f.write("blahablahjaha")
        # f.close()
        # print(file)
        pass

    def create_sensorStatusCheckboxes(self):
        all_sensors = data.get_sensors()
        for sensor in all_sensors:
            self.dict_sensorStatus[sensor] = {}
            self.dict_sensorStatus[sensor]['indicator'] = QtWidgets.QCheckBox(data.get_display_name(sensor),
                                                                              objectName=sensor)
            self.verticalLayout_sensorStatus.addWidget(self.dict_sensorStatus[sensor]['indicator'])

            # disables user input for checkboxes
            self.dict_sensorStatus[sensor]['indicator'].setAttribute(QtCore.Qt.WA_TransparentForMouseEvents)
            self.dict_sensorStatus[sensor]['indicator'].setFocusPolicy(QtCore.Qt.NoFocus)


        # Create a vertical spacer that forces checkboxes to the top
        spacerItem1 = QtWidgets.QSpacerItem(20, 1000000, QtWidgets.QSizePolicy.Minimum,
                                            QtWidgets.QSizePolicy.Expanding)
        self.verticalLayout_sensorStatus.addItem(spacerItem1)


    def update_sensorStatus(self):
        connected_sensors = data.get_sensors(is_connected=True)
        for sensor in self.dict_sensorStatus.keys():
            if sensor in connected_sensors:
                self.dict_sensorStatus[sensor]['indicator'].setChecked(True)
            else:
                self.dict_sensorStatus[sensor]['indicator'].setChecked(False)

    def create_connectionStatusCheckboxes(self):
        self.ind_RFBox = QtWidgets.QCheckBox("RF Box Disconnected", objectName = "ind_RFBox")
        self.verticalLayout_connectionStatus.addWidget(self.ind_RFBox)
        # disables user input for checkboxes
        self.ind_RFBox.setAttribute(QtCore.Qt.WA_TransparentForMouseEvents)
        self.ind_RFBox.setFocusPolicy(QtCore.Qt.NoFocus)

        self.ind_SDCard = QtWidgets.QCheckBox("SD Card Disconnected", objectName = "ind_SDCard")
        self.verticalLayout_connectionStatus.addWidget(self.ind_SDCard)
        # disables user input for checkboxes
        self.ind_SDCard.setAttribute(QtCore.Qt.WA_TransparentForMouseEvents)
        self.ind_SDCard.setFocusPolicy(QtCore.Qt.NoFocus)

        self.ind_connectionStatus = QtWidgets.QCheckBox("Network Drive Disconnected", objectName = "ind_connectionStatus")
        self.verticalLayout_connectionStatus.addWidget(self.ind_connectionStatus)
        # disables user input for checkboxes
        self.ind_connectionStatus.setAttribute(QtCore.Qt.WA_TransparentForMouseEvents)
        self.ind_connectionStatus.setFocusPolicy(QtCore.Qt.NoFocus)


        # Create a vertical spacer that forces checkboxes to the top
        spacerItem1 = QtWidgets.QSpacerItem(20, 1000000, QtWidgets.QSizePolicy.Minimum,
                                            QtWidgets.QSizePolicy.Expanding)
        self.verticalLayout_connectionStatus.addItem(spacerItem1)



    def update(self):
        self.update_sensorStatus()
        logger.debug('updating ' + self.objectName() + "...")