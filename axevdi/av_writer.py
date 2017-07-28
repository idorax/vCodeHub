#!/usr/bin/python
#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

import sys
import time

def b64str2hex(s):
    out = ""
    for c in s:
        out += "%02x" % ord(c)
    return out

def main(argc, argv):
    if argc != 2:
        sys.stderr.write("Usage: %s <base64-text-file>\n" % argv[0])
        return 1

    b64file = argv[1]

    from pymouse import PyMouse
    mouse = PyMouse()

    n = 5
    print "please move the mouse to drop your anchor in %d secs ..." % n
    while n > 0:
        x, y = mouse.position()
        print "mouse is at point(%d, %d), %d secs left ..." % (x, y, n)
        time.sleep(1)
        n -= 1
    x, y = mouse.position()
    print "mouse is locked to point(%d, %d) ..." % (x, y)

    print "please move your mouse away if you'd like to in 5 secs ..."
    time.sleep(5)

    print "move mouse to point(%d, %d) and click ..." % (x, y)
    x0, y0 = mouse.position()
    mouse.move(x, y)
    mouse.click(x, y)

    print "start to write ..."
    import virtkey
    v = virtkey.virtkey()

    with open(b64file, 'r') as f:
        i = 0
        for line in f.readlines():
            i += 1
            s0 = b64str2hex(line.rstrip())
            print "%010d %s %s" % (i, line.rstrip(), s0)

            # input lineno
            s = "%010d " % i
            for c in s:
                v.press_unicode(ord(c))
                v.release_unicode(ord(c))
                time.sleep(0.02)

            # input line
            s = s0
            for c in s:
                v.press_unicode(ord(c))
                v.release_unicode(ord(c))
                time.sleep(0.02)

            # print '\n' (Enter)
            v.press_keysym(65421)
            v.release_keysym(65421)
            time.sleep(0.02)

    print "sleep 5 secs ..."
    time.sleep(5)

    print "move mouse back to (%d, %d)" % (x0, y0)
    mouse.move(x0, y0)

    print "good bye ..."

    return 0

if __name__ == '__main__':
    argv = sys.argv
    argc = len(argv)
    sys.exit(main(argc, argv))
