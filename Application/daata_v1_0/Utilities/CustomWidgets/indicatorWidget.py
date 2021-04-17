from PyQt5 import QtCore
from PyQt5.QtWidgets import QCheckBox

class QIndicator(QCheckBox):
    def __init__(self, text, objectName):
        super().__init__(text, objectName = objectName)

        stylesheet = """
        QCheckBox::indicator {
            width: 13px;
            height: 13px;
            border: 1px solid black;
            }
            
            QCheckBox::indicator::unchecked {
            background-color : #d60000	/* red */
            
            
            }
            
            QCheckBox::indicator:unchecked:hover {
            background-color : #d60000	/* red */
            }
            
            QCheckBox::indicator:unchecked:pressed {
            background-color :  #d60000	/* red */
            }
            
            QCheckBox::indicator::checked {
            background-color : #16c916	/* dark green */
            }
            
            QCheckBox::indicator:checked:hover {
            background-color : #16c916	/* dark green */
            }
            
            QCheckBox::indicator:checked:pressed {
            background-color : #16c916	/* dark green */
            }
        """
        self.setStyleSheet(stylesheet)
        self.setAttribute(QtCore.Qt.WA_TransparentForMouseEvents)
        self.setFocusPolicy(QtCore.Qt.NoFocus)