import xlwt
from xlwt import Workbook
import matplotlib.pylab as plt

"""
Data parser for sensor data from wireless communication utility.
Data is stored in an excel spreadsheet.

version: 1.0
"""

#Enter in the bin file using it's absolute directory as the first paramater of this declaration
dataFile = open("C:/Users/user/Documents/GitHub/DAQ/Application/daata_v1_0/DataAcquisition/REALTEST.bin", mode="rb")

byte = dataFile.read(1)

dataRecord = []

dataSheet = Workbook()

#currentSheet = dataSheet.add_sheet('Sheet 1', cell_overwrite_ok=True)

sheetCounter = 1

DataDict = {}

byteIndex = 0

keyIndex = 0

keyList = []

packetCount = 0

dataSize = {}

endCode = [0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0]

while byte:
	dataRecord.append(int.from_bytes(byte, byteorder='big'))
	byte = dataFile.read(1)

while byteIndex < len(dataRecord):
#for i in range(len(dataRecord)):	
	if dataRecord[byteIndex] == 0x00:
		keyIndex = 0
		dataSize.clear()
		DataDict.clear()
		keyList.clear()
		currentSheet = dataSheet.add_sheet('Sheet ' + str(sheetCounter), cell_overwrite_ok=True)
		tempBytes = dataRecord[(byteIndex):(byteIndex + 8)]
		byteIndex += 1
		while tempBytes != endCode:
			tempBytes.clear()
			DataDict[((dataRecord[byteIndex + 1]) << 8) | dataRecord[byteIndex]] = dataRecord[byteIndex + 2]
			byteIndex += 3
			tempBytes = dataRecord[(byteIndex):(byteIndex + 8)]
		byteIndex += 8
		keyList = list(DataDict.keys())
		#while keyList[keyIndex]:
		for i in range(len(keyList)):
			dataSize[keyList[keyIndex]] = DataDict[keyList[keyIndex]]
			DataDict[keyList[keyIndex]] = []
			currentSheet.write(0, keyIndex, keyList[keyIndex])
			keyIndex += 1
		sheetCounter += 1

	if dataRecord[byteIndex] == 0x02:
		#byteIndex += 1
		keyIndex = 0
		#while keyList[keyIndex]:
		for i in range(len(keyList)):
			tempBytes = 0
			for i in range(0, dataSize[keyList[keyIndex]]):
				tempBytes = (tempBytes | (dataRecord[byteIndex + dataSize[keyList[keyIndex]] - i])) << ((dataSize[keyList[keyIndex]] - i - 1) * 8)
			DataDict[keyList[keyIndex]].append(tempBytes)
			currentSheet.write(packetCount + 1, keyIndex, tempBytes)
			byteIndex += dataSize[keyList[keyIndex]]
			keyIndex += 1
		byteIndex += 1
		packetCount += 1

	tempBytes = dataRecord[(byteIndex):(byteIndex + 8)]
	if tempBytes == endCode:
		byteIndex += 8

dataSheet.save('example.xls')
dataFile.close()

#Sorted list of dictionary pairs
plotList = sorted(DataDict.items())
x, y = zip(*plotList)

#plt.plot(x, y)
#plt.show()
