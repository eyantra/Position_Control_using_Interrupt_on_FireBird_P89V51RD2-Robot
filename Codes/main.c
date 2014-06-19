/***************************************************************************************************

This programme counts pulses from the left shaft encoder, compares it with required distance value and takes appropriate action

*****************************************************************************************************/

#include <intrins.h>
#include "p89v51rx2.H"

//program specific declarations
sbit LB=P1^0;	    //LEFT BACK Pin.
sbit LF=P1^1;		//LEFT FRONT Pin.
sbit RF=P1^2;		//RIGHT FRONT Pin.
sbit RB=P3^4;		//RIGHT BACK Pin.

sbit left_velocity=P1^3;  //Left velocity control pin. 
sbit right_velocity=P1^4; //Right velocity control pin. 

unsigned int left_shaft_count=0;//initialize a variable to store the count pulses of the left position encoder.

//initialization routine for Interrupt 1 setup.
void int1_setup() //Initalisation of the Int 1 should be done in the same sequence as given in this function.
{
TCON = 0x04; //Set Interrupt 1 trigger type to falling edge trigger.
IEN0 = 0x84; //External Interrupt 1 Enable and Global Interrupt Enable. 

P3 = 0x08; //set P3.3 (INT 1) as input port.
		   // Can also be written as INT1 = 0; (refer to p89v51rx2.H).
}


//ISR for external Interrupt 1
void int1_isr(void)interrupt 2     //ISR Routine for External Interrupt 1.
								   //Refer to chapter 4 in the software manual (table 4.1)for syntax explanation.
{
 left_shaft_count++;//when the slotted disc passes through the encoder pulses will be generated,each pulse will generate an interrupt and the right shaft count value will be incremented.
 IE1 = 0; // When interrupt is processed it is cleared automatically by the hardware  or it can also be cleared by the software.
 		  // Clearing IE1 flag to 0.	
}

// function for forward motion. 
void forward(void)
{
RF=1;
LF=1;
RB=0;
LB=0;
}

// function to stop robot.
void stop(void)
{
RF=0;
LF=0;
RB=0;
LB=0;
}

void main()
{ 
 unsigned int reqd_shaft_count_int=0;
 unsigned int distance=0;

 distance=100;  //Enter here the distance to be travelled in mm.     
 
 reqd_shaft_count_int=(unsigned int)(distance * 100 / 544);  //This equation will calculate the count required for distance to be travelled.
 
 int1_setup();//external linterrupt 1 initialization.
  					
 left_velocity = 1;	 //setting motor enable pines to 1
 right_velocity = 1; //setting motor enable pines to 1
 forward();	  //go forward
                              
 while(left_shaft_count<reqd_shaft_count_int); //wait till desired distance is covered.

 stop();	 //stop motor				  		

 while(1);
 
}//main ends                

