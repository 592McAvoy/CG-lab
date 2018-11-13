#pragma once
#include <GL/glew.h>
#include <GL/glut.h>  // GLUTÍ¼ÐÎ¿â
#include <GL/freeglut.h>

#include <string>       
#include <vector>
#include <iostream>
#include <stdlib.h>

#include "ogldev_math_3d.h"
#include "math.h"
#include "road.h"

class Move{
private:
	
	Vector3f pos;
	Vector3f rotation;
	float posTime;
	float rotateTime;
	int mode;

	Vector3f rand_target;
	bool arrive;
	DWORD arrive_time;
	
	Vector3f enemy_pos;
	bool escape;
	float escapeTime;
	DWORD timer;
	int direct = 1;
	int directX = 1;
	int directY = 1;
	int directZ = 1;

	Road road;

	Vector3f v;
	Vector3f a;
	Vector3f ds;
	


	Vector3f NormalPos(float time);
	Vector3f NormalRotate(float time);

	Vector3f SearchPos(float time);
	
	
	Vector3f RandomPos(float time);
	Vector3f RandomRotate(float time);

	Vector3f EscapePos(float time);
	Vector3f EscapeRotate(float time);

	Vector3f TestPos(float time);
	Vector3f TestRotate(float time);

	

public:
	Move(Vector3f init_pos, Vector3f init_rot) {
		pos = init_pos;
		rotation = init_rot;
		mode = 0;
		posTime = 0.0f;
		rotateTime = 0.0f;
		escapeTime = 0.0f;
		rand_target = Vector3f(0.0f, 0.0f, 0.0f);
		v = Vector3f(3.0f, 3.0f, 3.0f);
		a = Vector3f(0.8f, 0.8f, 0.8f);
		ds = Vector3f(0.0f, 0.0f, 0.0f);
		


		arrive = true;
		escape = false;
		road = Road();
	}

	void setA(Vector3f aa) {
		a = aa;
	}
	void setV(Vector3f vv) {
		v = vv;
	}
	Vector3f getV() {
		return v;
	}

	void setRot(Vector3f rr) {
		rotation = rr;
	}
	
	void reset() {
		rotateTime = 0;
	}
	
	Vector3f CalPos(Vector3f p, Vector3f direction);
	Vector3f CalRotate();

	Vector3f SearchRotate();

	
};

