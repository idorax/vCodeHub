#!/bin/bash
#
# Copyright (C) 2018, Vector Li (idorax@126.com). All rights reserved.
#

function _initPS4
{
	export PS4='[${FUNCNAME}@${BASH_SOURCE}:${LINENO}|${SECONDS}]+ '
}

function DEBUG
{
	typeset -l s=$DEBUG
	[[ $s == "yes" || $s == "true" ]] && _initPS4 && set -x
}
