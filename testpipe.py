import os
import stat
import errno
import posix

pipeName = "/tmp/testpipe"

try:
    os.mkfifo(pipeName)
except:
    pass
#
#
# try:
#     w = os.open(pipeName, os.O_WRONLY)
#     print(w)
# except OSError as ex:
#     print ("fuck ")
#     if ex.errno == errno.ENXIO:
#         pass
#
################3
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
