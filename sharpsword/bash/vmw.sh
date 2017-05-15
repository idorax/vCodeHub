#!/bin/bash
#
# Copyright (C) 2016, 2017, Vector Li (idorax@126.com). All rights reserved.
#

#
# vmw - Use meld to review html pages created by webrev
#       meld:   http://meldmerge.org/
#       webrev: https://github.com/joyent/webrev
#

[[ ! -x /usr/bin/meld ]] && echo "ERROR: meld not found" >&2 && exit 1

function usage
{
	typeset s1="http://idorax/foo/webrev/"
	typeset s2="idorax:/var/tmp/foo"
	echo "Usage: $1 [-F] <webrev>" >&2
	echo "  e.g." >&2
	echo "    1# access webrev via http://" >&2
	echo "       $1 $s1" >&2
	echo "    2# access webrev via host:path" >&2
	echo "       $1 $s2" >&2
}

scp_flag="false"
while getopts ':Fh' iopt; do
	case $iopt in
	F) scp_flag="true" ;;
	h) usage $0; exit 1 ;;
	esac
done
shift $(( OPTIND - 1 ))

if (( $# < 1 )); then
	usage $0
	exit 1
fi
webrev=$1

export PS4="\$ " && set -x
if [[ $webrev == "http://"* ]]; then
	cd /tmp && rm -f index.html

	rhost=$(echo $webrev | sed 's%http://%%')
	rhost=${rhost%%/*}

	wget -O index.html $webrev
	ws=$(egrep -i "Workspace" index.html  | sed 's%.*<td>%%')
	ws=$(dirname ${ws%% *})
	if [[ -z $ws ]]; then
		echo "ERROR: fail to get workspace from index.html" >&2
		exit 1
	fi

	rm -f /tmp/index.html
else
	rhost=$(echo $webrev | awk -F':' '{print $1}')
	ws=$(echo    $webrev | awk -F':' '{print $2}')
	[[ $ws == *"/webrev"* ]] && ws=${ws%/webrev*}
fi

f_cb_src=/tmp/webrev.cb1.ksh
f_cb_dst=/tmp/webrev.cb2.ksh

cat > $f_cb_src 2>/dev/null <<- EOF
	#!/usr/bin/ksh
	trap "rm -f \${.sh.file}" EXIT
	export PS4="\$(id -un)@\$(hostname)\$ " && set -x
	[[ ! -d $ws/webrev ]] && exit 1
	cd $ws && tar zcf /tmp/webrev.tgz webrev
	exit 0
EOF

dst_dir=/tmp/$rhost
[[ ! -d $dst_dir ]] && mkdir $dst_dir

scp $f_cb_src $rhost:$f_cb_dst && rm -f $f_cb_src
ssh $rhost /usr/bin/ksh $f_cb_dst
if (( $? != 0 )); then
	echo "ERROR: $ws/webrev not found on $rhost" >&2
	exit 1
fi
[[ ! -f $dst_dir/webrev.tgz ]] && scp_flag="true"
[[ $scp_flag == "true" ]] && scp $rhost:/tmp/webrev.tgz $dst_dir
rm -rf $dst_dir/webrev
cd $dst_dir && tar zxf webrev.tgz
rm -f /tmp/old && ln -s $dst_dir/webrev/raw_files/old /tmp/old
rm -f /tmp/new && ln -s $dst_dir/webrev/raw_files/new /tmp/new
exec meld /tmp/old /tmp/new > $dst_dir/meld.log 2>&1 &
