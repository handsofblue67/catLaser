#include <Arduino.h>
#include <Servo.h>

const int rate = 20;

Servo servoY;
Servo servoX;

int randX;
int randY;
  
int posX;
int posY;

void setup() 
{	
  // initialize the servos
  servoY.attach(8);
  servoX.attach(9);
  
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
        while(posX != randX || posY != randY)
	{    
		if(posX < randX)
		{
		    servoX.write(++posX);      
		    delay(posY-rate);
		}
    
		else if (posX > randX)
		{
			servoX.write(--posX);
			delay(posY-rate);
		}

		if(posY < randY)
		{
			servoY.write(posY++);
			delay(posY-rate);
		}
    
		else if (posY > randY)
		{
			servoY.write(posY--);
			delay(posY-rate);
		}
	}
}

void randomize()
{
        randY = random(35, 75);
	randX = random(45, 135);
 	
        posY = servoY.read();	
        posX = servoX.read();
        
        move();
}
