from lib.joystick import Joystick, JoystickState
from lib.robocar import Robocar


class Controller:
    def __init__(self):
        self.car = Robocar('/dev/cu.usbserial-1410')
        self.car.run_thread()

    def update(self, state: JoystickState):
        pass  # todo


controller = Controller()

ps4 = Joystick()
ps4.init(controller)
ps4.listen()
