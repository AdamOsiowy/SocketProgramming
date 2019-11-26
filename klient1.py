#!/usr/bin/python

import socket
import os
import time
from threading import Thread

BUFF_SIZE = 1024


class Server:
    server_list = []
    socket_list = []

    def __init__(self, IP, port):
        self.IP = IP
        self.port = port
        Server.server_list.append(self)
        self.socket_kliencki = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        Server.socket_list.append(self.socket_kliencki)

    def polacz_z_serverem(self):
        self.socket_kliencki.connect((self.IP, self.port))


def send_file_everything(filename):

    # sending file details
    print("Sending file details...")
    filesize = str(os.stat(filename).st_size)
    string_to_send = "" + filesize + " " + filename
    for socket in Server.socket_list:
        socket.sendall(string_to_send.encode())

    time.sleep(1)
    print("Sleeping...")
    time.sleep(1)
    print("Sleeping...")

    # sending file
    f = open(filename, 'rb')
    l = f.read(BUFF_SIZE)
    while l:
        # print("Sending...")
        for socket in Server.socket_list:
            socket.send(l)
        l = f.read(BUFF_SIZE)
    f.close()

    print("Done sending.")


def funkcja_dla_serverow(filename):
    for serv in Server.server_list:
        serv.polacz_z_serverem()
    send_file_everything(filename)
    for socket in Server.socket_list:
        socket.close()


if __name__ == '__main__':
    server1 = Server("150.254.79.29", 9000)
    server2 = Server("150.254.79.30", 9001)

    filename_to_send = input("Podaj nazwe pliku do przeslania: ").strip()

    funkcja_dla_serverow(filename_to_send)