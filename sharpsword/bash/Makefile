#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

TARGETS	= imod \
	  getCOperators \
	  vmw \
	  ethinfo

all: $(TARGETS)

%: %.sh
	cp $< $@ && chmod +x $@

clean:

clobber: clean
	rm -f $(TARGETS)
cl: clobber
