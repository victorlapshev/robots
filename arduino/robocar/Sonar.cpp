#include "Arduino.h"
#include "Sonar.h"

Sonar::Sonar(int pinTrig, int pinEcho) {
    _pinTrig = pinTrig;
    _pinEcho = pinEcho;

    pinMode(pinTrig, OUTPUT);
    pinMode(pinEcho, INPUT);
}

long Sonar::getDistance() {
    digitalWrite(_pinTrig, LOW);
    delayMicroseconds(5);
    digitalWrite(_pinTrig, HIGH);
    delayMicroseconds(10);
    digitalWrite(_pinTrig, LOW);

    pinMode(_pinEcho, INPUT);
    long duration = pulseIn(_pinEcho, HIGH);

    return (duration / 2) / 29.1;
}