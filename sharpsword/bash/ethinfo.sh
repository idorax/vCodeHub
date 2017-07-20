#!/bin/bash
#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

#
# ethinfo - Query info of network driver, such as mac, state, device id,
#           vendor id, bdf and driver etc.
#
# NOTE:     The output of CLI on Linux is not friendly, hence, we use
#           Solaris-style to display the output gracefully.
#

PATH=/usr/local/sbin:/usr/local/bin:/sbin:/bin:/usr/sbin:/usr/bin:/root/bin
export PATH=$PATH

NAME=$(basename $0)
TMPDIR=${TMPDIR:-"/tmp/.sandbox.$NAME"}
[[ ! -d $TMPDIR ]] && mkdir -p -m 0777 $TMPDIR

# __XXX_LIBCOMMON.SH__ # BEGIN

#
################################################################################
# global variables
################################################################################
#

#
# 1. align orientation for output, default is LEFT
#
g_align=${ALIGN:-"LEFT"}

#
# 2. print string on terminal with color, default is NO
#
g_isatty=${ISATTY:-"NO"}

#
# 3. user tag to highlight when printing string on terminal with color,
#    default is ""
#
g_utag=${UTAG:-""}

#
# 4. others
#
g_name=$(basename $0)

#
################################################################################
# common library functions
################################################################################
#

function print   { printf -- "$*\n"; }
function tolower { printf -- "$*\n" | tr 'A-Z' 'a-z'; }
function toupper { printf -- "$*\n" | tr 'a-z' 'A-Z'; }

#
# Functions to create colorful string
#
function _isatty
{
	typeset isatty=$(tolower $ISATTY)
	[[ $isatty == "yes" ]] && return 0
	[[ $isatty ==  "no" ]] && return 1
	[[ -t 1 && -t 2 ]] && return 0 || return 1
}
function str2gray    { _isatty && print "\033[1;30m$@\033[m" || print "$@"; }
function str2red     { _isatty && print "\033[1;31m$@\033[m" || print "$@"; }
function str2green   { _isatty && print "\033[1;32m$@\033[m" || print "$@"; }
function str2yellow  { _isatty && print "\033[1;33m$@\033[m" || print "$@"; }
function str2blue    { _isatty && print "\033[1;34m$@\033[m" || print "$@"; }
function str2magenta { _isatty && print "\033[1;35m$@\033[m" || print "$@"; }
function str2cyan    { _isatty && print "\033[1;36m$@\033[m" || print "$@"; }
function str2white   { _isatty && print "\033[1;37m$@\033[m" || print "$@"; }

function msg_info { str2green "[ INFO ] $@";     }
function msg_pass { str2cyan  "[ PASS ] $@";     }
function msg_fail { str2red   "[ FAIL ] $@" >&2; }

#
# SYNOPSIS
#	query_index_byfds <full FDs> <sub FDs> [delimiter]
#
# DESCRIPTION
#	Query index(es) by FDs via GNU awk
#
#	$1 - source full FDs list, e.g. A,B,C,D,E
#	$2 - sub FDs,              e.g. B,A,C
#	$3 - delimiter,            e.g. ","
#
# EXAMPLES
#	Example 1:
#		query_index_byfds "A,B,C,D,E" "B,A,C" ","
#		# will print "2,1,3"
#
# RETURN
#	Return !0 if fails, else return 0
#
function query_index_byfds
{
	typeset func="query_index_byfds"

	typeset sl=${1?"full FDs"}
	typeset sr=${2?"sub FDs"}
	typeset dc=${3:-","}

	sl=$(toupper $sl)
	sr=$(toupper $sr)

	typeset s=$(print | \
		awk -F"$dc" -v _SL_="$sl" -v _SR_="$sr" '{        \
			m = split(_SR_, a_rt);                    \
			n = split(_SL_, a_lf);                    \
			for (i = 1; i <= m; i++) {                \
				id = -1;                          \
				for (j = 1; j <= n; j++) {        \
					if (a_rt[i] == a_lf[j]) { \
						id = j;           \
						break;            \
					}                         \
				}                                 \
				if (i < m) {                      \
					printf("%d,", id);        \
				} else {                          \
					printf("%d\n", id);       \
				}                                 \
			}                                         \
		}')

	if [[ $s == *"-1"* ]]; then
		print "$func: failed to query index" \
		    "from <$sl> for <$sr> with dc<$dc>" >&2
		return 1
	fi

	print $s | tr ',' $dc

	return 0
}

#
# SYNOPSIS
#	iextract <file> <indexes> <delimiter>
#
# DESCRIPTION
#	Extract text file by specified index(es)
#
# EXAMPLES
#	Example 1:
#	The text file say foo.txt looks like:
#		a:b:c:d:e
#		1:2:3:4:5
#		!:@:#:$:%
#	Now call iextract foo.txt "3:5:1" ":",
#	we will get:
#		c:e:a
#		3:5:1
#		#:%:!
#
# RETURN
#	Return !0 if fails, else return 0
#
function iextract
{
	typeset func="iextract"

	typeset txtfile=${1?"*** text file"}
	typeset ids=${2?"*** IDs, e.g. 1:2:3"}
	typeset dc=${3?"*** delimiter, e.g. ':'"}

	if [[ ! -f $txtfile ]]; then
		print "$func: file $txtfile not found" >&2
		return 1
	fi

	awk -F"$dc" -v _IDS_="$ids" -v _DC_="$dc" '{        \
			n = split(_IDS_, a_ids);            \
			for (i = 1; i <= n; i++) {          \
				id = a_ids[i];              \
				printf("%s", $id);          \
				if (i < n) {                \
					printf("%s", _DC_); \
				}                           \
			}                                   \
			printf("\n");                       \
		}' $txtfile

	return 0
}

#
# SYNOPSIS
#	ishow <file> <delimiter> <align> <isatty> [utag]
#
# DESCRIPTION
#	Show text file more gracefully than cat(1)
#
# EXAMPLES
#	Example 1: ishow /tmp/foo.txt ':' 'L' 'no' ""
#
# RETURN
#	Return !0 if fails, else return 0
#
function ishow
{
	typeset func="ishow"

	typeset txtfile=${1?"*** text file"}
	typeset dc=${2?"*** delimiter, e.g. ':' "}
	typeset align=${3?"*** align, e.g. L, R"}
	typeset isatty=${4?"*** isatty, e.g. yes, no or auto"}
	typeset utag=${5:-""}

	if [[ ! -f $txtfile ]]; then
		print "$func: file $txtfile not found" >&2
		return 1
	fi

	align=$(toupper $align)
	isatty=$(tolower $isatty)

	typeset -i i=0
	typeset -i nf=$(head -1 $txtfile | awk -F"$dc" '{print NF}')
	typeset -i nwidth=0
	typeset fmt=""
	for (( i = 1; i <= nf; i++ )); do
		nwidth=$(awk -F"$dc" -v I=$i '{print $I}' $txtfile | wc -L)
		if [[ $align == "LEFT" || $align == "L" ]]; then
			fmt=$fmt"%-${nwidth}s$dc"
		elif [[ $align == "RIGHT" || $align == "R" ]]; then
			fmt=$fmt"%${nwidth}s$dc"
		else
			fmt=$fmt"%s$dc"
		fi
	done

	typeset f_out=$TMPDIR/${func}.out.$$
	awk -F"$dc" -v _FMT_="$fmt" '{                   \
			n = split(_FMT_, a_fmt);         \
			for (i = 1; i <= n; i++) {       \
				printf(a_fmt[i]" ", $i); \
			}                                \
			printf("\n");                    \
		}' $txtfile > $f_out

	#
	# isatty may be "no", "yes" or "auto". If it is "no", cat it and
	# return 0 immediately
	#
	[[ $isatty == "no" ]] && cat $f_out && return 0

	#
	# cat the text file for supporting to print colorful lines on terminal
	# if env ISATTY is not "no"
	#
	i=0
	typeset n=$(cat $f_out | wc -l)
	typeset line
	exec 3< $f_out # read file to fd=3
	while IFS='\n' read -u3 line; do
		if (( i == 0 )); then
			(( n > 1 )) && str2blue "$line" || str2red "$line"
		else
			if [[ -n $utag ]]; then
				[[ $line != *"$utag"* ]] && \
				    str2cyan "$line" || \
				    str2magenta "$line"
			else
				(( i % 2 == 0 )) && \
				    str2cyan "$line" || \
				    str2magenta "$line"
			fi
		fi
		(( i += 1 ))
	done
	exec 3<&-

	return 0
}

#
# SYNOPSIS
#	showaid_validate_po <p_flag> <o_flag>
#
# DESCRIPTION
#	Validate '-p' with '-o'
#
# EXAMPLES
#	None
#
# RETURN
#	Return !0 if fails, else return 0
#
function showaid_validate_po
{
	typeset func="showaid_validate_po"

	typeset p_flag=$1
	typeset o_flag=$2

	[[ $p_flag != "TRUE" ]] && return 0

	if [[ -z $o_flag ]]; then
		print "$g_name: -p requires -o" >&2
		return 1
	fi

	if [[ $o_flag == "all" ]]; then
		print "$g_name: \"-o all\" is invalid with -p" >&2
		return 1
	fi

	return 0
}

#
# SYNOPSIS
#	showaid_validate_o <o_flag> <s_fd>
#
# DESCRIPTION
#	Validate '-o' fields
#
# EXAMPLES
#	None
#
# RETURN
#	Return !0 if fails, else return 0
#
function showaid_validate_o
{
	typeset func="showaid_validate_o"

	typeset o_flag=$1
	typeset s_fd=$2

	[[ -z $o_flag ]] && return 0
	[[ $o_flag == "all" ]] && return 0

	typeset p
	typeset q
	typeset -i found
	for p in $(print $o_flag | tr ',' ' '); do
		p=$(tolower $p)
		found=1
		for q in $(print $s_fd | tr ':' ' '); do
			q=$(tolower $q)
			[[ $p == $q ]] && found=0 && break
		done

		if (( found != 0 )); then
			print "$g_name: field $p is invalid" >&2
			return 1
		fi
	done

	return 0
}

#
# SYNOPSIS
#	showaid_present <f_out> <p_flag> <o_flag> <s_fd>
#
# DESCRIPTION
#	Present the text file $f_out
#
# EXAMPLES
#	None
#
# RETURN
#	Return !0 if fails, else return 0
#
function showaid_present
{
	typeset func="showaid_present"

	typeset f_out=$1
	typeset p_flag=$2
	typeset o_flag=$3
	typeset s_fd=$4

	if [[ -n $o_flag && $o_flag != "all" ]]; then
		typeset o_ids=""
		typeset o_fd=$(print $o_flag | tr ',' ':')
		o_ids=$(query_index_byfds $s_fd $o_fd ':')
		(( $? != 0 )) && return 1

		typeset f_tmp=$TMPDIR/${func}.tmp.$$
		cp $f_out $f_tmp
		iextract $f_tmp $o_ids ':' > $f_out
	fi

	#
	# Convert '~' back to be ':' because we once converted ':' to be '~' for
	# values of those FDs (e.g. MAC) having ':' as default delimiter is ':'
	#
	typeset waveflag="TRUE"
	egrep "~" $f_out > /dev/null 2>&1 || waveflag="FALSE"

	if [[ $p_flag == "TRUE" ]]; then
		if [[ $waveflag == "TRUE" ]]; then
			typeset sc=""
			typeset -i n=$(print $o_flag | awk -F',' '{print NF}')
			(( n > 1 )) && sc="\\\:" || sc=":"

			cat $f_out | sed "s/~/$sc/g"
		else
			cat $f_out
		fi
	else
		if [[ $waveflag == "TRUE" ]]; then
			ishow $f_out ':' $g_align $g_isatty $g_utag | \
			    sed "s/~/:/g"
		else
			ishow $f_out ':' $g_align $g_isatty $g_utag
		fi
	fi

	return 0
}

# __XXX_LIBCOMMON.SH__ # END

#
# SYNOPSIS
#	show_eth_info [[-p] [-o <field>,...] [eth]|-h]
#
# DESCRIPTION
#	Body of ethinfo [[-p] [-o <field>,...] [eth]|-h]
#
# EXAMPLES
#	Example 1: show_eth_info
#
#	Example 2: show_eth_info -o all
#
#	Example 3: show_eth_info -p -o ETH,MAC,BDF
#
# RETURN
#	Return !0 if fails, else return 0
#
function show_eth_info
{
	typeset func="show_eth_info"

	typeset p_flag="FALSE" # '-p'
	typeset o_flag=""      # '-o:'
	typeset options=""
	options=$(getopt -o po:h \
	    --long parseable,output:,help \
	    -n "ERROR" -- "$@")
	if (( $? != 0 )); then
		usage $g_name
		return 1
	fi
	eval set -- "$options"
	while true; do
		case "$1" in
		-p|--parseable) p_flag="TRUE"; shift ;;
		-o|--output   ) o_flag="$2"; shift 2 ;;
		-h|--help     ) usage $g_name; return 1 ;;
		--            ) shift; break ;;
		*             ) break;
		esac
	done

	# validate -p and -o
	showaid_validate_po $p_flag "$o_flag" || return 1

	# get eths to show
	typeset eth=""
	typeset eths="$@"
	if [[ -z $eths ]]; then
		for eth in $(ls -1 /sys/class/net); do
			ethtool -i $eth > /dev/null 2>&1
			(( $? != 0 )) && continue
			eths+=" $eth"
		done
	fi

	# validate eth one by one
	typeset f_tmp=$TMPDIR/${func}.tmp.$$
	for eth in $eths; do
		ethtool -i $eth > $f_tmp 2>&1
		if (( $? != 0 )); then
			cat $f_tmp >&2
			return 1
		fi
	done

	# define FDs to show
	typeset s_fd=""
	typeset s_fd_def=""
	typeset s_fd_ext=""
	s_fd_def+="ETH:"
	s_fd_def+="MAC:"
	s_fd_def+="STATE:"
	s_fd_def+="DEVICEID:"
	s_fd_def+="VENDORID:"
	s_fd_def+="MTU:"
	s_fd_def+="SPEED:"
	s_fd_def+="BDF:"
	s_fd_def+="DRIVER:"
	s_fd_def+="IPv4"

	# if '-o ..." is specified, all FDs should be included
	s_fd=$s_fd_def

	# validate per field while -o is specified
	showaid_validate_o "$o_flag" $s_fd || return 1

	typeset f_out=$TMPDIR/${func}.out.$$
	if [[ $p_flag == "TRUE" ]]; then
		# if "-p" is spedified, we should not print out headers
		> $f_out
	else
		print $s_fd > $f_out
	fi

	typeset eth=""
	for eth in $eths; do
		typeset eth_root=/sys/class/net/$eth

		typeset f_mac=$eth_root/address
		typeset mac=""
		[[ -f $f_mac ]] && mac=$(cat $f_mac)
		mac=$(echo $mac | sed 's/:/~/g') # so as to display ':'

		typeset state="down"
		ethtool $eth 2> /dev/null | \
		    egrep -i 'Link detected: yes' > /dev/null 2>&1
		(( $? == 0 )) && state="up"

		typeset ipv4=$(ip addr show dev $eth | \
		    egrep 'inet ' | awk '{print $2}')

		typeset f_device=$eth_root/device/device
		typeset device_id=""
		[[ -f $f_device ]] && device_id=$(cat $f_device)

		typeset f_vendor=$eth_root/device/vendor
		typeset vendor_id=""
		[[ -f $f_vendor ]] && vendor_id=$(cat $f_vendor)

		typeset f_mtu=$eth_root/mtu
		typeset mtu=""
		[[ -f $f_mtu ]] && mtu=$(cat $f_mtu)

		typeset speed=""
		if [[ $state == "up" ]]; then
			speed=$(ethtool $eth 2>/dev/null | \
			    egrep "Speed:.*b/s" | awk '{print $2}')
		fi

		typeset f_bdf=$eth_root/device
		typeset bdf=$(readlink $f_bdf)
		if [[ -n $bdf ]]; then
			bdf=${bdf##*/}
			[[ $bdf == "0000:"* ]] && bdf=${bdf#*:}
			bdf=$(echo $bdf | sed 's/:/~/g') # so as to display ':'
		fi

		typeset f_driver=$eth_root/device/driver
		typeset driver=$(readlink $f_driver)
		if [[ -n $driver ]]; then
			driver=${driver#*/bus/}
			if [[ $driver != "usb/"* ]]; then
				driver=$(ethtool -i $eth | \
				    egrep -i '^driver:' | awk '{print $NF}')
			fi
		else
			driver=$(ethtool -i $eth | \
			    egrep -i '^driver:' | awk '{print $NF}')
		fi

		typeset out=""
		out+=":$eth"
		out+=":$mac"
		out+=":$state"
		out+=":$device_id"
		out+=":$vendor_id"
		out+=":$mtu"
		out+=":$speed"
		out+=":$bdf"
		out+=":$driver"
		out+=":$ipv4"
		echo ${out#:} >> $f_out
	done

	showaid_present $f_out $p_flag "$o_flag" $s_fd || return 1
	return 0
}

function cleanup
{
	rm -rf $TMPDIR/*.$$
	exit ${1}
}

function usage
{
	print "Usage: $1 [[[-p] -o field[,...]] [eth]|-h]" >&2
}

#
################################################################################
# main()
################################################################################
#

trap "cleanup \$?" EXIT

show_eth_info "$@"
exit $?
