class DraggableImage(QtWidgets.QLabel):
    def __init__(self, imgPath=None, parent=None):
        super(DraggableImage, self).__init__()
        self.setParent(parent)
        self.setFixedSize(64, 64)
        self.setPixmap(QtGui.QPixmap(imgPath))
        self.drag_start_pos = None

    def mousePressEvent(self, event):
        if event.button() == QtCore.Qt.LeftButton:
            self.setCursor(QtCore.Qt.ClosedHandCursor)
            # This will give us the start position when a drag is triggered
            self.drag_start_pos = event.pos()
            self.raise_()
        super(DraggableImage, self).mousePressEvent(event)

    def mouseMoveEvent(self, event):
        if self.drag_start_pos is not None:
            # While left button is clicked the widget will move along with the mouse
            self.move(self.pos() + event.pos() - self.drag_start_pos)
        super(DraggableImage, self).mouseMoveEvent(event)

    def mouseReleaseEvent(self, event):
        self.setCursor(QtCore.Qt.ArrowCursor)
        self.drag_start_pos = None

        parent_layout = self.parent().mainLayout

        all_images = [parent_layout.itemAt(i).widget() for i in range(parent_layout.count())]

        # sort the list of widgets by their x position
        order = sorted(all_images, key=lambda i: i.pos().x())

        # remove each item from the layout and insert the one that should go in that index.
        for idx, widget in enumerate(order):
            parent_layout.takeAt(idx)
            parent_layout.insertWidget(idx, widget)

        super(DraggableImage, self).mouseReleaseEvent(event)


class MainWindow(QtWidgets.QWidget):
    def __init__(self):
        super(MainWindow, self).__init__()
        self.mainLayout = QtWidgets.QHBoxLayout()
        self.setLayout(self.mainLayout)
        self._imgList = [
            "dog.jpg",
            "cat.jpg",
            "mouse.jpg",
            "fish.jpg"
        ]

        for img in self._imgList:
            draggableImage = DraggableImage(imgPath=img, parent=self)
            self.mainLayout.addWidget(draggableImage)


if __name__ == '__main__':
    import sys

    app = QtWidgets.QApplication(sys.argv)
    win = MainWindow()
    win.show()
    sys.exit(app.exec_())