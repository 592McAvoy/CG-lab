#pragma once
#include "ogldev_math_3d.h"

class Road {
private:
	Vector3f start;
	Vector3f end;
	Vector3f mid;
	int pointNum;
	int cnt;
	float t;

	int init() {
		float dist = pow((end.x - start.x), 2) + pow((end.y - start.y), 2) + pow((end.z - start.z), 2);
		dist = sqrt(dist);
		pointNum = ceil(dist / 0.01);
		t = 1 / (pointNum - 1);
		cnt = 0;
	}

public:	
	Road(Vector3f s, Vector3f m, Vector3f e) {
		start = s;
		mid = m;
		end = e;
		init();
		
	}
	Vector3f getPoint() {
		float x = start.x*pow((1 - cnt*t), 2) + mid.x * 2 * cnt*t*(1 - cnt*t) + pow(end.x*(cnt*t), 2);
		float y = start.y*pow((1 - cnt*t), 2) + mid.y * 2 * cnt*t*(1 - cnt*t) + pow(end.y*(cnt*t), 2);
		float z = start.z*pow((1 - cnt*t), 2) + mid.z * 2 * cnt*t*(1 - cnt*t) + pow(end.z*(cnt*t), 2);
		return Vector3f(x, y, z);
	}
	
		
	
};