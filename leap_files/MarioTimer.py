from threading import Timer

def hello():
    print "hello"
class MarioTimer(object):

    def __init__(self, interval, f, *args, **kwargs):
        self.interval = interval
        self.f = f
        self.args = args
        self.kwargs = kwargs

        self.timer = None

    def callback(self):
        self.f(*self.args, **self.kwargs)
        # self.start()

    def cancel(self):
        self.timer.cancel()
        # print "canceled"

    def start(self):
        self.timer = Timer(self.interval, self.callback)
        self.timer.start()
        # print "timer started"

    def reset(self):
        # print "timer reset"
        self.cancel()
        self.start()

# t = RepeatingTimer(3, hello)
# print "hello"
# t.start()
