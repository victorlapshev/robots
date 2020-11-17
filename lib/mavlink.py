from pymavlink import mavutil

# Send a positive x value, negative y, negative z,
# positive rotation and no button.
# https://mavlink.io/en/messages/common.html#MANUAL_CONTROL
# Warning: Because of some legacy workaround, z will work between [0-1000]
# where 0 is full reverse, 500 is no output and 1000 is full throttle.
# x,y and r will be between [-1000 and 1000].


# To active button 0 (first button), 3 (fourth button) and 7 (eighth button)
# It's possible to check and configure this buttons in the Joystick menu of QGC

class Mavlink:
    def __init__(self):
        self.master = mavutil.mavlink_connection('udpin:0.0.0.0:14550')
        self.master.wait_heartbeat()

    def test(self):
        self.master.mav.manual_control_send(
            self.master.target_system,
            500,
            -500,
            250,
            500,
            0
        )

        buttons = 1 + 1 << 3 + 1 << 7
        self.master.mav.manual_control_send(
            self.master.target_system,
            0,
            0,
            500,  # 500 means neutral throttle
            0,
            buttons
        )
