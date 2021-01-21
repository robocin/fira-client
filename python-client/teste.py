from fira_client import *
import socket

def main():
    c = FiraClient()
    

    while True:
        pkg = c.receive()
        print( pkg)
        
        #packet = packet_pb2.Packet()
        c.sendCommandsPacket(10,10)
        


if __name__ == '__main__':
  main()
