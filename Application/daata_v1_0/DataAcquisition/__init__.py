import serial
import time
from datetime import datetime


# This is the main variable that can be accessed from other areas of the code. Use 'DataAcquisition.data'
# Remember to acquire the data_collection_lock before reading this or values are subject to change
# partially through reading the data. Use 'with data_collection_lock:'
data = {
    'unix_time': {
        'values': [],
        'unit': 'Seconds',
        'unit_short': 's',
        'is_plottable': False,
        'is_external': False
    },
    'date_time': {
        'values': [],
        'display_name': 'Timestamp',
        'is_plottable': False,
        'is_external': False
    },
    'time_since_start': {
        'values': [],
        'display_name': 'Elapsed Time',
        'unit': 'Microseconds',
        'unit_short': 'us',
        'is_plottable': False,
        'is_external': True
    },
    'engine_speed': {
        'values': [],
        'display_name': 'Engine Speed',
        'unit_short': 'RPM',
        'is_plottable': True,
        'is_external': True
    },
    'fl_lds': {
        'values': [],
        'display_name': 'Front Left LDS',
        'unit_short': 'mm',
        'is_plottable': True,
        'is_external': True
    }
}

temp_data = {}
for item in data.keys():
    if data[item]['is_external']:
        temp_data[item] = {'value': None, 'has_been_updated': False, 'is_used': False}


def collect_data():
    from MainWindow import data_collection_lock
    _update_data(data_collection_lock, reset=True)
    while True:
        _update_data(data_collection_lock)
        print("collecting data")
        time.sleep(1)


def _update_data(lock, reset=False):
    with lock:
        if reset:
            for data_item in data.keys():
                data[data_item]['values'] = None
            data['unix_time']['values'] = []
            data['date_time']['values'] = []
        else:
            data['unix_time']['values'].append(time.time())
            data['date_time']['values'].append(datetime.date())
            for data_item in temp_data.keys():
                if temp_data[data_item]['is_used']:
                    if data[data_item]['values'] is None:
                        data[data_item]['values'] = []
                    data[data_item]['values'].append(temp_data[data_item]['value'])



