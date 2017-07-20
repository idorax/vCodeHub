#!/usr/bin/python
#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

import sys
import zbar
import Image

def decode_image(img_file):
    scanner = zbar.ImageScanner()
    scanner.parse_config('enable')
    pil = Image.open(img_file).convert('L')
    width, height = pil.size
    raw = pil.tobytes() # If Python 2.6, use pil.tostring()
    image = zbar.Image(width, height, 'Y800', raw)
    scanner.scan(image)
    data = ""
    for symbol in image:
        data += symbol.data
    return data

def main(argc, argv):
    if argc != 2:
        sys.stderr.write("Usage: %s <image>\n" % argv[0])
        return 1

    img_file = argv[1]
    s = decode_image(img_file)
    print s
    return 0

if __name__ == '__main__':
    argc, argv = len(sys.argv), sys.argv
    sys.exit(main(argc, argv))
