#!/usr/bin/python
#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

"""
    Extract some lines from a base64-text-file with lineno

    For example,

    $ base64 /bin/ls | cat -n > /tmp/foo

    $ ./av_extract /tmp/foo 1 10 100,200
"""

import sys

def main(argc, argv):
    if argc < 3:
        sys.stderr.write("Usage: %s " % argv[0] + \
                         "<base64-text-file-with-lineno> <lineno> ...\n")
        sys.stderr.write("e.g.\n")
        sys.stderr.write("       %s /tmp/foo 1\n" % argv[0])
        sys.stderr.write("       %s /tmp/foo 1 11 12\n" % argv[0])
        sys.stderr.write("       %s /tmp/foo 1 11 20,30\n" % argv[0])
        return 1

    base64textfile = argv[1]

    l_lineno = []
    i = 2
    while i < argc:
        if argv[i].find(',') != -1:
            l_entry = argv[i].split(',')
            istart  = int(l_entry[0])
            iend    = int(l_entry[1])
            j = istart
            while j <= iend:
                l_lineno.append(j)
                j += 1
        else:
            l_lineno.append(int(argv[i]))

        i += 1

    with open(base64textfile, 'r') as f:
        for line in f.readlines():
            s0 = line.rstrip('\n')
            if len(s0.strip()) == 0:
                continue

            if s0.find('\t') == -1:
                continue

            s1 = s0.split('\t')[0].strip().rstrip()
            try:
                n1 = int(s1)
            except:
                continue

            if n1 in l_lineno:
                print s0

    return 0

if __name__ == '__main__':
    argc, argv = len(sys.argv), sys.argv
    sys.exit(main(argc, argv))
