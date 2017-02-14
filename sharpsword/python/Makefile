#
# Copyright (C) 2012, 2017, Vector Li (idorax@126.com)
#

TARGET	= icodeadm \
	  ipyman

all: $(TARGET)

icodeadm: icodeadm.py
	cp $< $@
	chmod +x $@

ipyman: ipyman.py
	cp $< $@
	chmod +x $@

clean:

clobber: clean
	rm -f $(TARGET)
cl: clobber
