from PyQt5 import QtWidgets, uic, QtCore, QtGui
from PyQt5.QtCore import QSettings
import sys
import os

import pyqtgraph as pg
import numpy as np
from functools import partial
import threading
import DataAcquisition
from Utilities.Plotting import RealTimePlot

## Default plot configuration for pyqtgraph
pg.setConfigOption('background', 'w')   # white
pg.setConfigOption('foreground', 'k')   # black


Ui_Widget_Test, _ = uic.loadUiType(os.path.join(os.path.dirname(__file__), 'data_collection.ui'))  # loads the .ui file from QT Desginer


#Todo List      ####################################
## add refresh button in menu to scan for hardware changes when a new sensor is plugged in, to add a checkbox and graph for it
## add warning dialog if trying to start recording data while teensy is not plugged in (checked with data.is_connected)

class Widget_DataCollection(QtWidgets.QWidget, Ui_Widget_Test):
    def __init__(self, data_collection_thread, is_data_collecting):
        super().__init__()
        self.setupUi(self)
        self.hide()

        from MainWindow import is_data_collecting

        self.is_data_collecting = is_data_collecting
        is_data_collecting.set()
        self.data = DataAcquisition.data
        self.dict_sensors = {}  # instantiates sensor dictionary
        self.activeSensorCount = 0
        self.pos = 0
        self.y = np.zeros(100000, float)
        for n in range(100000):
            self.y[n] = np.sin(.1 * n)

        self.import_arduinoDict()
        self.create_sensorCheckboxes()
        self.label_activeSensorCount.setText('(' + str(self.activeSensorCount) + '/' + str(len(self.dict_sensors)) + ')')     # reset the label for number of active sensors
        self.create_graphs()

        self.connect_slotsAndSignals()


        self.timer = QtCore.QTimer()
        self.timer.timeout.connect(self.updateAll)
        self.timer.start(100)
        self.timer.setInterval(100)


        ## CODE IN CASE FOR WHEN APPLICATION IS EXITED
        ## right now only saves settings when the tab is closed
        self.settings = QSettings('DAATA', 'Data Collection')
        self.loadSettings()


    def import_arduinoDict(self):
        sensors = self.data.get_sensors(is_plottable=True, is_connected=True)
        for sensor in sensors:
            self.dict_sensors[sensor] = dict()
            self.dict_sensors[sensor]['unit'] = self.data.get_unit(sensor)
            self.dict_sensors[sensor]['unit_short'] = self.data.get_unit_short(sensor)
        print(self.dict_sensors)


    # Create checkboxes based on a list of strings
    def create_sensorCheckboxes(self):
        self.selectAll_checkbox = QtWidgets.QCheckBox("Select All", self.scrollAreaWidgetContents_2, objectName="selectAll_checkbox")
        self.selectAll_checkbox.setToolTip(self.selectAll_checkbox.objectName())
        self.gridLayout_2.addWidget(self.selectAll_checkbox)

        ## create a checkbox for each sensor in dictionary in self.scrollAreaWidgetContents_2
        for key in self.dict_sensors.keys():
            self.dict_sensors[key]['Checkbox'] = QtWidgets.QCheckBox(key, self.scrollAreaWidgetContents_2, objectName=key)
            self.gridLayout_2.addWidget(self.dict_sensors[key]['Checkbox'])

        # Create a vertical spacer that forces checkboxes to the top
        spacerItem1 = QtWidgets.QSpacerItem(20, 1000000, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Expanding)
        self.gridLayout_2.addItem(spacerItem1)


    def create_graphs(self):
        for key in self.dict_sensors.keys():
            self.dict_sensors[key]['Graph Widget'] = RealTimePlot(key, parent=self.scrollAreaWidgetContents)

            self.verticalLayout.addWidget(self.dict_sensors[key]['Graph Widget'])
            self.dict_sensors[key]['Graph Widget'].hide()

        # add a spacer to the bottom of the layout top align the graphs
        spacerItem2 = QtWidgets.QSpacerItem(20, 1000000, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Expanding)
        self.verticalLayout.addItem(spacerItem2)


    def connect_slotsAndSignals(self):
        self.button_record.clicked.connect(self.slot_changeRecordingState)
        # self.button_record.clicked.connect(self.updateGraph)
        for key in self.dict_sensors.keys():
            self.dict_sensors[key]['Checkbox'].clicked.connect(partial(self.slot_checkboxClicked, key))        ## partial creates a new function of slot_checkboxClicked for each key passed in
        self.selectAll_checkbox.clicked.connect(self.slot_checkboxSelectAll)


    def slot_changeRecordingState(self):
        if self.button_record.isChecked():
            self.indicator_onOrOff.setText("On")
            self.indicator_onOrOff.setStyleSheet("color: green;")
            self.button_record.setText("Stop Recording")
        else:
            self.indicator_onOrOff.setText("Off")
            self.indicator_onOrOff.setStyleSheet("color: red;")
            self.button_record.setText("Start Recording")


    def slot_checkboxSelectAll(self):
        if self.selectAll_checkbox.isChecked():
            for key in self.dict_sensors.keys():
                self.dict_sensors[key]['Checkbox'].setChecked(True)
                self.dict_sensors[key]['Graph Widget'].show()
                self.activeSensorCount = len(self.dict_sensors)
        else:
            for key in self.dict_sensors.keys():
                self.dict_sensors[key]['Checkbox'].setChecked(False)
                self.dict_sensors[key]['Graph Widget'].hide()
            self.activeSensorCount = 0
        self.label_activeSensorCount.setText('(' + str(self.activeSensorCount) + '/' + str(len(self.dict_sensors)) + ')')

    def slot_checkboxClicked(self, key):
        ## update the label displaying number of active sensors
        if self.dict_sensors[key]['Checkbox'].isChecked():
            self.activeSensorCount = self.activeSensorCount + 1
        else:
            self.activeSensorCount = self.activeSensorCount - 1
        self.label_activeSensorCount.setText('(' + str(self.activeSensorCount) + '/' + str(len(self.dict_sensors)) + ')')

        ## display/hide the graph
        if self.dict_sensors[key]['Checkbox'].isChecked():
            self.dict_sensors[key]['Graph Widget'].show()
        else:
            self.dict_sensors[key]['Graph Widget'].hide()

    def updateGraph(self):
        for key in self.dict_sensors.keys():
            self.dict_sensors[key]['Graph Widget'].update_graph()

    def updateAll(self):
        if self.isHidden():
            # print(self.objectName() + " is hidden")
            pass
        else:
            if self.button_record.isChecked():
                self.updateGraph()
                self.label_timeElapsed.setText(str(self.pos))
            # print('updating ' + self.objectName() + "...")



    def loadSettings(self):
        try:
            for key in self.settings.value('enabledSensors'):
                self.dict_sensors[key]['Checkbox'].setChecked(True)
                self.dict_sensors[key]['Graph Widget'].show()
                self.activeSensorCount = self.activeSensorCount + 1
                self.label_activeSensorCount.setText(
                    '(' + str(self.activeSensorCount) + '/' + str(len(self.dict_sensors)) + ')')
        except:
            pass

    def closeEvent(self, event):
        enabledSensors = []

        for key in self.dict_sensors.keys():
            if self.dict_sensors[key]['Checkbox'].isChecked():
                enabledSensors.append(key)

        self.settings.setValue('enabledSensors', enabledSensors)

        self.window().setWindowTitle('closed tab')


# if __name__ == "__main__":
#     app = QtWidgets.QApplication(sys.argv)
#     # widget = QtWidgets.QWidget()
#     ui = DataCollection()
#     ui.show()
#     sys.exit(app.exec_())