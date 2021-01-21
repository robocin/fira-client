import binascii
import socket

from pb_fira.packet_pb2 import *


class FiraClient:

    def __init__(self, visionIp='224.0.0.1', commandIp='127.0.0.1', visionPort=10002, commandPort=20011):
        """
        Init SSLClient object.
        Extended description of function.
        Parameters
        ----------
        ip : str
            Multicast IP in format '255.255.255.255'. 
        port : int
            Port up to 1024. 
        """
        
        self.visionIp = visionIp
        self.commandIp = commandIp
        self.visionPort = visionPort
        self.commandPort = commandPort

        # Connect vision and command sockets
        self.visionSocket = socket.socket(
            socket.AF_INET, socket.SOCK_DGRAM, socket.IPPROTO_UDP)
        self.visionSocket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        self.visionSocket.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_TTL, 128)
        self.visionSocket.setsockopt(socket.IPPROTO_IP, socket.IP_MULTICAST_LOOP, 1)

        self.commandSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        
        self.visionSocket.bind((self.visionIp, self.visionPort))
        self.commandAddress = (self.commandIp, self.commandPort)

    def receive(self):
        """Receive package and decode."""
        """Receive SSL wrapper package and decode."""
        data, _ = self.visionSocket.recvfrom(1024)
        decoded_data = Environment().FromString(data)
        
        return decoded_data
    
    def sendCommandsPacket(self, lspeed, rspeed):
        packet = self._fillCommandPacket(lspeed, rspeed)
        
        """Sends packet to grSim"""
        data = packet.SerializeToString()

        self.commandSocket.sendto(data, self.commandAddress)

    def _fillCommandPacket(self, lspeed, rspeed):
        pkt = Packet()
        d = pkt.cmd.robot_commands
        robot = d.add()
        robot.id          = 0
        robot.yellowteam  = False
        robot.wheel_left  = lspeed
        robot.wheel_right = rspeed
        return pkt
    
    def sendReplacementPacket(self, robotPositions = None, ballPosition = None):
        packet = self._fillReplacementPacket(robotPositions, ballPosition)
        """Sends packet to grSim"""
        data = packet.SerializeToString()

        self.commandSocket.sendto(data, self.commandAddress)
    
    def _fillReplacementPacket(self, robotPositions = None, ballPosition = None):
        packet = Packet()
        firaReplacement = packet.replace
        
        if ballPosition != None:
            firaBall = firaReplacement.ball
            firaBall.x = ballPosition.x
            firaBall.y = ballPosition.y
            firaBall.vx = ballPosition.v_x
            firaBall.vy = ballPosition.v_y
        
        if robotPositions != None:
            firaRobot = firaReplacement.robots
            for rbtPosition in robotPositions:
                rbt = firaRobot.add()
                rbt.yellowteam = rbtPosition.yellow
                rbt.id = rbtPosition.id
                rbt.x = rbtPosition.x
                rbt.y = rbtPosition.y
                rbt.dir = rbtPosition.theta
        return packet

