#
# Copyright (C) 2012, 2018, Vector Li (huanli@redhat.com). All rights reserved.
#

TARGETS	= icodeadm \
	  ipyman \
	  ibase64 \
	  dos2unix \
	  jsonfmt


all: $(TARGETS)

%: %.py
	cp $< $@
	chmod +x $@

clean:

clobber: clean
	rm -f $(TARGETS)
cl: clobber
