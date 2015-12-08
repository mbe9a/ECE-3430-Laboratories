/*
 * Michael Eller
 * mbe9a
 * ECE 3430
 * Lab 3 CORDIC
 */

#include <msp430.h>
#include "cordic.h"

void Cordic(calculations *calcs,char return_type) {
	//multiply by 1000 for greater accuracy
	calcs->x = calcs->x*MUL*1000;
	calcs->y = calcs->y*MUL*1000;
	calcs->angle = calcs->angle*MUL*1000;

	//atan hypotenuse
	if(return_type != 0){
		int i;
		for(i = 0; i < CORDIC_NTAB; i++){
			if(calcs->y > 0){
				clockwise(calcs, i);
			}
			else {
				counter_clockwise(calcs, i);
			}
		}
		calcs->x = calcs->x * GAIN / MUL;
		calcs->angle = calcs->angle / MUL;
	}
	//sine cosine
	else {
		calcs->x = calcs->x * GAIN;
		int j;
		for(j = 0; j < CORDIC_NTAB; j++){
			if(calcs->angle > 0){
				counter_clockwise(calcs, j);
			}
			else {
				clockwise(calcs, j);
			}
		}
		calcs->x = calcs->x / MUL;
		calcs->y = calcs->y / MUL;
		calcs->angle = calcs->angle / MUL;
	}
	return;
}
//rotate vector clockwise
void clockwise(calculations *calcs, int i) {
	long x = calcs->x;
	long y = calcs->y;
	calcs->x = x + (y >> i);
	calcs->y = y - (x >> i);
	calcs->angle = ANGLES[i] + calcs->angle;
}

//rotate vector cclockwise
void counter_clockwise(calculations *calcs, int i) {
	long x = calcs->x;
	long y = calcs->y;
	calcs->x = x - (y >> i);
	calcs->y = y + (x >> i);
	calcs->angle = calcs->angle - ANGLES[i];
}



