#include "Arduino.h"
#include "Motor.h"
#include "Pid.h"

#define MAX_FREQ        80
#define DISK_HOLES      19.0
#define WHEEL_RADIUS    0.03325

#define MIN_THROTTLE    30
#define MAX_THROTTLE    255

Motor::Motor(unsigned char pwmPin, unsigned char digitalPin, unsigned char sensorPin) {
    _pwmPin = pwmPin;
    _digitalPin = digitalPin;
    _sensorPin = sensorPin;
    _throttle = 0;
    _steps_counter = 0;
    _pid = new PID(0.1, 1, 0, 0.2, 0.09, 0.15);

    pinMode(pwmPin, OUTPUT);
    pinMode(digitalPin, OUTPUT);
    pinMode(sensorPin, INPUT);

    setDirection(true);
}

void Motor::setVelocity(float velocity) {
    if (velocity == _target_velocity) {
        return;
    }

    _target_velocity = velocity;
    _throttle = (int) (255.0 * velocity);   // todo: set correct one
    analogWrite(_pwmPin, _throttle);
}

void Motor::setDirection(bool direction) {
    if (direction == _direction) {
        return;
    }

    _direction = direction;
    digitalWrite(_digitalPin, direction ? HIGH : LOW);
}

void Motor::timer() {
    correctThrottle();
    setThrottle();
}

void Motor::handleSensorInterrupt() {
    unsigned long now = micros();

    _current_velocity = (1e6 / (now - _last_micros)) / MAX_FREQ;
    _last_micros = now;
    _steps_counter++;
}

void Motor::correctThrottle() {
    _throttle = (char) (255 * _pid->calculate(_target_velocity, _current_velocity));
}

void Motor::setThrottle() {
    analogWrite(_pwmPin, _throttle);
    _debug = _throttle;
}

void Motor::moveCm(unsigned short cm, float velocity, bool direction) {
    float initialVelocity = _target_velocity;
    bool initialDirection = _direction;

    unsigned long initialStep = _steps_counter;
    unsigned int stepsToPass;

    stepsToPass = (int) cm / (2 * PI * WHEEL_RADIUS * 100) * DISK_HOLES;

    setDirection(direction);
    setVelocity(velocity);

    while (_steps_counter < initialStep + stepsToPass) {
        delay(1);
    }

    setDirection(initialDirection);
    setVelocity(initialVelocity);
}

float Motor::getSpeed() {
    float rps = (_current_velocity * MAX_FREQ) / DISK_HOLES;

    return rps * 2 * PI * WHEEL_RADIUS;
}


debugInfo Motor::debug() {
    struct debugInfo res = {_throttle, _current_velocity, _target_velocity};

    return res;
}

