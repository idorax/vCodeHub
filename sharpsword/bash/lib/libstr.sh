#!/bin/bash
#
# Copyright (C) 2018, Vector Li (idorax@126.com). All rights reserved.
#

function print { printf -- "$*\n"; }

function _isatty
{
	typeset -l isatty=${ISATTY:-"auto"}
	[[ $isatty == "yes" ]] && return 0         # yes
	[[ $isatty == "no" ]] && return 1          # no
	[[ -t 1 && -t 2 ]] && return 0 || return 1 # auto
}

function str2gray    { _isatty && print "\033[1;30m$@\033[m" || print "$@"; }
function str2red     { _isatty && print "\033[1;31m$@\033[m" || print "$@"; }
function str2green   { _isatty && print "\033[1;32m$@\033[m" || print "$@"; }
function str2yellow  { _isatty && print "\033[1;33m$@\033[m" || print "$@"; }
function str2blue    { _isatty && print "\033[1;34m$@\033[m" || print "$@"; }
function str2magenta { _isatty && print "\033[1;35m$@\033[m" || print "$@"; }
function str2cyan    { _isatty && print "\033[1;36m$@\033[m" || print "$@"; }
function str2white   { _isatty && print "\033[1;37m$@\033[m" || print "$@"; }

function msg_info      { str2green   "[ INFO ] $*"; }
function msg_fail      { str2red     "[ FAIL ] $*"; }
function msg_pass      { str2cyan    "[ PASS ] $*"; }
function msg_warn      { str2magenta "[ WARN ] $*"; }
function msg_xres      { str2cyan    "[ XRES ] $*"; }
function msg_timeout   { str2red  "[ TIMEOUT ] $*"; }
function msg_highlight { str2yellow           "$*"; }

#
# get a string which is char * n, e.g. '#' * 8
#
function strX
{
	typeset char=${1?"*** char"}
	typeset width=${2?"*** width"}
	python -c "print \"$char\" * $width"
}

function strLRM
{
	typeset align=${1?"*** align, L/R/M"}
	typeset char=${2?"*** char"}
	typeset width=${3?"*** width >=0"}
	shift 3
	typeset s="$@"
	typeset s1=""
	typeset -i n=$(( width - ${#s} ))
	if [[ $align == 'L' ]]; then
		s1="$s$(strX $char $n)"
	elif [[ $align == 'R' ]]; then
		s1="$(strX $char $n)$s"
	elif [[ $align == 'M' ]]; then
		n=$(( n / 2 ))
		s1="$(strX $char $n)$s"
		n=$(( width - ${#s1} ))
		s1="$s1$(strX $char $n)"
	else
		s1="$s"
	fi

	print "$s1"
}

#
# str{L|R|M} <char> <width> <string>
# e.g.
# strL '=' 8 "0123" # ===> 0123====
# strR '=' 8 "0123" # ===> ====0123
# strM '=' 8 "0123" # ===> ==0123==
#
function strL { strLRM 'L' "$@"; }
function strR { strLRM 'R' "$@"; }
function strM { strLRM 'M' "$@"; }
