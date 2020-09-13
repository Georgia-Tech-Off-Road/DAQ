import logging
import sys
import pyqtgraph as pg
from PyQt5 import QtCore, QtWidgets
import numpy

from DataAcquisition import data

logger = logging.getLogger("Plotting")


class RealTimePlot(pg.PlotWidget):
    def __init__(self, sensor_name, parent=None, **kwargs):
        super().__init__(parent)
        self.sensor_name = sensor_name

        self.setMinimumSize(QtCore.QSize(0, 400))
        self.setMaximumSize(QtCore.QSize(16777215, 400))
        self.setMouseEnabled(False, False)  # disable mouse-scroll zooming on the graph
        self.setLabels(left=str(data.get_display_name(sensor_name))+" ("+str(data.get_unit_short(sensor_name))+")",
                       bottom='Time (s)',
                       title=str(data.get_display_name(sensor_name)) + ' Graph')  # set title and axes
        self.showGrid(x=True, y=True, alpha=.2)

        # Number of value to show on the x_axis
        self.samplingFreq = 400
        self.graph_width = kwargs.get("graph_width_seconds", 10) * self.samplingFreq

        # Frequency of values to show (if set to 5 will show every 5th value collected)
        self.show_width = kwargs.get("show_width", 5)


        self.valueArray = numpy.zeros(self.graph_width)
        self.curve = self.plot(self.valueArray, pen='b')
        self.initialCounter = 0

        self.show()

    def update_graph(self):
        index = data.get_most_recent_index()
        self.valueArray = data.get_values(self.sensor_name, index, self.graph_width)
        self.timeArray = data.get_values("time", index, self.graph_width)
        self.curve.setData(self.timeArray, self.valueArray)


if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    widget = QtWidgets.QWidget()
    ui = RealTimePlot("Engine Speed")
    ui.show()
    ui.update_graph()
    sys.exit(app.exec_())


