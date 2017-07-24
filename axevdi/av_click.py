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

BUDDY_PID = -1              # quit if the buddy pid is gone
F_EXIT = "/tmp/dadyisback"  # quit if the file is present
NSECS_DROP_ANCHOR = 10

def check_pid(pid):
    try:
        os.kill(pid, 0)
        return True
    except OSError, err:
        import errno
        return err.errno == errno.EPERM

def op_mouse(interval, count, outfile, delay, clearflag=False):
    from pymouse import PyMouse
    mouse = PyMouse()

    # remove F_EXIT
    if os.path.exists(F_EXIT):
        os.remove(F_EXIT)

    #
    # let user drop his anchor
    #
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

    #
    # delay for a while until guest is ready
    #
    if count == 0:
        return

    print "delay %d secs until my guest is ready ..." % delay
    time.sleep(delay)

    #
    # now repeat moving the mouse and click
    #
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
            print "Aha, DADY IS BACK, good bye ..."
            os.remove(F_EXIT)
            break

        if i >= count:
            print "Well, finished clicking %d times, good bye ..." % count
            break

        # abort if the buddy pid is not running
        pid = BUDDY_PID
        if pid != -1:
            if not check_pid(pid):
                print "I have to go as my buddy %d is gone, good bye ..." % pid
                break

        time.sleep(interval)

def usage(prog):
    sys.stderr.write("Usage: %s " % prog + \
                     "[-i interval] [-c count] [-o outfile] " + \
                     "[-d delay] [-B BuddyPID] [-F file2exit] [-C]\n")
    sys.stderr.write("e.g.   %s -i 10 -c 100 -o /tmp/fooXY -C\n" % prog)

def main(argc, argv):
    interval = 10   # every 10 secs to move the mouse then click
    count = -1      # almost endless loop
    outfile = ""    # file to save the pointXY where anchor is dropped
    delay = 1       # dealy N secs before moving mouse then click
    clearflag = False
    options, rargv = getopt.getopt(argv[1:],
            ":i:c:o:d:B:F:Ch",
            ["interval=", "count=", "outfile=",
            "delay=", "BuddyPID=", "file2exit", "clear", "help"])
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
        elif opt in ("-B", "--BuddyPID"):
            global BUDDY_PID
            BUDDY_PID = int(arg)
        elif opt in ("-F", "--file2exit"):
            global F_EXIT
            F_EXIT = arg
        else:
            usage(argv[0])
            return 1

    op_mouse(interval, count, outfile, delay, clearflag)
    return 0

if __name__ == '__main__':
    argc, argv = len(sys.argv), sys.argv
    sys.exit(main(argc, argv))
