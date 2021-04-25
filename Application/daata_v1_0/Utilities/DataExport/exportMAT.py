import numpy
import scipy.io as sio
import os
from DataAcquisition import data


def saveMAT(self, filename, directory):
    if filename == "":
        return
    if ".mat" not in filename:
        filename = filename + ".mat"


    dataDict = dict()
    dataDict['collected_data'] = dict()

    # connected_sensors = data.get_sensors(is_connected=True)
    sensorsList = ["time"]
    sensorsList = sensorsList + data.get_sensors(is_plottable=True, is_connected=True,is_derived=False)
    lastIndex = data.get_most_recent_index()

    for sensor in sensorsList:
        dataDict['collected_data'][sensor] = data.get_values(sensor, lastIndex, lastIndex+1)

    sio.savemat(os.path.join(directory, filename), dataDict, appendmat=True, oned_as="column")

if __name__ == "__main__":
    pass