/*
 * Brendon Koch (blk2jh)
 * Michael Eller (mbe9a)
 */
#ifndef ELECTRONICLEVEL_BLK2JH_MBE9A_CORDIC_H_
#define ELECTRONICLEVEL_BLK2JH_MBE9A_CORDIC_H_

typedef enum {
	first, second, third, fourth
} Quadrant;

typedef struct {
	int angle;
	int hypotenuse;
} calculations;

typedef struct {
	unsigned int angle;
	unsigned int tilt;
} measurements;

typedef struct {
	int x;
	int y;
	int z;
} coordinates;

measurements getDisplaySetting(coordinates* position);
calculations findQuadrantAngle(int x, int y);
Quadrant getQuad(int x, int y);
unsigned int findTotalAngle(int quadAngle, Quadrant quad);


#endif /* ELECTRONICLEVEL_BLK2JH_MBE9A_CORDIC_H_ */
