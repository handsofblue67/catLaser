#include <Arduino.h>
#include <VarSpeedServo.h>

VarSpeedServo yServo;  // create servo object to control a servo
VarSpeedServo xServo;

const int yServoPin = 8; // the digital pin used for the first servo
const int xServoPin = 9; // the digital pin used for the second servo
const int rate = 20;

int xRand;
int yRand;

int xPos;
int yPos;

void setup() {
	yServo.attach(yServoPin);  // attaches the servo on pin 8 to the servo object
	yServo.write(0, 255, false); // set the intial position of the servo, as fast as possible, run in background
	xServo.attach(xServoPin);  // attaches the servo on pin 9 to the servo object
	xServo.write(0, 255, true);  // set the intial position of the servo, as fast as possible, wait until done

	//noise from analog pin 0 will create a 'random' seed for RNG
	randomSeed(analogRead(0));
}

void loop()
{
	randomize();
	delay(2000);
}

void move()
{
	yServo.write(yRand, 48 * tan(yPos * 3.14 / 180) - 20, false);
	xServo.write(xRand, 48 * tan(yPos * 3.14 / 180) - 20, true);
}

void randomize()
{
	yRand = random(20, 72);
	xRand = random(45, 135);

	yPos = yServo.read();
	xPos = xServo.read();

	move();
}


