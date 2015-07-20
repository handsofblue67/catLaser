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

int xPos;
int yPos;

void setup() 
{
	yServo.attach(yServoPin);
	yServo.write(0, 255, false);
	xServo.attach(xServoPin);
	xServo.write(0, 255, true);  

	//noise from analog pin 0 will create a 'random' seed for RNG
	randomSeed(analogRead(0));
}

void loop()
{

	yRand = random(20, 72);
	xRand = random(45, 135);

	yPos = yServo.read();
	xPos = xServo.read();

	yServo.write(yRand, tangent(), false);
	xServo.write(xRand, tangent(), true);

	delay(2000);
}

int tangent()
{
	return 48 * tan(yPos * 3.14 / 180) - 20;
}

