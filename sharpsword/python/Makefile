#
# Copyright (C) 2012, 2017, Vector Li (idorax@126.com). All rights reserved.
#

TARGETS	= icodeadm \
	  ipyman \
	  ibase64 \
	  dos2unix

all: $(TARGETS)

icodeadm: icodeadm.py
	cp $< $@
	chmod +x $@

ipyman: ipyman.py
	cp $< $@
	chmod +x $@

ibase64: ibase64.py
	cp $< $@
	chmod +x $@

dos2unix: dos2unix.py
	cp $< $@
	chmod +x $@

clean:

clobber: clean
	rm -f $(TARGETS)
cl: clobber
