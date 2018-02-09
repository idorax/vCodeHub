#!/usr/bin/ksh
#
# Copyright (C) 2017, 2018, Vector Li (idorax@126.com). All rights reserved.
#

source ${.sh.file%/*}/include/libssh.ksh

export TMPDIR=/tmp
export ISATTY=auto

host=${1?"*** remote host"}
user=${2?"*** user"}
password=${3?"*** password"}
ssh_setup $host $user $password
exit $?
