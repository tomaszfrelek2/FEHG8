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


FEHMotor left_motor(FEHMotor::Motor0,9.0);
FEHMotor right_motor(FEHMotor::Motor1,9.0);
AnalogInputPin right_opt(FEHIO::P0_0);
AnalogInputPin mid_opt(FEHIO::P0_1);
AnalogInputPin left_opt(FEHIO::P0_2);

//Declarations for encoders & motors
DigitalEncoder right_encoder(FEHIO::P0_0);
DigitalEncoder left_encoder(FEHIO::P0_1);
FEHMotor right_motor(FEHMotor::Motor0,9.0);
FEHMotor left_motor(FEHMotor::Motor1,9.0);

void move_forward(int percent, int counts) //using encoders
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

void turn_right(int percent, int counts) //using encoders

{
    //Reset encoder counts
    right_encoder.ResetCounts();
    left_encoder.ResetCounts();

    //Set both motors to desired percent
    //hint: set right motor backwards, left motor forwards
    right_motor.SetPercent(-1*percent);
    left_motor.SetPercent(percent);

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
    left_motor.SetPercent(-1*percent);

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

void performanceTest1_moveTillLine() {
    int motor_percent = 25;

    while(!foundLine()) {
        left_motor.SetPercent(motor_percent);
        right_motor.SetPercent(motor_percent);

        LCD.WriteLine("Line not found!");
    }

    LCD.WriteLine("Line found!");
    
    int expected_counts = 0;
    turnToLine(motor_percent, expected_counts);
}

void turnToLine(int percent, int counts) {
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


int main() {
    
}




enum LineStates {
    MIDDLE,
    RIGHT,
    LEFT
};
void followLine(){
    int state = MIDDLE; // Set the initial state
    while (true) { // I will follow this line forever!
        switch(state) {
            // If I am in the middle of the line...
            case MIDDLE:
                // Set motor powers for driving straight
                /* Drive */

                left_motor.SetPercent(25);
                right_motor.SetPercent(25);

                if (right_opt.Value() > 2.0)  {
                    state = RIGHT; // update a new state
                }
                /* Code for if left sensor is on the line */

                if (left_opt.Value() > 2.2)  {
                    state = LEFT; // update a new state
                }
                
                break;
                

            // If the right sensor is on the line...
            case RIGHT:
                left_motor.SetPercent(25);
                right_motor.SetPercent(10);
                /* Drive */
                if( mid_opt.Value() > 2.7 ) {
                    state = MIDDLE;
                }

                if (left_opt.Value() > 2.2)  {
                    state = LEFT; // update a new state
                }
                break;

            // If the left sensor is on the line...
            case LEFT:
                left_motor.SetPercent(10);
                right_motor.SetPercent(25);


                if( mid_opt.Value() > 2.7 ) {
                    state = MIDDLE;
                }

                if (right_opt.Value() > 2.0)  {
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
}

