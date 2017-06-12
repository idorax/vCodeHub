#!/usr/bin/python
#
# Copyright (C) 2015, 2017, Vector Li (idorax@126.com). All rights reserved.
#

"""
ipyman - Handy utility to review not only module but also built-in class.

         sys, os, logging, ... are modules
         int, float, ..., list, tuple, dict, ... are classes.
"""

import sys
import getopt

CMD_DIR   = 1
CMD_DOC   = 2
CMD_HELP  = 3
CMD_OTHER = 4

CstrNONE    = 0
CstrGRAY    = 30
CstrRED     = 31
CstrGREEN   = 32
CstrYELLOW  = 33
CstrBLUE    = 34
CstrMAGENTA = 35
CstrCYAN    = 36
CstrWHITE   = 37
class Cstr:
        '''
        Cstr(object) -> Colorful string

        o none    : CstrNONE      :  0 : "%s" % str
        o gray    : CstrGRAY      : 30 : "\033[1;30m%s\033[m" % str
        o red     : CstrRED       : 31 : "\033[1;31m%s\033[m" % str
        o green   : CstrGREEN     : 32 : "\033[1;32m%s\033[m" % str
        o yellow  : CstrYELLOW    : 33 : "\033[1;33m%s\033[m" % str
        o blue    : CstrBLUE      : 34 : "\033[1;34m%s\033[m" % str
        o magenta : CstrMAGENTA   : 35 : "\033[1;35m%s\033[m" % str
        o cyan    : CstrCYAN      : 36 : "\033[1;36m%s\033[m" % str
        o white   : CstrWHITE     : 37 : "\033[1;37m%s\033[m" % str

        Return a colorful string representation of the object.

        Example:
        (1) print Cstr("Hello World", CstrRED)
        (2) print Cstr("Hello World")
        (3) print Cstr()
        '''

        def __init__(self, s="", color=CstrNONE):
                self.color = color
                self.tstr  = s
                self.cstr  = self._tocstr()

        def __str__(self):
                return (self.cstr)

        def __len__(self):
                return (len(self.tstr))

        def _tocstr(self):
                if self.color == CstrNONE:
                        return (self.tstr)

                if not self._isatty():
                        return (self.tstr)
                return ("\033[1;%dm%s\033[m" % (self.color, self.tstr))

        def _isatty(self):
                import os
                s = os.getenv("ISATTY")
                if s == None:
                        s = ""

                if s.upper() == "YES":
                        return (True)

                if s.upper() == "NO":
                        return (False)

                if sys.stdout.isatty() and sys.stderr.isatty():
                        return (True)
                return (False)

def usage(s):
        s = Cstr(s, CstrYELLOW)

        sys.stderr.write(\
            "Usage: %s <-r|--dir|-d|--doc|-h|--help> <module or class>\n" % s)
        sys.stderr.write("e.g.\n")
        sys.stderr.write("       %s -r sys # dir(sys)\n" % s)
        sys.stderr.write("       %s -d sys # print sys.__doc__\n" % s)
        sys.stderr.write("       %s -h sys # help(sys)\n" % s)

        s1 = Cstr("Cstr", CstrMAGENTA);
        sys.stderr.write("\n")
        sys.stderr.write("       %s -d %s # print Cstr.__doc__\n" % (s, s1))
        sys.stderr.write("       %s -h %s # help(Cstr)\n" % (s, s1))

	s2 = Cstr("this", CstrCYAN);
        sys.stderr.write("\n")
        sys.stderr.write("       %s\n" % \
			(Cstr("-- The Zen of Python -- : " + \
			"python -c \"import this\"", CstrCYAN)))
        sys.stderr.write("       %s -d %s # print this.__doc__\n" % (s, s2))

def main(argc, argv):
        global CMD_DIR
        global CMD_DOC
        global CMD_HELP
        global CMD_OTHER

        icmd = CMD_OTHER
        options, rargv = getopt.getopt(argv[1:],
                ":lrdh", ["dir", "doc", "help"])
        for opt, arg in options:
                if opt in ("-r", "--dir", "-l"):
                        icmd = CMD_DIR
                elif opt in ("-d", "--doc"):
                        icmd = CMD_DOC
                elif opt in ("-h", "--help"):
                        icmd = CMD_HELP
                else:
                        pass

        if icmd == CMD_OTHER:
                usage(argv[0])
                return 1

        if len(rargv) != 1:
                usage(argv[0])
                return 1

        obj = rargv[0]

        isMod = True
        try:
                exec "import %s" % obj.split('.')[0]
        except:
                isMod = False

        # get obj's type
        otype = repr(type(eval(obj)))
        cotype = Cstr(otype.replace('type ', ''), CstrBLUE)

        if icmd == CMD_DIR:
                ldo = dir(eval(obj))

                nmax = 0
                for x in ldo:
                        if len(x) > nmax:
                                nmax = len(x)

                #
                # Don't use pprint as it is not very graceful
                # : exec "from pprint import pprint; pprint(dir(\"%s\" %obj))"
                #
                n = 0
                for x in ldo:
                        n += 1

                        # get x's type
                        try:
                                xtype = repr(type(eval(obj+'.'+x)))
                        except:
                                xtype = "?"
                        cxtype = Cstr(xtype, CstrMAGENTA)

                        # format x as colorful string
                        if x.startswith("__"):
                                cx = Cstr(x, CstrRED)
                        elif x.startswith("_"):
                                cx = Cstr(x, CstrYELLOW)
                        else:
                                cx = x

                        fd1 = "%d" % n
                        fd2 = ""
                        fd3 = ""
                        fd4 = "# %s" % cxtype

                        if isMod:
                                cobj = Cstr(obj, CstrCYAN)
                                fd2, fd3 = "%s" % cotype, "%s.%s" % (cobj, cx)
                        else:
                                cobj = Cstr("<'%s'>" % obj, CstrBLUE)
                                fd2, fd3 = "%s" % cobj, "%s" % cx
                        fd3 += ' ' * (nmax - len(x))

                        print ' ' * 4 + "%s\t%s %s %s" % (fd1, fd2, fd3, fd4)

        elif icmd == CMD_DOC:
                exec("print %s.__doc__" % obj)
        elif icmd == CMD_HELP:
                exec("help(%s)" % obj)
        else:
                pass

        return 0

if __name__ == '__main__':
        argv = sys.argv
        argc = len(argv)
        sys.exit(main(argc, argv))
