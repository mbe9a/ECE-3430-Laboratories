/*
 * Brendon Koch (blk2jh)
 * Michael Eller (mbe9a)
 */
#include "accelerometer.h"

coordinates coords;
measurements meas;

void Calibration(ElectronicLevel *myLevel) {
	switch (myLevel->cal_step) {
	case 0:
		// at rest, max Z
		myLevel->cal_array[2][0] = getSample(0);
		// mid value of x
		myLevel->cal_array[3][0] = getSample(2);
		// mid value of y
		myLevel->cal_array[3][1] = getSample(1);
		myLevel->cal_step += 1;
		break;
	case 1:
		// flipped, min Z
		myLevel->cal_array[2][1] = getSample(0);
		myLevel->cal_step += 1;
		break;
	case 2:
		// max Y
		myLevel->cal_array[1][0] = getSample(1);
		// mid value of z
		myLevel->cal_array[3][2] = getSample(0);
		myLevel->cal_step += 1;
		break;
	case 3:
		// min Y
		myLevel->cal_array[1][1] = getSample(1);
		myLevel->cal_step += 1;
		break;
	case 4:
		// max X
		myLevel->cal_array[0][0] = getSample(2);
		myLevel->cal_step += 1;
		break;
	case 5:
		// min X
		myLevel->cal_array[0][1] = getSample(2);
		myLevel->cal_step += 1;

		// calculate the normalized max and min values for each axis
		myLevel->norm_array[0][0] = myLevel->cal_array[0][0] - myLevel->cal_array[3][0];
		myLevel->norm_array[0][1] = myLevel->cal_array[0][1] - myLevel->cal_array[3][0];
		myLevel->norm_array[1][0] = myLevel->cal_array[1][0] - myLevel->cal_array[3][1];
		myLevel->norm_array[1][1] = myLevel->cal_array[1][1] - myLevel->cal_array[3][1];
		myLevel->norm_array[2][0] = myLevel->cal_array[2][0] - myLevel->cal_array[3][2];
		myLevel->norm_array[2][1] = myLevel->cal_array[2][1] - myLevel->cal_array[3][2];
		break;
	}
}

void stateMachine(ElectronicLevel *myLevel, LEDinfo *LEDs) {
	int step = myLevel->cal_step;
	switch(myLevel->current_state) {
	case Start:
		UpdateDisplay(0x80);
		_delay_cycles(DELAY);
		UpdateDisplay(0x41);
		_delay_cycles(DELAY);
		UpdateDisplay(0x22);
		_delay_cycles(DELAY);
		UpdateDisplay(0x14);
		_delay_cycles(DELAY);
		UpdateDisplay(0x08);
		_delay_cycles(DELAY);
		UpdateDisplay(0x14);
		_delay_cycles(DELAY);
		UpdateDisplay(0x22);
		_delay_cycles(DELAY);
		UpdateDisplay(0x41);
		_delay_cycles(DELAY);
		myLevel->current_state = Calibrate;
		LEDs->state = Calibrate;
		break;
	case Calibrate:
		/*
		 * If calibration is finished move to the level state
		 * otherwise, Light LEDs according to calibration direction
		 * NW = flat, facing up
		 * SW = flat, facing down
		 */
		if (myLevel->cal_step > 5) {
			myLevel->current_state = Level;
			LEDs->state = Level;
		} else {
			if (step == 0) {
				LEDs->centLED = NW;
			} else if (step == 1) {
				LEDs->centLED = SW;
			} else if (step == 2) {
				LEDs->centLED = W;
			} else if (step == 3) {
				LEDs->centLED = E;
			} else if (step == 4) {
				LEDs->centLED = N;
			} else if (step == 5) {
				LEDs->centLED = S;
			}
		}
		break;
	case Level:
		// get x average
		coords.x = getX();
		// remove x offset
		coords.x -= myLevel->cal_array[3][0];
		// get y average
		coords.y = getY();
		// remove y offset
		coords.y -= myLevel->cal_array[3][1];
		// get z average
		coords.z = getZ();
		// remove z offset
		coords.z -= myLevel->cal_array[3][2];
		meas = getDisplaySetting(&coords);

		// if the tilt is below tolerance set the LED state to Flat to turn all LEDs on.
		if (meas.tilt < TOLERANCE) {
			LEDs->state = Flat;
		} else {
			// Set the center LED based on the direction of tilt.
			LEDs->state = Level;
			if (meas.angle < 2500 || meas.angle > 34500) {
				LEDs->centLED = N;
			} else if (meas.angle < 34500 && meas.angle > 30000) {
				LEDs->centLED = NW;
			} else if (meas.angle < 30000 && meas.angle > 25500) {
				LEDs->centLED = W;
			} else if (meas.angle < 25500 && meas.angle > 20250) {
				LEDs->centLED = SW;
			} else if (meas.angle < 20250 && meas.angle > 14500) {
				LEDs->centLED = S;
			} else if (meas.angle < 14500 && meas.angle > 10500) {
				LEDs->centLED = SE;
			} else if (meas.angle < 10500 && meas.angle > 6000) {
				LEDs->centLED = E;
			} else if (meas.angle < 6000 && meas.angle > 2500) {
				LEDs->centLED = NE;
			}
		}
		setBrightness(LEDs, meas.tilt);
		break;
	default:
		myLevel->current_state = Start;
	}
}
