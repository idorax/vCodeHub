#!/usr/bin/ksh
#
# Copyright (c) 2013, 2017, Vector Li (idorax@126.com)
#

source ${.sh.file%/*}/libdebug.ksh

#
# SYNOPSIS
#	goto_hang [flag] [return value]
#
# DESCRIPTION
#	Help to make running script suspend at a specific location say line N
#	if it is called at line N
#
#	$1 - flag: -f or -s, default is -f
#		   -f, touch /tmp/nohang to abort
#		   -s, kill -USR1 <pid>  to abort
#	$2 - return value you want to set, default is 0
#
# RETURN
#	0 or $2 set by user
#
# EXAMPLES
#	Example 1:
#
#	$ cat -n foo.ksh
#	1  #!/usr/bin/ksh
#	2  print "start"
#	3  goto_hang -f 0
#	4  print "cont"
#	5  goto_hang -s 1
#	6  print "end"
#	7  exit 0
#
#	L3: goto_hang -f 0, script hangs at L3 unless /tmp/nohang is touched
#	L5: goto_hang -s 1, script hangs at L5 unless it gets signal SIGUSR1
#
# NOTES
#	This is for debugging on purpose
#
function goto_hang
{
	typeset func=${.sh.fun}
	DEBUG $func

	typeset flag=${1:-"-f"}
	typeset rc=${2:-"0"}

	if [[ $flag == "-f" ]]; then
		typeset file=/tmp/nohang

		str2yellow "Now goto hang, pid=$$, to release," \
		    "please $(str2blue touch $file)" >&2

		rm -f $file
		while :; do
			[[ -f $file ]] && break
			sleep 10
		done

		str2yellow "$file is touched, exit hang" >&2
	else
		typeset sig="USR1"
		str2yellow "Now goto hang, pid=$$, to release," \
		    "please $(str2blue kill -$sig $$)" >&2
		trap "break" $sig
		while :; do
			sleep 10
		done
		str2yellow "signal $sig is got, exit hang" >&2
	fi

	return $rc
}

#
# SYNOPSIS
#	dump_env [file to save stdout]
#
# DESCRIPTION
#	Dump current environment variables
#
# RETURN
#	None
#
# NOTES
#	This is for debugging on purpose
#
function dump_env
{
	typeset func=${.sh.fun}
	DEBUG $func

	typeset fout=${1:-"/tmp/env.out"}
	env > $fout
	str2yellow "Current env is dumpped, " \
	    "please view file $(str2blue $fout)" >&2
}

#
# SYNOPSIS
#	format_path [path]
#
# DESCRIPTION
#	Format path, add default path and remove those duplicated items
#	then print the fomatted path to stdout
#
#	$1 - path, your path to format, optional, default is $PATH
#
# RETURN
#	None
#
function format_path
{
	typeset func=${.sh.fun}
	DEBUG $func

	typeset in_path=${1:-"$PATH"}
	typeset default_path="/usr/bin:/usr/sbin:/bin:/sbin"
	typeset this_path=$(print $in_path | tr ':' '\n' | \
	    sed -e "s%^/sbin$%%" -e "s%^/usr/sbin$%%" \
	    -e "s%^/bin$%%" -e "s%^/usr/bin$%%" | \
	    sort -u | tr '\n' ':')
	this_path=${this_path#:}
	this_path=${this_path%:}
	typeset ipath=$default_path:$this_path

	print -- $ipath
}
