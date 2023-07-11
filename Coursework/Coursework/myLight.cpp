#include "myLight.h"

XMFLOAT3 myLight::getAtten()
{
	XMFLOAT3 temp = XMFLOAT3(atten.x, atten.y, atten.z);
	return temp;
}

float myLight::getRange()
{
	return range;
}

float myLight::getCone()
{
	return cone;
}

void myLight::setRange(float r)
{
	range = r;
}

void myLight::setCone(float c)
{
	cone = c;
}

void myLight::setAtten(float x, float y, float z)
{
	atten = XMFLOAT3(x, y, z);
}

