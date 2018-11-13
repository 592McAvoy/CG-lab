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

enum State { CHASE = 0, SEARCH, WAIT, ESCAPE };
class Strategy {
private:	
	State state;
	
	Vector3f mypos;
	Vector3f myrot;
	Vector3f mydir;

	Vector3f target;
	Vector3f enemy;

	float time;	//time counter
	DWORD mark;	//time mark

	bool arrive;

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
	
public:
	Strategy(){}

	void init(Vector3f pos) {
		mypos = pos;
		myrot = Vector3f(0.0, 0.0, 0.0);
		myrot = Vector3f(1.0, -1.0, 0.0);
		target = mypos;
		enemy = mypos;
		time = 0.0;
		mark = GetTickCount();
		arrive = true;
		m_move = new Move(mypos, myrot);
		m_move->setV(Vector3f(0.0, 0.0, 0.0));
		state = SEARCH;
	};
	Vector3f getPos() {
		return mypos;
	}
	Vector3f getRot() {
		return myrot;
	}
	void update();
	
};

