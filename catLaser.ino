#include <Arduino.h>
#include <Servo.h>

Servo servoY;
Servo servoX;

long rate;
  
long randX;
long randY;
  
int posX;
int posY;

void setup() 
{	
  // initialize the servos
  servoY.attach(8);
  servoX.attach(9);
  //Serial.begin(9600);
  
  //noise from analog pin 0 will create a 'random' seed for RNG
  randomSeed(analogRead(0));
}

void loop() 
{
        randomize();
	delay(1000);
}

void move()
{
        while(posX != randX || posY != randY)
	{    
/*              Serial.print("posX and y: ");
                Serial.println(posX);
                Serial.println(posY);
                Serial.print("\n");*/

		int xDiff = posX - randX;
		int yDiff = posY - randY;
		
                if(xDiff > 0)
		{
			
		        servoX.write(++posX);      
			if(posY < 50)
				delay(posY/4);
      
			else
				delay(-(posY/4));
		}
    
		else if (xDiff < 0)
		{
			servoX.write(--posX);
			if (posY < 50)
				delay(posY/4);

			else
				delay(-(posY/4));


			delay(1);
		}
		if(yDiff > 0)
		{
			servoY.write(posY++);
			if(posY < 50)
				delay(posY/4);
        
			else
				delay(-(posY/4));
		}
    
		else if (yDiff < 0)
		{
			servoY.write(posY--);
			if (posY < 50)
				delay(posY/4);

			else
				delay(-(posY/4));


			delay(1);
		}
		
	}
}

void randomize()
{
      	//rate = random(20, 60);
        
        randY = random(35, 75);
	randX = random(45, 135);
       
	 /*Serial.print("randX and y: ");
        Serial.println(randX);
        Serial.println(randY);
        Serial.print("Starting RNG\n");*/
 	
        posY = servoY.read();	
        posX = servoX.read();
        
        move();
}
