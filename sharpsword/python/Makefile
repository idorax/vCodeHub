#
# Copyright (C) 2012, 2017, Vector Li (idorax@126.com). All rights reserved.
#

TARGET	= icodeadm \
	  ipyman \
	  dos2unix

all: $(TARGET)

icodeadm: icodeadm.py
	cp $< $@
	chmod +x $@

ipyman: ipyman.py
	cp $< $@
	chmod +x $@

dos2unix: dos2unix.py
	cp $< $@
	chmod +x $@

clean:

clobber: clean
	rm -f $(TARGET)
cl: clobber
