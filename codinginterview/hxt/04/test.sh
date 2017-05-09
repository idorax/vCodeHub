#!/bin/bash
#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

function runit
{
	echo "$(id -un)@$(hostname)\$ $*"
	eval "$@"
	echo
}

runit "./foo '%20' 'We are happy.'"
runit "./foo '%20' 'We  are happy.'"
runit "./foo '%20' 'We  are happy. '"
runit "./foo '%20' 'We  are happy.  '"
runit "./foo '%20' ' We  are happy.  '"
runit "./foo '%20' '  We  are happy.  '"

runit "./foo '%20$' 'We are happy.'"
runit "./foo '%20$' 'We  are happy.'"
runit "./foo '%20$' 'We  are happy. '"
runit "./foo '%20$' 'We  are happy.  '"
runit "./foo '%20$' ' We  are happy.  '"
runit "./foo '%20$' '  We  are happy.  '"

runit "./foo '%' 'We are happy.'"
runit "./foo '%' 'We  are happy.'"
runit "./foo '%' 'We  are happy. '"
runit "./foo '%' 'We  are happy.  '"
runit "./foo '%' ' We  are happy.  '"
runit "./foo '%' '  We  are happy.  '"
