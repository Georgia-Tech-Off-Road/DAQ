import os
from win32 import win32api     # installed with "pip3.6 install pywin32"

from func_timeout import func_timeout, FunctionTimedOut
import datetime
import logging
logger = logging.getLogger("GTORNetwork")

def get_GTORNetworkDrive():
    try:
        return_value = func_timeout(.05, _get_GTORNetworkDrive)  # win32api will time out if a network drive is present but not accessible
        return return_value
    except FunctionTimedOut:
        logger.error('DriveConnectionError: GTOR Network Drive may exist but is not connected, try connecting to GaTech VPN or opening GTOR Network Drive')


def _get_GTORNetworkDrive():
    # create a list of all possible drive letter combinations
    alphabet = 'ABCDEFGHIJKLMNOPQRSTUVWYXZ'
    drive_possibilities = list()
    for letter in alphabet:
        drive = '{}:\\'.format(letter)
        drive_possibilities.append(drive)

    # check if the GTOR Network drive is present
    for drive in drive_possibilities:
        if os.path.exists(drive):
            drive_info = win32api.GetVolumeInformation(drive)
            logger.debug("Drive info: ({})".format(drive))
            if drive_info[0] == 'Data':         # GTOR Network drive has the name "Data"
                logger.debug("Network drive found ({})".format(drive))
                return drive

    logger.error("DriveConnectionError: Network drive cannot be found")
    return

def generate_data_save_location():
    return os.path.join(get_GTORNetworkDrive(), "DAATA Archive")

def generate_standardized_filename(filename,category = ""):
    now = datetime.datetime.today()
    date = now.strftime("%Y-%m-%d")
    list = [
        date,
        filename,
        category
    ]

    standardized_filename = ""

    for element in list:
        standardized_filename += element+"_"


    return standardized_filename[:-1]

if __name__ == "__main__":
    print(generate_standardized_filename("hello","tests"))
    pass