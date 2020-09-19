import threading
n = 0

def printit():
    global n
    threading.Timer(1.0, printit).start()
    print("Hello, World!" + str(n))
    n+=1

printit()
