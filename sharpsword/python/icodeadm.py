#! /usr/bin/python
# -*- coding: utf-8 -*-

#
# Copyright (C) 2012, Vector Li (idorax@126.com). All rights reserved.
#

""" icodeadm - check or format print source files """

import sys
import os
import getopt
import unicodedata

g_version = "2.1"
g_date    = "2012/07/09"
g_author  = "Vector"
g_email   = "idorax@126.com"
g_name    = "icodeadm"
g_desc    = "Light utility to check or print " \
	    "soure files if any line's width > 80 chars"

MAX_LINENO_LEN = len("100000") # The max num of lines is 99999 by default
LINENO_FMT="% 5d"              # to support max line_no  99999
EXT_LEN = len("| ") + len(" |") + MAX_LINENO_LEN
MAX_LEN = 80 + EXT_LEN
TAB_LEN = 8
CMD_PRINT = 0
CMD_CHECK = 1
CMD_RAW   = 2
CMD_OTHER = 3

#
# Public functions to print out colorful string on terminal
#
def isatty():
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

def str2gray(s):
	if isatty():
		return ("\033[1;30m%s\033[m" % s)
	return (s)

def str2red(s):
	if isatty():
		return ("\033[1;31m%s\033[m" % s)
	return (s)

def str2green(s):
	if isatty():
		return ("\033[1;32m%s\033[m" % s)
	return (s)

def str2yellow(s):
	if isatty():
		return ("\033[1;33m%s\033[m" % s)
	return (s)

def str2blue(s):
	if isatty():
		return ("\033[1;34m%s\033[m" % s)
	return (s)

def str2magenta(s):
	if isatty():
		return ("\033[1;35m%s\033[m" % s)
	return (s)

def str2cyan(s):
	if isatty():
		return ("\033[1;36m%s\033[m" % s)
	return (s)

def str2white(s):
	if isatty():
		return ("\033[1;37m%s\033[m" % s)
	return (s)

#
# Public functions for str op
#
def xstr(s, width, align):
	out = ""
	n = width - len(s)
	if align == 'L':
		out = s + ' ' * n
	elif align == 'R':
		out = ' ' * n + s
	elif align == 'M':
		n /= 2
		out = ' ' * n + s
		n = width - len(out)
		out += ' ' * n
	else:
		pass

	return (out)

def lxstr(s, n):
	return xstr(s, n, 'L')

def mxstr(s, n):
	return xstr(s, n, 'M')

def rxstr(s, n):
	return xstr(s, n, 'R')

def print2(s=""):
	sys.stderr.write("%s\n" % s)

#
# Public functions to unicode op
#

#
# Notice: text must be unicoded before call unicode_len()
#
def unicode_len(us):
	n = 0
	for c in us:
		n += 1
		if isinstance(c, unicode):
			if unicodedata.east_asian_width(c) != 'Na':
				n += 1
	return (n)

def ustrlen(s):
	us = unicode(s, 'utf-8') # must unicode when call unicode_len()
	sz = unicode_len(us)
	return (sz)

def tab2spaces(s):
	i_str = unicode(s, 'utf-8') # must unicode when call unicode_len()
	o_str = ""

	# convert tab to spaces
	ntab = TAB_LEN
	ch = ''
	for ch in i_str:
		#print "DEBUG> char=[%c], ntab=%d" % (ch, ntab)
		if ch == '\t':
			if ntab > 0:
				o_str += ' ' * ntab
			ntab = 0
		else:
			o_str += ch
			ntab -= unicode_len(ch)

		if ntab <= 0:
			ntab = TAB_LEN

	out = "%s" % o_str.encode('utf-8') # encode back after done
	return (out)

def rget_space_index(s, n):
	i = 0
	while n > 0:
		if s[n-1] != ' ':
			i = n
			break
		n -= 1
	return (i)

#
# private functions
#
def fmtprint_head():
	print '+' + '-' * (MAX_LEN - 2) + '+'

def fmtprint_scale():
	s = str2blue("12345678") + str2yellow("12345678")
	fmtprint_text(' ' * MAX_LINENO_LEN + s * 5)

def fmtprint_text(text, nline=0):
	nsize  = ustrlen(text)
	lsize = (MAX_LEN - EXT_LEN) - nsize
	if lsize >= 0:
		if nline > 0:
			s_nline = str2gray(LINENO_FMT % nline)
			print "| %s %s%s |" % (s_nline, text, ' ' * lsize)
		else:
			print "| %s%s |" % (text,
				' ' * (lsize + MAX_LINENO_LEN))
	else:
		if nline > 0:
			s_nline   = str2gray(LINENO_FMT % nline)
			s_text80  = text[0:80]
			s_text80p = str2magenta(text[80:])
			s_nsize   = str2blue("[80+%d]" % (nsize - 80))
			print "| %s %s%s | %s" % (s_nline,
				s_text80, s_text80p, s_nsize)
		else:
			print "| %s |" % (text)

def op_file(filename, icmd):
	try:
		file = open(filename, 'r')
	except:
		print2("open file %s failed" % filename)
		return (1)

	if icmd == CMD_PRINT:
		fmtprint_head()
		fmtprint_text("File: %s" % filename)
		fmtprint_head()
		fmtprint_scale()
		fmtprint_head()
	elif icmd == CMD_CHECK:
		err_flag1 = 0 # check 80
		err_flag2 = 0 # check ends with spaces
		err_msg1 = ""
		err_msg2 = ""
	elif icmd == CMD_RAW:
		fmtprint_head()
		fmtprint_text("File: %s" % filename)
		fmtprint_head()
		n = MAX_LEN - len("|  |")
		print "| %s |" % str2red(   lxstr(" ^I : tab", n))
		#print "| %s |" % str2yellow(lxstr("  _ : space", n))
		print "| %s |" % str2blue(  lxstr("  $ : end", n))
		fmtprint_head()
	else:
		pass

	fileLines = file.readlines()
	line_no = 0
	for line in fileLines:
		line_no += 1

		if icmd == CMD_PRINT:
			text = tab2spaces(line.strip('\r\n'))
			fmtprint_text(text, line_no)
		elif icmd == CMD_CHECK:
			text = tab2spaces(line.strip('\r\n'))
			line_size = ustrlen(text)
			if line_size > 80:
				err_flag1  += 1
				plus_size   = line_size - 80
				s_line_no   = str2gray(LINENO_FMT % line_no)
				s_text_80   = text[0:80]
				s_text_80p  = str2magenta(text[80:])
				s_plus_size = str2blue("[80+%d]" % plus_size)
				err_msg1   += "   %s %s%s %s\n" % (
					s_line_no, s_text_80,
					s_text_80p, s_plus_size)

			if text.endswith(' '):
				err_flag2 += 1
				s_line_no = str2gray(LINENO_FMT % line_no)
				n = line_size
				index = rget_space_index(text, n)
				s_texta = text[0:index]
				s_textb = text[index:]
				s_textb = s_textb.replace(' ', str2magenta("_"))
				s_plus = str2blue("[%d-%d]" % (index+1, n))
				err_msg2 += "  %s %s%s %s\n" % (
					s_line_no, s_texta,
					s_textb, s_plus)
		elif icmd == CMD_RAW:
			text = line.strip('\r\n')
			s_line_no = str2gray(LINENO_FMT % line_no)
			s_text = text.replace('\t', str2red("^I"))
			s_text = s_text.replace(' ', str2yellow("_"))
			s_text += str2blue("$")
			print "%s %s" % (s_line_no, s_text)
		else:
			pass
	file.close()

	if icmd == CMD_PRINT:
		fmtprint_head()
	elif icmd == CMD_CHECK:
		rc = 0
		if err_flag1 != 0 or err_flag2 != 0:
			print str2red("[ FAIL ] check file %s" % filename)
			rc = 1

		if err_flag1 != 0:
			err80 = "line more than 80 chars found !!"
			print "%s %s\n" % (' ' * 8, str2yellow(err80))
			print err_msg1

		if err_flag2 != 0:
			errspace = "line ends with spaces found !!"
			print "%s %s\n" % (' ' * 8, str2yellow(errspace))
			fmtprint_head()
			fmtprint_scale()
			fmtprint_head()
			print err_msg2

		if rc != 0:
			return 1

		print str2cyan("[ PASS ] check file %s" % filename)
	else:
		pass

	return 0

def highlight_head():
	s = '*' * (MAX_LEN - EXT_LEN)
	print2(str2yellow(s))

def highlight_text(s):
	s = '*' + s + '*'
	print2(str2yellow(s))

def usage(prog):
	n = MAX_LEN - EXT_LEN - 2
	highlight_head()
	highlight_text(mxstr(g_desc, n))
	highlight_text(mxstr("%s v%s" % (g_name, g_version), n))
	highlight_text(mxstr("", n))
	highlight_text(mxstr("If bug found, please contact %s<%s>" %
		(g_author, g_email), n))
	highlight_head()

	print2()
	print2("Usage: %s <-c|-p|-r> <file 1> [file 2] ..." % prog)
	print2("       -c: check if any line > 80")
	print2("       -p: print")
	print2("       -r: rawly print, like vim :set list e.g. '\\t' => '^I'")
	print2("e.g.:")
	print2("       %s -c /tmp/*.ksh" % prog)
	print2("       %s -p /tmp/a.py /tmp/b.c" % prog)
	print2("       %s -r /tmp/a.ksh" % prog)

def main(argc, argv):
	icmd = CMD_OTHER
	options, rargv = getopt.getopt(argv[1:],
		":pcr", ["print", "check", "raw"])
	for opt, arg in options:
		if opt in ("-p", "--print"):
			icmd = CMD_PRINT
		elif opt in ("-c", "--check"):
			icmd = CMD_CHECK
		elif opt in ("-r", "--raw"):
			icmd = CMD_RAW
		else:
			pass
	if icmd == CMD_OTHER:
		usage(argv[0])
		return 1

	ret = 0
	for e in rargv:
		ret += op_file(e, icmd)

	return (ret)

if __name__ == "__main__":
	sys.exit(main(len(sys.argv), sys.argv))
