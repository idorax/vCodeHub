#!/usr/bin/python
#
# Copyright (C) 2016, Vector Li (idorax@126.com)
#

"""
        debug module for Python programming
	-----------------------------------
        This debug module is for providing ksh-style debugging (by setting a
        poweful PS4 and set -x) while doing Python programming.

        Q: How to use this feature?
        A: Pretty easy. Add these two-line code to your Python script
           e.g.
               if __name__ == "__main__":
           +       import debug
           +       debug.init()
               ...

        Note the default debug environment variable is DEBUG, let's just take
        it for example,

        1. Turn debug on for all functions
           $ export DEBUG=yes

        2. Turn debug off
           $ export DEBUG=no

        3. Turn debug on for one function in specific file
           $ export DEBUG=foo@foo1.py

        4. Turn debug on for a couple of functions
           $ export DEBUG=foo:foo2:foo3

        5. Turn debug on for all functions in some files
           $ export DEBUG=@foo1.py:@foo2.py
"""

import sys
import os
import re
import inspect
import atexit
import time

class Cstr(str):
        """
        Cstr(object) -> Colorful string

        Return a colorful string representation of the object.

        0. togray()    : str2gray
        1. tored()     : str2red
        2. togreen()   : str2green
        3. toyellow()  : str2yellow
        4. toblue()    : str2blue
        5. tomagenta() : str2magenta
        6. tocyan()    : str2cyan
        7. towhite()   : str2white
        8. tonone()    : str2none (color is stripped)

        Examples:
        (1) s1 = "Hello World"
            s2 = Cstr(s1)
            print s2.tored()

        (2) print Cstr("Hello World").tored()

        (3) s1 = Cstr("Hello World").tored()
            s2 = Cstr(s1).tonone()
        """

        def __init__(self, s=""):
                self.s = s

        def setstr(self, s):
                self.s = s

        def getstr(self):
                return (self.s)

        def _isatty(self):
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

        def togray(self):
                if self._isatty():
                        return ("\033[1;30m%s\033[m" % self.s)
                return (self.s)

        def tored(self):
                if self._isatty():
                        return ("\033[1;31m%s\033[m" % self.s)
                return (self.s)

        def togreen(self):
                if self._isatty():
                        return ("\033[1;32m%s\033[m" % self.s)
                return (self.s)

        def toyellow(self):
                if self._isatty():
                        return ("\033[1;33m%s\033[m" % self.s)
                return (self.s)

        def toblue(self):
                if self._isatty():
                        return ("\033[1;34m%s\033[m" % self.s)
                return (self.s)

        def tomagenta(self):
                if self._isatty():
                        return ("\033[1;35m%s\033[m" % self.s)
                return (self.s)

        def tocyan(self):
                if self._isatty():
                        return ("\033[1;36m%s\033[m" % self.s)
                return (self.s)

        def towhite(self):
                if self._isatty():
                        return ("\033[1;37m%s\033[m" % self.s)
                return (self.s)

        def tonone(self):
                s = self.s
                s = re.sub(r'\033\[1;3.m', '', s)
                s = re.sub(r'\033\[m', '', s)
                s = s.strip('\r\n')
                return (s)

class Debug:
        """
        Debug -> Object

        Customize a callback tracer() which could be used by sys.settrace()

        NOTE: There are some environment variables which can be reset from shell
              o DEBUG_UTAG : debug unique tag, default is "__DEBUG__: "
              o TRACER_RPATH_DEPTH : relative path depth of file, default is 0
              o TRACER_SLEEP_NSECS : nsecs to sleep befor printing debug info to
                                     stderr, default is 0
              o TRACER_VERBOSE     : verbose level to print when "return" is
                                     being traced, can be one of 0, 1, 2, and 3,
                                     default is 0
                                     if >= 1, print caller
                                     if >= 2, print caller and locals
                                     if >= 3, print caller, locals and globals

        Examples:
        (1) foo = Debug()
            sys.settrace(foo.tracer)

        (2) foo = Debug("XXX_DEBUG")
            sys.settrace(foo.tracer)

        (3) foo = Debug("DEBUG", True)
            sys.settrace(foo.tracer)
        """
        def __init__(self, env="DEBUG", dumpvars=False):
                self.debug_envname = env
                self.debug = "no"
                self.dumpvars = dumpvars

                self.debug_utag_envname = "DEBUG_UTAG"
                self.debug_utag         = "__DEBUG__: "

                self.tracer_rpath_depth_envname = "TRACER_RPATH_DEPTH"
                self.tracer_sleep_nsecs_envname = "TRACER_SLEEP_NSECS"
                self.tracer_verbose_envname     = "TRACER_VERBOSE"
                self.tracer_rpath_depth         = 0
                self.tracer_sleep_nsecs         = 0
                self.tracer_verbose             = 0
                self.tracer_outfile             = "/tmp/debug.vars"
                self.tracer_start_time          = time.time()

                s = os.getenv(self.debug_envname)
                if s is not None and len(s) > 0:
                        self.debug = s

                s = os.getenv(self.debug_utag_envname)
                if s is not None and len(s) > 0:
                        self.debug_utag = s

                s = os.getenv(self.tracer_rpath_depth_envname)
                if s is not None and len(s) > 0:
                        self.tracer_rpath_depth = int(s.split(':')[-1])

                s = os.getenv(self.tracer_sleep_nsecs_envname)
                if s is not None and len(s) > 0:
                        self.tracer_sleep_nsecs = int(s)

                s = os.getenv(self.tracer_verbose_envname)
                if s is not None and len(s) > 0:
                        self.tracer_verbose = int(s)

                if self.dumpvars:
                        with open(self.tracer_outfile, 'w') as fd:
                                fd.write("%s=%s\n" % \
                                    (self.debug_envname, self.debug))
                                fd.write("%s='%s'\n" % \
                                    (self.debug_utag_envname, self.debug_utag))
                                fd.write("%s=%d\n" % \
                                    (self.tracer_rpath_depth_envname,
                                     self.tracer_rpath_depth))
                                fd.write("%s=%d\n" % \
                                    (self.tracer_sleep_nsecs_envname,
                                     self.tracer_sleep_nsecs))
                                fd.write("%s=%d\n" % \
                                    (self.tracer_verbose_envname,
                                     self.tracer_verbose))

        def _debug_query(self, funcname="", filename=""):
                """
                1. If debug is in ['no', 'off', 'none'], return False
                2. If debug is in ['yes', 'on', 'all'],  return True
                3. for obj in funcname@filename funcname @filename:
                       if obj is in debug.split(':')
                           return True
                """
                s = self.debug.lower()
                if s in ['no', 'none', 'off']:
                        return False
                if s in ['yes', 'all', 'on']:
                        return True

                l_obj = ['', '', '']
                l_obj[0] = funcname + '@' + filename.split('/')[-1]
                l_obj[1] = funcname
                l_obj[2] = '@' + filename.split('/')[-1]
                for obj in l_obj:
                        if obj in self.debug.split(":"):
                                return True

                return False

        def _get_caller(self, frame):
                prevframe = frame.f_back

                try:
                        prevlineno = prevframe.f_lineno
                except:
                        prevlineno = -1

                try:
                        prevfuncname = prevframe.f_code.co_name
                except:
                        prevfuncname = "None"

                try:
                        prevfilename = prevframe.f_code.co_filename
                except:
                        prevfilename = "None"

                s = "%s@%s:%d" % (prevfuncname, prevfilename, prevlineno)
                return s

        def tracer(self, frame, event, arg):
                #print Cstr("1:FRAME: " + str(frame)).tocyan()
                #print Cstr("2:EVENT: " + event).toblue()
                #print Cstr("3:ARG:   " + str(arg)).tored()

                #
                # Reference: https://pymotw.com/2/sys/tracing.html
                #
                # NOTE: c_call, c_return, and c_exception are not supported
                #       to be traced
                #
                if event in ["line", "return", "call", "exception"]:
                        seconds  = int(time.time() - self.tracer_start_time)
                        lineno   = frame.f_lineno
                        funcname = frame.f_code.co_name
                        filename = frame.f_code.co_filename

                        #
                        # For /usr/lib/python*/*.py, always print
                        # its absolute path
                        #
                        if not filename.startswith("/usr/lib"):
                                n = self.tracer_rpath_depth
                                filename = '/'.join(filename.split('/')[n:])

                        flag = self._debug_query(funcname, filename)
                        if not flag:
                                return None

                        try:
                                thisframe = inspect.getsourcelines(frame)
                                lineno_base = thisframe[1]
                                lineno_offset = lineno - lineno_base
                                lineco = thisframe[0][lineno_offset].rstrip()
                        except:
                                errmsg = "-- Oops, source code not found --"
                                lineco = Cstr(errmsg).tomagenta()

                        s  = self.debug_utag
                        s += Cstr('[').togray()
                        s += Cstr(funcname).tomagenta() + '()'
                        s += Cstr('@').togray()
                        s += Cstr(filename).toblue()
                        s += Cstr(':').togray()
                        s += Cstr("%d" % lineno).togray()
                        s += Cstr('|').togray()
                        s += Cstr("%d" % seconds).tomagenta()
                        s += Cstr(']').togray()
                        if event == "line":
                                s += '+ ' + lineco

                        elif event == "return":
                                s0 = s

                                rtag = "<RETURN>"
                                s += "++ %s ++ %s" % \
                                    (Cstr(rtag).tocyan(),
                                     Cstr(str(arg)).tocyan())

                                if self.tracer_verbose >= 3:
                                        rtag = "<GLOBALS>"
                                        gvars = str(frame.f_globals)
                                        s += '\n' + s0
                                        s += "++ %s ++ %s" % \
                                            (Cstr(rtag).tocyan(),
                                             Cstr(gvars).tocyan())

                                if self.tracer_verbose >= 2:
                                        rtag = "<LOCALS>"
                                        try:
                                                arg_info = \
                                                    inspect.getargvalues(frame)
                                                lvars = str(arg_info.locals)
                                        except:
                                                lvars = ""
                                        s += '\n' + s0
                                        s += "++ %s ++ %s" % \
                                            (Cstr(rtag).tocyan(),
                                             Cstr(lvars).tocyan())

                                if self.tracer_verbose >= 1:
                                        rtag = "</CALLER>"
                                        sprev = self._get_caller(frame)
                                        s += '\n' + s0
                                        s += "++ %s ++ %s" % \
                                            (Cstr(rtag).tocyan(),
                                             Cstr(sprev).tocyan())

                        elif event == "call":
                                s0 = s

                                if self.tracer_verbose >= 1:
                                        rtag = "<CALLER> "
                                        sprev = self._get_caller(frame)
                                        s += "++ %s ++ %s" % \
                                            (Cstr(rtag).tocyan(),
                                             Cstr(sprev).tocyan())
                                        s += '\n' + s0

                                try:
                                        arg_info = inspect.getargvalues(frame)
                                        funcargs = str(arg_info.locals)
                                except:
                                        funcargs = ""
                                fd4 = 'def ' + funcname + funcargs +':'
                                s += '++ ' + Cstr(fd4).tocyan()

                        elif event == "exception":
                                s += '++ ' + Cstr(str(arg)).tored()

                        else:
                                pass

                        if self.tracer_sleep_nsecs != 0:
                                time.sleep(self.tracer_sleep_nsecs)

                        sys.stderr.write("%s\n" % s)

                return self.tracer

def init(env="DEBUG", dumpvars=False):
        atexit.register(fini)

        dbg = Debug(env, dumpvars)
        sys.settrace(dbg.tracer)

def fini():
        #
        # NOTE: If we don't reset sys.settrace(), such an exception is printed
        #       to stderr once 'call' is traced
        #
        #       Exception TypeError: "'NoneType' object is not callable" in \
        #       <function _remove at 0xb740ecdc> ignored
        #
        #       To trace all functions called, try:
        #       $ python -m trace --listfuncs <foo.py>
        #       $ python -m trace --trace     <foo.py>
        #
        sys.settrace(None)
