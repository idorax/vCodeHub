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

def hex2str(n):
    s_hex = "%x" % n
    if s_hex.startswith("0x"):
        s_hex = s_hex[2:]

    n = len(s_hex)
    m = n % 8
    if m != 0:
        s_hex = '0' * (8 - m) + s_hex
        n += (8 - m)
    i = n
    l = []
    while i >= 8:
        l.append('0x' + s_hex[i-8:i])
        i -= 8
    return "%s" % ' '.join(l)

def main(argc, argv):
    if argc != 4:
        sys.stderr.write("Usage: %s <a> <b> <c>\n" % argv[0])
        return 1

    a = argv[1]
    b = argv[2]
    c = argv[3]
    ax = str2hex(a)
    bx = str2hex(b)
    cx = str2hex(c)

    axbx = ax * bx
    if axbx != cx:
        print "0x%x * 0x%x = " % (ax, bx)
        print "got: 0x%x" % axbx
        print "exp: 0x%x" % cx
        print "res: FAIL"
        return 1

    print "got: %s" % hex2str(axbx)
    print "exp: %s" % c
    print "res: PASS"
    return 0

if __name__ == '__main__':
    argv = sys.argv
    argc = len(argv)
    sys.exit(main(argc, argv))
