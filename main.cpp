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






int main() {
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

