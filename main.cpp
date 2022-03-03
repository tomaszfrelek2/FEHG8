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


//minimum value to detect line, sensor value must be greater to trigger line sensors
float leftCriticalValue = 2.0;
float rightCriticalValue =1.0;
float midCriticalValue = 2.5;

//limit value to detect light, sensor value must be lower to trigger light sensor
float cdsCriticalValue = 1.7;
//values lower, but still higher than red will be treated as blue
int cdsBlue;
//values lower will be treated as red
float cdsRed = 0.8;

//Declarations for encoders & motors
 DigitalEncoder right_encoder(FEHIO::P1_0);
 DigitalEncoder left_encoder(FEHIO::P2_0);
 
FEHMotor right_motor(FEHMotor::Motor0,9.0);\
//left motor is inverted
FEHMotor left_motor(FEHMotor::Motor1,9.0);
FEHMotor new_motor(FEHMotor::Motor2,9.0);
//arm servo
FEHServo arm_servo(FEHServo::Servo3);

bool senseLine();
bool senseLight();
void followLine();
void turn_right(int , int );
void turn_left(int , int );
void turnToLine(int,int);
void move_forward(int, int);
void goFromJukeBoxToRamp();
void jukeBoxButton();


void testSensors(){
    LCD.Write("CDS value: ");
    LCD.WriteLine(cdsCell.Value());
    LCD.Write("mid value"); 
    LCD.WriteLine(mid_opt.Value());
    LCD.Write("right value");
    LCD.WriteLine(right_opt.Value());
    LCD.Write("left value");
    LCD.WriteLine(left_opt.Value());
    LCD.Write("top left switch");
    LCD.WriteLine(topLeftSwitch.Value());
    LCD.Write("top right switch");
    LCD.WriteLine(topRightSwitch.Value());
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
   
    left_motor.SetPercent(0);
    right_motor.SetPercent(0);
    Sleep(2.0);
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
void firstPreformanceTask(){
        setUpForJukeBox();
        jukeBoxButton();
        goFromJukeBoxToRamp();
}
void turnTester(double inches){
    int turnRad = (int) (inches * 41);
    turn_right(25, turnRad);
    LCD.WriteLine(turnRad);
    Sleep(3.0);
}
void setUpServo(){
    arm_servo.SetMin(650);
    arm_servo.SetMax(1830);//Set arm servo to 0 degrees robot_arm.SetDegree(0);
    arm_servo.SetDegree(180);
}
int main() {
    //90 degree = 3.34 inches, 136 counts
//45 dergree = 1.66 inches, 68 counts

 //Pseudocode for 2nd preformance task
 setUpServo();
 //Robot navigates to the top of the ramp

 move_forward(25,19 * 41);
 turn_right(25,68);
 move_forward(25,26*41);
 // robot turns left and goes until it finds the line
 turn_left(25,68);
 while(!senseLine()){
     left_motor.SetPercent(-25);
     right_motor.SetPercent(25);
 }
 
 //robot turns to allign with line
 turn_left(25,136);
 // robot follows line to sink
 //while(topLeftSwitch.Value() || topRightSwitch.Value()){
    followLine();
 //}
 left_motor.SetPercent(0);
 right_motor.SetPercent(0);
 Sleep(2.0);
 // robot alligns itself with sink-tbd
 //robot deposits tray
 //robot backs up
 move_forward(-25,1 * 41);
 //robot turns 45 degrees left
 turn_left(25,68);
 //robot moves 5.5 inches forward
 move_forward(52, 5*41 + 21);  //5.5 inches 
 //robot turns 45 degrees left 
 turn_left(25,68);
 //robot moves forward until it rams the wall
 while(topLeftSwitch.Value() || topRightSwitch.Value()){
    left_motor.SetPercent(-25);
    right_motor.SetPercent(25);
 }   
 Sleep(2.0);
 //robot extends "pusher"
 //Robot backs up
 move_forward(-15,5*41);
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

void goFromJukeBoxToRamp(){
    //Robot drives forward until a rear corner hits the wall
    while(bottomLeftSwitch.Value() && bottomRightSwitch.Value()){
        right_motor.SetPercent(-10);
        left_motor.SetPercent(-25);
        
    }
            if(!bottomLeftSwitch.Value()){
        while(bottomRightSwitch.Value()){
            right_motor.SetPercent(-25);
            left_motor.SetPercent(0);
        }
    }//if the top right switch is pressed down, allign the left side of the robot
    else{
        while(bottomLeftSwitch.Value()){
            right_motor.SetPercent(0);
            left_motor.SetPercent(25);
        }
    }
    //Robot aligns itself with the jukebox
    right_motor.SetPercent(0);
    left_motor.SetPercent(0);

    move_forward(25, 533);
    turn_left(25, 216);
    move_forward(50, 645);
    move_forward(-60, 450);
    LCD.WriteLine("Shleep");
    Sleep(5.0);
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
    while (senseLine() && (topLeftSwitch.Value() || topRightSwitch.Value())) { // I will follow this line while at least one of the bumper switches is unpressed
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
}

void moveTillLine() {
    int motor_percent = 25;

    while(!senseLine()) {
        left_motor.SetPercent(motor_percent);
        right_motor.SetPercent(motor_percent);
    }
    
    int expected_counts = 326;
    turnToLine(motor_percent, expected_counts);
}

void turnToLine(int percent, int counts) {
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(-1*percent);
    left_motor.SetPercent(percent);

    //While the average of the left and right encoder is less than counts,
    //keep running motors
    while((left_encoder.Counts() + right_encoder.Counts()) / 2. < counts);

    //Turn off motors
    right_motor.Stop();
    left_motor.Stop();
}

void move_forward(int percent, int counts) //using encoders
{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    right_motor.SetPercent(percent);
    left_motor.SetPercent(-1*percent);

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

void shaftEncoding()
{
    int motor_percent = 25; //Input power level here
    int expected_counts_left = 225;
    int expected_counts_right = 260;
    int expected_counts_14inches = 567; //Input theoretical counts here
    int expected_counts_10inches = 405; //Input theoretical counts here
    int expected_counts_4inches = 162*2; //Input theoretical counts here

    float x, y; //for touch screen

    //Initialize the screen
    LCD.Clear(BLACK);
    LCD.SetFontColor(WHITE);

    LCD.WriteLine("Shaft Encoder Exploration Test");
    LCD.WriteLine("Touch the screen");
    while(!LCD.Touch(&x,&y)); //Wait for screen to be pressed
    while(LCD.Touch(&x,&y)); //Wait for screen to be unpressed

    move_forward(motor_percent, expected_counts_14inches);
    turn_left(motor_percent, expected_counts_left);
    move_forward(motor_percent, expected_counts_10inches);
    turn_right(motor_percent, expected_counts_right);
    move_forward(motor_percent, expected_counts_4inches);

    Sleep(2.0); //Wait for counts to stabilize
    //Print out data
    LCD.Write("Theoretical Counts: ");
    LCD.WriteLine(0);
    LCD.Write("Motor Percent: ");
    LCD.WriteLine(motor_percent);
    LCD.Write("Actual LE Counts: ");
    LCD.WriteLine(left_encoder.Counts());
    LCD.Write("Actual RE Counts: ");
    LCD.WriteLine(right_encoder.Counts());
}