import xlwt
from xlwt import Workbook
import matplotlib.pylab as plt
from pathlib import Path
import csv
from codecs import decode
import struct

"""
Data parser for sensor data from wireless communication utility.
Data is stored in a CSV file that can be accessed via Excel.

version: 1.1
"""

# Enter in the bin file using it's absolute directory as the first paramater of this declaration
dataFile = open("C:/Users/Benjamin/GitHub/DAQ/Application/daata_v1_0/DataAcquisition/TESTPETER.BIN.part", mode="rb")

# MISC testing
"""
testBytes = list(dataFile.read(8))
print(testBytes)
print(bin(testBytes[0]))

print(dataFile.read(1))
print(dataFile.read(1))

while True:
	if dataFile.read(1) == 0x04:
		print("Found it")

print(len(dataRecord))
print(dataRecord[0])
"""

bytes = dataFile.read()

dataRecord = list(bytes)

# The CSV file that's written to while the data is recorded
csvFile = open('C:/Users/Benjamin/GitHub/DAQ/Application/daata_v1_0/DataAcquisition/DataRecord.csv', 'w')

csvWriter = csv.writer(csvFile, lineterminator='\n')

csvList = []

imuValues = []

speedSensorValues = []

#dataSheet = Workbook()

#currentSheet = dataSheet.add_sheet('Sheet 1', cell_overwrite_ok=True)

sheetCounter = 1

DataDict = {}

byteIndex = 0

keyIndex = 0

keyList = []

packetCount = 0

dataSize = {}

endCode = [0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xf0]

def float_to_bin(num):
    return bin(struct.unpack('!I', struct.pack('!f', num))[0])[2:].zfill(32)

def bin_to_float(binary):
    return struct.unpack('!f',struct.pack('!I', int(binary, 2)))[0]

while byteIndex < len(dataRecord):	
	if dataRecord[byteIndex] == 0x00:
		keyIndex = 0
		packetCount = 0
		csvList.clear()
		dataSize.clear()
		DataDict.clear()
		keyList.clear()
		
		tempBytes = dataRecord[(byteIndex):(byteIndex + 8)]
		byteIndex += 1
		while tempBytes != endCode:
			tempBytes.clear()
			DataDict[((dataRecord[byteIndex + 1]) << 8) |
				 dataRecord[byteIndex]] = dataRecord[byteIndex + 2]
			byteIndex += 3
			tempBytes = dataRecord[(byteIndex):(byteIndex + 8)]
		byteIndex += 8

		#currentSheet = dataSheet.add_sheet('Sheet ' + str(sheetCounter), cell_overwrite_ok=True)

		keyList = list(DataDict.keys())
		for i in range(len(keyList)):
			dataSize[keyList[keyIndex]] = DataDict[keyList[keyIndex]]
			DataDict[keyList[keyIndex]] = []
			csvList.append(keyList[keyIndex])
			#currentSheet.write(0, keyIndex, keyList[keyIndex])
			keyIndex += 1
		sheetCounter += 1
		csvWriter.writerow(csvList)
		#print(dataSize.values())
		
	if dataRecord[byteIndex] == 0x02:
		csvList.clear()
		keyIndex = 0
		imuValues.clear()
		speedSensorValues.clear()

		for i in range(len(keyList)):
			tempBytes = 0
			if dataSize[keyList[keyIndex]] == 28:
				for j in range(7):
					tempBytes = 0
					for i in range(4):
						tempBytes = (tempBytes |
							 ((dataRecord[byteIndex + 4 - i + (j * 4)]) <<
							 	 ((4 - i - 1) * 8)))
					binOfTempBytes = bin(tempBytes)
					IEEEconvert = bin_to_float(binOfTempBytes)
					imuValues.append(IEEEconvert)
				csvList.append(imuValues)
				DataDict[keyList[keyIndex]].append(tempBytes)
			elif keyList[keyIndex] == 201:
				speedSensorValues.clear()
				"""
				for i in range(4):
					tempBytes = (tempBytes |
							((dataRecord[byteIndex + 4 - i]) <<
								((4 - i - 1) * 8)))
				speedSensorValues.append(tempBytes)
				"""
				#print(speedSensorValues)
				tempBytes = 0
				for i in range(2):
					tempBytes = (tempBytes |
							((dataRecord[byteIndex + 6 - i]) <<
								((2 - i - 1) * 8)))
				#speedSensorValues.append(tempBytes)
				#print(speedSensorValues)
				csvList.append(tempBytes)
				DataDict[keyList[keyIndex]].append(tempBytes)
			elif keyList[keyIndex] == 202:
				speedSensorValues.clear()
				"""
				for i in range(4):
					tempBytes = (tempBytes |
							((dataRecord[byteIndex + 4 - i]) <<
								((4 - i - 1) * 8)))
				speedSensorValues.append(tempBytes)
				"""
				#print(speedSensorValues)
				tempBytes = 0
				for i in range(2):
					tempBytes = (tempBytes |
							((dataRecord[byteIndex + 6 - i]) <<
								((2 - i - 1) * 8)))
				#speedSensorValues.append(tempBytes)
				csvList.append(tempBytes)
				#print(speedSensorValues)
				DataDict[keyList[keyIndex]].append(tempBytes)
			else:
				for i in range(0, dataSize[keyList[keyIndex]]):
					tempBytes = (tempBytes |
						 ((dataRecord[byteIndex + dataSize[keyList[keyIndex]] - i]) <<
						 	 ((dataSize[keyList[keyIndex]] - i - 1) * 8)))
				if keyList[keyIndex] == 212:
					binOfTempBytes = bin(tempBytes)
					IEEEconvert = bin_to_float(binOfTempBytes)
					#print(IEEEconvert)
					tempBytes = IEEEconvert
				csvList.append(tempBytes)
				DataDict[keyList[keyIndex]].append(tempBytes)
			#currentSheet.write(packetCount + 1, keyIndex, tempBytes)
			byteIndex += dataSize[keyList[keyIndex]]
			keyIndex += 1
		csvWriter.writerow(csvList)
		byteIndex += 1
		packetCount += 1

	tempBytes = dataRecord[(byteIndex) : (byteIndex + 8)]
	if tempBytes == endCode:
		byteIndex += 8 

# Saves the data in excel form to "example.xls" in the root 
# of the directory (most likely inside \DAQ folder)
	#dataSheet.save('example.xls')

dataFile.close()

# Optional plotting of the data after the most recent settings are sent
"""
plotList = sorted(DataDict.items())
x, y = zip(*plotList)

plt.plot(x, y)
plt.show()

"""
