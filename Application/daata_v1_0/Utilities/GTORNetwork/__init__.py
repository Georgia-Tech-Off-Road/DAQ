














######################## Experimental auto-fetch  of GTOR network drive
# import os
# import multiprocessing
#
#
# drive_possibilities = ['A', 'B', 'C', 'D', 'Z']
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