#!/usr/bin/ksh
#
# Copyright (C) 2018, Vector Li (idorax@126.com). All rights reserved.
#

#
# ipkgadm - RPM package admin
#

typeset -C g=\
(
	version="v1.1"
	date="02/10/2018"
	author="Vector Li"
	email="idorax@126.com"
	name="$(basename $0)"
	desc="Light utility to administrate RPM pkg"
)

#
################################################################################
# DEFAULT RPM pkg list which are used generally
#
#       This part can be EDIT manually, but NOT highly recommended
#
################################################################################
#

# P1 pkg list, *REQUIRED*
rpm_pkglist=""
rpm_pkglist+=" vim-enhanced"	# VIM
rpm_pkglist+=" ksh"		# Korn Shell
rpm_pkglist+=" expect"		# expect
rpm_pkglist+=" gcc gcc-c++"	# gcc, g++
rpm_pkglist+=" gdb"		# gdb
rpm_pkglist+=" git"		# git
rpm_pkglist+=" tree"		# tree, very nice
rpm_pkglist+=" screen"		# screen
rpm_pkglist+=" stardict"	# start dict
rpm_pkglist+=" pidgin"		# For IRC
rpm_pkglist+=" meld"		# Visual diff
rpm_pkglist+=" shutter"		# Feature-rich Screenshot Tool
rpm_pkglist+=" xchm"		# To read *.chm
rpm_pkglist+=" fbreader"	# To read *.epub
rpm_pkglist+=" mpg123"		# To play *.mp3
rpm_pkglist+=" wget"		# The non-interactive network downloader

# P2 pkg list, *OPTIONAL*
p2_rpm_pkglist=""
p2_rpm_pkglist+=" google-chrome-stable"			# For bluejeans
p2_rpm_pkglist+=" python2-pexpect python3-pexpect"

rpm_pkglist+=" $p2_rpm_pkglist"

################################################################################
# Functions to get colorful string                                             #
################################################################################

function _isatty
{
	typeset -l isatty=${ISATTY:-"auto"}
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

################################################################################
# Functions to format-print string                                             #
################################################################################

#
# get a string which is char * n, e.g. '#' * 8
#
function strX
{
	typeset char=${1?"*** char"}
	typeset width=${2?"*** width"}
	typeset out=""
	(( width > 0 )) && out=$(printf "%0${width}d" 0 | tr '0' $char)
	print -- "$out"
	return 0
}

function _strLMR
{
	typeset align=${1?"*** align, L/M/R"}
	typeset char=${2?"*** char"}
	typeset width=${3?"*** width >=0"}
	shift 3
	typeset s="$@"
	typeset s1=""
	typeset -i n=$(( width - ${#s} ))
	if [[ $align == 'L' ]]; then
		s1="$s$(strX $char $n)"
	elif [[ $align == 'M' ]]; then
		n=$(( n / 2 ))
		s1="$(strX $char $n)$s"
		n=$(( width - ${#s1} ))
		s1="$s1$(strX $char $n)"
	elif [[ $align == 'R' ]]; then
		s1="$(strX $char $n)$s"
	else
		s1="$s"
	fi

	print -- "$s1"
}

#
# str{L|M|R} <char> <width> <string>
# e.g.
# strL '=' 8 "0123" # ===> 0123====
# strM '=' 8 "0123" # ===> ==0123==
# strR '=' 8 "0123" # ===> ====0123
#
function strL { _strLMR 'L' "$@"; }
function strM { _strLMR 'M' "$@"; }
function strR { _strLMR 'R' "$@"; }

function str2upper
{
	typeset -u s="$@"
	print -- "$s"
}

function str2lower
{
	typeset -l s="$@"
	print -- "$s"
}

function msg_pass
{
	print "[ $(str2cyan PASS) ] $*"
}

function msg_info
{
	print "[ $(str2green INFO) ] $*"
}

function msg_fail
{
	print "[ $(str2red FAIL) ] $*"
}

function msg_fatal
{
	print "[ $(str2red FATAL) ] $*"
}

function msg_highlight
{
	str2yellow "$*"
}

function check_release
{
	typeset -l s=$(grep -E '^ID=' /etc/os-release | awk -F'=' '{print $NF}')
	print $s
	if [[ $s == *"fedora"* || $s == *"rhel"* ]]; then
	       return 0
	fi

	return 1
}

function check_pkg
{
	typeset pkg_type=$1
	typeset pkg=$2
	typeset op=${3:-"+"}
	typeset flag=""
	[[ $op == "+" ]] && flag="POS" || flag="NEG"

	# 'yum list installed | egrep ...' also works
	rpm -q $pkg > /dev/null 2>&1
	ret=$?

	[[ $flag == "NEG" ]] && ret=$(( !ret ))

	return $ret
}

function op_pkg
{
	typeset pkglist="$@"
	typeset -i ret=0

	typeset pkg_type="RPM"
	typeset s0=""
	typeset i pkg op
	for i in $pkglist; do
		#
		# check op and extract pkg name
		# e.g.
		# +tree   => op='+', pkg="tree"
		# -expect => op='-', pkg="expect"
		#
		# '+' means install if pkg does not exist
		# '-' mease remove  if pkg does     exist
		#
		if [[ $i == '+'* ]]; then
			op='+'; pkg=${i#+}
		elif [[ $i == '-'* ]]; then
			op='-'; pkg=${i#-}
		elif [[ $i == "\-"* ]]; then
			op='-'; pkg=${i#\\-}
		else
			op='+'; pkg=$i
		fi

		typeset s0="RPM pkg: $pkg"
		check_pkg $pkg_type $pkg $op
		typeset rc=$?
		if (( rc == 0 )); then
			if [[ $op == '+' ]]; then
				msg_pass "$s0 ...installed"
			else #$op == '-'
				msg_pass "$s0 ...not installed as expect"
			fi
		else
			if [[ $op == '+' ]]; then
				msg_fail "$s0 ...NOT installed, expect yes"
			else #$op == '-'
				msg_fail "$s0 ...installed, but expect NOT"
			fi
			(( ret += 1 ))
		fi

		#
		# if op is '-'(NEG), but pkg does exist,
		# we need to remove it
		#
		if (( g_op == 0 )) && [[ $op == '-' ]] && (( rc != 0 )); then
			msg_info "$s0 ......unintalling, wait"

			yum --assumeyes remove $pkg

			check_pkg $pkg_type $pkg $op
			if (( $? == 0 )); then
				msg_pass "$s0 ......succeed to remove"
				(( ret -= 1 ))
			else
				msg_fail "$s0 ......fail to remove"
				(( ret += 1 ))
			fi
		fi

		#
		# if op is '+' (POS), but pkg not exist,
		# we need to install it
		#
		if (( g_op == 0 )) && [[ $op == '+' ]] && (( rc != 0 )); then
			msg_info "$s0 ......intalling, wait"

			yum --assumeyes install $pkg

			check_pkg $pkg_type $pkg $op
			if (( $? == 0 )); then
				msg_pass "$s0 ......succeed to install"
				(( ret -= 1 ))
			else
				msg_fail "$s0 ......fail to install"
				(( ret += 1 ))
			fi
		fi
	done

	return $ret
}

function print_prompt
{
	typeset s="${*}"
	typeset s1=""
	typeset -i n=0
	if (( ${#s} == 80 )); then
		s1=${s}
	else
		n=$(( 80 - ${#s} ))
		n=$(( n / 2  - 1 ))
		s1='*'
		s1+=$(strX '#' $n | sed 's/#/ /g')
		s1+=$s
		n=$(( 80 - ${#s1} - 1 ))
		s1+=$(strX '#' $n | sed 's/#/ /g')
		s1+='*'
	fi

	msg_highlight "$s1"
}

function get_prompt
{
	print_prompt "$(strX '\*' 80)"
	print_prompt "${g.desc}"
	print_prompt "${g.name} ${g.version}"
	print_prompt ""
	print_prompt "If bug found, please contact ${g.author} <${g.email}>"
	print_prompt "$(strX '\*' 80)"
}

function usage
{
	get_prompt

	cat >&2 <<- EOF
	Usage: $1 [-v] <-n|-y|-d> [+<pkg>|-- -<pkg>|-f <file>]
	     -v: verbose, debug mode
	     -n: no, just check
	     -y: yes, check and install/remove
	     -d: dump, dump default pkg list
	     -f: file, pkg list is defined in file

	Examples:
	     $1 -n # check default pkg list
	     $1 -y # check default pkg list and op +/- pkg list
	     $1 -d # dump default pkg list
	     $1 -n +tree      # check pkg file/tree installed
	     $1 -n -- -tree   # check pkg file/tree not installed
	     $1 -n -f pkg.txt # check pkg list in pkg.txt
	     $1 -y +tree      # check pkg file/tree, and intall if not exist
	     $1 -y -f pkg.txt # op +/- pkg list in pkg.txt

	Notes:
	     + : means pkg should be installed
	     - : means pkg should NOT be installed

	     pkg list file defined by user looks like:
	     + tree
	     - tree
	EOF
}

g_rels=$(check_release)
if (( $? != 0 )); then
	msg_info "Oops, the release is $g_rels, UNSUPPORTED"
	exit 1
fi

g_argv_n=1
g_argv_y=1
g_dump=1
g_verbose=1
g_op=1
g_file=""
while getopts ':nycidf:vh' iopt; do
	case $iopt in
	n|c) g_argc=1; g_argv_n=0; g_op=1 ;;
	y|i) g_argc=1; g_argv_y=0; g_op=0 ;;
	d) g_dump=0 ;;
	f) g_file=$OPTARG ;;
	v) g_verbose=0 ;;
	h) usage $0; exit 1 ;;
	:) print -u2 $(str2red "Option '-$OPTARG' wants an argument"); exit 1 ;;
	'?') # '?' must be quoted here
	   print -u2 $(str2red "Option '-$OPTARG' not supported"); exit 1 ;;
	esac
done
shift $((OPTIND - 1))

#
# ---- reserved options ---------------
# -c == -n : check
# -i == -y : check and install/unintall
# -d       : dump default pkg list
#

#
# dump default pkglist and exit immediately
# in case we don't like to op by default
# e.g.
# root# ipkgadm -d > /tmp/ipkg.txt
# root# vi /tmp/ipkg.txt # <== remove or add some entries
# root# ipkgadm -y -f /tmp/ipkg.txt
#
#
if (( g_dump == 0 )); then
	pkglist="$rpm_pkglist"
	print -- "$pkglist" | \
	    tr -s ' ' '\n' | \
	    sed -e '/^$/d' \
	        -e 's/^/+ /g' \
	        -e 's/^+ +/+ /' \
	        -e 's/^+ -/- /g'
	exit 0
fi

#
# -n or -y must be specified but not both
#
if (( g_argv_n + g_argv_y == 2 )); then
	usage $0
	exit 1
fi
if (( g_argv_n + g_argv_y == 0 )); then
	msg_fatal "-n and -y are exclusive !!!\n"
	usage $0
	exit 1
fi

#
# If user does not specify a pkg list via file,
# try to get pkg list from argv,
# if argv is NULL, try to get default pkg list
#
pkglist="$@"
if [[ -z "$g_file" ]]; then
	[[ -z "$pkglist" ]] && pkglist="$rpm_pkglist"
else
	pkglist=$(tr -s '\t' ' ' < $g_file | \
	    sed "s/ \{1,\}//g" | \
	    egrep -v "^#" | \
	    sed '/^$/d')
fi

#
# OP pkg via pkg list
#
op_pkg $pkglist
exit $?
