#ifndef Motor_h
#define Motor_h

#include "Arduino.h"
#include "Pid.h"

struct debugInfo {
    unsigned char throttle;
    float velocity;
    float targetVelocity;
};

class Motor {
public:
    Motor(unsigned char pwmPin, unsigned char digitalPin, unsigned char sensorPin);

    void setDirection(bool direction);

    void setVelocity(float velocity);

    void moveCm(unsigned short cm, float velocity, bool direction);

    void handleSensorInterrupt();

    void timer();

    debugInfo debug();          // todo: remove

    float getSpeed();

private:
    unsigned char _sensorPin;
    unsigned char _pwmPin;
    unsigned char _digitalPin;
    unsigned char _throttle;
    unsigned char _correctionCounter;

    bool _direction;

    float _target_velocity;
    float _current_velocity;
    float _current_rpm;
    float _debug;           // todo: remove

    unsigned long _last_micros;
    unsigned long _steps_counter;

    PID *_pid;

    void correctThrottle();

    void setThrottle();
};

#endif
