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
#include "Move.h"

enum State { CHASE = 0, SEARCH, WAIT, ESCAPE, GOBACK, APPROACH };

class Strategy {
protected:	
	State state;
	
	Vector3f mypos;
	Vector3f myrot;
	Vector3f mydir;

	Vector3f target;
	Vector3f enemy;
	Vector3f protect;

	Vector3f temp;
	Vector3f EMPTY = Vector3f(0.0f, 0.0f, 0.0f);

	float time;	//time counter
	DWORD mark;	//time mark

	Move* m_move;

	void inScope() {
		mypos.x = mypos.x < -25 ? -25 : mypos.x;
		mypos.x = mypos.x > 25 ? 25 : mypos.x;
		mypos.y = mypos.y < -10 ? -10 : mypos.y;
		mypos.y = mypos.y > 10 ? 10 : mypos.y;
		mypos.z = mypos.z < -5 ? -5 : mypos.z;
		mypos.z = mypos.z > 10 ? 10 : mypos.z;
			
	}	

	void chase();
	void search();
	void wait();
	void escape();
	void goback();
	void approach();
	
public:
	Strategy(){}

	void init();

	Vector3f getPos() {
		return mypos;
	}
	Vector3f getRot() {
		return myrot;
	}

	void setTarget(Vector3f tt) {
		target = tt;
	}
	void setEnemy(Vector3f ee) {
		enemy = ee;
	}
	void setProtect(Vector3f pp) {
		protect = pp;
	}
	void update();
	
};

class FKRStrategy :public Strategy
{
public:
	FKRStrategy(){}
	void init();
	void update();
};

class AntiFKRStrategy :public Strategy
{
public:
	AntiFKRStrategy() {}
	void init();
	void update();
};
