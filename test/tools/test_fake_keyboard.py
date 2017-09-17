#!/usr/bin/env python3

import os
import sys
from time import sleep
from socket import socket


def checksum(s):
    if s[0] == '$':
        s = s[1:]
    ret = ord(s[0])
    for c in s[1:]:
        ret ^= ord(c)
    return ret



def make_msg(key):
    m = '$FAKBD,{}'.format(hex(ord(key))[2:])


def make_gps_msg(header, body):
    m = '${},{}'.format(header, body)
    cksum = checksum(m)
    m = '{}*{}'.format(m, "%X" % cksum)
    return m


def make_key_msg(key):
    return make_gps_msg('FAKBD', hex(ord(key))[2:])


def make_time_msg():
    return make_gps_msg('GPRMC', '091717,A,5103.5403,N,00741.5742,E,055.3,022.4,230610,000.3,W')


def send(host, port, text):
    s = socket()
    s.connect((host, port))
    f = s.makefile(mode='w')
    #f.write(make_time_msg() + '\n')
    sleep(0.5)
    for k in text:
        f.write(make_key_msg(k) + '\n')
        f.flush()


if __name__ == '__main__':
    send(host='localhost', port=10110, text='hello\n1234\ngoodbye\n')
    #for x in sys.argv[1:]:
    #    print(f'{x} => {make_msg(x)}')
