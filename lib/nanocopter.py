import struct
import threading
from time import sleep

import serial


class NanoCopter:
    def __init__(self, port):
        self.serial = serial.Serial(
            port=port,
            baudrate=9600,
            parity=serial.PARITY_NONE,
            stopbits=serial.STOPBITS_ONE,
            bytesize=serial.EIGHTBITS
        )
        self.serial.isOpen()

        self.roll = 0
        self.pitch = 0
        self.yaw = 0
        self.throttle = 0
        self.thread = None

    def _send_data(self):
        values = (self.throttle, self.yaw, self.pitch, self.roll)
        string = b''

        for i in values:
            string += struct.pack('!B', i)

        self.serial.write(string)

    def _thread_method(self):
        while True:
            self._send_data()
            sleep(0.01)

    def run_thread(self):
        self.thread = threading.Thread(target=self._thread_method, args=())
        self.thread.start()
