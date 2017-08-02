#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

TARGETS	= av_encode \
	  av_decode \
	  av_encode_dynamic \
	  av_uniq \
	  av_verify \
	  av_amend \
	  av_poll_img \
	  av_server \
	  av_client \
	  av_click \
	  av_click_once \
	  av_writer \
	  av_reader \
	  av_extract

all: $(TARGETS)

av_encode: av_encode.py
	cp $< $@
	chmod +x $@

av_decode: av_decode.py
	cp $< $@
	chmod +x $@

av_encode_dynamic: av_encode_dynamic.py
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

av_click: av_click.py
	cp $< $@
	chmod +x $@

av_click_once: av_click_once.py
	cp $< $@
	chmod +x $@

av_writer: av_writer.py
	cp $< $@
	chmod +x $@

av_reader: av_reader.py
	cp $< $@
	chmod +x $@

av_poll_img: av_poll_img.ksh
	cp $< $@
	chmod +x $@

av_server: av_server.ksh
	cp $< $@
	chmod +x $@

av_client: av_client.ksh
	cp $< $@
	chmod +x $@

av_extract: av_extract.py
	cp $< $@
	chmod +x $@

clean:

clobber: clean
	rm -f $(TARGETS)
cl: clobber
