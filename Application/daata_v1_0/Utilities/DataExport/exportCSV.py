import csv
import os
from DataAcquisition import data



def saveCSV(self, filename, directory):
    # with open(filename, 'w', newline='') as csvfile:

    #TODO add smarter functionality to automatically make it a csv file
    if ".csv" not in filename:
        filename = filename + ".csv"

    csvfile = open(os.path.join(directory, filename), 'w')
    writer = csv.writer(csvfile, dialect='excel', lineterminator = '\n')

    # connected_sensors = data.get_sensors(is_connected=True)
    sensorsList = data.get_sensors(is_plottable=True, is_connected=True,is_derived=False)
    print(sensorsList)
    index = 0
    # lastIndex = data.get_most_recent_index()
    lastIndex = data.get_most_recent_index()

    rowData = list()
    rowData.append(data.get_display_name('time'))
    for sensor in sensorsList:
        rowData.append(data.get_display_name(sensor))
    writer.writerow(rowData)


    while (index <=lastIndex):
        print(index)
        rowData = list()
        rowData.append(data.get_value('time',index))
        for sensor in sensorsList:
            rowData.append(data.get_value(sensor,index))
        print(data.get_value('time',index))
        writer.writerow(rowData)
        index += 1


    csvfile.close()

if __name__ == "__main__":
    saveCSV("testcsv.csv","C:/Users/Ryan/Downloads")