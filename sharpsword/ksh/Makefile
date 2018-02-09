#
# Copyright (C) 2017, 2018, Vector Li (idorax@126.com). All rights reserved.
#

TARGET	= ssh_setup \
	  ipkgadm

all: $(TARGET)

ssh_setup: ssh_setup.ksh
	cp $< $@
	chmod +x $@

ipkgadm: ipkgadm.ksh
	cp $< $@
	chmod +x $@

clean:

clobber: clean
	rm -f $(TARGET)
cl: clobber
