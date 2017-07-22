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

def move_mouse(pointTL, pointRB, interval):
    x1 = int(pointTL[0])
    y1 = int(pointTL[1])
    x2 = int(pointRB[0])
    y2 = int(pointRB[1])
    nsecs = int(interval)

    # clear the screen
    os.system("clear")

    # remove F_EXIT
    if os.path.exists(F_EXIT):
        os.remove(F_EXIT)

    # now move the mouse to keep alive
    x, y = x1, y1
    from pymouse import PyMouse
    m = PyMouse()
    while True:
        print "move mouse to point(%d, %d), touch %s to abort me ..." % \
              (x, y, F_EXIT)
        m.position()
        m.move(x, y)
        m.click(x, y)
        x += 10
        y += 10

        if x >= x2:
            x = x2
        if y >= y2:
            y = y2

        if os.path.exists(F_EXIT):
            print "Aha, dady is back, good bye ..."
            os.remove(F_EXIT)
            break

        time.sleep(nsecs)

def main(argc, argv):
    if argc != 4:
        sys.stderr.write("Usage: %s <X1,Y1> <X2,Y2> <interval>\n" % argv[0])
        return 1

    move_mouse(argv[1].split(','), argv[2].split(','), argv[3])
    return 0

if __name__ == '__main__':
    argc, argv = len(sys.argv), sys.argv
    sys.exit(main(argc, argv))
