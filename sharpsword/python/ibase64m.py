#!/usr/bin/python
#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

""" encode/decode ASCII text created by base64(1) with password """

import sys

g_default_wrap_width = 76 # pls refer to base64(1)

def getsecid(password="1234567890)(*&^%$#@!"):
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

def main(argc, argv):
	if argc != 4:
		sys.stderr.write("Usage: %s <password> <E|D> <file>\n" % \
				 argv[0]);
		return 1

	password = argv[1]
	op = argv[2]
	fb = argv[3]

	secid = getsecid(password)
	if op.upper() == 'E':
		with open(fb, 'r') as fd:
			for line in fd.readlines():
				print encode(secid, line.rstrip())
	else:
		with open(fb, 'r') as fd:
			for line in fd.readlines():
				print decode(secid, line.rstrip())

	return 0

if __name__ == '__main__':
	sys.exit(main(len(sys.argv), sys.argv))
