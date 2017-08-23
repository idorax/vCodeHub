#!/usr/bin/python
#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

import sys
import qrcode
import zbar
import Image

NUM_LINES_ENCODE = 10 # encode 10 lines by default

def extract_lineno(nlines):
    l_lineno = []
    for line in nlines.split('\n'):
        s0 = line.rstrip('\n')
        if len(s0.strip()) == 0:
            continue

        if s0.find('\t') == -1:
            continue

        s1 = s0.split('\t')[0].strip().rstrip()
        try:
            n1 = int(s1)
        except:
            continue

        if n1 not in l_lineno:
            l_lineno.append(n1)

    l_lineno.sort()
    return l_lineno

def get_text_bylineno(nlines, lineno):
    out = ""

    for line in nlines.split('\n'):
        s0 = line.rstrip('\n')
        if len(s0.strip()) == 0:
            continue

        if s0.find('\t') == -1:
            continue

        s1 = s0.split('\t')[0].strip().rstrip()
        try:
            n1 = int(s1)
        except:
            continue

        if n1 == lineno:
            out = s0.split('\t')[1].strip().rstrip()
            break

    return out

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

def encode_image(s, img_file):
    img = qrcode.make(s)
    img.save(img_file)
    out = decode_image(img_file)
    if s != out: # validate line by line
        print ">" * 120
        print "Oops, failed to encode the input, now validate line by line ..."
        print "Input:\n%s\nOutput:\n%s\n" % (s, out)

        if len(out.strip().rstrip()) == 0:
            print "... the output is nothing, exit ..."
            print "<" * 120
            return 1

        l_lineno_exp = extract_lineno(s)

        for lineno in l_lineno_exp:
            got = get_text_bylineno(out, lineno)
            if len(got) == 0:
                print "<<<got<<<: got nothing by lineno %d ..." % lineno
                print "<" * 120
                return 1

            exp = get_text_bylineno(s,   lineno)
            if len(exp) == 0:
                print ">>>exp>>>: got nothing by lineno %d ..." % lineno
                print "<" * 120
                return 1

            if got != exp:
                print ">>>exp>>>: %d\t%s" % (lineno, exp)
                print "<<<got<<<: %d\t%s" % (lineno, got)
                if got[0:76] != exp: # NOTE: only validate the first 76 chars
                    print "<" * 120
                    return 1
            else:
                print ">>>GOOD<<: %d\t%s" % (lineno, exp)

        print "ALSO OKAY !!!"
        print "<" * 120

    return 0

def read_mlines(l, n, start, m):
    out = ""
    i = 0
    while i < m:
        index = i + start
        if index > n - 1:
            break
        out += l[index]
        i += 1
    return out

def encode_mlines(out, start, m, prefix, width):
    # 1. try to encode all in one
    k = start
    sw = '0' * (width - len(str(k)))
    img_file = "%s%s%d.png" % (prefix, sw, k)
    rc = encode_image(out.rstrip(), img_file)
    if rc == 0:
        print "NICE: %s : %s" % (img_file, out.rstrip())
        return

    # 2. try to encode line by line
    k = start
    for line in out.split('\n'):
        s = line.rstrip()
        if len(s) == 0:
            continue

        sw = '0' * (width - len(str(k)))
        img_file = "%s%s%d.png" % (prefix, sw, k)
        k += 1
        rc = encode_image(s, img_file)
        if rc == 0:
            sx = "OKAY: %s" % img_file
            print "%s : %s" % (sx, s)
        else:
            sx = "WARN: %s" % img_file
            print "%s : %s" % (sx, s)
            print "%s   %s" % (' ' * len(sx), decode_image(img_file))

def main(argc, argv):
    if argc != 3:
        sys.stderr.write("Usage: %s <prefix:width> <base64-text-file>\n" % \
                         argv[0])
        return 1

    prefix = argv[1].split(':')[0]
    width = int(argv[1].split(':')[1])
    base64textfile = argv[2]

    m = NUM_LINES_ENCODE
    with open(base64textfile, 'r') as f:
        l = f.readlines()
        n = len(l)

        start = 0
        while start < n:
            out = read_mlines(l, n, start, m)
            start += m
            encode_mlines(out, start, m, prefix, width)

    return 0

if __name__ == '__main__':
    argc, argv = len(sys.argv), sys.argv
    sys.exit(main(argc, argv))
