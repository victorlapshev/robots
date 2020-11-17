import struct
import threading
from time import sleep

import serial


class Robocar:
    def __init__(self, port):
        self.serial = serial.Serial(
            port=port,
            baudrate=9600,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            bytesize=serial.EIGHTBITS
        )
        self.serial.isOpen()

        self.left_engine_speed = 0
        self.right_engine_speed = 0
        self.thread = None
        self.flags = 0

    def _send_data(self):
        values = (self.left_engine_speed, self.right_engine_speed, self.flags)
        string = b''

        for i in values:
            string += struct.pack('!B', i)

        self.serial.write(string)

        print(self.flags)

    def _thread_method(self):
        while True:
            self._send_data()
            sleep(0.01)

    def set_engines_speed(self, left: float, right: float):
        self.left_engine_speed = int(left * 255)
        self.right_engine_speed = int(right * 255)

    def set_direction(self, left: bool, right: bool):
        if left:
            self.flags = self.flags | (1 << 0)
        else:
            self.flags = self.flags & ~(1 << 0)
        if right:
            self.flags = self.flags | (1 << 1)
        else:
            self.flags = self.flags & ~(1 << 1)

    def run_thread(self):
        self.thread = threading.Thread(target=self._thread_method, args=())
        self.thread.start()

    def setup_radio(self):
        self.serial.write('AT+RXA=0x78,0x78,0x78,0x78,0x78'.encode())  # rx address
        sleep(1)

        self.serial.write('AT+TXA=0x78,0x78,0x78,0x78,0x78'.encode())  # tx address
        sleep(1)

        self.serial.write('AT+FREQ=2.430G'.encode())  # freq
        sleep(1)
