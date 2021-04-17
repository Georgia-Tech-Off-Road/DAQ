import csv
import os
from DataAcquisition import data



def saveCSV(self, filename, directory):
    # with open(filename, 'w', newline='') as csvfile:

    #TODO add smarter functionality to automatically make it a csv file
    if filename == "":
        return
    if ".csv" not in filename:
        filename = filename + ".csv"


    csvfile = open(os.path.join(directory, filename), 'w')
    writer = csv.writer(csvfile, dialect='excel', lineterminator = '\n')



    # connected_sensors = data.get_sensors(is_connected=True)
    sensorsList = ["time"]
    sensorsList = sensorsList + data.get_sensors(is_plottable=True, is_connected=True,is_derived=False)

    lastIndex = data.get_most_recent_index()
    print(sensorsList)
    sensorData = list()

    for index, sensor in enumerate(sensorsList):
        row = [sensor] + data.get_values(sensor, lastIndex, lastIndex+1)
        sensorData.append(row)
        writer.writerow(sensorData[index])

    # for sensor in sensorsList:
    #
    #
    # while (index <=lastIndex):
    #     print(index)
    #     rowData = list()
    #     rowData.append(data.get_value('time',index))
    #     for sensor in sensorsList:
    #         rowData.append(data.get_value(sensor,index))
    #     print(data.get_value('time',index))
    #     writer.writerow(rowData)
    #     index += 1


    csvfile.close()

if __name__ == "__main__":
    pass