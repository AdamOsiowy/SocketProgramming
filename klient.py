#!/usr/bin/python

import socket
import os

BUFF_SIZE = 1024


def send_file_details(filename, s):
    filesize = str(os.stat(filename).st_size)
    string_to_send = "" + filesize + " " + filename
    s.sendall(string_to_send.encode())

    # data_received = socket.recv(1024).decode()
    #print(data_received)


def send_file(filename, socket):
    f = open(filename, 'rb')
    l = f.read(BUFF_SIZE)
    while l:
        print("Sending...")
        socket.send(l)
        l = f.read(BUFF_SIZE)

    print("Done sending.")


s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
host = "150.254.79.46"
port = int(input("Port: "))

s.connect((host, port))

send_file_details("kotXD.jpg", s)

s.close()
