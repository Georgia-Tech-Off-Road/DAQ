import xlwt
from xlwt import Workbook

"""
Data parser for sensor data from wireless communication utility.
Data is stored in an excel spreadsheet.

version: 1.0
"""

dataFile = open("TEST1.bin", "rb")

byte = dataFile.read(1)

dataSheet = Workbook()

sheet1 = dataSheet.add_sheet('Sheet 1')

DataDict = {}

while byte:
	print(byte)
	byte = datafile.read(1)

""" while byte:
	sensorID = '0'
	if byte == '1':
		while byte == '1':
			i = 0
			DataDict.add(byte)
			sheet1.write(0, i, byte)
			i += 1
			byte = dataFile.read(1)
	sensorID += 1
    byte = dataFile.read(1) """
	