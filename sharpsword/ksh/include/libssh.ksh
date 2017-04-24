#!/usr/bin/ksh
#
# Copyright (c) 2016, 2017, Vector Li (idorax@126.com)
#

source ${.sh.file%/*}/libdebug.ksh

function ssh_cp
{
	typeset func=${.sh.fun}
	DEBUG $func

	typeset verbose="NO"
	typeset password=""
	typeset timeout=300

	typeset stamp=$(printf "%X" $(date +"%s%N"))
	typeset f_exp=$TMPDIR/${func}.exp.$stamp
	typeset f_out=$TMPDIR/${func}.out.$stamp
	typeset f_tmp=$TMPDIR/${func}.tmp.$stamp
	trap "rm -f $f_exp $f_out $f_tmp" EXIT

	typeset iopt
	while getopts ":Nvo:t:p:" iopt; do
		case $iopt in
		v) verbose="YES" ;;
		o) f_exp=$OPTARG ;;
		t) timeout=$OPTARG ;;
		p) password=$OPTARG ;;
		:) print -u2 "Option '-$OPTARG' wants an argument"; return 1 ;;
		'?') print -u2 "Option '-$OPTARG' not supported"; return 1 ;;
		esac
	done
	shift $((OPTIND - 1))

	typeset s_usage="Usage: $func [-v] [-o f_exp] [-t timeout]"
	s_usage+=" -p <password> [--] <scp(1) usage>"
	s_usage+="\ne.g.   $func -p l1admin    root@foo:/tmp/a /tmp/b"
	s_usage+="\n       $func -p l1admin -- /tmp/b root@foo:/tmp/a"
	s_usage+="\n       $func -p l1admin -- -r guest@foo:/tmp/d1 /tmp/d2"
	s_usage+="\n       $func -p l1admin -- -r /tmp/d2 guest@foo:/tmp/d1"
	if (( $# < 2 )); then
		print -u2 -- "$s_usage"
		return 1
	fi

	typeset scp_opts0=""
	scp_opts0+=" -o StrictHostKeyChecking=no"
	scp_opts0+=" -o UserKnownHostsFile=/dev/null"
	typeset scp_opts=${SCP_OPTS:-"$scp_opts0"}
	cat > $f_exp 2> /dev/null << EOF
#!/usr/bin/expect
set timeout $timeout
spawn scp $scp_opts $*
expect {
	-nocase "password: " {
		sleep .2
		send "$password\r"
		exp_continue
	}

	"Connection refused" {
		puts stderr "*** sshd not running?"
		exit -1
	}

	"Permission denied" {
		puts stderr "*** 1) (For root) PermitRootLogin no?"
		puts stderr "*** 2) (For non-root) illegal user?"
		puts stderr "*** 3) invalid password?"
		puts stderr "*** 4) other unknown reason?"
		exit -2
	}

	timeout {
		puts stderr "*** Timed out in $timeout secs"
 		exit -3
	}

	eof {
		catch wait result
		set ret [lindex \$result 3]
		exit \$ret
	}
}
EOF
	debug_cat $f_exp

	expect -f $f_exp > $f_out 2>&1
	typeset -i ret=$?
	tr '\r' ' ' < $f_out > $f_tmp
	sed 's/ $//' $f_tmp  > $f_out
	debug_cat $f_out

	if (( $ret != 0 )); then
		cat $f_out >&2
	else
		[[ $verbose == "YES" ]] && sed 's/^/PROLOGUE: /g' $f_out >&2
	fi

	return $ret
}

function ssh_do
{
	typeset func=${.sh.fun}
	DEBUG $func

	typeset verbose="NO"
	typeset password=""
	typeset timeout=300

	typeset stamp=$(printf "%X" $(date +"%s%N"))
	typeset f_exp=$TMPDIR/${func}.exp.$stamp
	typeset f_out=$TMPDIR/${func}.out.$stamp
	typeset f_tmp=$TMPDIR/${func}.tmp.$stamp
	trap "rm -f $f_exp $f_out $f_tmp" EXIT

	typeset iopt
	while getopts ":vo:t:p:" iopt; do
		case $iopt in
		v) verbose="YES" ;;
		o) f_exp=$OPTARG ;;
		t) timeout=$OPTARG ;;
		p) password=$OPTARG ;;
		:) print -u2 "Option '-$OPTARG' wants an argument"; return 1 ;;
		'?') print -u2 "Option '-$OPTARG' not supported"; return 1 ;;
		esac
	done
	shift $((OPTIND - 1))

	typeset s_usage="Usage: $func [-v] [-o f_exp] [-t timeout]"
	s_usage+=" -p <password> [--] <ssh(1) usage>"
	s_usage+="\ne.g.   $func -p l1admin    root@foo uname -a"
	s_usage+="\n       $func -p l1admin -- root@foo uname -a"
	s_usage+="\n       $func -p l1admin -- foo      uname -a"
	s_usage+="\nNOTE: -l <login_name> is UNSUPPORTED"

	if [[ -z $password ]]; then
		print -u2 -- "$s_usage"
		return 1
	fi

	typeset uath=${1?"*** [<user>@]<host>"}
	shift
	typeset argv="$@"
	if [[ -z "$argv" ]]; then
		print -u2 -- "$s_usage"
		return 1
	fi

	typeset rcmd_utag1="_RCMDUTAG=${stamp}START"
	typeset rcmd_utag2="_RCMDUTAG=${stamp}END"
	argv="echo $rcmd_utag1; $argv; echo $rcmd_utag2=\$?"

	typeset ssh_opts0="-n"
	ssh_opts0+=" -o StrictHostKeyChecking=no"
	ssh_opts0+=" -o UserKnownHostsFile=/dev/null"
	typeset ssh_opts=${SSH_OPTS:-"$ssh_opts0"}
	cat > $f_exp 2> /dev/null << EOF
#!/usr/bin/expect
set timeout $timeout
spawn ssh $ssh_opts $uath "$argv"
expect {
	-nocase "password: " {
		sleep .2
		send "$password\r"
		exp_continue
	}

	"Connection refused" {
		puts stderr "*** sshd not running?"
		exit -1
	}

	"Permission denied" {
		puts stderr "*** 1) (For root) PermitRootLogin no?"
		puts stderr "*** 2) (For non-root) illegal user?"
		puts stderr "*** 3) invalid password?"
		puts stderr "*** 4) other unknown reason?"
		exit -2
	}

	timeout {
		puts stderr "*** Timed out in $timeout secs?"
 		exit -3
	}

	eof {
		catch wait result
		set ret [lindex \$result 3]
		exit \$ret
	}
}
EOF
	debug_cat $f_exp

	expect -f $f_exp > $f_out 2>&1
	typeset -i ret=$?
	tr '\r' ' ' < $f_out > $f_tmp
	sed 's/ $//' $f_tmp  > $f_out
	debug_cat $f_out

	typeset sutag1=$(egrep -n "^$rcmd_utag1" $f_out)
	typeset sutag2=$(egrep -n "^$rcmd_utag2" $f_out)
	if [[ -n $sutag1 && -n $sutag2 ]]; then
		# get return code of rcmd
		ret=${sutag2##*=}

		# delete spawn ... and write the output of rcmd to stdout
		typeset -i m=$(print $sutag1 | awk -F':' '{print $1}')
		typeset -i n=$(print $sutag2 | awk -F':' '{print $1}')
		(( m += 1 ))
		(( n -= 1 ))
		(( $m <= $n )) && sed -n "${m},${n}p" $f_out

		if [[ $verbose == "YES" ]]; then
			(( m -= 1 ))
			(( n += 1 ))
			sed -n "1,${m}p"  $f_out | sed 's/^/PROLOGUE: /g' >&2
			sed -n "${n},\$p" $f_out | sed 's/^/EPILOGUE: /g' >&2
		fi
	else
		# fail to start rcmd actually or timeout
		cat $f_out >&2
	fi

	return $ret
}

function _verify_ssh_passwordless
{
	typeset func=${.sh.fun}
	DEBUG $func

	typeset host=$1
	typeset user=$2
	typeset retries=${3:-"9"}

	typeset stamp=$(printf "%X" $(date +"%s%N"))
	typeset f_out=$TMPDIR/${func}.out.$stamp
	typeset f_exp=$TMPDIR/${func}.exp.$stamp
	trap "rm -f $f_out $f_exp" EXIT

	typeset ssh_opts_default=""
	ssh_opts_default+=" -o StrictHostKeyChecking=no"
	ssh_opts_default+=" -o UserKnownHostsFile=/dev/null"
	ssh_opts_default+=" -F /dev/null"
	typeset ssh_opts=${SSH_OPTS:-"$ssh_opts_default"}
	typeset -i i
	for (( i = 1; i <= retries + 1; i++ )); do
		typeset timeout=$(( 10 * $i ))

		cat > $f_exp 2> /dev/null << EOF
#!/usr/bin/expect
set timeout $timeout
spawn ssh $ssh_opts $user@$host true
expect {
	-nocase "password:" {
		exit 1
	}

	"Connection refused" {
		exit 2
	}

	"Permission denied" {
		exit 3
	}

	timeout {
		exit 4
	}

	eof {
		exit 0
	}
}
EOF
		debug_cat $f_exp

		expect -f $f_exp > $f_out 2>&1
		typeset -i ret=$?
		debug_cat $f_out

		(( $ret == 0 )) && return 0
		(( $ret != 4 )) && return 1 # retry only if time out
	done

	return 1
}

function ssh_setup
{
	typeset func=${.sh.fun}
	DEBUG $func

	typeset dst_host=$1
	typeset dst_user=$2
	typeset dst_password=$3

	_verify_ssh_passwordless $dst_host $dst_user && return 0

	typeset src_user=$(id -un)
	typeset src_home=$(getent passwd $src_user | \
		    awk -F':' '{print $6}')
	typeset f_rsa="$src_home/.ssh/id_rsa.pub"
	if [[ ! -f $f_rsa ]]; then
		typeset ssh_key_dir="$src_home/.ssh"
		typeset ssh_key_priv="$ssh_key_dir/id_rsa"
		typeset ssh_key_pub="$ssh_key_dir/id_rsa.pub"

		if [[ ! -d $ssh_key_dir ]]; then
			mkdir -m 0700 -p $ssh_key_dir || return 1
		fi

		if [[ ! -f $ssh_key_priv ]]; then
			ssh-keygen -q -t rsa -N '' -f $ssh_key_priv || return 1
		fi
	fi

	typeset src_rsa=$(< $f_rsa)

	typeset f_cb_src=$TMPDIR/${.sh.fun}.setup.$$
	typeset f_cb_dst=/tmp/$(basename $f_cb_src).dst.$$
	trap "rm -f $f_cb_src" EXIT
	cat > $f_cb_src 2> /dev/null << EOF
		#!/usr/bin/bash
		#set -x
		trap "rm -f \$0" EXIT
		rsa="$src_rsa"
		user=$dst_user
		home=\$(getent passwd \${user} | awk -F':' '{print \$6}')
		[[ ! -d \${home}/.ssh ]] && mkdir -m 0700 \${home}/.ssh
		f_auth="\${home}/.ssh/authorized_keys2"
		if [[ ! -f \${f_auth} ]]; then
			echo \${rsa} > \${f_auth}
		else
			egrep "\${rsa%%+*}" \${f_auth} > /dev/null 2>&1
			(( \$? != 0 )) && echo \${rsa} >> \${f_auth}
		fi

		exit 0
EOF
	debug_cat $f_cb_src
	chmod 0755 $f_cb_src

	ssh_cp -p $dst_password \
	    $f_cb_src $dst_user@$dst_host:$f_cb_dst || return 1
	ssh_do -p $dst_password \
	    $dst_user@$dst_host $f_cb_dst || return 1

	_verify_ssh_passwordless $dst_host $dst_user 3 || return 1

	return 0
}
