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

class Move{
private:
	
	Vector3f pos;
	Vector3f rotation;
	float posTime;
	float rotateTime;
	int direct = -1;
	int mode;

	Vector3f rand_target;
	bool arrive;
	DWORD arrive_time;
	
	Vector3f enemy_pos;
	bool escape;
	float escapeTime;
	DWORD timer;
	int directX = 1;
	int directY = 1;
	int directZ = 1;


	Vector3f NormalPos(float time);
	Vector3f NormalRotate(float time);

	Vector3f SearchPos(float time);
	Vector3f SearchRotate(float time);
	
	Vector3f RandomPos(float time);
	Vector3f RandomRotate(float time);

	Vector3f EscapePos(float time);
	Vector3f EscapeRotate(float time);

public:
	Move(Vector3f init_pos, Vector3f init_rot) {
		pos = init_pos;
		rotation = init_rot;
		mode = 0;
		posTime = 0.0f;
		rotateTime = 0.0f;
		escapeTime = 0.0f;
		rand_target = Vector3f(0.0f, 0.0f, 0.0f);
		arrive = true;
		escape = false;
	}

	void setMode(int m) {
		mode = m;
	}

	Vector3f getPos(float time) {
		switch (mode) {
			case 0:return NormalPos(time);
			case 1:return SearchPos(time);
			case 2:return RandomPos(time);
			case 3:return EscapePos(time);
			default:printf("mode error\n");
				break;

		}

		return pos;
	}

	Vector3f getRotation(float time) {
		switch (mode) {
			case 0:return NormalRotate(time);
			case 1:return SearchRotate(time);
			case 2:return RandomRotate(time);
			case 3:return EscapeRotate(time);
			default:printf("mode error\n");
				
		}
		return rotation;
	}

};

