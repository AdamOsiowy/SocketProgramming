#!/usr/bin/python

import socket
import os
import time
import sys

BUFF_SIZE = 1024

globalna = 1


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
        try:
            self.socket_kliencki.connect((self.IP, self.port))
            global globalna
            globalna = 1
        except:
            print("Nie udalo polaczyc sie z serwerem")
            print("------\n")
            globalna = 0

    def __str__(self):
        return f"IP: {self.IP}, port: {self.port}"


def send_file_everything(filename, serverlist):
    global globalna
    if globalna == 1:
        # sending file details
        print("Wysylam szczegoly pliku...")
        filesize = str(os.stat(filename).st_size)
        string_to_send = "" + filesize + " " + filename
        for server in serverlist:
            server.socket_kliencki.sendall(string_to_send.encode())

        time.sleep(2)

        # sending file
        print("Wysylam plik...")
        f = open(filename, 'rb')
        l = f.read(BUFF_SIZE)
        while l:
            for server in serverlist:
                server.socket_kliencki.send(l)

            l = f.read(BUFF_SIZE)
        f.close()

        print("Skonczono wysylanie pliku.")
        print("------\n")


def funkcja_dla_serverow(filename, serverlist):
    for serv in serverlist:
        serv.polacz_z_serverem()
    send_file_everything(filename, serverlist)
    for server in serverlist:
        try:
            server.socket_kliencki.close()
        except:
            pass


class Menu:
    def __init__(self):
        self.wczytaj_serwery()
        self.wybory()

    def wczytaj_serwery(self):
        try:
            with open('server_list.txt', 'r') as file:
                server_list = file.readlines()
                for line in server_list:
                    server_data = line.strip()
                    server_ip, server_port = server_data.split()
                    server_port = int(server_port)
                    Server(server_ip, server_port)

        except:
            print("Blad podczas wczytywania serwerow z listy")

    def wybory(self):
        while 1:
            print("Co chcialbys zrobic?")
            print("------")
            print("1. Wyslac plik na serwery")
            print("2. Wyswietlic liste serwerow")
            print("3. Wyjsc z programu")
            wybor1 = int(input("Wybieram: "))
            print("")

            if wybor1 == 1:
                print("Do kogo chcialbys wyslac plik?")
                print("------")
                print("1. Do wszystkich serwerow")
                print("2. Na wybrane serwery")
                wybor2 = int(input("Wybieram: "))
                print("")

                if wybor2 == 1:
                    filename_to_send = input("Podaj nazwe pliku do przeslania: ").strip()
                    funkcja_dla_serverow(filename_to_send, Server.server_list)

                if wybor2 == 2:
                    print("Lista serverow")
                    print("------")
                    i = 1
                    for server in Server.server_list:
                        print(str(i), str(server))
                        i = i+1
                    print("\n Ktore servery chcialbys wybrac? Jesli 1,2,5 wpisz '1,2,5'")
                    print("------")
                    wybor_serverow = input("Wybieram servery: ").split()
                    filename_to_send = input("Podaj nazwe pliku do przeslania: ").strip()
                    wybor_serverow = list(map(int, wybor_serverow))
                    lista_serverow = []
                    for i in wybor_serverow:
                        lista_serverow.append(Server.server_list[i-1])
                    funkcja_dla_serverow(filename_to_send, lista_serverow)

            if wybor1 == 2:
                i = 1
                for server in Server.server_list:
                    print(str(i), str(server))
                    i = i + 1
            if wybor1 == 3:
                sys.exit()


if __name__ == '__main__':
    menu = Menu()