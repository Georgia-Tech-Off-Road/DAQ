from PyQt5 import QtWidgets
from abc import ABCMeta, abstractmethod


class DAATALayout(QtWidgets.QWidget):
    def __init__(self, **kwargs):
        super().__init__()
        __metaclass__ = ABCMeta

        self.updateFreq = 1        # refresh 1 time a second

    @abstractmethod
    def update(self):
        raise NotImplementedError






