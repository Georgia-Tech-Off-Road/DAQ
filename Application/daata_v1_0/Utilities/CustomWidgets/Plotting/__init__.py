import logging
import sys, os
import time
import pyqtgraph as pg
from functools import partial
from PyQt5 import QtCore, QtWidgets, uic, QtGui
import numpy
from PyQt5.QtWidgets import QGridLayout

from DataAcquisition import data

logger = logging.getLogger("Plotting")


uiPlotWidget, _ = uic.loadUiType(os.path.join(os.path.dirname(__file__), 'graphWidget.ui'))  # loads the .ui file from QT Desginer
class CustomPlotWidget(QtWidgets.QWidget, uiPlotWidget):
    def __init__(self, sensor_name, parent=None, **kwargs):
        super().__init__()
        self.setupUi(self)
        self.sensor_name = sensor_name
        self.setMinimumSize(QtCore.QSize(200, 200))
        self.setMaximumSize(QtCore.QSize(16777215, 400))


        sizePolicy = QtWidgets.QSizePolicy(QtWidgets.QSizePolicy.MinimumExpanding, QtWidgets.QSizePolicy.MinimumExpanding)
        sizePolicy.setVerticalStretch(4)
        sizePolicy.setHorizontalStretch(4)
        self.setSizePolicy(sizePolicy)




        self.plotWidget.setMouseEnabled(False, False)  # disable mouse-scroll zooming on the graph
        self.plotWidget.setLabels(left=str(data.get_display_name(sensor_name))+" ("+str(data.get_unit_short(sensor_name))+")",
                                  bottom='Time (s)',
                                  title=str(data.get_display_name(sensor_name)) + ' Graph')  # set title and axes
        self.plotWidget.showGrid(x=True, y=True, alpha=.2)
        self.plotWidget.setBackground(None)
        # Number of value to show on the x_axis
        self.samplingFreq = 200
        self.graph_width = kwargs.get("graph_width_seconds", 10) * self.samplingFreq

        self.embedLayout = kwargs.get("layout", None)      # the layout object the plot is embedded within

        # Frequency of values to show (if set to 5 will show every 5th value collected)
        self.show_width = kwargs.get("show_width", 5)

        # Row and column span of plot on the grid
        self.rowSpan = kwargs.get("rowspan", 1)
        self.rowSpan = kwargs.get("columnspan", 1)


        self.valueArray = numpy.zeros(self.graph_width)
        self.plot = self.plotWidget.plot(self.valueArray, pen='b', width = 1)
        self.initialCounter = 0

        self.configFile = QtCore.QSettings('DAATA_plot', self.sensor_name)
        self.configFile.clear()
        self.loadStylesheet()
        self.loadSettings()

    def loadStylesheet(self):
        self.stylesheetDefault = """
        CustomPlotWidget {
        background-color: white;
        }
        CustomPlotWidget * { 
        background-color: transparent;
        }
        """

        self.stylesheetHighlight = """
        CustomPlotWidget {
        background-color: white;
        border: 3px solid #196dff;
        }
        CustomPlotWidget * { 
        background-color: transparent;
        }
        """"""
        CustomPlotWidget {
        background-color: white;
        }
        CustomPlotWidget * { 
        background-color: transparent;
        }
        """
        self.setStyleSheet(self.stylesheetDefault)

    def set_graphWidth(self, seconds):
        try:
            self.graph_width = int(seconds) * self.samplingFreq
        except:
            self.graph_width = 10 * self.samplingFreq


    def set_yMinMax(self, yMin, yMax):
        self.plotWidget.setYRange(0, 100)
        self.enable_autoRange(True)
        self.plotWidget.setLimits(yMin = None, yMax = None)
        if yMax != 'auto' and yMin == 'auto':
            self.plotWidget.setLimits(yMax = int(yMax))
        if yMin != 'auto' and yMax == 'auto':
            self.plotWidget.setLimits(yMin = int(yMin))
        if (yMin != 'auto') and (yMax != 'auto'):
            self.plotWidget.setLimits(yMin = int(yMin), yMax = int(yMax))
        if (yMin == 'auto') and (yMax == 'auto'):
            self.enable_autoRange(True)
            self.plotWidget.setLimits(yMin = None, yMax = None)

    def enable_autoRange(self, enable=True):
        self.plotWidget.enableAutoRange(enable)

    def set_height(self, height):
        self.setMinimumSize(QtCore.QSize(200, height))
        self.setMaximumSize(QtCore.QSize(16777215, height))

    def update_graph(self):
        index_time = data.get_most_recent_index()
        index_sensor = data.get_most_recent_index(sensor_name=self.sensor_name)
        self.valueArray = data.get_values(self.sensor_name, index_sensor, self.graph_width)
        self.timeArray = data.get_values("time_internal_seconds", index_time, self.graph_width)
        self.plot.setData(self.timeArray, self.valueArray)

    def open_SettingsWindow(self):
        PlotSettingsDialog(self, self.embedLayout, self.sensor_name)

    def connectSignalSlots(self):
        self.button_settings.clicked.connect(partial(self.open_SettingsWindow, self))

    def saveSettings(self):
        pass

    def loadSettings(self):
        if self.configFile.value("yMin") == None:
            self.configFile.setValue("yMin","auto")
        if self.configFile.value("yMax") == None:
            self.configFile.setValue("yMax","auto")
        self.set_graphWidth(self.configFile.value("graph_width"))


        self.set_yMinMax(self.configFile.value("yMin"), self.configFile.value("yMax"))

    ## allow color scheme of class to be changed by CSS stylesheets
    def paintEvent(self, pe):
        opt = QtGui.QStyleOption()
        opt.initFrom(self)
        p = QtGui.QPainter(self)
        s = self.style()
        s.drawPrimitive(QtGui.QStyle.PE_Widget, opt, p, self)


uiSettingsDialog, _ = uic.loadUiType(os.path.join(os.path.dirname(__file__), 'plotSettings.ui'))  # loads the .ui file from QT Desginer
class PlotSettingsDialog(QtWidgets.QDialog, uiSettingsDialog):
    def __init__(self, parent, embedLayout, sensor_name):
        super().__init__()
        self.setupUi(self)
        self.parent = parent
        self.embedLayout = embedLayout
        self.sensor_name = sensor_name
        self.window().setWindowTitle(data.get_display_name(sensor_name) + " Plot Settings")
        self.connectSlotsSignals()
        self.reposition()


        self.configFile = QtCore.QSettings('DAATA_plot', self.sensor_name)
        self.loadSettings()
        self.parent.setStyleSheet(self.parent.stylesheetHighlight)
        returnValue = self.exec()

    def loadSettings(self):
        self.lineEdit_graph_width_seconds.setText(str(self.parent.graph_width/self.parent.samplingFreq))
        self.lineEdit_yMin.setText(self.configFile.value("yMin"))
        self.lineEdit_yMax.setText(self.configFile.value("yMax"))

    def applySettings(self):
        self.saveSettings()
        self.parent.set_yMinMax(self.configFile.value("yMin"),self.configFile.value("yMax"))
        self.parent.set_graphWidth(self.configFile.value("graph_width"))
        self.lineEdit_yMin.setText(self.configFile.value("yMin"))
        self.lineEdit_yMax.setText(self.configFile.value("yMax"))

    def saveSettings(self):
        self.configFile.setValue("graph_width", self.lineEdit_graph_width_seconds.text())
        self.configFile.setValue("yMin", self.lineEdit_yMin.text())
        self.configFile.setValue("yMax", self.lineEdit_yMax.text())

    # direction can be 'up','left','right','down'
    def sendMoveSignal(self, direction):
        self.embedLayout.moveWidget(self.parent, direction)

    def reposition(self, **kwargs):
        xOverride = kwargs.get("xOverride", 0)
        yOverride = kwargs.get("yOverride", 0)

        if xOverride == 0:
            x = self.embedLayout.parent().mapToGlobal(QtCore.QPoint(0, 0)).x() + self.embedLayout.parent().width() + 20
            # x = self.parent.mapToGlobal(QtCore.QPoint(0, 0)).x() + self.embedLayout.width()

        else:
            x = xOverride

        if yOverride == 0:
            y = self.embedLayout.parent().mapToGlobal(QtCore.QPoint(0, 0)).y() + self.embedLayout.parent().height()/3
        else:
            y = yOverride

        self.move(x,y)

    def resetYMax(self):
        self.parent.enable_autoRange(True)
        self.lineEdit_yMax.setText("auto")
        self.configFile.setValue("yMax", self.lineEdit_yMax.text())
        self.parent.set_yMinMax(self.configFile.value("yMin"),self.configFile.value("yMax"))

        # self.lineEdit_yMin.setText(self.configFile.value("yMin"))

    def resetYMin(self):
        self.parent.enable_autoRange(True)
        self.lineEdit_yMin.setText("auto")
        self.configFile.setValue("yMin", self.lineEdit_yMin.text())
        self.parent.set_yMinMax(self.configFile.value("yMin"),self.configFile.value("yMax"))

        # self.lineEdit_yMax.setText(self.configFile.value("yMax"))

    def connectSlotsSignals(self):
        self.pushButton_apply.clicked.connect(self.applySettings)
        self.button_moveDown.clicked.connect(partial(self.sendMoveSignal, 'down'))
        self.button_moveLeft.clicked.connect(partial(self.sendMoveSignal, 'left'))
        self.button_moveRight.clicked.connect(partial(self.sendMoveSignal, 'right'))
        self.button_moveUp.clicked.connect(partial(self.sendMoveSignal, 'up'))

        self.button_resetYMax.clicked.connect(self.resetYMax)
        self.button_resetYMin.clicked.connect(self.resetYMin)

    def closeEvent(self, e):
        self.parent.setStyleSheet(self.parent.stylesheetDefault)
        del self


class GridPlotLayout(QGridLayout):
    def __init__(self, parent = None):
        super(GridPlotLayout, self).__init__(parent)
        spacing = 6
        self.setContentsMargins(spacing,spacing,spacing,spacing)
        self.setSpacing(spacing)


    def moveWidget(self, widg, direction):
        oldRow = self.rowOf(widg)
        oldCol = self.colOf(widg)

        if 'up' == direction:
            newRow = oldRow - 1
            newCol = oldCol
        elif 'left' == direction:
            newRow = oldRow
            newCol = oldCol - 1
        elif 'right' == direction:
            newRow = oldRow
            newCol = oldCol+1
        elif 'down' == direction:
            newRow = oldRow + 1
            newCol = oldCol
        else:
            raise ValueError('moveWidget(self, widg, direction): argument 2 has invalid \'str\' value (up, left, right, down)')


        try:
            displacedWidg = self.widgetAtPosition(newRow,newCol)
            if displacedWidg == None:
                return AttributeError
            self.removeWidget(widg)
            self.removeWidget(displacedWidg)
            self.addWidget(widg, newRow, newCol)
            self.addWidget(displacedWidg, oldRow, oldCol)
        except AttributeError:
            pass

    def widgetAtPosition(self, row, col):
        return self.itemAtPosition(row, col).widget()

    def rowOf(self, widg):
        index = self.indexOf(widg)
        position = self.getItemPosition(index)
        return position[0]

    def colOf(self, widg):
        index = self.indexOf(widg)
        position = self.getItemPosition(index)
        return position[1]

    def rowSpanOf(self, widg):
        index = self.indexOf(widg)
        position = self.getItemPosition(index)
        return position[2]

    def colSpanOf(self, widg):
        index = self.indexOf(widg)
        position = self.getItemPosition(index)
        return position[3]