#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

TARGETS	= av_encode \
	  av_decode \
	  av_amend

all: $(TARGETS)

av_encode: av_encode.py
	cp $< $@
	chmod +x $@

av_decode: av_decode.py
	cp $< $@
	chmod +x $@

av_amend: av_amend.py
	cp $< $@
	chmod +x $@

clean:

clobber: clean
	rm -f $(TARGETS)
cl: clobber
