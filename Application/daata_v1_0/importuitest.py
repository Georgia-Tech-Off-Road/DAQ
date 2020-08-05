from PyQt5 import QtWidgets, uic
import ui_DAATA
import sys


class arduinoSettingAndData(object):
    def __init__(self, portCOM, baudrate):

        self.arduino = serial.Serial()
        self.arduino.port = portCOM
        self.arduino.baudrate = 115200

        # self.arduino.is_open()
        print("Hello")


    def import_Settings(self):
        pass
    """
        Matlab code (to be converted into Python)
        % Sends a code to Arduino to start setup
        disp('Executing...');
        % writeline(arduino, "start_arduinoSetup");
        while (num2str(readline(arduino))~="start_setup")
            % disp("No response from Arduino...");
        writeline(arduino, "start_arduinoSetup");

            end
            num_sensors = str2num(readline(arduino));
            num_sensorSettings = str2num(readline(arduino));
            enabledSensors = read(arduino, 1, "uint8");
            disp("MatLab setup initializing...");


        sensorList = strings(num_sensors, num_sensorSettings);

        while (num2str(readline(arduino))~="begin_sensor")
            disp("No response from Arduino...");
            writeline(arduino, "start_listSend");
        end

        disp("here");
        for n = 1:num_sensors
        for s = 1:num_sensorSettings
        sensorList(n, s) = num2str(readline(arduino));
        % disp(sensorList(n, s));
        pause(.1);
        end
        end

        disp("Finished setup");

        % delete(arduino);
    """

class DAATA_Ui(QtWidgets.QMainWindow, ui_DAATA.Ui_MainWindow):
    def __init__(self):
        super(DAATA_Ui, self).__init__()
        self.setupUi(self)
        self.connect_SlotsAndSignals()
        self.create_SensorCheckboxes()

    # Create checkboxes based on a list of strings
    def create_SensorCheckboxes(self):
        self.checkBox1 = QtWidgets.QCheckBox("checkBox 1", self.scrollAreaWidgetContents_2)
        self.gridLayout.addWidget(self.checkBox1)
        self.checkBox2 = QtWidgets.QCheckBox("checkBox 2", self.scrollAreaWidgetContents_2)
        self.gridLayout.addWidget(self.checkBox2)
        self.checkBox3 = QtWidgets.QCheckBox("checkBox 3", self.scrollAreaWidgetContents_2)
        self.gridLayout.addWidget(self.checkBox3)

        # Create a vertical spacer that forces checkboxes to the top
        spacerItem1 = QtWidgets.QSpacerItem(20, 1000000, QtWidgets.QSizePolicy.Minimum, QtWidgets.QSizePolicy.Expanding)
        self.gridLayout.addItem(spacerItem1)

    def connect_SlotsAndSignals(self):
        self.button_Record.clicked.connect(self.change_RecordingState)

    def change_RecordingState(self):
        if self.button_Record.isChecked():
            self.indicator_OnOrOff.setText("On")
            self.indicator_OnOrOff.setStyleSheet("color: green;")
            self.button_Record.setText("Stop Recording")
        else:
            self.indicator_OnOrOff.setText("Off")
            self.indicator_OnOrOff.setStyleSheet("color: red;")
            self.button_Record.setText("Start Recording")

    def change_ActiveSensors(self):
        pass


    # self.gridLayout.addItem(spacerItem, 10, 0, 1, 1)
    # self.checkBox_4 = QtWidgets.QCheckBox(self.scrollAreaWidgetContents_2)
    # self.checkBox_4.setObjectName("checkBox_4")
    # self.gridLayout.addWidget(self.checkBox_4, 8, 0, 1, 1)
    # self.checkBox = QtWidgets.QCheckBox(self.scrollAreaWidgetContents_2)
    # self.checkBox.setObjectName("checkBox")
    # self.gridLayout.addWidget(self.checkBox, 2, 0, 1, 1)
    # self.checkBox_2 = QtWidgets.QCheckBox(self.scrollAreaWidgetContents_2)
    # self.checkBox_2.setObjectName("checkBox_2")
    # self.gridLayout.addWidget(self.checkBox_2, 4, 0, 1, 1)
    # self.checkBox_3 = QtWidgets.QCheckBox(self.scrollAreaWidgetContents_2)
    # self.checkBox_3.setObjectName("checkBox_3")
    # self.gridLayout.addWidget(self.checkBox_3, 9, 0, 1, 1)
    # self.checkBox_6 = QtWidgets.QCheckBox(self.scrollAreaWidgetContents_2)
    # self.checkBox_6.setObjectName("checkBox_6")
    # self.gridLayout.addWidget(self.checkBox_6, 6, 0, 1, 1)
    # self.checkBox_7 = QtWidgets.QCheckBox(self.scrollAreaWidgetContents_2)
    # self.checkBox_7.setObjectName("checkBox_7")
    # self.gridLayout.addWidget(self.checkBox_7, 7, 0, 1, 1)
    # self.checkBox_8 = QtWidgets.QCheckBox(self.scrollAreaWidgetContents_2)
    # self.checkBox_8.setObjectName("checkBox_8")
    # self.gridLayout.addWidget(self.checkBox_8, 5, 0, 1, 1)


# Code that starts the GUI
app = QtWidgets.QApplication(sys.argv)
daata = DAATA_Ui()
daata.show()
app.exec_()


DAQduino = arduinoSettingAndData('COM9', 115200)
