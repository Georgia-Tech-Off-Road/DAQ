

# available_drives = ['%s:' % d for d in string.ascii_uppercase if os.directory.exists('%s:' % d)]
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
import ctypes
import ctypes.util

from func_timeout import func_set_timeout, func_timeout, FunctionTimedOut
import logging
logger = logging.getLogger("GTORNetwork")


def find_GTORNetworkDrive():
    alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWYXZ'
    drive_possibilities = list()
    for letter in alphabet:
        drive = '{}:\\'.format(letter)
        drive_possibilities.append(drive)
    networkDrive = ''

    for drive in drive_possibilities:
        try:
            networkDrive = check_driveLetter(drive)
        except FunctionTimedOut:
            errorMsg = 'DriveConnectionError: {} drive may exist but is not connected, try reconnecting {} drive by opening network drive folder'.format(drive,drive)
            logger.error(errorMsg)


    if networkDrive == '':
        logger.error("DriveConnectionError: Network drive cannot be found")
        return ''
    else:
        return networkDrive


@func_set_timeout(.05)
def check_driveLetter(drive):
    if os.path.exists(drive):
        drive_info = win32api.GetVolumeInformation(drive)
        print(drive_info)
        if drive_info[0] == 'Data':
            return drive
            print("Network drive is {}".format(drive))
        else:
            return ''
    else:
        return ''


if __name__ == "__main__":
    something = find_GTORNetworkDrive()
    print(something)