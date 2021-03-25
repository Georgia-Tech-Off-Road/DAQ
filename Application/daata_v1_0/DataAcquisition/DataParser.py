import xlwt
from xlwt import Workbook

"""
Data parser for sensor data from wireless communication utility.
Data is stored in an excel spreadsheet.

version: 1.0
"""

dataFile = open("sample.bin", "rb")

byte = dataFile.read(1)

dataSheet = Workbook()

sheet1 = dataSheet.add_sheet('Sheet 1')

while byte:


    byte = dataFile.read(1)
