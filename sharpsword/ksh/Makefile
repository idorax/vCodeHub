#
# Copyright (C) 2017, Vector Li (idorax@126.com)
#

TARGET	= ssh_setup

all: $(TARGET)

ssh_setup: ssh_setup.ksh
	cp $< $@
	chmod +x $@

clean:

clobber: clean
	rm -f $(TARGET)
cl: clobber
