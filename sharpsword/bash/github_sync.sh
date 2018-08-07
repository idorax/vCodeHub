#!/bin/bash
#
# Copyright (C) 2018, Vector Li (idorax@126.com). All rights reserved.
#

#
# GitHub ADMIN (1) : SYNC origin master with upstream master
#
# NOTE:
#
#     1. sexpect is required to do 'git push origin master', to download
#        sexpect, please visit:
#            https://github.com/clarkwang/sexpect.git
#
#     2. These env should be set before you use this utility:
#        o GITHUB_USER
#        o GITHUB_PASSWORD or GITHUB_PASSWORD_HOOK
#
#        For example,
#        $ export GITHUB_USER='idorax'
#        $ export GITHUB_PASSWORD_HOOK='/tmp/idorax_password_hook'
#

function print { printf -- "$*\n"; }

function _isatty
{
	typeset -l isatty=$ISATTY
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

function git_cmd_prompt
{
	echo "$(id -un)@$(hostname):$(str2yellow $(pwd -P))\$ $(str2cyan $*)"
}

function run_git_cmd
{
	git_cmd_prompt "$@"
	eval "$*"
	typeset -i rc=$?
	echo
	return $rc
}

function draw_split_line
{
	typeset s=$(python -c "print '-' * 120")
	echo
	str2blue "$s"
	echo
	sleep 2
}

function get_github_user
{
	typeset user=${GITHUB_USER}
	if [[ -z $user ]]; then
		fatal "please set user via env GITHUB_USER"
	fi

	echo "$user"
	return 0
}

function get_github_password
{
	typeset password=${GITHUB_PASSWORD}
	[[ -z $password ]] && password=$(eval $($GITHUB_PASSWORD_HOOK))

	if [[ -z $password ]]; then
		fatal "please set password via env" \
		      "GITHUB_PASSWORD or GITHUB_PASSWORD_HOOK"
	fi

	echo "$password"
	return 0
}

function git_push_prompt
{
	typeset -u s_input="NO"
	printf "\n>>> Are you sure you want to continue pushing (Yes/No)? "
	read s_input
	[[ $s_input == "Y" || $s_input == "YES" ]] && return 0 || return 1
}

function fatal
{
	echo "!! $*" >&2
	exit 1
}

function do_setup
{
	rm -rf $g_wsroot
	mkdir $g_wsroot
}

function do_cleanup
{
	cd /tmp
	rm -rf $g_wsroot
}

function do_clone
{
	cd $g_wsroot
	run_git_cmd git clone $g_origin_master

	cd $g_wsroot/$g_repo
	run_git_cmd git checkout master

	cd /tmp
}

function do_sync
{
	cd $g_wsroot/$g_repo
	run_git_cmd git remote -v
	run_git_cmd git remote add upstream $g_upstream_master
	run_git_cmd git remote -v
	run_git_cmd git fetch upstream master
	run_git_cmd git rebase upstream/master
	cd /tmp
}

function do_push
{
	typeset user=$(get_github_user)
	typeset password=$(get_github_password)

	export SEXPECT_SOCKFILE=/tmp/sexpect-ssh-$$.sock
	trap '{ sexpect c && sexpect w; } > /dev/null 2>&1' EXIT

	cd $g_wsroot/$g_repo
	unset DISPLAY
	git_cmd_prompt '# ===>' $(str2red $g_origin_master)
	git_cmd_prompt git push origin master
	sexpect spawn  git push origin master

	while :; do
		sexpect expect -nocase -re 'Username.*:|Password.*:'
		typeset ret=$?
		if (( $ret == 0 )); then
			typeset out=$(sexpect expect_out)
			if [[ $out == "Username"* ]]; then
				sexpect send -enter "$user"
				continue
			else # "Password"
				git_push_prompt || exit 1

				sexpect send -enter "$password"
				break
			fi
		elif sexpect chkerr -errno $ret -is eof; then
			sexpect wait
			exit
		elif sexpect chkerr -errno $ret -is timeout; then
			sexpect close
			sexpect wait
			fatal "timeout waiting for username/password prompt"
		else
			fatal "unknown error: $ret" >&2
		fi
	done

	sexpect interact
}

repo=${1:-"skt"} # can be sktm
case $repo in
"skt")
	g_origin_master=https://github.com/idorax/skt.git
	g_upstream_master=https://github.com/RH-FMK/skt.git
	g_repo=skt
	g_wsroot=/tmp/foo-$g_repo
	;;

"sktm")
	g_origin_master=https://github.com/idorax/sktm.git
	g_upstream_master=https://github.com/RH-FMK/sktm.git
	g_repo=sktm
	g_wsroot=/tmp/foo-$g_repo
	;;

"sexpect")
	g_origin_master=https://github.com/idorax/sexpect.git
	g_upstream_master=https://github.com/clarkwang/sexpect.git
	g_repo=sexpect
	g_wsroot=/tmp/foo-$g_repo
	;;

*)
	echo "Oops, unknown repo" >&2
	exit 1
	;;
esac

#trap "do_cleanup" EXIT
do_setup
draw_split_line
do_clone
draw_split_line
do_sync
draw_split_line
do_push
