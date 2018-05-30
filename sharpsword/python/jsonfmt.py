#!/usr/bin/python3
#
# Copyright (C) 2018, Vector Li (huanli@redhat.com). All rights reserved.
#

""" Format JSON file by deserializing JSON text to a Python Object
    and serializing the Python Object again.

    NOTE: The default indent of formatted JSON file is 4.
"""

import sys
import json
import collections
import getopt

g_debug = False
g_indent = 4


def debug(s):
    if g_debug is True:
        print("DEBUG> " + s)


def usage(s):
    sys.stderr.write("Usage: %s [-t <indent>] [-d] <json file>\n" % argv[0])
    sys.stderr.write("\t-t: --indent\n")
    sys.stderr.write("\t-d: --debug\n")
    sys.stderr.write("e.g.\n")
    sys.stderr.write("       %s -t 8 -d foo.json\n" % argv[0])
    sys.stderr.write("       %s --indent=4 --debug foo.json\n" % argv[0])


def main(argc, argv):
    options, rargv = getopt.getopt(argv[1:],
                                   ":t:dh", ["indent=", "debug", "help"])
    for opt, arg in options:
        if opt in ("-d", "--debug"):
            global g_debug
            g_debug = True
        elif opt in ("-t", "--indent"):
            global g_indent
            g_indent = int(arg)
        elif opt in ("-h", "--help"):
            usage(argv[0])
            return 1
        else:
            usage(argv[0])
            return 1

    argc = len(rargv)
    if argc == 0:
        usage(argv[0])
        return 1

    json_file = rargv[0]
    with open(json_file, 'r') as f:
        txt = ''.join(f.readlines())
        obj = json.loads(txt, object_pairs_hook=collections.OrderedDict)

        debug(str(type(txt)))
        debug(txt)
        debug(str(type(obj)))
        debug(str(obj))

        out = json.dumps(obj, indent=g_indent)
        print(out)

    return 0

if __name__ == '__main__':
    argv = sys.argv
    argc = len(argv)
    sys.exit(main(argc, argv))
