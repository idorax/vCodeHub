#!/usr/bin/python
#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

import sys
import os
import time

F_EXIT = "/tmp/dadyisback"
NSECS_DROP_ANCHOR = 10

def move_mouse_and_click(interval, ntimes):
    nsecs = int(interval) # every nsecs to move mouse then click
    count = int(ntimes)   # total times to click

    # clear the screen
    os.system("clear")

    # remove F_EXIT
    if os.path.exists(F_EXIT):
        os.remove(F_EXIT)

    # now repeat moving the mouse to keep alive
    from pymouse import PyMouse
    mouse = PyMouse()

    n = NSECS_DROP_ANCHOR
    print "please move the mouse to drop your anchor in %d secs ..." % n
    while n > 0:
        x, y = mouse.position()
        print "mouse is at point(%d, %d), %d secs left ..." % (x, y, n)
        time.sleep(1)
        n -= 1

    x, y = mouse.position()
    print "mouse is locked to point(%d, %d) ..." % (x, y)

    i = 0
    while True:
        i += 1
        print "%d: move mouse to point(%d, %d) and click, " % (i, x, y) + \
              "touch %s to abort me ..." % F_EXIT
        mouse.move(x, y)
        mouse.click(x, y)

        if os.path.exists(F_EXIT):
            print "Aha, dady is back, good bye ..."
            os.remove(F_EXIT)
            break

        if i >= count:
            print "Well, finished clicking %d times, good bye ..." % count
            break

        time.sleep(nsecs)

        # always clear the screen so as to catch rqcode png file at ease
        os.system("clear")

def main(argc, argv):
    if argc != 3:
        sys.stderr.write("Usage: %s <interval> <ntimes>\n" % argv[0])
        sys.stderr.write("e.g.   %s 10 100\n" % argv[0])
        return 1

    move_mouse_and_click(argv[1], argv[2])
    return 0

if __name__ == '__main__':
    argc, argv = len(sys.argv), sys.argv
    sys.exit(main(argc, argv))
