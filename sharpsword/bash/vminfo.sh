#!/bin/bash
#
# Copyright (C) 2020, Vector Li (idorax@126.com). All rights reserved.
#

#
# Get IPv4 address of VM running in VirtualBox
#

FILE=$(readlink -f $BASH_SOURCE)
NAME=$(basename $FILE)
CDIR=$(dirname $FILE)
TMPDIR=${TMPDIR:-"/tmp"}

function fmt_mac
{
	typeset mac_in=$1
	typeset mac_out=""
	for (( i = 1; i <= 12; i += 2 )); do
		typeset cell=$(echo $mac | cut -c$i-$((i+1)))
		mac_out+="$cell:"
	done
	typeset mac=${mac_out%:}
	echo $mac
	return 0
}

function vbox_get_vm_ipv4
{
	typeset vm=$1
	typeset ipv4=$(VBoxManage guestproperty enumerate $vm | \
		       egrep 'Net.*IP' | awk -F',' '{print $2}' | \
		       awk '{print $NF}')
	[[ -z $ipv4 ]] && return 1
	echo $ipv4
	return 0
}

function vbox_get_vm_mac
{
	typeset vm=$1
	typeset mac=$(VBoxManage guestproperty enumerate $vm |  \
		      egrep 'Net.*MAC' | awk -F',' '{print $2}' | \
		      awk '{print $NF}')
	[[ -z $mac ]] && return 1
	echo $(fmt_mac $mac)
	return 0
}

function vbox_get_vm_state
{
	typeset vm=$1
	VBoxManage list runningvms | egrep "\"$vm\"" > /dev/null 2>&1
	(( $? == 0 )) && echo "up" || echo "down"
	return 0
}

function vbox_list_vms
{
	typeset vms=$(VBoxManage list vms | awk '{print $1}' | sed 's%"%%g')
	echo $vms
	return 0
}

vms="$@"
[[ -z $vms ]] && vms=$(vbox_list_vms)

f_out=$TMPDIR/$NAME.out.$$
trap "rm -f $f_out" EXIT

echo "VM:STATE:MAC:IPv4" > $f_out
for vm in $vms; do
	state=$(vbox_get_vm_state $vm)
	if [[ $state == "down" ]]; then
		echo "$vm:$state::" >> $f_out
		continue
	fi

	mac=$(vbox_get_vm_mac $vm)
	ipv4=$(vbox_get_vm_ipv4 $vm)
	echo "$vm:$state:$(echo $mac | sed 's/:/~/g'):$ipv4" >> $f_out
done
cat $f_out | lsgen
exit 0
