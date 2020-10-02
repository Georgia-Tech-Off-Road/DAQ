import logging
import sys, os
import pyqtgraph as pg
from PyQt5 import QtCore, QtWidgets, uic
import numpy
from PyQt5.QtWidgets import QGridLayout

from DataAcquisition import data

logger = logging.getLogger("Plotting")
uiFile, _ = uic.loadUiType(os.path.join(os.path.dirname(__file__), 'graphWidget.ui'))  # loads the .ui file from QT Desginer


class CustomPlotWidget(QtWidgets.QWidget, uiFile):
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

        # Number of value to show on the x_axis
        self.samplingFreq = 200
        self.graph_width = kwargs.get("graph_width_seconds", 10) * self.samplingFreq

        # Frequency of values to show (if set to 5 will show every 5th value collected)
        self.show_width = kwargs.get("show_width", 5)

        # Row and column span of plot on the grid
        self.rowSpan = kwargs.get("rowspan", 1)
        self.rowSpan = kwargs.get("columnspan", 1)


        self.valueArray = numpy.zeros(self.graph_width)
        self.plot = self.plotWidget.plot(self.valueArray, pen='b')
        self.initialCounter = 0


    def set_height(self, height):
        self.setMinimumSize(QtCore.QSize(200, height))
        self.setMaximumSize(QtCore.QSize(16777215, height))

    def update_graph(self):
        index = data.get_most_recent_index()
        self.valueArray = data.get_values(self.sensor_name, index, self.graph_width)
        self.timeArray = data.get_values("time", index, self.graph_width)
        self.plot.setData(self.timeArray, self.valueArray)



class GridPlotLayout(QGridLayout):
    def __init__(self, parent = None):
        super(GridPlotLayout,self).__init__(parent)
        self.setContentsMargins(6,2,6,0)


        # self.setSizePolicy(QtGui.QSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Expanding))

    # def itemIndex(self, item):
    #     for i in range(self.layout.count()):
    #         if self.layout.itemAt(i).graphicsItem() is item:
    #             return i
    #     raise Exception("Could not determine index of item " + str(item))


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