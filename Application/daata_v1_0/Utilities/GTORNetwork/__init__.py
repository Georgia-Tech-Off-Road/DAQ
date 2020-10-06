# import os, string
# available_drives = ['%s:' % d for d in string.ascii_uppercase if os.path.exists('%s:' % d)]
# print(available_drives)
# from pywin32 import win32wnet
# import sys
#
# print(win32wnet.WNetGetUniversalName(sys.argv[1], 1))

# drive_letters = 'ABCDEFGHIJKLMNOPQRSTUVWYXZ'
# for char in drive_letters:
#     print(char)



##################### Experimental auto-fetch  of GTOR network drive
import os
import multiprocessing
import win32api     # installed with "pip3.6 install pywin32"

def find_GTORNetworkDrive():
    drive_possibilities = 'ABCDEFGHIJKLMNOPQRSTUVWYXZ'
    available_drives = list()
    for letter in drive_possibilities:

        print("Checking: ", f'{letter}:')
        if os.path.exists(f'{letter}:'):
            available_drives.append('{}:\\'.format(letter))
    print(available_drives)
    for drive in available_drives:
        drive_info = win32api.GetVolumeInformation(drive)
        print(drive_info)
        if drive_info[0] == 'Data':
            print("Network drive is {}".format(drive))


if __name__ == "__main__":
    find_GTORNetworkDrive()