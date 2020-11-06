#!/usr/bin/python3
import socket
from time import sleep
import os
import select

def recvall(clientsocket):
   text = ""
   while True:
      leave = True
      for i in range(2):
         data = clientsocket.recv(4096)
         if (data):
            leave = False
            break
         sleep(4)
      if (data):
         text += data.decode("utf-8")
      if (leave and text == ""):
         raise Exception("Disconnected")
      elif (leave and text != ""):
         return text

def communicate(clientsocket, addr):
   while True:
      try:
         text = recvall(clientsocket)
         if (text != ""):
            print("received : "+ text)
         else:
            print("nothing received")
         if (text == "CONNECTED"):
            #run exploit zerologon
            res = 1
            if (res == 1):
               msg = "PWNED"
            else:
               msg = "FAILURE"
         clientsocket.send(msg.encode('ascii'))
      except:
         print("Client disconnected")
         clientsocket.close()
         break

def createServer():
   serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
   host = "localhost"
   port = 9996
   serversocket.bind((host, port))
   serversocket.listen(5)
   while True:
      clientsocket,addr = serversocket.accept()
      print("Got a connection from %s" % str(addr))
      communicate(clientsocket, addr)

def main():
   createServer()

if __name__ == "__main__":
   main()