#pragma once
#include "ogldev_math_3d.h"

class Road {
private:
	Vector3f start;
	Vector3f end;
	Vector3f mid;
	int pointNum;
	int cnt;

	Vector3f lastP;
	Vector3f newP;


public:	
	Road() {}

	void Init(Vector3f s, Vector3f m, Vector3f e) {
		start = s;
		mid = m;
		end = e;
		float dist1 = pow((mid.x - start.x), 2) + pow((mid.y - start.y), 2) + pow((mid.z - start.z), 2);
		dist1 = sqrt(dist1);
		float dist2 = pow((end.x - mid.x), 2) + pow((end.y - mid.y), 2) + pow((end.z - mid.z), 2);
		dist2 = sqrt(dist2);
		pointNum = ceil(dist1+dist2 / 0.005);
		cnt = 1;
		lastP = newP = start;
	}
	
	//2½×±´Èû¶ûÇúÏß
	Vector3f getPoint() {
		float t = (double)cnt / pointNum;
		float x = start.x * pow(1 - t, 2) + mid.x * (1 - t) * t * 2 + end.x * pow(t, 2);		
		float y = start.y * pow(1 - t, 2) + mid.y * (1 - t) * t * 2 + end.y * pow(t, 2);
		float z = start.z * pow(1 - t, 2) + mid.z * (1 - t) * t * 2 + end.z * pow(t, 2);

		//printf("pos %d: (%f, %f, %f)\n", cnt, x, y, z);
		lastP = newP;
		newP = Vector3f(x, y, z);
		
		cnt += 1;

		return newP;
	}

	Vector3f getRotation() {
		float t = (double)cnt / pointNum;
		float dx = -2* start.x * pow(1 - t, 2) + 2 * mid.x * (1 - 2 * t)  + end.x * 2 * t;
		float dy = -2 * start.y * pow(1 - t, 2) + 2 * mid.y * (1 - 2 * t) + end.y * 2 * t;
		float dz = -2 * start.z * pow(1 - t, 2) + 2 * mid.z * (1 - 2 * t) + end.z * 2 * t;

		Vector3f rr(180 / 3.14 *atan( dy / dz), 180 / 3.14 *atan( dz / dx), 180 / 3.14 *atan( dy / dx));
		//printf("rot %d: (%f, %f, %f) dx=%f dy=%f dz=%f\n", cnt, rr.x, rr.y, rr.z, dx, dy, dz);
		return rr;
	}

	
		
	
};