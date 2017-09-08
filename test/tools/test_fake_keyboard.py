#!/bin/python3

import os
import sys
from socket import socket


def checksum(s):
    if s[0] == '$':
        s = s[1:]
    ret = ord(s[0])
    for c in s[1:]:
        ret ^= ord(c)
    return ret


def make_msg(key):
    m = f'$FAKBD,{hex(ord(key))}'
    cksum = checksum(m)
    m = f'{m}*{"%X" % cksum}'
    return m

def send(host, port, text):
    s = socket()
    s.connect((host, port))
    f = s.makefile(mode='w')
    for k in text:
        f.write(make_msg(k) + '\n')
        f.flush()


if __name__ == '__main__':
    send(host='localhost', port=10110, text='hello\n1234\ngoodbye\n')
    #for x in sys.argv[1:]:
    #    print(f'{x} => {make_msg(x)}')
