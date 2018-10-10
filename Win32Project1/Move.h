#pragma once
#include <GL/glew.h>
#include <GL/glut.h>  // GLUT图形库
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
		rand_target = Vector3f(0.0f, 0.0f, 0.0f);
		arrive = true;
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
		}
	}

	Vector3f getRotation(float time) {
		switch (mode) {
			case 0:return NormalRotate(time);
			case 1:return SearchRotate(time);
			case 2:return RandomRotate(time);
			case 3:return EscapeRotate(time);
			default:printf("mode error\n");
		}
	}

};


//普通前进
Vector3f Move::NormalPos(float time) {
	posTime += 0.01;
	pos.x = 8 * sinf(0.05*posTime);
	if (pos.y > 2 || pos.y <-2) {
		direct = -direct;
	}
	pos.y += direct*0.0005;

	return pos;
}

Vector3f Move::NormalRotate(float time) {
	rotation.y = -20.0f;
	rotation.x = sinf(2.0f*time);//模拟飞行时的小震动
	rotation.z = -10.0f + sinf(2.0f*time);
	return rotation;
}

//搜索
Vector3f Move::SearchPos(float time) {
	if (pos.y > 2 || pos.y <-3) {
		direct = -direct;
	}

	if (cosf(time) >= -0.5) {
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

Vector3f Move::SearchRotate(float time) {
	if (cosf(time) <= 0.5) {
		rotateTime += 0.0035;
		rotation.y = -20.0f + 40 * sinf(rotateTime);
		rotation.z = -10.0f;
	}
	
	rotation.x = sinf(2.0f*time);//模拟飞行时的小震动
	rotation.z = -10.0f + sinf(2.0f*time);

	return rotation;
}

// 随机运动
Vector3f Move::RandomPos(float time) {
	if (arrive) {
		rand_target = Vector3f(
			0.01f*((rand() % 3 - 1)*rand() % 300 + 350),
			0.01f*((rand() % 3 - 1)*rand() % 300),
			0.01f*(rand() % 300));
		arrive = false;
		printf("target %f, %f, %f\n", rand_target.x, rand_target.y, rand_target.z);
	}
	pos.x += (rand_target.x > pos.x)?0.005f:-0.005f;
	pos.y += (rand_target.y > pos.y)?0.005f:-0.005f;
	pos.z += (rand_target.z > pos.z)?0.005f:-0.005f;

	printf("pos %f, %f, %f\n", pos.x, pos.y, pos.z);
	if (abs(rand_target.x - pos.x)<0.1&&abs(rand_target.y - pos.y)<0.1&&abs(rand_target.z - pos.z)<0.1) {
		arrive = true;
	}

	return pos;
}
Vector3f Move::RandomRotate(float time) {
	/*rotation.y = -20.0f;
	rotation.x = sinf(2.0f*time);//模拟飞行时的小震动
	rotation.z = -10.0f + sinf(2.0f*time);*/
	return rotation;
}

// 躲避运动
Vector3f Move::EscapePos(float time) {
	pos.x += (rand() % 3 - 1) * (rand() % 2 * 0.00001);
	pos.y += (rand() % 3 - 1) * (rand() % 2 * 0.00001);
	pos.z += (rand() % 1)*0.00001;
	return pos;
}
Vector3f Move::EscapeRotate(float time) {
	rotation.y = -20.0f;
	rotation.x = sinf(2.0f*time);//模拟飞行时的小震动
	rotation.z = -10.0f + sinf(2.0f*time);
	return rotation;
}