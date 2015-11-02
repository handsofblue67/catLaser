#include <Arduino.h>
#include <Servo.h>
enum Side{XMIN, XMAX, YMIN, YMAX};

Servo yServo;
Servo xServo;

Side side;

const int yServoPin = 8; // the digital pin used for the first servo
const int xServoPin = 9; // the digital pin used for the second servo
const byte led = 13; 

volatile bool willCalibrate = false; //this is checked each time through loop, changed by interrupt on bPin0
volatile bool state = false; //this the switch that progresses the calibration through each phase, changed by interrupt on bPin1

const byte bPin0 = 2;
const byte bPin1 = 3;
const byte bPin2 = 4;
const byte bPin3 = 5;


int xMin = 45;
int yMin = 50;
int xMax = 165;
int yMax = 100;

int xDest;
int yDest;
int rate = 100;
int xPos;
int yPos;

void calibrate()
{
	willCalibrate = true;
}

void cycle()
{
	state = true;
}

void setup()
{
	digitalWrite(bPin0, HIGH);
	digitalWrite(bPin1, HIGH);
	attachInterrupt(0, calibrate, CHANGE);
	attachInterrupt(1, cycle, CHANGE);
	pinMode(bPin2, INPUT);
	pinMode(bPin3, INPUT);

	// initialize the servos
	yServo.attach(8);
	xServo.attach(9);

	//noise from analog pin 0 will create a 'random' seed for RNG
	randomSeed(analogRead(0));
}

void loop()
{
	randomize();
}

void move()
{
	while (xPos != xDest || yPos != yDest)
	{
		if (willCalibrate == true)
		{
			digitalWrite(led, HIGH);
			changeRange();
		}
		
		if (xPos < xDest)
		{
			xServo.write(++xPos);
			delay(rate);
		}

		else if (xPos > xDest)
		{
			xServo.write(--xPos);
			delay(rate);
		}

		if (yPos < yDest)
		{
			yServo.write(++yPos);
			delay(calcRate());
		}

		else if (yPos > yDest)
		{
			yServo.write(--yPos);
			delay(calcRate());
		}
	}
}

void randomize()
{
	yDest = random(yMin, yMax);
	xDest = random(xMin, xMax);
	move();
}

//function that is called after the interrupt pin 2 is changed
//cycles through all four extremes
//each of the four extremes is moved closer to center by pulling up pin 4 and further from center by pulling up pin 5, cycle by pulling up pin 3
void changeRange()
{
	//adjust xMin block
	side = XMIN;
	showSide();
	state = false;//interrupt from bPin1 will switch this
	while (state == false && xMin > 0)
	{
		if (digitalRead(bPin2) == HIGH)
		{
			xServo.write(xPos = --xMin);
			delay(rate);
		}

		else if (digitalRead(bPin3) == HIGH)
		{
			xServo.write(xPos = ++xMin);
			delay(rate);
		}
	}

	//adjust xMax block
	digitalWrite(led, LOW);
	side = XMAX;
	showSide();
	state = false;
	digitalWrite(led, HIGH);
	while (state == false && xMax < 180)
	{
		if (digitalRead(bPin2) == HIGH)
		{
			xServo.write(xPos = ++xMax);
			delay(rate);
		}

		else if (digitalRead(bPin3) == HIGH)
		{
			xServo.write(xPos = --xMax);
			delay(rate);
		}
	}

	//adjust yMin block
	digitalWrite(led, LOW);
	side = YMIN;
	showSide();
	state = false;
	digitalWrite(led, HIGH);
	while (state == false && yMin > 0)
	{
		if (digitalRead(bPin2) == HIGH)
		{
			yServo.write(yPos = --yMin);
			delay(calcRate());
		}

		else if (digitalRead(bPin3) == HIGH)
		{
			yServo.write(yPos = ++yMin);
			delay(calcRate());
		}
	}

	//adjust yMax block
	digitalWrite(led, LOW);
	side = YMAX;
	showSide();
	state = false;
	digitalWrite(led, HIGH);
	while (state == false && yMax < 180)
	{
		if (digitalRead(bPin2) == HIGH)
		{
			yServo.write(yPos = ++yMax);
			delay(calcRate());
		}

		else if (digitalRead(bPin3) == HIGH)
		{
			yServo.write(yPos = --yMax);
			delay(calcRate());
		}
	}
	
	displayPath();
	digitalWrite(led, LOW);
	willCalibrate = false;
	state = false;
}

//outline the area of operation
void displayPath()
{
	center();

	xDest = xMax;
	yDest = yMin;
	move();

	yDest = yMax;
	move();

	xDest = xMin;
	move();

	yDest = yMin;
	move();

	xDest = xMax;
	move();
	
	center();
}

//center the servos
void center()
{
	xDest = (xMin + xMax) / 2;
	yDest = (yMin + yMax) / 2;
	move();
}

//calculate a delay time that increases with y, a work in progress
int calcRate()
{
	double yPosR = degreesToRadians(yPos);
	double yMinR = degreesToRadians(yMin);
	double yMaxR = degreesToRadians(yMax);
	rate = 150 * sq(cos((yMinR * PI + yPosR) / (yMaxR - yMinR))) + 1;
	return rate;
}

//convert degrees to radians
double degreesToRadians(int degree)
{
	return (degree * PI / 180);
}
 
//display sides borders
//to be called by the changeRange function
void showSide()
{
	switch(side)
	{
		case XMIN:
		{
			xDest = xMin;
			yDest = yMax;
			move();
			yDest = yMin;
			move();
			yDest = (yMax + yMin) / 2;
			move();
			break;
		}

		case XMAX:
		{
			xDest = xMax;
			move();
			yDest = yMin;
			move();
			yDest = yMax;
			move();
			yDest = (yMax + yMin) / 2;
			move();
			break;
		}

		case YMIN:
		{
			yDest = yMin;
			move();
			xDest = xMin;
			move();
			xDest = xMax;
			move();
			xDest = (xMax + xMin) / 2;
			move();
			break;
		}

		case YMAX:
		{
			yDest = yMax;
			move();
			xDest = xMin;
			move();
			xDest = xMax;
			move();
			xDest = (xMax + xMin) / 2;
			move();
			break;
		}
	}
}
