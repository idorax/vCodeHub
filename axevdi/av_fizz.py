#!/usr/bin/python
#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

#
# This utility is filed to activate desktop automatically because
# the stupid VDI desktop locks its screen every N minutes by force.
#
# NOTE: To use pymouse, packages in the following should be installed,
#       1. sudo apt-get install python-xlib
#       2. sudo apt-get install python-pip
#       3. sudo pip install pymouse
#

import sys
import os
import time

F_EXIT = "/tmp/dadyisback"
NSECS_DROP_ANCHOR = 10

def move_mouse(interval):
    nsecs = int(interval)

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

    x0, y0 = mouse.position()
    print "mouse is locked to point(%d, %d) ..." % (x0, y0)

    i = 0
    while True:
        i += 1
        if i % 2 == 0:
            x = x0 + 10
            y = y0 + 10
        else:
            x = x0 - 10
            y = y0 - 10

        if i >= 1024:
            i = 0

        print "move mouse to point(%d, %d), touch %s to abort me ..." % \
              (x, y, F_EXIT)
        mouse.move(x, y)
        mouse.click(x, y)

        if os.path.exists(F_EXIT):
            print "Aha, dady is back, good bye ..."
            os.remove(F_EXIT)
            break

        time.sleep(nsecs)

def main(argc, argv):
    if argc != 2:
        sys.stderr.write("Usage: %s <interval>\n" % argv[0])
        sys.stderr.write("e.g.   %s 10\n" % argv[0])
        return 1

    move_mouse(argv[1])
    return 0

if __name__ == '__main__':
    argc, argv = len(sys.argv), sys.argv
    sys.exit(main(argc, argv))
