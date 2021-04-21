import DaataLogging

import sys
from PyQt5.QtWidgets import QApplication

from MainWindow import MainWindow


app = QApplication(sys.argv)
daata = MainWindow()
daata.show()
sys.exit(app.exec_())


