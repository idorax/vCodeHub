#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

TARGETS	= av_encode \
	  av_decode \
	  av_uniq \
	  av_verify \
	  av_amend \
	  av_post_img \
	  av_poll_img

all: $(TARGETS)

av_encode: av_encode.py
	cp $< $@
	chmod +x $@

av_decode: av_decode.py
	cp $< $@
	chmod +x $@

av_uniq: av_uniq.py
	cp $< $@
	chmod +x $@

av_verify: av_verify.py
	cp $< $@
	chmod +x $@

av_amend: av_amend.py
	cp $< $@
	chmod +x $@

av_post_img: av_post_img.ksh
	cp $< $@
	chmod +x $@

av_poll_img: av_poll_img.ksh
	cp $< $@
	chmod +x $@

clean:

clobber: clean
	rm -f $(TARGETS)
cl: clobber
