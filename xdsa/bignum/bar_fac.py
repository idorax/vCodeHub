#!/usr/bin/python
#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

import sys

def str2hex(s):
    l = s.split(' ')

    i = len(l)
    out = ""
    while i > 0:
        i -= 1
        e = l[i]
        if e.startswith("0x"):
            e = e[2:]
        out += e

    out = "0x%s" % out
    n = eval("%s * %d" % (out, 0x1))
    return n

def main(argc, argv):
    if argc != 3:
        sys.stderr.write("Usage: %s <num> <exp>\n" % argv[0])
        return 1

    a = argv[1]
    b = argv[2]
    ax = str2hex(a)
    bx = str2hex(b)

    i = 1
    m = 1
    while i <= ax:
        m *= i
        i += 1

    print "Input : %s  " % b
    print "Output: 0x%x == 0x%x!" % (m, ax)
    print
    if m == bx:
        print "Result: PASS"
    else:
        print "Result: FAIL"

    return 0

if __name__ == '__main__':
    argv = sys.argv
    argc = len(argv)
    sys.exit(main(argc, argv))
