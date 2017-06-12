#!/usr/bin/python
#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

"""
ibase64 - encode/decode file like base64 does but support
          to do the job with password
"""

import sys
import getopt
import base64

g_default_wrap_width = 76 # pls refer to base64(1)

def getsecid(password):
	m = 0
	i = 0
	while i < len(password):
		c = password[i]
		i += 1
		m += ord(c) * i
	return m

def sumasii(s):
	m = 0
	i = 0
	while i < len(s):
		m += ord(s[i])
		i += 1
	return m

def encode(secid, s):
	nwrap = g_default_wrap_width

	if len(s) != nwrap:
		return s

	m = sumasii(s)
	n = (m % nwrap + secid % nwrap) / 2
	c = s[n]
	return c + s[0:n] + s[n+1:]

def decode(secid, s):
	nwrap = g_default_wrap_width

	if len(s) != nwrap:
		return s

	m = sumasii(s)
	n = (m % nwrap + secid % nwrap) / 2
	c = s[0]
	s1 = s[1:]
	return s1[0:n] + c + s1[n:]

def usage(s):
	sys.stderr.write("Usage: %s [-p password] <-e|-d> <file>\n" % s)

def main(argc, argv):
	password = ""
	op = "E"

	options, rargv = getopt.getopt(argv[1:],
			":p:ed", ["password=", "encode", "decode"])
	for opt, arg in options:
		if opt in ("-p", "--password="):
			password = arg
		elif opt in ("-e","--encode"):
			op = "E"
		elif opt in ("-d","--decode"):
			op = "D"
		else:
			pass

	argc = len(rargv)
	if argc != 1:
		usage(argv[0])
		return 1

	fb = rargv[0]

	flag = False
	if len(password) > 0:
		secid = getsecid(password)
		flag = True

	if op == 'E':
		nwrap = g_default_wrap_width
		with open(fb, 'r') as fd:
			b64data = base64.b64encode(fd.read())
			n = len(b64data)
			i = 0
			while i < n:
				line = b64data[i:i+nwrap]
				i += nwrap
				if flag:
					line = encode(secid, line)
				sys.stdout.write(line + '\n')
	else:
		b64data = ""

		with open(fb, 'r') as fd:
			for line in fd.readlines():
				if flag:
					line = "%s\n" % \
					       decode(secid, line.rstrip())
				b64data += line

		sys.stdout.write(base64.b64decode(b64data))

	return 0

if __name__ == '__main__':
	sys.exit(main(len(sys.argv), sys.argv))
