import time
import threading
from DataAcquisition.Data import Data
from DataAcquisition.DataImport import DataImport

data_collection_lock = threading.Lock()  # Creates a lock for data synchronization

# This is the main variable that can be accessed from other areas of the code. Use 'DataAcquisition.data'
data = Data(data_collection_lock)

# Set this to true if you want to test the code without having hardware connected
use_fake_inputs = True


def collect_data():
    from MainWindow import is_data_collecting
    data_import = DataImport(data, data_collection_lock, use_fake_inputs, is_data_collecting)
    while True:
        data.reset()
        data_import.check_connected()
        time.sleep(.1)
        while is_data_collecting.is_set():
            data_import.read_data()
            print("collecting data")
            time.sleep(.001)
