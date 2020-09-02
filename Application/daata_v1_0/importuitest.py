from PyQt5 import QtWidgets, uic, QtCore
import sys

import pyqtgraph as pg
import numpy as np
from functools import partial
import json

dict_sensors = {}  # instantiates sensor dictionary
activeSensorCount = 0
pos = 0
xAxisWidth = 60


y = np.zeros(1000,float)
for n in range(1000):
    y[n] = np.sin(n)



## Default plot configuration for pyqtgraph
pg.setConfigOption('background', 'w')   # white
pg.setConfigOption('foreground', 'k')   # black



class arduinoSettingAndData(object):
    def __init__(self, portCOM, baudrate):
        self.arduino = serial.Serial()
        self.arduino.port = portCOM
        self.arduino.baudrate = 115200

        # self.arduino.is_open()
        print("Hello")

    def import_Settings(self):
        pass



Ui_MainWindow, _ = uic.loadUiType('ui_DAATA.ui')  # loads the .ui file from QT Desginer
class DAATA_Ui(QtWidgets.QMainWindow, Ui_MainWindow):
    def __init__(self):
        super(DAATA_Ui, self).__init__()
        self.setupUi(self)

        self.import_arduinoDict()

        self.create_sensorCheckboxes()
        self.label_activeSensorCount.setText('(' + str(activeSensorCount) + '/' + str(len(dict_sensors)) + ')')     # reset the label for number of active sensors
        self.create_graphs()

        self.connect_slotsAndSignals()

        # Example of setting stylesheets
        # self.scrollArea.setStyleSheet("""
        #     QScrollArea {
        #         border: 2px solid black;
        #         border-radius: 0px;
        #         background-color: blue;
        #         }
        #     """)



    def import_arduinoDict(self):
        global dict_sensors
        # temp matrix that represents information imported from Arduino code

        sensor_names = [
            ['LDS', 2, 1, 'seconds (s)', 'extension length (cm)'],
            ['Engine', 4, 0, 'seconds (s)', 'engine speed (rps)'],
            ['Accel', 16, 1, 'seconds (s)', 'acceleration (m^2/s)'],
            ['Brakes', 3, 2, 'seconds (s)', 'braking pressure (lbs of force)'],
            ['Phone', 2, 1, 'seconds (s)', 'signal (bars of data)']


        ]

        ## assigning each matrix index to a key/value pair in dictionary
        for index, sensor in enumerate(sensor_names):
            dict_sensors[sensor[0]] = {}
            dict_sensors[sensor[0]]['Byte Count'] = sensor[1]
            dict_sensors[sensor[0]]['On/Off'] = sensor[2]
            dict_sensors[sensor[0]]['X Units'] = sensor[3]
            dict_sensors[sensor[0]]['Y Units'] = sensor[4]


    # Create checkboxes based on a list of strings
    def create_sensorCheckboxes(self):
        global dict_sensors

        self.selectAll_checkbox = QtWidgets.QCheckBox("Select All", self.scrollAreaWidgetContents_2, objectName="selectAll_checkbox")
        self.selectAll_checkbox.setToolTip(self.selectAll_checkbox.objectName())
        self.gridLayout.addWidget(self.selectAll_checkbox)

        ## create a checkbox for each sensor in dictionary in self.scrollAreaWidgetContents_2
        for key in dict_sensors.keys():
            dict_sensors[key]['Checkbox'] = QtWidgets.QCheckBox(key, self.scrollAreaWidgetContents_2, objectName=key)
            self.gridLayout.addWidget(dict_sensors[key]['Checkbox'])

        # Create a vertical spacer that forces checkboxes to the top
        spacerItem1 = QtWidgets.QSpacerItem(20, 1000000, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Expanding)
        self.gridLayout.addItem(spacerItem1)


    def create_graphs(self):
        global dict_sensors
        for key in dict_sensors.keys():
            dict_sensors[key]['Graph Widget'] = pg.PlotWidget(self.scrollAreaWidgetContents)
            dict_sensors[key]['Graph Widget'].setMinimumSize(QtCore.QSize(0, 400))
            dict_sensors[key]['Graph Widget'].setMaximumSize(QtCore.QSize(16777215, 400))
            dict_sensors[key]['Graph Widget'].setMouseEnabled(False, False)             # disable mouse-scroll zooming on the graph
            dict_sensors[key]['Graph Widget'].setLabels(left=dict_sensors[key]['Y Units'], bottom=dict_sensors[key]['X Units'], title=key + ' Graph')     # set title and axes
            dict_sensors[key]['Graph Widget'].showGrid(x=True, y=True, alpha=.2)

            x = np.arange(100)
            y = np.random.randint(0,100,60)
            print(y)

            dict_sensors[key]['Plot'] = dict_sensors[key]['Graph Widget'].plot(pen='b')
            # dict_widgets[key]['Graph Widget']['Curve'] = pg.plot(x, y, pen='b')
            # dict_widgets[key]['Graph Widget'].addWidget = dict_widgets[key]['Graph Widget']['Curve']


            self.verticalLayout.addWidget(dict_sensors[key]['Graph Widget'])
            dict_sensors[key]['Graph Widget'].hide()

        # add a spacer to the bottom of the layout top align the graphs
        spacerItem2 = QtWidgets.QSpacerItem(20, 1000000, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Expanding)
        self.verticalLayout.addItem(spacerItem2)


    def connect_slotsAndSignals(self):
        self.button_record.clicked.connect(self.slot_changeRecordingState)
        self.button_record.clicked.connect(self.updateGraph)
        for key in dict_sensors.keys():
            dict_sensors[key]['Checkbox'].clicked.connect(partial(self.slot_checkboxClicked, key))        ## partial creates a new function of slot_checkboxClicked for each key passed in
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
        global activeSensorCount

        if self.selectAll_checkbox.isChecked():
            for key in dict_sensors.keys():
                dict_sensors[key]['Checkbox'].setChecked(True)
                dict_sensors[key]['Graph Widget'].show()
                activeSensorCount = len(dict_sensors)
        else:
            for key in dict_sensors.keys():
                dict_sensors[key]['Checkbox'].setChecked(False)
                dict_sensors[key]['Graph Widget'].hide()
            activeSensorCount = 0
        self.label_activeSensorCount.setText('(' + str(activeSensorCount) + '/' + str(len(dict_sensors)) + ')')

    def slot_checkboxClicked(self, key):
        global activeSensorCount

        ## update the label displaying number of active sensors
        if dict_sensors[key]['Checkbox'].isChecked():
            activeSensorCount = activeSensorCount + 1
        else:
            activeSensorCount = activeSensorCount - 1
        self.label_activeSensorCount.setText('(' + str(activeSensorCount) + '/' + str(len(dict_sensors)) + ')')

        ## display/hide the graph
        if dict_sensors[key]['Checkbox'].isChecked():
            dict_sensors[key]['Graph Widget'].show()
        else:
            dict_sensors[key]['Graph Widget'].hide()

    def updateGraph(self):
        global pos
        global y

        pos += 1
        dict_sensors['LDS']['Plot'].setData(y[pos:pos+xAxisWidth])
        dict_sensors['LDS']['Plot'].setPos(pos,0)

def update():
    global pos
    global y
    global dict_sensors

    pos += 1
    dict_sensors['LDS']['Plot'].setData(y[pos:pos+xAxisWidth])
    dict_sensors['LDS']['Plot'].setPos(pos,0)


def main():
    app = QtWidgets.QApplication(sys.argv)
    daata = DAATA_Ui()
    daata.show()
    sys.exit(app.exec_())





if __name__ == "__main__":
    main()


DAQduino = arduinoSettingAndData('COM9', 115200)
# while True:
    # updateGraph()

