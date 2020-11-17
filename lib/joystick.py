import pygame


class Joystick(object):
    controller = None
    axis_data = None
    button_data = None
    hat_data = None
    screen = None
    clock = None
    ser = None
    controller_func = None
    joy_state = None

    def init(self, controller_func):
        pygame.init()
        self.controller_func = controller_func
        self.screen = pygame.display.set_mode((600, 600))
        self.clock = pygame.time.Clock()
        self.joy_state = JoystickState()
        pygame.joystick.init()

        self.controller = pygame.joystick.Joystick(0)
        self.controller.init()

    def listen(self):
        if not self.axis_data:
            self.axis_data = {}

        if not self.button_data:
            self.button_data = {}
            for i in range(self.controller.get_numbuttons()):
                self.button_data[i] = False

        if not self.hat_data:
            self.hat_data = {}
            for i in range(self.controller.get_numhats()):
                self.hat_data[i] = (0, 0)

        while True:
            for event in pygame.event.get():

                if event.type == pygame.JOYAXISMOTION:
                    self.axis_data[event.axis] = round(event.value, 2)
                elif event.type == pygame.JOYBUTTONDOWN:
                    self.button_data[event.button] = True
                elif event.type == pygame.JOYBUTTONUP:
                    self.button_data[event.button] = False
                elif event.type == pygame.JOYHATMOTION:
                    self.hat_data[event.hat] = event.value

            if 0 in self.axis_data:
                self.joy_state.axis[0]['x'] = self.axis_data[0]

            if 1 in self.axis_data:
                self.joy_state.axis[0]['y'] = self.axis_data[1]

            if 2 in self.axis_data:
                self.joy_state.axis[1]['x'] = self.axis_data[2]

            if 5 in self.axis_data:
                self.joy_state.axis[1]['y'] = self.axis_data[5]

            self.controller_func.update(self.joy_state)


class JoystickState:
    def __init__(self):
        self.axis = ({'x': 0, 'y': 0}, {'x': 0, 'y': 0})
        self.buttons = {}  # todo
