#!/usr/bin/python
#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

import sys
import os
import time

def op_mouse(x, y):
    from pymouse import PyMouse
    mouse = PyMouse()

    #print "move to point(%d, %d) and click ..." % (x, y)
    x0, y0 = mouse.position() # save the old position
    mouse.move(x, y)
    mouse.click(x, y)
    mouse.move(x0, y0)        # move back
    #print "done"

def main(argc, argv):
    if argc != 3:
        sys.stderr.write("Usage: %s <X> <Y>\n" % argv[0])
        return 1

    x = int(argv[1])
    y = int(argv[2])
    op_mouse(x, y)
    return 0

if __name__ == '__main__':
    argc, argv = len(sys.argv), sys.argv
    sys.exit(main(argc, argv))
