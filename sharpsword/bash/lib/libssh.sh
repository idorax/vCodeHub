#!/bin/bash
#
# Copyright (C) 2018, Vector Li (idorax@126.com). All rights reserved.
#

function ssh_with_password
{
	typeset func="ssh_with_password"

	typeset timeout=${1?"**** timeout     required, e.g. 10 **********"}
	typeset port=${2?"******* port        required, e.g. 22 **********"}
	typeset user=${3?"******* user        required, e.g. root ********"}
	typeset password=${4?"*** password    required, e.g. Passw0rd ****"}
	typeset rhost=${5?"****** remote host required, e.g. 100.3.6.132 *"}
	shift 5
	typeset rcmd="$@"
	if [[ -z "$rcmd" ]]; then
		echo "*** cmd?" >&2
		return 1
	fi

	typeset stamp=$(date +"%Y%m%d%H%M%S")$(( RANDOM ))
	typeset f_callback=/tmp/$stamp.ssh_callback.exp
	cat > $f_callback 2> /dev/null << EOF
#!/usr/bin/expect
set timeout $timeout

spawn ssh -p $port -o StrictHostKeyChecking=no \\
	  -F /dev/null $user@$rhost $rcmd
expect {
	-nocase "password:" {
		sleep .2
		send "$password\r"
		exp_continue
	}

	"Connection refused" {
		puts stderr "*** sshd not running?"
		exit 1
	}

	timeout {
		puts stderr "*** Time out"
		exit 1
	}

	eof {
		catch wait result
		set ret [lindex \$result 3]
		exit \$ret
	}
}
EOF
	#
	# callback and strip '\r' added by expect script
	#
	typeset f_all=/tmp/${func}.all.$stamp
	typeset f_out=/tmp/${func}.out.$stamp
	expect -f $f_callback > $f_all 2>&1
	typeset -i ret=$?
	cat $f_all | tr '\r' ' ' | sed 's/ $//g' > $f_out
	if (( $ret == 0 )); then
		sed -e "/^spawn ssh.*/d" \
		    -e "/^$user@.*assword/d" \
		    -e "/^Password:/d" \
		    -e "/^Warning: Permanently added.* known hosts./d" \
		    $f_out
	else # print all message (including output from expect) to stderr
		cat $f_out >&2
	fi

	rm -f $f_all $f_out $f_callback

	return $ret
}

#
# SYNOPSIS
#	scp_with_password <timeout> <port> <password> \
#	                  <src file> <user>@<dst file>
#
# DESCRIPTION
#	scp file with password by using expect
#
# RETURN
#	Return !0 if fail, else return 0
#
# Example:
#	1. scp_with_password 30 26 Passw0rd /tmp/foo root@10.1.2.3:/tmp/bar
#	2. scp_with_password 30 26 Passw0rd root@10.1.2.3:/tmp/bar /tmp/foo
#
function scp_with_password
{
	typeset func="scp_with_password"

	typeset timeout=${1?"**** timeout     required, e.g. 10 **********"}
	typeset port=${2?"******* port        required, e.g. 22 **********"}
	typeset password=${3?"*** password    required, e.g. Passw0rd ****"}
	shift 3
	typeset rargv="$@" # e.g. /tmp/foo1 root@10.1.2.3:/tmp/foo2

	typeset stamp=$(date +"%Y%m%d%H%M%S")$(( RANDOM ))
	typeset f_callback=/tmp/$stamp.scp_callback.exp
	cat > $f_callback 2> /dev/null << EOF
#!/usr/bin/expect
set timeout $timeout

spawn scp -P $port -o StrictHostKeyChecking=no \\
	  -F /dev/null $rargv
expect {
	-nocase "password:" {
		sleep .2
		send "$password\r"
		exp_continue
	}

	"Connection refused" {
		puts stderr "*** sshd not running?"
		exit 1
	}

	timeout {
		puts stderr "*** Time out"
		exit 1
	}

	eof {
		catch wait result
		set ret [lindex \$result 3]
		exit \$ret
	}
}
EOF
	#
	# callback and strip '\r' added by expect script
	#
	typeset f_all=/tmp/${func}.all.$stamp
	typeset f_out=/tmp/${func}.out.$stamp
	expect -f $f_callback > $f_all 2>&1
	typeset -i ret=$?
	cat $f_all | tr '\r' ' ' | sed 's/ $//g' > $f_out
	if (( $ret == 0 )); then
		sed -e "/^spawn scp.*/d" \
		    -e "/.*@.*assword:/d" \
		    -e "/^Password:/d" \
		    -e "/^Warning: Permanently added.* known hosts./d" \
		    $f_out
	else # print all message (including output from expect) to stderr
		cat $f_out >&2
	fi

	rm -f $f_all $f_out $f_callback

	return $ret
}
