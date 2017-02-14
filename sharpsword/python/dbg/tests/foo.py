#!/usr/bin/python
#
# Copyright (C) 2016, Vector Li (idorax@126.com)
#

import sys
import os

def foo(d="/tmp"):
    if os.path.isdir(d):
        return True
    else:
        return False

def main(argc, argv):
    foo()
    return 0

if __name__ == "__main__":
    import debug
    debug.init('DEBUG', True)
    argc, argv = len(sys.argv), sys.argv
    sys.exit(main(argc, argv))
