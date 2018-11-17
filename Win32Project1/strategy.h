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
		mypos.y = mypos.y < -4 ? -4 : mypos.y;
		mypos.y = mypos.y > 6 ? 6 : mypos.y;
		mypos.z = mypos.z < -5 ? -5 : mypos.z;
		mypos.z = mypos.z > 10 ? 10 : mypos.z;
			
	}	
	void checkBoundry() {
		Vector3f vtmp = m_move->getV();
		Vector3f dtmp = mydir;
		if (abs(mypos.x) == 25) {
			vtmp.x = 0;
			dtmp.x = mypos.x > 0 ? -1 : 1;
		}
		if (mypos.y == -4 || mypos.y == 6) {
			vtmp.y = 0;
			dtmp.y = mypos.y > 0 ? -1 : 1;
		}
		if (mypos.z == -5 || mypos.z == 10) {
			vtmp.z = 0;
			dtmp.z = mypos.z > 0 ? -1 : 1;
		}
		mydir = dtmp;
		m_move->setV(vtmp);
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

class HumanStrategy :public Strategy
{
public:
	HumanStrategy() {}
	void init();
	void update();
private:
	void inScope() {
		mypos.x = mypos.x < -10 ? -10 : mypos.x;
		mypos.x = mypos.x > 10 ? 10 : mypos.x;
		mypos.z = mypos.z < -5 ? -5 : mypos.z;
		mypos.z = mypos.z > 10 ? 10 : mypos.z;
	}
	void checkBoundry() {
		Vector3f vtmp = m_move->getV();
		Vector3f dtmp = mydir;
		if (abs(mypos.x) == 10) {
			vtmp.x = 0;
			dtmp.x = mypos.x > 0 ? -1 : 1;
		}
		if (mypos.z == -5 || mypos.z == 10) {
			vtmp.z = 0;
			dtmp.z = mypos.z > 0 ? -1 : 1;
		}
		mydir = dtmp;
		m_move->setV(vtmp);
	}

};
