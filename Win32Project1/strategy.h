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

enum State { CHASE = 0, SEARCH, WAIT, ESCAPE, GOBACK, APPROACH,HOME };

class Strategy {
protected:	
	State state;
	
	Vector3f mypos;
	Vector3f myrot;
	Vector3f mydir;

	Vector3f target;
	Vector3f enemy;
	Vector3f protect;

	Vector3f home;

	Vector3f temp;
	
	float time;	//time counter
	DWORD mark;	//time mark

	Move* m_move;

	void inScope() {
		mypos.x = mypos.x < -20 ? -20 : mypos.x;
		mypos.x = mypos.x > 20 ? 20 : mypos.x;
		mypos.y = mypos.y < 0 ? 0 : mypos.y;
		mypos.y = mypos.y > 16 ? 16 : mypos.y;
		mypos.z = mypos.z < -10 ? -10 : mypos.z;
		mypos.z = mypos.z > 8 ? 8 : mypos.z;
			
	}	
	void checkBoundry() {
		Vector3f vtmp = m_move->getV();
		Vector3f dtmp = mydir;
		if (abs(mypos.x) == 20) {
			vtmp.x = 0;
			dtmp.x = mypos.x > 0 ? -1 : 1;
		}
		if (mypos.y == 0 || mypos.y == 16) {
			vtmp.y = 0;
			dtmp.y = mypos.y > 0 ? -1 : 1;
		}
		if (mypos.z == -10 || mypos.z == 8) {
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
	void setPos(const Vector3f p) {
		mypos = p;
	}
	void setDir(const Vector3f d) {
		mydir = d;
	}

	void update();
};

class FKRStrategy :public Strategy
{
public:
	FKRStrategy(){}
	void init();
	void update();
	void setEnemy(Vector3f ee, Vector3f e0) {
		enemy = ee;
		enemy0 = e0;
		multi = 1;
	}
	void setEnemy(Vector3f ee) {
		enemy = ee;
		multi = 0;
	}
private:
	int multi = 0;
	Vector3f enemy0;
};

class AntiFKRStrategy :public Strategy
{
public:
	AntiFKRStrategy() {}
	void init();
	void update();
	void likeHome() { homesick = true; }
	void setDisturb(Vector3f dd) {
		disturb = dd;
	}
	bool disturbed = false;

private:
	Vector3f disturb;
	bool homesick = false;
	void gohome();
	void getDisturbed();
};

class HumanStrategy :public Strategy
{
public:
	HumanStrategy() {}
	void init();
	void update();
private:

	void walk_out();
	void human_escape();
	void rand_walk();

	bool escape = false;


	void inScope() {
		mypos.x = mypos.x < -10 ? -10 : mypos.x;
		mypos.x = mypos.x > 13 ? 13 : mypos.x;
		mypos.z = mypos.z < -5 ? -5 : mypos.z;
		mypos.z = mypos.z > 12 ? 12 : mypos.z;
	}
	void checkBoundry() {
		Vector3f vtmp = m_move->getV();
		Vector3f dtmp = mydir;
		if (mypos.x == -10 || mypos.x == 13) {
			vtmp.x = 0;
			dtmp.x = mypos.x > 0 ? -1 : 1;
		}
		if (mypos.z == -5 || mypos.z == 12) {
			vtmp.z = 0;
			dtmp.z = mypos.z > 0 ? -1 : 1;
		}
		mydir = dtmp;
		m_move->setV(vtmp);
	}

};
