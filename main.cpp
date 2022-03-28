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
 float leftCriticalValue = 2.7;

float rightCriticalValue = 1.0;
float midCriticalValue = 2.7;


//limit value to detect light, sensor value must be lower to trigger light sensor
float cdsCriticalValue = 1.7;
//values lower, but still higher than red will be treated as blue
int cdsBlue;
//values lower will be treated as red
float cdsRed = 0.8;

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
// goes until it finds the line
 moveTillLine();
 //follows line
 followLine();
    LCD.Write("top left switch: ");
    LCD.WriteLine(topLeftSwitch.Value());
    LCD.Write("top right switch: ");
    LCD.WriteLine(topRightSwitch.Value()); // robot alligns itself with sink
 moveTillCollision(0);
    LCD.Write("top left switch: ");
    LCD.WriteLine(topLeftSwitch.Value());
    LCD.Write("top right switch: ");
    LCD.WriteLine(topRightSwitch.Value()); // robot alligns itself with sink
 //robot deposits tray
arm_servo.SetDegree(50);
Sleep(2.0);
arm_servo.SetDegree(170);
}

void ticketSlide(){
//robot backs up
 //while(!senseLine()){
     left_motor.SetPercent(25);
    right_motor.SetPercent(-25);
 //}
 Sleep(1.0);
 
 //robot turns 45 degrees left
 left_motor.SetPercent(25);
 right_motor.SetPercent(25);
 Sleep(.65);
 //robot moves 5.5 inches forward
 left_motor.SetPercent(-25);
 right_motor.SetPercent(25);
 Sleep(2.8);
 //robot turns 45 degrees left 
 left_motor.SetPercent(25);
 right_motor.SetPercent(25);
 Sleep(.60);

 
 //robot moves forward until it rams the wall
 while(topLeftSwitch.Value() || topRightSwitch.Value()){
    left_motor.SetPercent(-25);
    right_motor.SetPercent(25);
 }   

 //robot extends "pusher"
 slide_servo.SetDegree(90);
 //Robot backs up
 left_motor.SetPercent(25);
 right_motor.SetPercent(-25);
 Sleep(2.0);

 //robot turns towards burger
 left_motor.SetPercent(25);
 right_motor.SetPercent(0);
 Sleep(1.75);
 slide_servo.SetDegree(0);
 while(topLeftSwitch.Value() || topRightSwitch.Value()){
    left_motor.SetPercent(-25);
    right_motor.SetPercent(25);
 }  
 left_motor.SetPercent(0);
right_motor.SetPercent(0);
}



void moveUpRampPT4(){
    move_forward(25, (int) (15.25 * countsPerInch));
    Sleep(3.0);
    turn_right(25, (int) (45 * countsPerDegree));
    Sleep(3.0);
    move_forward(60, (int) (25 * countsPerInch));
    Sleep(3.0);
    turn_left(25,(int) (95 * countsPerDegree));
    Sleep(3.0);
    
   left_motor.SetPercent(0);
   right_motor.SetPercent(0);
}

void burgerFlip(){
    move_forward(-25,(int) (4.5 * countsPerInch));
    turn_right(25, (int) (100 * countsPerDegree));
    move_forward(-25,(int) (6.5 * countsPerInch));
    flip_servo.SetDegree(180); 
    Sleep(2.0);
    flip_servo.SetDegree(60); 
}


void leverFlip(int lever){
    move_forward(-25,(int) 3 * countsPerInch);
    if(lever == 0){//if left lever is the target
        turn_left(25,(int) (30 * countsPerDegree));
        //move_forward(25,(int) (7.5 * countsPerInch));
        while(!senseLine()){
            left_motor.SetPercent(-25);
            right_motor.SetPercent(25);
        }
        LCD.Write("mid value"); 
        LCD.WriteLine(mid_opt.Value());
        LCD.Write("right value");
        LCD.WriteLine(right_opt.Value());
        LCD.Write("left value");
        LCD.WriteLine(left_opt.Value());
        LCD.WriteLine("Line Found");

        Sleep(1.45);
        left_motor.SetPercent(0);
        right_motor.SetPercent(0);

        turn_right(25,(int) (26 * countsPerDegree));
        move_forwardTime(25,0.65);
    }
    else if(lever == 2){//if right lever is the target
        turn_right(25,(int) (35 * countsPerDegree));
        // move_forward(25,(int) (6.5 * countsPerInch));
        while(!senseLine()){
            left_motor.SetPercent(-25);
            right_motor.SetPercent(25);
        }
        LCD.Write("mid value"); 
        LCD.WriteLine(mid_opt.Value());
        LCD.Write("right value");
        LCD.WriteLine(right_opt.Value());
        LCD.Write("left value");
        LCD.WriteLine(left_opt.Value());
        LCD.WriteLine("Line Found");

        Sleep(1.0);
        left_motor.SetPercent(0);
        right_motor.SetPercent(0);

        turn_left(25,(int) (46 * countsPerDegree));
        move_forwardTime(25,0.8);

    }else{//if middle lever is the target
        move_forwardPT4(25,(int) (9.8 * countsPerInch));
    }
    arm_servo.SetDegree(105);
    Sleep(1.0);
    arm_servo.SetDegree(180);
    Sleep(1.0);
    move_forward(-25, (int) (5.0 * countsPerInch));
    arm_servo.SetDegree(30);
    Sleep(1.0);
    move_forwardTime(25, 1.5);
    Sleep(1.0);
    arm_servo.SetDegree(70);
    Sleep(0.5);
    move_forward(-25,(int) (3 * countsPerInch));
    arm_servo.SetDegree(180);

}


int main() {
    
    // while(true){
    //     testSensors();
    // }
    
    // RPS.InitializeTouchMenu();
    // //int lever = RPS.GetIceCream();
    // LCD.WriteLine(RPS.GetIceCream());
    // while(!senseLight()){
    // }
    setUpServo();

    moveUpRampPT4();
    trayDeposit();
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
    //false is blue, true is red
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
     if(lightColor){
        right_motor.SetPercent(-25);
        left_motor.SetPercent(-25);
        Sleep(.2);
     }else{
        right_motor.SetPercent(25);
        left_motor.SetPercent(25);
        Sleep(.2);
    }
     right_motor.SetPercent(0);
     left_motor.SetPercent(0);
    //Robot drives forward until a corner hits the jukebox
    while(topLeftSwitch.Value() && topRightSwitch.Value()){
        right_motor.SetPercent(50);
        left_motor.SetPercent(-50);
    }
    //if the top left switch is pressed down, allign the right side of the robot
    
    if(!topLeftSwitch.Value()){
        // while(topLeftSwitch.Value()){
        //     right_motor.SetPercent(25);
        //     left_motor.SetPercent(0);
        // }
        float t_now;
        t_now = TimeNow();
        while(TimeNow()-t_now<2.0) {
            right_motor.SetPercent(25);
            left_motor.SetPercent(0);
        }
    }//if the top right switch is pressed down, allign the left side of the robot
    else if(!topRightSwitch.Value()){
        // while(topLeftSwitch.Value()){
        //     right_motor.SetPercent(0);
        //     left_motor.SetPercent(-25);
        // }
        float t_now;
        t_now = TimeNow();
        while(TimeNow()-t_now<2.0) {
            right_motor.SetPercent(0);
            left_motor.SetPercent(-25);
        }
    }
    //Robot aligns itself with the jukebox
    right_motor.SetPercent(0);
    left_motor.SetPercent(0);

}



//Returns true if a line is sensed, false otherwise
bool senseLine(){
     if(left_opt.Value() > leftCriticalValue || right_opt.Value() > rightCriticalValue || mid_opt.Value() > midCriticalValue){
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
        left_motor.SetPercent(motor_percent - 2);
        right_motor.SetPercent(motor_percent);
    }
    left_motor.Stop();
    right_motor.Stop();
    
    
}

//forward = 0, backward = 1
void moveTillCollision(int option){
    float t_now;

    if(option == 1){
        while(bottomLeftSwitch.Value() || bottomRightSwitch.Value()){
            left_motor.SetPercent(23);
            right_motor.SetPercent(-25);
        }
        //if the left switch collides first, turn off the left wheel
        // else if the right switch collides first, turn off the right wheel
        if(bottomLeftSwitch.Value() == 0){
            left_motor.Stop();
        } else if(bottomRightSwitch.Value() == 0){
            right_motor.Stop();
        }
        //continue trying to hit the wall until both switches are triggered, or 5 seconds have passed
        t_now = TimeNow();
        while((bottomLeftSwitch.Value() || bottomRightSwitch.Value()) || (TimeNow()-t_now < 5.0)){        
            
        }

    } else{
        //robot moves forward while both buttons are unpressed
        while(topLeftSwitch.Value() || topRightSwitch.Value()){
            left_motor.SetPercent(-23);
            right_motor.SetPercent(25);
        }
        //if the left switch collides first, turn off the left wheel
        // else if the right switch collides first, turn off the right wheel
        if(topLeftSwitch.Value() == 0){
            left_motor.Stop();
        } else if(topRightSwitch.Value() == 0){
            right_motor.Stop();
        }
        //continue trying to hit the wall until both switches are triggered, or 5 seconds have passed
        t_now = TimeNow();
        while((topLeftSwitch.Value() || topRightSwitch.Value()) || (TimeNow()-t_now < 5.0)){        
            
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
// void check_heading(float heading)
// {
//     //You will need to fill out this one yourself and take into account
//     //checking for proper RPS data and the edge conditions
//     //(when you want the robot to go to 0 degrees or close to 0 degrees)

//     /*
//         SUGGESTED ALGORITHM:
//         1. Check the current orientation of the QR code and the desired orientation of the QR code
//         2. Check if the robot is within the desired threshold for the heading based on the orientation
//         3. Pulse in the correct direction based on the orientation
//     */
//     // Check if receiving proper RPS coordinates and whether the robot is within an acceptable range
//     while(( RPS.Heading() >= -1.5) && (RPS.Heading() < heading - 3 || RPS.Heading() > heading + 3))
//     {   
//         LCD.Write("Heading: ");
//          LCD.WriteLine(RPS.Heading());
//         if(RPS.Heading() > heading)
//         {
//             // Pulse the motors for a short duration in the correct direction
//             pulse_forward(-PULSE_POWER, PULSE_TIME);
//         }
//         else if(RPS.Heading() < heading)
//         {
//             // Pulse the motors for a short duration in the correct direction
//            pulse_counterclockwise(PULSE_POWER, PULSE_TIME);
//         }
//         Sleep(RPS_WAIT_TIME_IN_SEC);
//     }
//     LCD.Write("Heading: ");
//          LCD.WriteLine(RPS.Heading());
// }