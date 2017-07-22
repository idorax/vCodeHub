#!/usr/bin/python
#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

import sys

def main(argc, argv):
    if argc != 2:
        sys.stderr.write("Usage: %s <base64-text-file>\n" % argv[0])
        return 1

    base64textfile = argv[1]
    l_lineno = []
    d_line = {}
    with open(base64textfile, 'r') as f:
        for line in f.readlines():
            s0 = line.rstrip('\n')
            if len(s0.strip()) == 0:
                continue

            s1 = s0.split('\t')[0].strip().rstrip()
            n1 = int(s1)
            if n1 not in l_lineno:
                l_lineno.append(n1)
                d_line[n1] = s0

    l_lineno.sort()
    for i in l_lineno:
        print d_line[i]

    return 0

if __name__ == '__main__':
    argc, argv = len(sys.argv), sys.argv
    sys.exit(main(argc, argv))
