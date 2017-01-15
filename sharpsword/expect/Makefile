#
# Copyright (C) 2013, Vector Li (idorax@126.com)
#

TARGET	= vlogin \
	  vsu

all: $(TARGET)

vlogin: vlogin.exp
	cp $< $@
	chmod +x $@

vsu: vsu.exp
	cp $< $@
	chmod +x $@

clean:

clobber: clean
	rm -f $(TARGET)
cl: clobber
