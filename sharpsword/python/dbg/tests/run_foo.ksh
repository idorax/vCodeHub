#!/usr/bin/ksh
#
# Copyright (C) 2017, Vector Li (idorax@126.com)
#

export PYTHONPATH=../
export DEBUG=${DEBUG:-"yes"}
python ./foo.py
rm -f $PYTHONPATH/*.pyc
