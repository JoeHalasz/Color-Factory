#include "MathStuff.h"
#include <cmath>


Vec3 roundVec3(Vec3 var)
{
	var.x = float(std::floor((var.x * 100) + .5))/100;
	var.y = float(std::floor((var.y * 100) + .5))/100;
	var.z = float(std::floor((var.z * 100) + .5))/100;
	return var;
}