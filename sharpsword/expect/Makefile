#
# Copyright (C) 2013, 2018, Vector Li (huanli@redhat.com). All rights reserved.
#

TARGET	= vlogin \
	  vsu \
	  vopenvpn

all: $(TARGET)

%: %.exp
	cp $< $@
	chmod +x $@

clean:

clobber: clean
	rm -f $(TARGET)
cl: clobber
