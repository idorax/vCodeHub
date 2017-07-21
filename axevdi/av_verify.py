#!/usr/bin/python
#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

import sys

def verify(l_lineno):
    max_lineno = 1
    for n in l_lineno:
        if n > max_lineno:
            max_lineno = n

    rc = 0
    i = 1
    while i <= max_lineno:
        if i not in l_lineno:
            sys.stderr.write("Oops, miss line %d\n" % i)
            rc += 1
        i += 1
    return rc

def main(argc, argv):
    if argc != 2:
        sys.stderr.write("Usage: %s <base64-text-file>\n" % argv[0])
        return 1

    base64textfile = argv[1]
    l_lineno = []
    with open(base64textfile, 'r') as f:
        for line in f.readlines():
            s0 = line.rstrip('\n')
            s1 = s0.split('\t')[0].strip().rstrip()
            n1 = int(s1)
            if n1 not in l_lineno:
                l_lineno.append(n1)

    rc = verify(l_lineno)

    return rc

if __name__ == '__main__':
    argc, argv = len(sys.argv), sys.argv
    sys.exit(main(argc, argv))
