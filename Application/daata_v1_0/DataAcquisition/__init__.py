import time
import threading
import logging
from DataAcquisition.Data import Data
from DataAcquisition.DataImport import DataImport

logger = logging.getLogger("DataAcquisition")

data_collection_lock = threading.Lock()  # Creates a lock for data synchronization

# This is the main variable that can be accessed from other areas of the code. Use 'DataAcquisition.data'
data = Data(data_collection_lock)

# Set this to true if you want to test the code without having hardware connected
use_fake_inputs = True


def collect_data():
    from MainWindow import is_data_collecting
    data_import = DataImport(data, data_collection_lock, use_fake_inputs, is_data_collecting)
    logger.debug("Running collect_data")
    while True:
        data_import.check_connected()
        time.sleep(.1)
        if is_data_collecting.is_set():
            data.reset()
            logger.debug("Collecting data")
            while is_data_collecting.is_set():
                data_import.update()
                time.sleep(.0001)
