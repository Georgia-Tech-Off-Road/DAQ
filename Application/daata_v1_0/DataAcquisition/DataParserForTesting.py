import xlwt
from xlwt import Workbook
import matplotlib.pylab as plt
from pathlib import Path
import csv
from codecs import decode
import struct
import serial
from serial.tools import list_ports

# print(list(list_ports.comports()))

"""
Data parser for sensor data from wireless communication utility.
Data is stored in a CSV file that can be accessed via Excel.

version: wirelessTesting
"""

# Enter in the bin file using it's absolute directory as the first paramater of this declaration
# dataFile = open("C:/Users/user/Documents/GitHub/DAQ/Application/daata_v1_0/DataAcquisition/DYNO3.BIN", mode="rb")

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

#teensy_port = get_Teensy_Port_V2()
teensy_ser = serial.Serial(port='COM3', baudrate=115200)
teensy_found = False

def get_Teensy_port(self):
        # Teensy USB serial microcontroller program id data:
        vendor_id = "16C0"
        product_id = "0483"
        serial_number = "12345"

        for port in list(list_ports.comports()):
            if port[2] == "USB VID:PID=%s:%s SNR=%s"%(vendor_id, product_id, serial_number):
                return port[0]

def get_Teensy_Port_V2():
        """Discover where is Teensy."""
        ports_avaiable = list(list_ports.comports())
        #teensy_port = tuple()
        for port in ports_avaiable:
            if port[1].startswith("USB Serial Device"):
                teensy_port = port
        if teensy_port:
            return teensy_port

#print(get_Teensy_Port_V2)

teensy_ser.reset_input_buffer()

bytesRead = teensy_ser.read(100)
#print(bytesRead)
dataRecord = list(bytesRead)
#print(len(dataRecord))
bytesInBuffer = 0
#print(bytesInBuffer)
numBytesToRead = 0

# The CSV file that's written to while the data is recorded
csvFile = open('C:/Users/Benjamin/GitHub/DAQ/Application/daata_v1_0/DataAcquisition/DataRecord.csv', 'w')

csvWriter = csv.writer(csvFile)

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
#print(endCode)

def float_to_bin(num):
    return bin(struct.unpack('!I', struct.pack('!f', num))[0])[2:].zfill(32)

def bin_to_float(binary):
    return struct.unpack('!f',struct.pack('!I', int(binary, 2)))[0]


settingsReceived = False

#while byteIndex < len(dataRecord):	
while(teensy_ser.is_open):
	if settingsReceived == False:
		teensy_ser.write(b'\x00\xff\xff\xff\xff\xff\xff\xff\xf0')

	if dataRecord[byteIndex] == 0x00 or dataRecord[byteIndex] == 0x01:
		#print("settings being parsed")
		settingsReceived = True
		keyIndex = 0
		packetCount = 0
		csvList.clear()
		dataSize.clear()
		DataDict.clear()
		keyList.clear()
		numBytesToRead = 0
		
		send = ((0x01 << 64) | 0xfffffffffffffff0)
		#print(send)
		sendbin = bin(send)
		#bytes([int(i) for i in send])
		#print(bytes(sendbin.encode()))
		#print(sendbin.encode())
		teensy_ser.write(b'\x03\xff\xff\xff\xff\xff\xff\xff\xf0')

		tempBytes = dataRecord[(byteIndex):(byteIndex + 8)]
		#print(tempBytes)
		byteIndex += 1
		while not(tempBytes == endCode):
			tempBytes.clear()
			DataDict[((dataRecord[byteIndex + 1]) << 8) |
				 dataRecord[byteIndex]] = dataRecord[byteIndex + 2]
			byteIndex += 3
			tempBytes = dataRecord[(byteIndex):(byteIndex + 8)]
			#print(tempBytes)
			#print(endCode == tempBytes)
		byteIndex += 8

		#currentSheet = dataSheet.add_sheet('Sheet ' + str(sheetCounter), cell_overwrite_ok=True)

		keyList = list(DataDict.keys())
		for i in range(len(keyList)):
			dataSize[keyList[keyIndex]] = DataDict[keyList[keyIndex]]
			numBytesToRead += DataDict[keyList[keyIndex]]
			DataDict[keyList[keyIndex]] = []
			csvList.append(keyList[keyIndex])
			#currentSheet.write(0, keyIndex, keyList[keyIndex])
			keyIndex += 1
		sheetCounter += 1
		csvWriter.writerow(csvList)
		#print(dataSize.keys())
		
		dataRecord.clear()
		byteIndex = 0
		bytesInBuffer = teensy_ser.in_waiting
		print(bytesInBuffer)
		currentPacket = teensy_ser.read(bytesInBuffer)
		print(currentPacket)
		dataRecord = list(currentPacket)

		numBytesToRead += 8
		print(numBytesToRead)

		#teensy_ser.write(b'\x03\xff\xff\xff\xff\xff\xff\xff\xf0')
		
	if dataRecord[byteIndex] == 0x02 or dataRecord[byteIndex] == 0x03:
		print("hello you made it")
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

	if len(dataRecord) - byteIndex < numBytesToRead:
		dataRecord.clear()
		byteIndex = 0
		currentPacket = teensy_ser.read(100)
		print(currentPacket)
		dataRecord = list(currentPacket)

	if packetCount > 10:
		print("closed")
		teensy_ser.close()

	 

	

# Saves the data in excel form to "example.xls" in the root 
# of the directory (most likely inside \DAQ folder)
	#dataSheet.save('example.xls')

#dataFile.close()

# Optional plotting of the data after the most recent settings are sent
"""
plotList = sorted(DataDict.items())
x, y = zip(*plotList)

plt.plot(x, y)
plt.show()

"""
