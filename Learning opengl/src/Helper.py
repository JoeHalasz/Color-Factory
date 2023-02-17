
import math



posx = -5.333
offset = 0

while (offset != .5 and offset != -.5):

	offset = round((posx - int(posx))*100)/100
	print(offset)
	toCheck = .5

	if (posx < 0):
		toCheck *= -1

	if (offset < toCheck):
			posx += .01
	else:
		posx -= .01

	print(posx)