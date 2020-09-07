import logging
import sys
import pyqtgraph as pg
from PyQt5 import QtCore, QtWidgets

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

        # Number of values to show on the x_axis
        self.graph_width = kwargs.get("graph_width", 200)
        # Frequency of values to show (if set to 5 will show every 5th value collected)
        self.show_width = kwargs.get("show_width", 5)

        self.curve = self.plot(pen='b')
        self.show()

    def update_graph(self):
        index = data.get_most_recent_index()
        self.plot(data.get_values("time", index, self.graph_width),
                  data.get_values(self.sensor_name, index, self.graph_width),
                  pen='b')
        self.setXRange(max(data.get_value("time", index) - 10, 0), max(data.get_value("time", index), 0))


if __name__ == "__main__":
    app = QtWidgets.QApplication(sys.argv)
    widget = QtWidgets.QWidget()
    ui = RealTimePlot("Engine Speed")
    ui.show()
    ui.update_graph()
    sys.exit(app.exec_())


