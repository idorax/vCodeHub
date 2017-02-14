#!/usr/bin/ksh
#
# Copyright (c) 2017, Vector Li (idorax@126.com)
#

source ${.sh.file%/*}/include/libssh.ksh

export TMPDIR=/tmp
export ISATTY=auto

host=${1?"*** remote host"}
user=${2?"*** user"}
password=${3?"*** password"}
ssh_setup $host $user $password 
exit $?
