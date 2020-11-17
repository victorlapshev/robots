#ifndef ROBOCAR_SKETCH_SONAR_H
#define ROBOCAR_SKETCH_SONAR_H

class Sonar {
public:
    Sonar(int pinTrig, int pinEcho);

    long getDistance();

private:
    int _pinTrig;
    int _pinEcho;
};

#endif //ROBOCAR_SKETCH_SONAR_H
