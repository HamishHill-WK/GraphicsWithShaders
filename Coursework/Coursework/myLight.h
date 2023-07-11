#pragma once
#include "Light.h"

class myLight : public Light{

private:

	float range = 0;
	float cone = 0;

	XMFLOAT3 atten;
public:

	XMFLOAT3 getAtten();	
	float getRange();	
	float getCone();

	void setRange(float r);
	void setCone(float c);

	void setAtten(float x, float y, float z);
};



