#include <nRF24L01.h>
#include <RF24.h>
#include "Motor.h"
#include "Sonar.h"
#include "timer-api.h"

#define TRACK_WIDTH 0.125

#define PIN_RADIO_CE 10
#define PIN_RADIO_CSN 9

#define PIN_MOTOR_RIGHT_SENSOR  2
#define PIN_MOTOR_LEFT_SENSOR   3
#define PIN_MOTOR_RIGHT_DIGITAL 4
#define PIN_MOTOR_RIGHT_PWM     5
#define PIN_MOTOR_LEFT_PWM      6
#define PIN_MOTOR_LEFT_DIGITAL  7
#define PIN_SONAR_ECHO          8
#define PIN_SONAR_TRIG          9


Motor leftMotor(PIN_MOTOR_LEFT_PWM, PIN_MOTOR_LEFT_DIGITAL, PIN_MOTOR_LEFT_SENSOR);
Motor rightMotor(PIN_MOTOR_RIGHT_PWM, PIN_MOTOR_RIGHT_DIGITAL, PIN_MOTOR_RIGHT_SENSOR);
Sonar sonar(PIN_SONAR_TRIG, PIN_SONAR_ECHO);
// RF24 radio(PIN_RADIO_CE, PIN_RADIO_CSN);

// void _radio_init() {
//     radio.begin();
//     radio.setChannel(30);
//     radio.setDataRate(RF24_1MBPS);
//     radio.setPALevel(RF24_PA_HIGH);
//     radio.openReadingPipe(1, 0x7878787878LL);
//     radio.startListening();
// }

void leftMotorInterrupt() { leftMotor.handleSensorInterrupt(); }

void rightMotorInterrupt() { rightMotor.handleSensorInterrupt(); }

void timer_handle_interrupts(int timer) {
    leftMotor.timer();
    rightMotor.timer();
}

void setup() {
    Serial.begin(115200);
    //_radio_init();

    timer_init_ISR_1KHz(TIMER_DEFAULT);

    attachInterrupt(digitalPinToInterrupt(PIN_MOTOR_LEFT_SENSOR), leftMotorInterrupt, RISING);
    attachInterrupt(digitalPinToInterrupt(PIN_MOTOR_RIGHT_SENSOR), rightMotorInterrupt, RISING);
}


int mode = 0;
int distance = 0;
int lastDistance = 0;
bool turnDirection = true;
int stuckCounter = 0;
int turnLength = (int) ((2 * PI * TRACK_WIDTH) / 4 * 100);


void loop() {
//    leftMotor.setVelocity(0.5);
//
//    struct debugInfo debug = leftMotor.debug();
//    Serial.print(debug.throttle);
//    Serial.print(" ");
//    Serial.print(debug.velocity * 255);
//    Serial.print(" ");
//    Serial.println(debug.targetVelocity * 255);
//    delay(10);


    timer_stop_ISR(TIMER_DEFAULT);
    distance = sonar.getDistance();
    timer_init_ISR_1KHz(TIMER_DEFAULT);

    if (distance < 15) {
        mode = 1;
    }

    if (distance == lastDistance) {
        stuckCounter++;
    } else {
        stuckCounter = 0;
    }

    if (stuckCounter > 10) {
        mode = 2;
    }

    switch (mode) {
        case 0:
            leftMotor.setVelocity(0.7);
            leftMotor.setDirection(true);
            rightMotor.setVelocity(0.7);
            rightMotor.setDirection(true);
            break;

        case 1:
            leftMotor.setVelocity(0);
            leftMotor.setDirection(true);

            rightMotor.moveCm(turnLength, 0.5, false);
            mode = 0;
            break;

        case 2:
            leftMotor.setVelocity(1);
            leftMotor.setDirection(false);
            rightMotor.setVelocity(1);
            rightMotor.setDirection(false);
            delay(150);
            break;
    }

    lastDistance = distance;
    delay(20);
}

