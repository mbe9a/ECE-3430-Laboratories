import math

'''
------------------
Michael Eller
mbe9a
Requirements portions of the CORDIC lab
14 September 2015
------------------
This is a class to perform the CORDIC algorithm in an object
oriented manner. The various parameters are calculated two ways: using the native
python math functions and using the CORDIC algorithm.
------------------
'''
class CORDIC(object):
	'''
		all parameters are default
		self.angles is a hardcoded list of arctan(1/(2^i))*multiplier
		self.c_gain is also hardcoded- gain is calculated from self.angles
	'''
	def __init__(self, x = 1, y = 0, multiplier = 256, bits = 16, start_angle = 45, angle_i = 0):
		self.x_orig = x
		self.y_orig = y
		self.x = x * multiplier
		self.y = y * multiplier
		self.angle_i = angle_i

		self.bits = bits
		self.multiplier = multiplier
		self.start_angle = start_angle
		self.angle_error = 0
		self.hyp_error = 0
		self.measured = 0
		self.expected = self.get_calc_theta()
		self.data = []

		self.angles = [11520, 6801, 3593, 1824, 916, 458, 229, 115,
		57, 29, 14, 7, 4, 2, 1, 0]
		self.c_gain = 0.6072529351031390

	#CORDIC algorithm for arctan and hypotenuse
	def cordic_calc_H(self):
		for i in range (0, self.bits):
			if self.y > 0:
				self.clockwise(i)
			else:
				self.counter_clockwise(i)
			self.data.append((i, self.angles[i], self.angle_i, self.x, self.y))
		for entry in self.data:
			print entry
		self.measured = self.get_theta()
		self.measured_H = self.x * self.c_gain / self.multiplier
		self.calc_H = self.getH()
		self.angle_error = math.fabs(self.measured - self.expected) / self.expected
		self.hyp_error = math.fabs(self.calc_H - self.measured_H) / self.calc_H

	#CORDIC algorithm for Sine and Cosine
	def cordic_calc_SC(self, angle):
		start_angle = angle
		self.angle_i = int(angle* self.multiplier)
		self.x = int(self.x * self.c_gain)
		for i in range (0, self.bits):
			if self.angle_i > 0:
				self.counter_clockwise(i)
			else:
				self.clockwise(i)
			self.data.append((i, self.angles[i], self.angle_i, self.x, self.y))
		for entry in self.data:
			print entry
		self.sine = float(self.y) / self.multiplier
		self.sine_exp = math.sin(math.radians(start_angle))
		self.cosine = float(self.x) / self.multiplier
		self.cosine_exp = math.cos(math.radians(start_angle))
		self.sine_err = math.fabs(self.sine - self.sine_exp) / self.sine_exp
		self.cosine_err = math.fabs(self.cosine - self.cosine_exp) / self.cosine_exp

	#counter clockwise rotation based on the sign of Y
	def counter_clockwise(self, i):
		x = self.x
		y = self.y
		self.x = x - (y >> i)
		self.y = y + (x >> i)
		self.angle_i = self.angle_i - self.angles[i]

	#clockwise rotation based on the sign of X
	def clockwise(self, i):
		x = self.x
		y = self.y
		self.x = x + (y >> i)
		self.y = y - (x >> i)
		self.angle_i = self.angles[i] + self.angle_i

	def get_theta(self):
		return float(self.angle_i) / self.multiplier

	#sanity check
	def get_calc_theta(self):
		return math.degrees(math.atan2(self.y, self.x))

	def getH(self):
		return math.sqrt(math.pow(self.x_orig, 2) + math.pow(self.y_orig, 2))