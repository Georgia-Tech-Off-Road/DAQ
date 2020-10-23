from PyQt5 import QtWidgets, QtGui, uic, QtCore
import os

''' "saveLocationDialog" configFile settings


'''


uiFile, _ = uic.loadUiType(os.path.join(os.path.dirname(__file__), 'saveLocationDialog.ui'))  # loads the .ui file from QT Designer
class popup_dataSaveLocation(QtWidgets.QDialog, uiFile):
    def __init__(self):
        super().__init__()
        self.setupUi(self)
        self.configFile = QtCore.QSettings('DAATA', 'saveLocationDialog')
        self.loadSettings()

        self.toggle_frames()
        self.connectSlotsSignals()
        returnValue = self.exec()

    ## imported methods
    from Utilities.DataExport.exportCSV import saveCSV


    def loadSettings(self):
        self.checkBox_local.setChecked(self.configFile.value("checkBox_local") == 'true')
        self.checkBox_networkDrive.setChecked(self.configFile.value("checkBox_ND") == 'true')
        self.checkBox_SDCard.setChecked(self.configFile.value("checkBox_SD") == 'true')


        self.lineEdit_filenameLocal.setText(self.configFile.value("default_localFilename"))
        self.lineEdit_folderLocal.setText(self.configFile.value("default_localDirectory"))

        self.lineEdit_filenameND.setText(self.configFile.value("default_NDFilename"))
        self.lineEdit_folderND.setText(self.configFile.value("default_NDFolder"))

        self.lineEdit_filenameSD.setText(self.configFile.value("default_SDFilename"))
        self.lineEdit_folderSD.setText(self.configFile.value("default_SDFolder"))

    def toggle_frames(self):
        if self.checkBox_local.isChecked():
            self.widget_local.show()
        else:
            self.widget_local.hide()


        if self.checkBox_networkDrive.isChecked():
            self.widget_networkDrive.show()
        else:
            self.widget_networkDrive.hide()

        if self.checkBox_SDCard.isChecked():
            self.widget_SDCard.show()
        else:
            self.widget_SDCard.hide()

    def saveData(self):
        if self.checkBox_local.isChecked():
            localFilename = self.lineEdit_filenameLocal.text()
            localFolder = self.lineEdit_folderLocal.text()
            localPath = os.path.join(localFolder,localFilename)

            self.saveCSV(localFilename,localFolder)

            # save current values as defaults
            self.configFile.setValue("default_localFilename", localFilename)
            self.configFile.setValue("default_localDirectory", localFolder)




        if self.checkBox_networkDrive.isChecked():
            NDFilename = self.lineEdit_filenameND.text()
            NDFolder = self.lineEdit_folderND.text()
            NDPath = os.path.join(NDFolder, NDFilename)
            NDFile = open(NDPath, 'w')
            NDFile.write("this is a test file for saving to the network drive")
            NDFile.close()

            self.configFile.setValue("default_NDFilename", NDFilename)
            self.configFile.setValue("default_NDFolder", NDFolder)



        if self.checkBox_SDCard.isChecked():
            SDFilename = self.lineEdit_filenameSD.text()
            SDFolder = self.lineEdit_folderSD.text()
            SDPath = os.path.join(SDFolder, SDFilename)
            SDFile = open(SDPath, 'w')
            SDFile.write("this is a test file for saving to the SD Card")
            SDFile.close()

            self.configFile.setValue("default_SDFilename", SDFilename)
            self.configFile.setValue("default_SDFolder", SDFolder)

        self.configFile.setValue("checkBox_local", self.checkBox_local.isChecked())
        self.configFile.setValue("checkBox_ND", self.checkBox_networkDrive.isChecked())
        self.configFile.setValue("checkBox_SD", self.checkBox_SDCard.isChecked())

        self.close()

    def change_NDDir(self):
        dir = QtGui.QFileDialog.getExistingDirectory(None, 'Save Data Location', os.path.expanduser('~'))  # select a folder in the C drive
        self.lineEdit_folderND.setText(dir)

    def change_localDir(self):
        dir = QtGui.QFileDialog.getExistingDirectory(None, 'Save Data Location', os.path.expanduser('~'))  # select a folder in the C drive
        self.lineEdit_folderLocal.setText(dir)



    def connectSlotsSignals(self):
        self.checkBox_local.clicked.connect(self.toggle_frames)
        self.checkBox_networkDrive.clicked.connect(self.toggle_frames)
        self.checkBox_SDCard.clicked.connect(self.toggle_frames)
        self.pushButton_save.clicked.connect(self.saveData)
        self.pushButton_browseDir.clicked.connect(self.change_localDir)
        self.pushButton_browseNDDir.clicked.connect(self.change_NDDir)
        # print(self.label.text())
        # for child in self.findChildren(QtWidgets.QCheckBox):
        #     print(child.objectName())


if __name__ == "__main__":
    import sys
    app = QtWidgets.QApplication(sys.argv)
    ui = popup_dataSaveLocation()
    sys.exit(app.exec_())
