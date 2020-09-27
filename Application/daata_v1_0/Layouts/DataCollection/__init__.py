from PyQt5 import QtWidgets, uic, QtCore, QtGui
from PyQt5.QtCore import QSettings
import sys
import os

import pyqtgraph as pg
import numpy as np
from functools import partial
import threading
import DataAcquisition
from DataAcquisition import data
from Utilities.Plotting import RealTimePlot
from Layouts import DAATALayout

import logging

## Default plot configuration for pyqtgraph
pg.setConfigOption('background', 'w')   # white
pg.setConfigOption('foreground', 'k')   # black


uiFile, _ = uic.loadUiType(os.path.join(os.path.dirname(__file__), 'data_collection.ui'))  # loads the .ui file from QT Desginer

logger = logging.getLogger("DataCollection")
debugCounter = 0;

#Todo List      ####################################
## add refresh button in menu to scan for hardware changes when a new sensor is plugged in, to add a checkbox and graph for it
## add warning dialog if trying to start recording data while teensy is not plugged in (checked with data.is_connected)

class Layout_DataCollection(DAATALayout, uiFile):
    def __init__(self, data_collection_thread, is_data_collecting):
        super().__init__()
        self.setupUi(self)
        self.hide()


        self.updateFreq = 30
        self.data = DataAcquisition.data
        self.dict_sensors = {}  # instantiates sensor dictionary
        for key in self.data.get_sensors(is_plottable=True, is_connected=True):
            self.dict_sensors[key] = dict()

        self.activeSensorCount = 0

        self.create_sensorCheckboxes()
        self.label_activeSensorCount.setText('(' + str(self.activeSensorCount) + '/' + str(len(self.dict_sensors)) + ')')     # reset the label for number of active sensors
        self.create_graphDimensionComboBox()
        self.create_graphs()

        self.connect_slotsAndSignals()

        from MainWindow import is_data_collecting
        self.is_data_collecting = is_data_collecting


        ## CODE IN CASE FOR WHEN APPLICATION IS EXITED
        ## right now only saves settings when the tab is closed
        self.settings = QSettings('DAATA', 'Data Collection')
        self.loadSettings()




    # Create checkboxes based on a list of strings
    #TODO make this update with Homepage if a new sensor gets connected ##################################################################
    def create_sensorCheckboxes(self):
        self.selectAll_checkbox = QtWidgets.QCheckBox("Select All", self.scrollAreaWidgetContents_2, objectName="selectAll_checkbox")
        self.selectAll_checkbox.setToolTip(self.selectAll_checkbox.objectName())
        self.gridLayout_2.addWidget(self.selectAll_checkbox)

        ## create a checkbox for each sensor in dictionary in self.scrollAreaWidgetContents_2
        for key in self.data.get_sensors(is_plottable = True, is_connected = True):
            self.dict_sensors[key]['Checkbox'] = QtWidgets.QCheckBox(self.data.get_display_name(key), self.scrollAreaWidgetContents_2, objectName=key)
            self.gridLayout_2.addWidget(self.dict_sensors[key]['Checkbox'])

        # Create a vertical spacer that forces checkboxes to the top
        spacerItem1 = QtWidgets.QSpacerItem(20, 1000000, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Expanding)
        self.gridLayout_2.addItem(spacerItem1)

    ## make a checkbox for any newly connected sensors
    def update_sensorCheckboxes(self):
        connected_sensors = data.get_sensors(is_plottable = True, is_connected=True)
        for key in self.dict_sensors.keys():
            if key in connected_sensors:
                if self.dict_sensors[key]['Checkbox'] is None:
                    self.dict_sensors[key]['Checkbox'] = QtWidgets.QCheckBox(self.data.get_display_name(key), self.scrollAreaWidgetContents_2, objectName=key)
                else:
                    pass
            else:
                try:
                    self.scrollAreaWidgetContents_2.removeWidget(self.dict_sensors[key]['Checkbox'])
                except:
                    pass



    def create_graphDimensionComboBox(self):
        maxRows = 4
        maxCols = 3

        for row in range(1,maxRows+1):
            for col in range(1,maxCols+1):
                self.comboBox_graphDimension.addItem("{0}x{1}".format(row,col))

    def slot_graphDimension(self):
        currDim = self.comboBox_graphDimension.currentText().split('x')
        maxRows = int(currDim[0])
        maxCols = int(currDim[1])
        row = 0
        col = 0



        for key in self.dict_sensors.keys():
            if self.dict_sensors[key]['Graph Widget'].isVisible():
                try:
                    self.gridLayout_graphs.removeWidget(self.dict_sensors[key]['Graph Widget'])
                    self.dict_sensors[key]['Graph Widget'].hide()
                    # self.dict_sensors[key]['Graph Widget'].deleteLater()
                except:
                    pass
                    print(key + " is " + self.dict_sensors[key]['Graph Widget'].isVisible())

        leftMar, topMar, rightMar, botMar = self.gridLayout_graphs.getContentsMargins()
        hSpace = self.gridLayout_graphs.horizontalSpacing()
        vSpace = self.gridLayout_graphs.verticalSpacing()

        graphHeight = (self.scrollArea_graphs.height()-topMar-botMar-vSpace*(maxRows)) / maxRows

        for key in self.dict_sensors.keys():
            if self.dict_sensors[key]['Checkbox'].isChecked():
                # self.dict_sensors[key]['Graph Widget'] = RealTimePlot(key, parent=self.scrollAreaWidgetContents,
                #                                                       graph_width_seconds=10)
                # self.dict_sensors[key]['Graph Widget'].setObjectName(key)

                if col == maxCols:
                    col = 0
                    row += 1
                self.dict_sensors[key]['Graph Widget'].set_height(graphHeight)
                self.gridLayout_graphs.addWidget(self.dict_sensors[key]['Graph Widget'], row, col)
                self.dict_sensors[key]['Graph Widget'].show()
                col += 1
            else:
                self.dict_sensors[key]['Graph Widget'].hide()



        global debugCounter
        # add a spacer to the bottom of the layout top align the graphs
        spacerItem2 = QtWidgets.QSpacerItem(20, 1000000, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Expanding)
        self.gridLayout_graphs.addItem(spacerItem2)
        debugCounter+=1
        for child in self.findChildren(QtWidgets.QSpacerItem):
            print(child)



    def create_graphs(self):
        for key in self.dict_sensors.keys():
            self.dict_sensors[key]['Graph Widget'] = RealTimePlot(key, parent=self.scrollAreaWidgetContents, graph_width_seconds = 8)
            self.dict_sensors[key]['Graph Widget'].setObjectName(key)
            self.dict_sensors[key]['Graph Widget'].hide()




    def connect_slotsAndSignals(self):
        self.button_display.clicked.connect(self.slot_changeDisplayingState)
        for key in self.dict_sensors.keys():
            self.dict_sensors[key]['Checkbox'].clicked.connect(partial(self.slot_checkboxClicked, key))        ## partial creates a new function of slot_checkboxClicked for each key passed in
            # self.dict_sensors[key]['Checkbox'].clicked.connect(self.slot_graphDimension)
        self.selectAll_checkbox.clicked.connect(self.slot_checkboxSelectAll)

        self.comboBox_graphDimension.currentTextChanged.connect(self.slot_graphDimension)



    def slot_changeDisplayingState(self):
        if self.button_display.isChecked():
            self.indicator_onOrOff.setText("On")
            self.indicator_onOrOff.setStyleSheet("color: green;")
            self.button_display.setText("Stop Collecting Data")
            self.is_data_collecting.set()
        else:
            self.indicator_onOrOff.setText("Off")
            self.indicator_onOrOff.setStyleSheet("color: red;")
            self.button_display.setText("Start Collecting Data")
            self.is_data_collecting.clear()



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
        self.slot_graphDimension()

    def slot_checkboxClicked(self, key):
        ## update the label displaying number of active sensors
        if self.dict_sensors[key]['Checkbox'].isChecked():
            self.activeSensorCount = self.activeSensorCount + 1
        else:
            self.activeSensorCount = self.activeSensorCount - 1
        self.label_activeSensorCount.setText('(' + str(self.activeSensorCount) + '/' + str(len(self.dict_sensors)) + ')')

        ## display/hide the graph
        if self.dict_sensors[key]['Checkbox'].isChecked():
            # self.dict_sensors[key]['Graph Widget'].show()
            self.slot_graphDimension()
        else:
            # self.dict_sensors[key]['Graph Widget'].hide()
            self.slot_graphDimension()

    def updateGraph(self):
        for key in self.dict_sensors.keys():
            if self.dict_sensors[key]['Graph Widget'].isVisible():
                self.dict_sensors[key]['Graph Widget'].update_graph()

    def updateTimeElapsed(self):
        try:
            secondsElapsed = DataAcquisition.data.get_value("time", DataAcquisition.data.get_most_recent_index())
            secondsElapsedInt = int(secondsElapsed)
            hoursElapsed = int(secondsElapsedInt / 3600)
            minutesElapsed = int((secondsElapsedInt - hoursElapsed * 3600) / 60)
            secondsElapsed = secondsElapsed % 60
            formatTime = "{hours:02d}:{minutes:02d}:{seconds:05.2f}"
            strTime = formatTime.format(hours=hoursElapsed, minutes=minutesElapsed, seconds=secondsElapsed)
            self.label_timeElapsed.setText(strTime)
        except TypeError:
            pass

    def update(self):
        # update only
        # if the Data Collection tab is the current tab
        # or if the Layouts tab is the current tab

        if self.is_data_collecting.is_set():
            self.updateGraph()
            self.updateTimeElapsed()
            self.update_sensorCheckboxes()
            # logger.debug('updating ' + self.objectName() + "...")

        #### temporary implementation of global recording button update
        if self.is_data_collecting.is_set():
            self.indicator_onOrOff.setText("On")
            self.indicator_onOrOff.setStyleSheet("color: green;")
            self.button_display.setText("Stop Collecting Data")
            self.button_display.setChecked(True)
        else:
            self.indicator_onOrOff.setText("Off")
            self.indicator_onOrOff.setStyleSheet("color: red;")
            self.button_display.setText("Start Collecting Data")
            self.button_display.setChecked(False)

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

class plotContainer:
    def __init__(self):
        pass



# if __name__ == "__main__":
#     app = QtWidgets.QApplication(sys.argv)
#     # widget = QtWidgets.QWidget()
#     ui = DataCollection()
#     ui.show()
#     sys.exit(app.exec_())