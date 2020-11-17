from lib.nanocopter import NanoCopter
from lib.joystick import Joystick, JoystickState


class Controller:
    def __init__(self):
        self.drone = NanoCopter('/dev/cu.usbserial-1410')
        self.drone.run_thread()

    def update(self, state: JoystickState):
        self.drone.yaw = int((state.axis[0]['x'] + 1) * 128)
        self.drone.throttle = int((state.axis[0]['y'] + 1) * 128)
        self.drone.roll = int((state.axis[1]['x'] + 1) * 128)
        self.drone.pitch = int((state.axis[1]['y'] + 1) * 128)


controller = Controller()

ps4 = Joystick()
ps4.init(controller)

ps4.listen()
