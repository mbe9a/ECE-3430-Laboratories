#ifndef CORDIC_H
#define CORDIC_H

// Associated with 16-bit values, 8-bit fixed-point fractions.
#define CORDIC_1K 0x0000009B // decimal = 155
#define MUL 256 // scaling multiplier
#define CORDIC_NTAB 16 // the number of angles

#define ATAN_HYP 1
#define SIN_COS 0

//cordic gain
#define GAIN 0.607
//hard coded angles
#define ANGLES (long[16]){11520000,6801000,3595000,1824000,916000,458000,229000,115000,57000,29000,14000,7000,4000,2000,1000,0}

typedef struct {
	long x,y,angle;
} calculations;

// Function prototypes
void Cordic(calculations *calcs,char return_type);
/*
 * This function uses the CORDIC algorithm to compute either the
 * hypotenuse/arctangent or the sine/cosine.  Which computations are performed
 * depends on the value of return_type: if (return_type == 0), the function
 * returns the sine/cosine; else it returns the hypotenuse/arctangent.
 *
 * To compute the hypotenuse/arctangent, the input arguments are (x,y). The
 * function computes the hypotenuse = sqrt(x^2+y^2) and atan(y/x), and
 * returns the results as angle = atan() and x = hypotenuse.
 *
 * To compute the sine/cosine, the input argument is angle, which represents the
 * angle with respect to the positive horizontal axis. The function computes
 * sin(angle) and cos(angle), and returns the values as y = sin(angle) and
 * x = cos(angle).
 */
//function prototypes for rotations used in CORDIC
void clockwise(calculations *calcs, int i);
void counter_clockwise(calculations *calcs, int i);

#endif
