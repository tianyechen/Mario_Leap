import os
import stat
import errno
import posix

pipeName = "/tmp/testpipe"

try:
    os.mkfifo(pipeName)
except:
    pass


##using manual open and close
# fifo = open(pipeName, 'w')
# fifo.write("enter")
# fifo.write('\n')
# fifo.flush()
# fifo.close

##using with to open and close the file
with open(pipeName, 'w' ) as fifo:
    print("Igothere")
    while True:
        print("i'm open")
        command = input()
        print("got" + command)
        fifo.write(command)
        fifo.write('\n')
        fifo.flush()
        print("wrote")
        # print(command)
