/********************************/
/*      Proteus Test Code       */
/*     OSU FEH Spring 2020      */
/*        Drew Phillips         */
/*    02/03/20  Version 3.0.1     */
/********************************/

// AM 02/03/20

/* Include preprocessor directives */
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include <FEHMotor.h>
#include <FEHServo.h>
#include <FEHAccel.h>
#include <FEHBattery.h>
#include <FEHBuzzer.h>
#include <FEHRPS.h>
#include <FEHSD.h>
#include <string.h>
#include <stdio.h>
// RPS Delay time
#define RPS_WAIT_TIME_IN_SEC 0.35

// Shaft encoding counts for CrayolaBots
#define COUNTS_PER_INCH 40.5
#define COUNTS_PER_DEGREE 2.48

// Defines for pulsing the robot
#define PULSE_TIME .1
#define PULSE_POWER 25

// Define for the motor power
#define POWER 25

// Orientation of QR Code
#define PLUS 0
#define MINUS 1

//90 degree = 3.34 inches, 136 counts
//45 dergree = 1.66 inches, 68 counts

//eclarations of optical sensors and cds cells and bump switches
AnalogInputPin right_opt(FEHIO::P1_3);
AnalogInputPin mid_opt(FEHIO::P1_5);
AnalogInputPin left_opt(FEHIO::P1_7);
AnalogInputPin cdsCell(FEHIO::P3_7);
    
DigitalInputPin topLeftSwitch(FEHIO::P3_0);
DigitalInputPin topRightSwitch(FEHIO::P0_7);
DigitalInputPin bottomLeftSwitch(FEHIO::P3_1);
DigitalInputPin bottomRightSwitch(FEHIO::P0_1);
float countsPerInch = 40.5;
float countsPerDegree = 2.48;

//minimum value to detect line, sensor value must be greater to trigger line sensors
// float leftCriticalValue = 2.0;
 float leftCriticalValue = 5;

float rightCriticalValue = 1.0;
float midCriticalValue = 2.7;


//limit value to detect light, sensor value must be lower to trigger light sensor
float cdsCriticalValue = 1.7;
//values lower, but still higher than red will be treated as blue
int cdsBlue;
//values lower will be treated as red
float cdsRed = 0.8;

//rps Coordinates of light is X:10.6, Y:15.7

//Declarations for encoders & motors
 DigitalEncoder right_encoder(FEHIO::P1_0);
 DigitalEncoder left_encoder(FEHIO::P2_0);
 
FEHMotor right_motor(FEHMotor::Motor0,9.0);
//left motor is inverted
FEHMotor left_motor(FEHMotor::Motor1,9.0);
FEHMotor new_motor(FEHMotor::Motor2,9.0);
//arm servo, max is up, min is down
FEHServo arm_servo(FEHServo::Servo3);
//min is up, max is down
FEHServo slide_servo(FEHServo::Servo7);
//flip servo
FEHServo flip_servo(FEHServo::Servo0);

bool senseLine();
void moveTillCollision(int);
void moveTillLine();
bool senseLight();
void followLine();
void turn_right(int , int );
void turn_left(int , int );
void turnToLine(int,int);
void move_forward(int, int);
void move_forwardTime(int, float);

void iceCreamToFinalButton();
void move_forwardPT4(int, int);

void goFromJukeBoxToRamp();
void jukeBoxButton();
void goUpRamp();
void trayDeposit();
void ticketSlide();
void burgerFlip();
void setUpServo();
void iceCreamBonus();
void moveUpRampShaftEncoding();
void trayToTickey();
void sliderToFlip();
void leverFlip(int);
void reverseTicket(int);
void leverToJukebox();
void check_heading(float );
void check_y(float , int );
void check_x(float , int );
void turn_counterclockwise(int , int ) ;
void pulse_counterclockwise(int , float ) ;
void pulse_forward(int , float ) ;
void finalButton();




void testSensors(){
    LCD.Write("CDS value: ");
    LCD.WriteLine(cdsCell.Value());
    LCD.Write("mid value"); 
    LCD.WriteLine(mid_opt.Value());
    LCD.Write("right value");
    LCD.WriteLine(right_opt.Value());
    LCD.Write("left value");
    LCD.WriteLine(left_opt.Value());
    LCD.Write("top left switch: ");
    LCD.WriteLine(topLeftSwitch.Value());
    LCD.Write("top right switch: ");
    LCD.WriteLine(topRightSwitch.Value());
    LCD.Write("bottom left switch");
    LCD.WriteLine(bottomLeftSwitch.Value());
    LCD.Write("bottom right switch");
    LCD.WriteLine(bottomRightSwitch.Value());
    Sleep(3.0);
}

void setUpForJukeBox(){
    //PseudoCode for JukeBox
    //Robot waits until it senses the start light
    //  while(!senseLight()){

    // }
    //Robot moves rotates slightly, then moves forward until it encounters the line
    
    //moves robot forweard to clear starting box, and turns it
    
    right_motor.SetPercent(25);
    left_motor.SetPercent(-25);
    

    Sleep(2.0);
    
    LCD.WriteLine("Moved");

     right_motor.SetPercent(25);
     left_motor.SetPercent(25);
     Sleep(.2);
    

    // LCD.WriteLine("Turned");
    //runs until it hits line
    while(!senseLine()){
        right_motor.SetPercent(25);
        left_motor.SetPercent(-25);
    }
    // LCD.Write("mid value");
    // LCD.WriteLine(mid_opt.Value());
    // LCD.Write("right value");
    // LCD.WriteLine(right_opt.Value());
    // LCD.Write("left value");
    // LCD.WriteLine(left_opt.Value());
    
    LCD.WriteLine("Found Line");
   
    // left_motor.SetPercent(0);
    // right_motor.SetPercent(0);
    // Sleep(2.0);
    //Robot turns to face jukebox
    right_motor.SetPercent(25);
    left_motor.SetPercent(25);
    Sleep(2.0);
    right_motor.SetPercent(0);
    left_motor.SetPercent(0);


    //Robot follows the line until it ends
    followLine();


     LCD.WriteLine("Turned");
}


void turnTester(double inches){
    int turnRad = (int) (inches * 41);
    turn_right(25, turnRad);
    LCD.WriteLine(turnRad);
    Sleep(3.0);
}

void setUpServo(){
    //armServo starts upright
    arm_servo.SetMin(650);
    arm_servo.SetMax(1830);
    arm_servo.SetDegree(180);
    //slide servo starts down
    slide_servo.SetMin(540);
    slide_servo.SetMax(2311);
    slide_servo.SetDegree(0);
    //initialize flip servo servo, 90 degrees is horizontal
    flip_servo.SetMin(750);
    flip_servo.SetMax(2325);
    flip_servo.SetDegree(80);
}


void trayDeposit(){
    //set arm down so it doesn't colide with lever
    slide_servo.SetDegree(180);
// goes until it hits left wall
 moveTillCollision(0);
 //backs up 
move_forwardPT4(-45,(int) (3.4 * countsPerInch));
 //turns towards sink and resets arm
turn_left(25,(int) (countsPerDegree*100));
Sleep(.3);
slide_servo.SetDegree(0);
 
 
 moveTillCollision(0);
    LCD.Write("top left switch: ");
    LCD.WriteLine(topLeftSwitch.Value());
    LCD.Write("top right switch: ");
    LCD.WriteLine(topRightSwitch.Value());
 //robot deposits tray
arm_servo.SetDegree(50);
Sleep(1.0);
arm_servo.SetDegree(170);
}



void trayToTicketSlide() {
    //robot backs up (around 8 inches to prevent part collision)
    move_forwardPT4(-35, (int)(9 * countsPerInch));
    
    //robot turns 47 degrees left
    turn_left(35, (int)(47 * countsPerDegree));

    //robot moves 15 inches forward
    move_forwardPT4(35, (int)(20 * countsPerInch));
    slide_servo.SetDegree(180);

    //robot turns 47 degrees left 
    turn_left(15, (int)(30* countsPerDegree));
    

    //adjusts heading
    //check_heading(345);
    //adjusting code for higher precision
    // move_forwardPT4(25,(int)(15 * countsPerInch));
    // turn_left(25,(int)(20 * countsPerDegree));
    
    //robot moves forward until it rams the wall
    moveTillCollision(0);   
    //robot readjusts itself
    // move_forwardPT4(-25,(int)(2 * countsPerInch));
    // turn_left(25, (int) (25 * countsPerDegree));
    // moveTillCollision(0);


    //robot extends "pusher"
    slide_servo.SetDegree(95);
    Sleep(.2);
    //Robot backs up
    // move_forwardPT4(-35, (int)(3* countsPerInch));  
    // turn_right(50, (int) (10 * countsPerDegree));  
    // move_forwardPT4(-35, (int)(5* countsPerInch)); 
move_forwardPT4(-40,(int)(5 * countsPerInch));
left_motor.SetPercent(-40);
right_motor.SetPercent(-40);
Sleep(1.0);
left_motor.Stop();
right_motor.Stop();
// move_forwardPT4(25,(int) (.5 * countsPerInch));
// left_motor.SetPercent(-40);
// right_motor.SetPercent(-40);
// Sleep(1.0);
// left_motor.Stop();
// right_motor.Stop();


}


void sliderToFlip() {
    //reset slide arm
    slide_servo.SetDegree(180);
    //turn towards the line path following along burger flip
    move_forwardPT4(-25,(int) (5 * countsPerInch));
    turn_left(25, (int) (140* countsPerDegree));
    setUpServo();
    //drive forward till that line
    moveTillLine();

    //turn right to get aligned with wall
    turn_right(25, (int) (100 * countsPerDegree));

    //move to the wall to adjust
    moveTillCollision(0);

    //flip the burger
    burgerFlip();
}


void moveUpRampPT4(){
    move_forwardPT4(45, (int) (15.5 * countsPerInch));
    Sleep(.5);
    turn_right(25, (int) (55 * countsPerDegree));
    Sleep(.5);
    move_forwardPT4(60, (int) (37.75 * countsPerInch));
    
    Sleep(.5);
    turn_left(25,(int) (100* countsPerDegree));
    Sleep(.5);
    //corrects position
    if((RPS.Y() > 0) && RPS.X() > 0){
        check_heading(180);
    }

    LCD.WriteLine("Moved Up ramp");
   left_motor.SetPercent(0);
   right_motor.SetPercent(0);
}



void burgerFlip(){
    //backing up a certain distance
    move_forwardPT4(-25,(int) (2.5* countsPerInch));

    //turning to alig with burger flip
    turn_right(25, (int) (110 * countsPerDegree));
    check_heading(270);
    //moving back for flipping burger
    move_forwardTime(-35,2.0);

    //setting up the servo and flipping the burger
    flip_servo.SetDegree(180); 
    Sleep(2.0);
    flip_servo.SetDegree(60); 
}



void leverFlip(int lever){
    Sleep(.5);
    move_forwardPT4(-25,(int) 3 * countsPerInch);

    //turning according to which line to follow
    if(lever == 0){//if left lever is the target
        turn_left(25,(int) (35 * countsPerDegree));
    }  else if(lever == 2){//if right lever is the target
        turn_right(25,(int) (35 * countsPerDegree));
        
    }

    //move till a line is found

  
    
    //follow the line till the end of the line
    //followLine();
    if(lever == 0){//if left lever is the target
        moveTillLine();

        move_forwardPT4(25,(int)(4 * countsPerInch));    

        turn_right(25,(int) (30 * countsPerDegree));
        
    }  else if(lever == 2){//if right lever is the target
        moveTillLine();

        move_forwardPT4(25,(int)(4 * countsPerInch));    

        turn_left(25,(int) (35 * countsPerDegree));
        move_forwardPT4(-35, (int)(3 * countsPerInch));

        //move_forwardPT4(35, (int)(2 * countsPerInch));
    } else{
             //turn_left(25,(int) (5 * countsPerDegree));
            move_forwardPT4(35, (int)(4 * countsPerInch));

    }
    LCD.WriteLine("Adjusted");
    //move to hit lever 
    move_forwardPT4(25, (int) (5.5 * countsPerInch));
    
    //pulling arm down
    arm_servo.SetDegree(95);
    Sleep(0.5);
    //back up
    move_forwardPT4(-25, (int) (5.5 * countsPerInch));

    //reset arm 
    arm_servo.SetDegree(0);
    Sleep(0.5);

    move_forwardPT4(25, (int) (5.5 * countsPerInch));
    Sleep(4.0);
    //raise lever arm
    arm_servo.SetDegree(70);
    Sleep(1.0);

    // //move forward for some time
    // move_forwardTime(25, 1.5);
    // Sleep(1.0);

    //set arm to a lowered degree to push lever back up
    // arm_servo.SetDegree(70);
    // Sleep(0.5);

    //moving backwards for lowering the lever
    move_forwardPT4(-25,(int) (3.5 * countsPerInch));
    arm_servo.SetDegree(180);
}
void burgerToLever(){

    move_forwardPT4(45,(int) (10 * countsPerInch));
    turn_right(45,(int) (90 * countsPerDegree));
    moveTillCollision(1);
    move_forwardPT4(45,(int) (18 * countsPerInch));
    turn_right(45, (int)(55 * countsPerDegree));
    move_forwardPT4(-45, (int)(3.5 * countsPerInch));
    leverFlip(RPS.GetIceCream());

}

void leverToJukebox() {
    //move backwards from lever area
    move_forwardPT4(-35, (int) (6.0 * countsPerInch));

    //turning to face wall close to burger_flip
    turn_left(25, (int) (45 * countsPerDegree));
    setUpServo();

    //move to align with wall
    moveTillCollision(1);
    
    //moving back to go down the ramp
    move_forwardPT4(45, (int)(14.5* countsPerInch)); 
    
    //turning towards thr ramp
    turn_left(25, (int) (110 * countsPerDegree)); 
    check_heading(180);
     //moving forward down the ramp till the wall close to the red button
    move_forwardPT4(35,(int) (37* countsPerInch));
    turn_left(25,(int) (45 * countsPerDegree));
    move_forwardPT4(35,(int) (2 * countsPerInch));
    turn_right(25,(int) (45 * countsPerDegree));

    moveTillCollision(0);
}

int main() {
   
     while(true){
        LCD.Write("Heading: ");
        LCD.WriteLine(RPS.Heading());
        Sleep(1.5);

     }

    // while(true){
    //     testSensors();
    // }
    //     RPS.InitializeTouchMenu();

    // while(true){
    //    LCD.WriteLine("X: ");
    //    LCD.WriteLine(RPS.X());
    //     LCD.WriteLine("Y: ");
    //    LCD.WriteLine(RPS.Y());

    //     Sleep(1.5);
    // }
    RPS.InitializeTouchMenu();
    LCD.WriteLine(RPS.GetIceCream());
    t_now = TimeNow();
    while(!senseLight() && (TimeNow()-t_now < 30.0)){

    }

    
    setUpServo();

    moveUpRampPT4();
    trayDeposit();
    trayToTicketSlide();
    sliderToFlip();
    burgerToLever();
    leverToJukebox();
    jukeBoxButton();
    finalButton();
    //RPS.GetIceCreamLever 0-left, 1-mid, 2-right

}

int RPSProgram(){
    //Declare variables
    float touch_x, touch_y;
    int n;
    char points[4] = {'A','B','C','D'};

    //Call this function to initialize the RPS to a course
    RPS.InitializeTouchMenu();

    //Open SD file for writing
    FEHFile *fptr = SD.FOpen("RPS_Test.txt","w");

    //Wait for touchscreen to be pressed and released
    LCD.WriteLine("Press Screen to Start");
    while(!LCD.Touch(&touch_x, &touch_y));
    while(LCD.Touch(&touch_x, &touch_y));

    //Clear screen
    LCD.Clear();

    //Write initial screen info
    LCD.WriteRC("X Position:",2,0);
    LCD.WriteRC("Y Position:",3,0);
    LCD.WriteRC("   Heading:",4,0);

    //Step through each path point to record position and heading
    for (n=0; n<=3; n++)
    {
        //Write point letter
        LCD.WriteRC("Touch to set point ",0,0);
        LCD.WriteRC(points[n],0,20);

        //Wait for touchscreen to be pressed and display RPS data
        while(!LCD.Touch(&touch_x, &touch_y))
        {
            LCD.WriteRC(RPS.X(),2,12); //update the x coordinate
            LCD.WriteRC(RPS.Y(),3,12); //update the y coordinate
            LCD.WriteRC(RPS.Heading(),4,12); //update the heading

            Sleep(10); //wait for a 10ms to avoid updating the screen too quickly
        }
        while(LCD.Touch(&touch_x, &touch_y));

        //Print RPS data for this path point to file
        SD.FPrintf(fptr, "%f %f\n", RPS.X(), RPS.Y());
    }

    //Close SD file
    SD.FClose(fptr);

    return 0;
}

void jukeBoxButton(){
    float light = 0;
    //robot navigaets to left wall
    move_forwardPT4(-35,(int)(13.5* countsPerInch));
    turn_right(25,(int) (115 * countsPerDegree));
    moveTillCollision(0);
    //robot navigates to light
    move_forwardPT4(-35,(int) (4* countsPerInch));
    turn_left(25,(int) (105 * countsPerDegree));
    move_forwardPT4(25,(int) (1*countsPerInch));
    //robot senses light
    //false is blue, true is red
    Sleep(.5);
    bool lightColor = false;
    LCD.Write("CDS: ");
    LCD.WriteLine(cdsCell.Value());
    // //Robot detects the light color, default color is blue
     if(cdsCell.Value() < cdsRed){

         lightColor = true;
     }
     LCD.Write("CDS: ");
     LCD.WriteLine(lightColor);

    // //Robot turns left(blue) or right(red), depending on the light color
      //move_forwardPT4(-25,(int)(3 * countsPerInch));

if(lightColor){//turn  45 degrees right
        turn_right(25,(int) (55 * countsPerDegree));
        move_forwardPT4(25,(int)(2 * countsPerInch));
        turn_left(25,(int) (55 * countsPerDegree));

     }else{//turn left
        turn_left(25,(int) (55 * countsPerDegree));
        move_forwardPT4(25,(int)(2 * countsPerInch));
        turn_right(25,(int) (55 * countsPerDegree));

    }
    //backs up an dlowers arm
    move_forwardPT4(-25,(int) (3 * countsPerInch));
    arm_servo.SetDegree(50);
    Sleep(.5);
    //rams button
    move_forwardTime(25,2.0);
}
void finalButton(){
    setUpServo();
    move_forwardPT4(-25,(int)(2 * countsPerInch));
    turn_left(55, (int)(100 * countsPerDegree));
    // moveTillCollision(1);
    move_forwardPT4(45, (int)(9 * countsPerInch));
    turn_right(45,(int) (45* countsPerDegree));
    move_forwardTime(45, 10);

}


//Returns true if a line is sensed, false otherwise
bool senseLine(){
     if(left_opt.Value() > leftCriticalValue || right_opt.Value() > rightCriticalValue || mid_opt.Value() > midCriticalValue){
         LCD.WriteLine("Line Sensed");
         return true;

     }else{
         return false;
     }
}


//Returns true if a light is sensed, does not differentiate different colors of light
bool senseLight(){
    //If a light is sensed, return true
    if(cdsCell.Value() < cdsCriticalValue){
        return  true;
    } else{
        return false;
    }

}

enum LineStates {
    MIDDLE,
    RIGHT,
    LEFT
};

//Follows the line
void followLine(){
    
    int state = MIDDLE; // Set the initial state
    float t_now;
    t_now = TimeNow();
    while (senseLine() && (topLeftSwitch.Value() && topRightSwitch.Value())) { // I will follow this line until it ends or until one of the top bump switches is presed
        switch(state) {
            // If I am in the middle of the line...
            case MIDDLE:
                // Set motor powers for driving straight
                /* Drive */

                left_motor.SetPercent(-25);
                right_motor.SetPercent(25);

                if (right_opt.Value() > rightCriticalValue)  {
                    state = RIGHT; // update a new state
                }
                /* Code for if left sensor is on the line */

                if (left_opt.Value() > leftCriticalValue)  {
                    state = LEFT; // update a new state
                }
                
                break;
                

            // If the right sensor is on the line...
            case RIGHT:
                left_motor.SetPercent(-25);
                right_motor.SetPercent(5);
                /* Drive */
                if( mid_opt.Value() > midCriticalValue ) {
                    state = MIDDLE;
                }

                if (left_opt.Value() > leftCriticalValue)  {
                    state = LEFT; // update a new state
                }
                break;

            // If the left sensor is on the line...
            case LEFT:
                left_motor.SetPercent(-5);
                right_motor.SetPercent(25);


                if( mid_opt.Value() > midCriticalValue ) {
                    state = MIDDLE;
                }

                if (right_opt.Value() > rightCriticalValue)  {
                    state = RIGHT; // update a new state
                }

            /* Mirror operation of RIGHT state */
                break;

            default: // Error. Something is very wrong.
                left_motor.SetPercent(0);
                right_motor.SetPercent(0);
                break;

        }
// Sleep a bit
        }
        left_motor.SetPercent(0);
        right_motor.SetPercent(0);

        LCD.WriteLine("Line followed");
        
        
}

void moveTillLine() {
    int motor_percent = 25;

    while(!senseLine()) {
        left_motor.SetPercent(-motor_percent + 2);
        right_motor.SetPercent(motor_percent);
    }
    left_motor.Stop();
    right_motor.Stop();
    
    
}

//forward = 0, backward = 1
void moveTillCollision(int option){
    float t_now;

    if(option == 1){
        while(bottomLeftSwitch.Value() && bottomRightSwitch.Value()){
            left_motor.SetPercent(23);
            right_motor.SetPercent(-25);
        }
        //if the left switch collides first, turn off the left wheel
        // else if the right switch collides first, turn off the right wheel
        if(bottomLeftSwitch.Value() == 0){
            left_motor.SetPercent(0);
        } else if(bottomRightSwitch.Value() == 0){
            right_motor.SetPercent(0);
        }
        //continue trying to hit the wall until both switches are triggered, or 5 seconds have passed
        t_now = TimeNow();
        while((bottomLeftSwitch.Value() || bottomRightSwitch.Value()) && (TimeNow()-t_now < 3.0)){        
            LCD.WriteLine("Loop");
            LCD.WriteLine(TimeNow());
        }

    } else{
        //robot moves forward while both buttons are unpressed
        while(topLeftSwitch.Value() && topRightSwitch.Value()){
            left_motor.SetPercent(-23);
            right_motor.SetPercent(25);
        }
        //if the left switch collides first, turn off the left wheel
        // else if the right switch collides first, turn off the right wheel
        if(!(topLeftSwitch.Value())){
            left_motor.SetPercent(0);
        } else if(!(topRightSwitch.Value())){
            right_motor.SetPercent(0);
        }
        //continue trying to hit the wall until both switches are triggered, or 5 seconds have passed
        t_now = TimeNow();
        while((topLeftSwitch.Value() || topRightSwitch.Value()) && (TimeNow()-t_now < 3.0)){        
            LCD.WriteLine("Loop");
            LCD.WriteLine(TimeNow());
        }


    }
    left_motor.Stop();
    right_motor.Stop();
}

void move_forwardTime(int percent, float time) //using encoders
{


    //Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(-1*percent);
    Sleep(time);
    

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}
void move_forwardPT4(int percent, int counts) //using encoders
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(percent);
    if(percent < 0) {
        left_motor.SetPercent((-1*percent) - 2);
    } else {
        left_motor.SetPercent((-1*percent) + 2);
    }
    

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

void turn_right(int percent, int counts) //using encoders
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    //hint: set right motor backwards, left motor forwards
    right_motor.SetPercent(-1*percent);
    left_motor.SetPercent(-1*percent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

void turn_left(int percent, int counts) //using encoders
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

/* 
 * Use RPS to move to the desired heading
 */
void check_heading(float heading)
{
    //You will need to fill out this one yourself and take into account
    //checking for proper RPS data and the edge conditions
    //(when you want the robot to go to 0 degrees or close to 0 degrees)

    /*
        SUGGESTED ALGORITHM:
        1. Check the current orientation of the QR code and the desired orientation of the QR code
        2. Check if the robot is within the desired threshold for the heading based on the orientation
        3. Pulse in the correct direction based on the orientation
    */
    // Check if receiving proper RPS coordinates and whether the robot is within an acceptable range
    while(( RPS.Heading() >= 0) && (RPS.Heading() < heading - 3 || RPS.Heading() > heading + 3))
    {   
        LCD.Write("Heading: ");
         LCD.WriteLine(RPS.Heading());
        if(RPS.Heading() > heading)
        {
            // Pulse the motors for a short duration in the correct direction
            pulse_forward(-PULSE_POWER, PULSE_TIME);
        }
        else if(RPS.Heading() < heading)
        {
            // Pulse the motors for a short duration in the correct direction
           pulse_counterclockwise(PULSE_POWER, PULSE_TIME);
        }
        Sleep(RPS_WAIT_TIME_IN_SEC);
    }
    LCD.Write("Heading: ");
         LCD.WriteLine(RPS.Heading());
}

/*
 * Pulse forward a short distance using time
 */
void pulse_forward(int percent, float seconds) 
{
    // Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(-percent);

    // Wait for the correct number of seconds
    Sleep(seconds);

    // Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

/*
 * Pulse counterclockwise a short distance using time
 */
void pulse_counterclockwise(int percent, float seconds) 
{
    // Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(percent);

    // Wait for the correct number of seconds
    Sleep(seconds);

    // Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

/*
 * Turn counterclockwise using shaft encoders where percent is the motor percent and counts is the distance to travel
 */
void turn_counterclockwise(int percent, int counts) 
{
    // Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    // Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(-percent);

    // While the average of the left and right encoder are less than counts,
    // keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    // Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

/* 
 * Use RPS to move to the desired x_coordinate based on the orientation of the QR code
 */
void check_x(float x_coordinate, int orientation)
{
    // Determine the direction of the motors based on the orientation of the QR code 
    int power = PULSE_POWER;
    if(orientation == MINUS){
        power = -PULSE_POWER;
    }

    // Check if receiving proper RPS coordinates and whether the robot is within an acceptable range
    while((RPS.X() > 0 ) && (RPS.X() < x_coordinate - 1 || RPS.X() > x_coordinate + 1))
    {
        if(RPS.X() > x_coordinate )
        {
            // Pulse the motors for a short duration in the correct direction
            pulse_forward(-power, PULSE_TIME);
        }
        else if(RPS.X() < x_coordinate)
        {
            // Pulse the motors for a short duration in the correct direction
            pulse_forward(power, PULSE_TIME);
        }
        Sleep(RPS_WAIT_TIME_IN_SEC);
    }
}

/* 
 * Use RPS to move to the desired y_coordinate based on the orientation of the QR code
 */
void check_y(float y_coordinate, int orientation)
{
    // Determine the direction of the motors based on the orientation of the QR code
    int power = PULSE_POWER;
    if(orientation == MINUS){
        power = -PULSE_POWER;
    }

    // Check if receiving proper RPS coordinates and whether the robot is within an acceptable range
    while(( RPS.Y() > 0) && (RPS.Y() < y_coordinate - 1.5 || RPS.Y() > y_coordinate + 1.5))
    {
        if(RPS.Y() > y_coordinate)
        {
            // Pulse the motors for a short duration in the correct direction
            pulse_forward(-power, PULSE_TIME);
        }
        else if(RPS.Y() < y_coordinate)
        {
            // Pulse the motors for a short duration in the correct direction
           pulse_forward(power, PULSE_TIME);
        }
        Sleep(RPS_WAIT_TIME_IN_SEC);
    }
}
