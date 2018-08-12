#!/bin/bash
#
# Copyright (C) 2018, Vector Li (huanli@redhat.com). All rights reserved.
#

#
# This utility is to connect VPN without manually inputting password + token.
#
# Note it requires two utilities, PyOTP [1] and sexpect [2].
#
# [1] PyOTP is a Python library for generating and verifying one-time passwords.
#     Here is an example to use it,
#
#     $ git clone https://github.com/pyotp/pyotp.git /tmp/pyotp
#     $ export PYTHONPATH=/tmp/pyotp/src:$PYTHONPATH
#     $ python
#     >>> import pyotp
#     >>> import base64
#     >>> token_secret = base64.b32encode('Hello world')
#     >>> totp = pyotp.TOTP(token_secret)
#     >>> token = totp.now()
#     >>> print token
#     265040
#
# [2] sexpect is another Expect implementation designed in the client/server
#     model which also supports attach/detach (like GNU screen).
#     To use it in bash, you have to build it on your system, e.g.
#
#     $ git clone https://github.com/clarkwang/sexpect.git /tmp/sexpect
#     $ cd /tmp/sexpect
#     $ make
#     $ export PATH=/tmp/sexpect:$PATH
#     $ which sexpect
#     /tmp/sexpect/sexpect
#
# And the environment variables in the following should be set in your bashrc
# as well,
#
#     o VPN_CONF,           e.g. /etc/openvpn/ovpn-pek11-tcp.conf
#     o VPN_PASSWORD or VPN_PASSWORD_HOOK,
#                           e.g. '123456789'
#                             or ~/.vpn/password_hook
#     o VPN_PYOTP_PATH,     e.g. /tmp/pyotp/src
#     o VPN_TOKEN_SECRET or VPN_TOKEN_SECRET_HOOK,
#                           e.g. 'CDOrzXyzOrzXyzOrzXyzOrzXyzOrzXyz'
#                             or ~/.vpn/token_secret_hook
#

g_git_pyotp="https://github.com/pyotp/pyotp.git"
g_git_sexpect="https://github.com/clarkwang/sexpect.git"

function get_vpn_user
{
	typeset user=${VPN_USER:-"$(id -un)"}
	echo "$user"
	return 0
}

function get_vpn_password
{
	typeset password=${VPN_PASSWORD:-"$(eval $($VPN_PASSWORD_HOOK))"}
	if [[ -z $password ]]; then
		echo "Oops, fail to get password," \
		     "please set it via env VPN_PASSWORD or" \
		     "VPN_PASSWORD_HOOK" >&2
		return 1
	fi

	echo "$password"
	return 0
}

function get_vpn_token
{
	typeset f_py_cb=/tmp/.vpn_token.py
	cat > $f_py_cb << EOF
#!/usr/bin/python
import sys
import datetime
import time

def main(argc, argv):
    if argc != 3:
        sys.stderr.write("Usage: %s <token secret> <pyotp path>\\n" % argv[0])
        return 1

    token_secret = argv[1]
    pyotp_path = argv[2]

    sys.path.append(pyotp_path)
    import pyotp
    totp = pyotp.TOTP(token_secret)

    #
    # The token is expected to be valid in 5 seconds,
    # else sleep 5s and retry
    #
    while True:
        tw = datetime.datetime.now() + datetime.timedelta(seconds=5)
        token = totp.now()
        if totp.verify(token, tw):
            print "%s" % token
            return 0
        time.sleep(5)

    return 1

if __name__ == '__main__':
    argv = sys.argv
    argc = len(argv)
    sys.exit(main(argc, argv))
EOF

	typeset pyotp_path=$VPN_PYOTP_PATH
	if [[ -z $pyotp_path ]]; then
		echo "Oops, env VPN_PYOTP_PATH is not set," \
		     "please try to set it via:" >&2
		echo "    \$ mkdir /tmp/pyotp" >&2
		echo "    \$ git clone $g_git_pyotp /tmp/pyotp" >&2
		echo "    \$ export VPN_PYOTP_PATH=/tmp/pyotp/src" >&2
		return 1
	fi

	typeset token_secret=$VPN_TOKEN_SECRET
	if [[ -z $token_secret ]]; then
		token_secret=$(eval $($VPN_TOKEN_SECRET_HOOK))
	fi
	if [[ -z $token_secret ]]; then
		echo "Oops, fail to get token secret," \
		     "please set it via env VPN_TOKEN_SECRET or" \
		     "VPN_TOKEN_SECRET_HOOK" >&2
		return 1
	fi

	python $f_py_cb $token_secret $pyotp_path
	typeset ret=$?
	rm -f $f_py_cb
	return $ret
}

function get_vpn_conf
{
	typeset conf=$VPN_CONF
	if [[ -z $conf ]]; then
		echo "Oops, fail to get vpn conf," \
		     "please set it via env VPN_CONF" >&2
		return 1
	fi

	echo "$conf"
	return 0
}

function check_sexpect
{
	type sexpect 2>&1 | egrep 'not found' > /dev/null 2>&1
	(( $? != 0 )) && return 0

	echo "Oops, sexpect not found, please git it a try:" >&2
	echo "    \$ git clone $g_git_sexpect /tmp/sexpect" >&2
	echo "    \$ (cd /tmp/sexpect; make)" >&2
	echo "    \$ export PATH=/tmp/sexpect:$PATH" >&2
	return 1
}

vpn_user=$(get_vpn_user)
(( $? != 0 )) && exit 1
vpn_password=$(get_vpn_password)
(( $? != 0 )) && exit 1
vpn_token=$(get_vpn_token)
(( $? != 0 )) && exit 1
vpn_conf=$(get_vpn_conf)
(( $? != 0 )) && exit 1

check_sexpect || exit 1

export SEXPECT_SOCKFILE=/tmp/sexpect-ssh-$$.sock
trap '{ sexpect close && sexpect wait; } > /dev/null 2>&1' EXIT

sexpect spawn sudo openvpn --config $vpn_conf
sexpect set -timeout 60 # XXX: 'set' should be invoked after server is running

while :; do
	sexpect expect -nocase -re "Username:|Password:"
	ret=$?
	if (( $ret == 0 )); then
		out=$(sexpect expect_out)
		if [[ $out == *"Username:"* ]]; then
			sexpect send -enter "$vpn_user"
		elif [[ $out == *"Password:"* ]]; then
			sexpect send -enter "$vpn_password$vpn_token"
			break
		else
			echo "*** unknown catch: $out" >&2
			exit 1
		fi
	elif sexpect chkerr -errno $ret -is eof; then
		sexpect wait
		exit 0
	elif sexpect chkerr -errno $ret -is timeout; then
		sexpect close
		sexpect wait
		echo "*** timeout waiting for username/password prompt" >&2
		exit 1
	else
		echo "*** unknown error: $ret" >&2
		exit 1
	fi
done

sexpect interact
