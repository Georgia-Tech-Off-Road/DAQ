#!/usr/bin/env python3
"""
Georgia Institute of Technology - CS1301
HW07 - File I/O
"""

__author__ = """Andrew Hellrigel"""
__collab__ = """I worked on the homework assignment alone, using only this semester's course materials and the python 3.3.7 documentation"""



##########################
print("In Function")

f = open("TEST.txt", "r")
while True:
    data = f.readline()
    print(data)
f.close()


##########################

