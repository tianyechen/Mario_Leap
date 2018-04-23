################################################################################
# Copyright (C) 2012-2013 Leap Motion, Inc. All rights reserved.               #
# Leap Motion proprietary and confidential. Not for distribution.              #
# Use subject to the terms of the Leap Motion SDK Agreement available at       #
# https://developer.leapmotion.com/sdk_agreement, or another agreement         #
# between Leap Motion and you, your company or other organization.             #
################################################################################

import sys
sys.path.insert(0, "/home/tianyec/Documents/6.835/final_project/jank_mario/leap/LeapSDK/lib/x64")


import Leap, thread, time


from Leap import CircleGesture, KeyTapGesture, ScreenTapGesture, SwipeGesture
from MarioTimer import MarioTimer
import os
import stat
import errno
import posix


class SampleListener(Leap.Listener):
    finger_names = ['Thumb', 'Index', 'Middle', 'Ring', 'Pinky']
    bone_names = ['Metacarpal', 'Proximal', 'Intermediate', 'Distal']
    state_names = ['STATE_INVALID', 'STATE_START', 'STATE_UPDATE', 'STATE_END']
    is_running = False
    is_jumping = False
    run_timer = None
    jump_timer = None
    pipeName = "/tmp/testpipe"
    fifo = open(pipeName, 'w')

    ##value needs to be a string
    def write_to_pipe(self, value):
        self.fifo.write(value)
        self.fifo.write('\n')
        self.fifo.flush()
        print "wrote %s to pipe" %(value)

    def stop_mario(self):
        print "stop"
        self.is_running = False
        self.write_to_pipe('s')

    def stop_jump(self):
        print "stop"
        # print "pipe two ss"
        self.is_jumping = False
        self.write_to_pipe('s')
        # self.write_to_pipe('s')


    def on_init(self, controller):
        print "Initialized"
        try:
            os.mkfifo(pipeName)
        except:
            pass

    def on_connect(self, controller):
        print "Connected"

        # Enable gestures
        # controller.enable_gesture(Leap.Gesture.TYPE_CIRCLE);
        controller.enable_gesture(Leap.Gesture.TYPE_KEY_TAP);
        # controller.enable_gesture(Leap.Gesture.TYPE_SCREEN_TAP);
        controller.enable_gesture(Leap.Gesture.TYPE_SWIPE);

    def on_disconnect(self, controller):
        # Note: not dispatched when running in a debugger.
        print "Disconnected"

    def on_exit(self, controller):
        self.fifo.close
        print "Exited"

    def on_frame(self, controller):
        # Get the most recent frame and report some basic information
        frame = controller.frame()

        # print "Frame id: %d, timestamp: %d, hands: %d, fingers: %d, tools: %d, gestures: %d" % (
        #       frame.id, frame.timestamp, len(frame.hands), len(frame.fingers), len(frame.tools), len(frame.gestures()))

        # Get hands
        # for hand in frame.hands:
        #
        #     handType = "Left hand" if hand.is_left else "Right hand"
        #
        #     print "  %s, id %d, position: %s" % (
        #         handType, hand.id, hand.palm_position)
        # Get gestures
        for gesture in frame.gestures():
            for hand in frame.hands:
                handType = "Left hand" if hand.is_left else "Right hand"

                # print "  %s, id %d, position: %s" % (
                #     handType, hand.id, hand.palm_position)

            if gesture.type == Leap.Gesture.TYPE_SWIPE:
                if not self.is_jumping:
                    print "jump"
                    self.is_jumping = True
                    self.jump_timer = MarioTimer(.01, self.stop_jump)
                    self.jump_timer.start()
                    self.write_to_pipe('b')
                else:
                    self.jump_timer.reset()
                swipe = SwipeGesture(gesture)
                print "Swipe id: %d, direction: %s" % (
                        gesture.id,swipe.direction)
                # print "Swipe id: %d, state: %s, position: %s, direction: %s, speed: %f" % (
                #         gesture.id, self.state_names[gesture.state],
                #         swipe.position, swipe.direction, swipe.speed)

            if gesture.type == Leap.Gesture.TYPE_KEY_TAP:
                if not self.is_running:
                    if handType == "Right hand":
                        print "run right"
                        self.write_to_pipe('d')
                    else:
                        print "run_left"
                        self.write_to_pipe('a')
                    self.is_running = True
                    self.run_timer = MarioTimer(.01, self.stop_mario)
                    self.run_timer.start()
                else:
                    self.run_timer.reset()
                keytap = KeyTapGesture(gesture)
                print "Hand Type: %s, Key Tap id: %d" % (
                        handType, gesture.id)

                # print "Hand Type: %s, Key Tap id: %d, %s, position: %s, direction: %s" % (
                #         handType, gesture.id, self.state_names[gesture.state],
                #         keytap.position, keytap.direction )

            # if gesture.type == Leap.Gesture.TYPE_SCREEN_TAP:
            #     screentap = ScreenTapGesture(gesture)
            #     print "  Screen Tap id: %d, %s, position: %s, direction: %s" % (
            #             gesture.id, self.state_names[gesture.state],
            #             screentap.position, screentap.direction )

        # if not (frame.hands.is_empty and frame.gestures().is_empty):
        #     print ""

    def state_string(self, state):
        if state == Leap.Gesture.STATE_START:
            return "STATE_START"

        if state == Leap.Gesture.STATE_UPDATE:
            return "STATE_UPDATE"

        if state == Leap.Gesture.STATE_STOP:
            return "STATE_STOP"

        if state == Leap.Gesture.STATE_INVALID:
            return "STATE_INVALID"

def main():
    # Create a sample listener and controller
    listener = SampleListener()
    controller = Leap.Controller()

    # Have the sample listener receive events from the controller
    controller.add_listener(listener)

    # Keep this process running until Enter is pressed
    print "Press Enter to quit..."
    try:
        sys.stdin.readline()
    except KeyboardInterrupt:
        pass
    finally:
        # Remove the sample listener when done
        controller.remove_listener(listener)


if __name__ == "__main__":
    main()
