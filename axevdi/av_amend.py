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
    with open(base64textfile, 'r') as f:
        for line in f.readlines():
            s0 = line.rstrip('\n')
            s1 = s0.split('\t')[-1]
            if len(s1) > 76:
                m = len(s0)
                n = len(s1) - 76
                print s0[0:m-n]
            else:
                print s0
    return 0

if __name__ == '__main__':
    argc, argv = len(sys.argv), sys.argv
    sys.exit(main(argc, argv))
