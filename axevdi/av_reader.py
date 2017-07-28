#!/usr/bin/python
#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

import sys
import time

def hex2b64str(s):
    i = 0
    n = len(s)
    out = ""
    while i < n:
        e = s[i:i+2]
        out += chr(int(e, 16))
        i += 2
    return out

def main(argc, argv):
    if argc != 2:
        sys.stderr.write("Usage: %s <hex-text-file>\n" % argv[0])
        return 1

    #
    # The format of per line in the hex-text-file looks like:
    #     <lineno>   <hex-text>, e.g.
    #     0000000001 49776f6a49454e7663486c796157646f64
    #
    hexfile = argv[1]

    with open(hexfile, 'r') as f:
        for line in f.readlines():
            l0 = line.rstrip().split(' ')
            lineno  = l0[0]
            hextext = l0[-1]
            if len(hextext) == 0:
                continue

            s = hex2b64str(hextext)
            print "%s %s" % (lineno, s)

    return 0

if __name__ == '__main__':
    argv = sys.argv
    argc = len(argv)
    sys.exit(main(argc, argv))
