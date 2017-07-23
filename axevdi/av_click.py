#!/usr/bin/python
#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

#
# This utility is filed to move mouse and click automatically,
# and it can be used to activate desktop periodically as well.
#
# NOTE: To use pymouse, packages in the following should be installed,
#       1. sudo apt-get install python-xlib
#       2. sudo apt-get install python-pip
#       3. sudo pip install pymouse
#

import sys
import os
import getopt
import time

F_EXIT = "/tmp/dadyisback"
NSECS_DROP_ANCHOR = 10

def op_mouse(interval, count, outfile, delay, clearflag=False):
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

    if len(outfile) > 0:
        print "save mouse point(%d, %d) to file %s ..." % (x, y, outfile)
        os.system("echo %d,%d > %s" % (x, y, outfile))

    if count == 0:
        return

    print "delay %d secs until my guest is ready ..." % delay
    time.sleep(delay)

    if count == -1:
        count = 1 << 64

    i = 0
    while count > 0:
        # clear the screen so as to catch rqcode png file at ease
        if clearflag:
            os.system("clear")

        i += 1
        print "%d: move mouse to point(%d, %d) and click, " % (i, x, y) + \
              "touch %s to abort me ..." % F_EXIT
        x0, y0 = mouse.position() # save the old position
        mouse.move(x, y)
        mouse.click(x, y)
        mouse.move(x0, y0)        # move back

        if os.path.exists(F_EXIT):
            print "Aha, dady is back, good bye ..."
            os.remove(F_EXIT)
            break

        if i >= count:
            print "Well, finished clicking %d times, good bye ..." % count
            break

        time.sleep(interval)

def usage(prog):
    sys.stderr.write("Usage: %s " % prog + \
                     "[-i interval] [-c count] [-o outfile] [-C]\n")
    sys.stderr.write("e.g.   %s -i 10 -c 100 -o /tmp/fooXY -C\n" % prog)

def main(argc, argv):
    interval = 10   # every 10 secs to move the mouse then click
    count = -1      # almost endless loop
    outfile = ""    # file to save the pointXY where anchor is dropped
    delay = 1       # dealy N secs before moving mouse then click
    clearflag = False
    options, rargv = getopt.getopt(argv[1:],
            ":i:c:o:d:Ch",
            ["interval=", "count=", "outfile=", "delay", "clear", "help"])
    for opt, arg in options:
        if opt in ("-i", "--interval"):
            interval = int(arg)
        elif opt in ("-c", "--count"):
            count = int(arg)
        elif opt in ("-o", "--outfile"):
            outfile = arg
        elif opt in ("-d", "--delay"):
            delay = int(arg)
        elif opt in ("-C", "--clear"):
            clearflag = True
        else:
            usage(argv[0])
            return 1

    op_mouse(interval, count, outfile, delay, clearflag)
    return 0

if __name__ == '__main__':
    argc, argv = len(sys.argv), sys.argv
    sys.exit(main(argc, argv))
