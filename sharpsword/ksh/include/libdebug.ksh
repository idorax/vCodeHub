#!/usr/bin/ksh
#
# Copyright (c) 2013, 2017, Vector Li (idorax@126.com)
#

function _isatty
{
	typeset -l isatty=${ISATTY:-"no"}
	[[ $isatty == "yes" ]] && return 0         # yes
	[[ $isatty == "no" ]] && return 1          # no
	[[ -t 1 && -t 2 ]] && return 0 || return 1 # auto
}

function str2gray    { _isatty && print "\033[1;30m$@\033[m" || print -- "$@"; }
function str2red     { _isatty && print "\033[1;31m$@\033[m" || print -- "$@"; }
function str2green   { _isatty && print "\033[1;32m$@\033[m" || print -- "$@"; }
function str2yellow  { _isatty && print "\033[1;33m$@\033[m" || print -- "$@"; }
function str2blue    { _isatty && print "\033[1;34m$@\033[m" || print -- "$@"; }
function str2magenta { _isatty && print "\033[1;35m$@\033[m" || print -- "$@"; }
function str2cyan    { _isatty && print "\033[1;36m$@\033[m" || print -- "$@"; }
function str2white   { _isatty && print "\033[1;37m$@\033[m" || print -- "$@"; }


DEBUG_TAG=${DEBUG_TAG:-"_DEBUG_: "}

#
# SYNOPSIS
#	debug_query <identity> ...
#
# DESCRIPTION
#	Parse and check if need to enable debug for the specified function or
#	script filename. The function or script filename is separated via ":"
#	"|" or ",".
#
# RETURN
#	return 0 if true, else return 1
#
# NOTES
#	1. When identity is 'none' 'no' or 'off', return 1
#	2. When identity is 'all' 'yes' or 'on',  return 0
#	3. When identity is function name or script filename, return 0
#
function debug_query
{
	typeset -l object=""
	typeset identity="$1"
	eval "object=\${$identity}"
	shift

	#
	# Note: The suffix (,:|) of ${object} is required to avoid wildcard is
	#	parsed in command line before passing to debug_query.
	#
	object=$(print "${object},:|" | sed -e 's/,/|/g' -e 's/:/|/g')
	object=${object%%"|||"}
	[[ -z $object ]] && return 1

	typeset keyword="none no off"
	typeset -l i=""
	for i in $keyword; do
		[[ $i == +($object) ]] && return 1
	done

	typeset names=$(print $@ | sed -e 's/:/ /g' -e 's/|/ /g' -e 's/,/ /g')
	keyword="all yes on"
	for i in $keyword $names; do
		[[ $i == +($object) ]] && return 0
	done

	return 1
}

#
# SYNOPSIS
#	debug_getPS4
#
# DESCRIPTION
#	Get a colorful and powerful PS4 for better debugging.
#	The PS4 includes:
#	o file name
#	o func name
#	o lineno
#	o total seconds
#
# RETURN
#	Always return 0
#
# NOTES
#	There are two private environment variables:
#	o ISATTY  : If set to be "yes", the PS4 will be colorful by force
#	o ISAPATH : If set to be "yes", the filename will be absolute path
#
function debug_getPS4
{
	typeset file=$(str2blue    \${.sh.file##*/})
	typeset func=$(str2magenta \${.sh.fun})
	typeset lineno=$(str2gray  \${LINENO})
	typeset secs=$(str2magenta \${SECONDS%.*})

	typeset -l isapath=${ISAPATH} # .sh.file with absolute path
	[[ $isapath == "yes" ]] && file=$(str2blue \${.sh.file})

	#
	# NOTE: debug info are saved in stderr, for saving those stderr
	#       from remote, we add a tag $DEBUG_TAG when we extract
	#       those stderr from remote.
	#
	typeset s="$DEBUG_TAG"
	s=$s$(str2gray "[")$func
	s=$s$(str2gray "@")$file
	s=$s$(str2gray ":")$lineno
	s=$s$(str2gray "|")$secs
	s=$s$(str2gray "]+ ")

	print "$s"

	return 0
}

#
# SYNOPSIS
#	debug_strip <file>
#
# DESCRIPTION
#	strip the tag '$DEBUG_TAG' from debug info
#
# RETURN
#	None
#
function debug_strip
{
	typeset f_err=${1?}
	egrep -v -- "^$DEBUG_TAG" $f_err
}

#
# SYNOPSIS
#	debug_cat [file1, file2, ...]
#
# DESCRIPTION
#	cat file if debug is on
#
#	$@ - file list
#
# EXAMPLES
#	Example 1: cat some files in a function if debug is on
#		function foo1
#		{
#			typeset func=${.sh.fun}
#			... ...
#			debug_cat /tmp/foo.out.$$ /tmp/foo.err.$$
#			... ...
#		}
#
# RETURN
#	None
#
# NOTES
#	For shell, POSIX function is the same as C macro
#
debug_cat()
{
	set -o | tr -s ' ' ':' | egrep -i '^xtrace:on$' > /dev/null 2>&1 && \
	    cat -n $@ | sed "s/^/$DEBUG_TAG/g" >&2
}

#
# SYNOPSIS
#	DEBUG <func or file name1> [name2, ...]
#
# DESCRIPTION
#	A macro to enable debug
#
#	$@ - function or file name list
#
# EXAMPLES
#	Example 1: enable debug in a function
#		function foo1
#		{
#			typeset func=${.sh.fun}
#			DEBUG $func
#			... ...
#		}
#
#	Example 2: enable debug in a file
#		NAME=$(basename ${.sh.file})
#		DEBUG $NAME
#		... ...
#
# RETURN
#	None
#
# NOTES
#	For shell, POSIX function is the same as C macro
#
DEBUG()
{
	debug_query DEBUG $@ && typeset PS4=$(debug_getPS4) && set -x
}
