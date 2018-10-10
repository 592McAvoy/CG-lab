#pragma once
#include <GL/glew.h>
#include <GL/glut.h>  // GLUT图形库
#include <GL/freeglut.h>

#include <string>       
#include <vector>
#include <iostream>

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

public:
	Move(Vector3f init_pos, Vector3f init_rot) {
		pos = init_pos;
		rotation = init_rot;
		mode = 0;
		posTime = 0.0f;
		rotateTime = 0.0f;
	}

	void setMode(int m) {
		mode = m;
	}

	Vector3f getPos(float scale) {
		switch (mode) {
			case 0:return NormalPos(scale);
			case 1:return SearchPos(scale);
			default:printf("mode error\n");
		}
	}

	Vector3f getRotation(float scale) {
		switch (mode) {
			case 0:return NormalRotate(scale);
			case 1:return SearchRotate(scale);
			default:printf("mode error\n");
		}
	}

	//普通前进
	Vector3f NormalPos(float scale) {
		posTime += 0.01;
		pos.x = 8 * sinf(0.05*posTime);
		if (pos.y > 2 || pos.y <-2) {
			direct = -direct;
		}
		pos.y += direct*0.0005;
			
		return pos;
	}

	Vector3f NormalRotate(float scale) {
		rotation.y = -20.0f;
		rotation.x = sinf(2.0f*scale);//模拟飞行时的小震动
		rotation.z = -10.0f + sinf(2.0f*scale);
		return rotation;
	}

	//搜索
	Vector3f SearchPos(float scale) {
		if (pos.y > 2 || pos.y <-3) {
			direct = -direct;
		}

		if (cosf(scale) >= -0.5) {
			posTime += 0.01;
			pos.x = 8 * sinf(0.05*posTime);
			pos.y += direct*0.001;
		}
		else {
			pos.x = pos.x;
			pos.y = pos.y;
		}	
		
		

		return pos;
	}

	Vector3f SearchRotate(float scale) {
		if (cosf(scale) <= 0.5) {
			rotateTime += 0.0035;
			rotation.y = -20.0f + 40 * sinf(rotateTime);
			rotation.z = -10.0f;
		}
		else {
			
		}
		

		rotation.x = sinf(2.0f*scale);//模拟飞行时的小震动
		rotation.z = -10.0f + sinf(2.0f*scale);
		
		return rotation;
	}

};