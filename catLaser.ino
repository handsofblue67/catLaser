#include <Arduino.h>
#include <VarSpeedServo.h>

VarSpeedServo yServo;  // create servo object to control a servo
VarSpeedServo xServo;

const int yServoPin = 8; // the digital pin used for the first servo
const int xServoPin = 9; // the digital pin used for the second servo

int xMin = 45;
int yMin = 20;
int xMax = 135;
int yMax = 72;

int xRand;
int yRand;

int rate;

int xPos;
int yPos;

void setup() 
{
	yServo.attach(yServoPin);
	xServo.attach(xServoPin);
	yServo.write(yMin + yMax / 2, 255, false); //center servos
	xServo.write(xMin + xMax / 2, 255, true);  

	//noise from analog pin 0 will create a 'random' seed for RNG
	randomSeed(analogRead(0));
}

void loop()
{

	yRand = random(yMin, yMax);
	xRand = random(xMin, xMax);

	yPos = yServo.read();
	xPos = xServo.read();
	rate = tangent();
	yServo.write(yRand, rate, false);
	xServo.write(xRand, rate, true);

	delay(random(500, 4000));
}

int tangent()
{
	return 147 - (48 * tan(yPos * 3.14 / 180));
}

