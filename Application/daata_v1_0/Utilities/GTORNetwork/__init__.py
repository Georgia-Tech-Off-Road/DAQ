# import os, string
# available_drives = ['%s:' % d for d in string.ascii_uppercase if os.path.exists('%s:' % d)]
# print(available_drives)
from pywin32 import win32wnet
import sys

print(win32wnet.WNetGetUniversalName(sys.argv[1], 1))

# drive_letters = 'ABCDEFGHIJKLMNOPQRSTUVWYXZ'
# for char in drive_letters:
#     print(char)



###################### Experimental auto-fetch  of GTOR network drive
# import os
# import multiprocessing
#
#
# drive_possibilities = '['A', 'B', 'C', 'D', 'Z']'
# index = -1
# available_drives = list()
#
#
# def check_letter(indexer):
#     global drive_possibilities
#     global available_drives
#     print(indexer)
#     print("Checking: ", f'{drive_possibilities[indexer]}:')
#     if os.path.exists(f'{drive_possibilities[indexer]}:'):
#         print("yay")
#         available_drives.append(f'{drive_possibilities[indexer]}:')
#
#
# if __name__ == "__main__":
#     for i in range(len(drive_possibilities)):
#         index = index + 1
#         print(index)
#         check = multiprocessing.Process(target=check_letter, args=[index])
#         check.start()
#         check.join(0.5)
#         if check.is_alive():
#             print("killing check")
#             check.terminate()
#     print(available_drives)