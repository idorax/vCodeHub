#!/usr/bin/python
#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

import sys
import qrcode

def encode_image(s, img_file):
    img = qrcode.make(s)
    img.save(img_file)

def main(argc, argv):
    if argc != 3:
        sys.stderr.write("Usage: %s <prefix:width> <base64-text-file>\n" % \
                         argv[0])
        return 1

    prefix = argv[1].split(':')[0]
    width = int(argv[1].split(':')[1])
    base64textfile = argv[2]
    with open(base64textfile, 'r') as f:
        k = 1
        for line in f.readlines():
            sw = '0' * (width - len(str(k)))
            img_file = "%s%s%d.png" % (prefix, sw, k)
            k += 1
            s = line.rstrip()
            print "%s : %s" % (img_file, s)
            encode_image(s, img_file)

    return 0

if __name__ == '__main__':
    argc, argv = len(sys.argv), sys.argv
    sys.exit(main(argc, argv))
