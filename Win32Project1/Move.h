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
	Vector3f rot_bias;
	
	
	Vector3f v;
	Vector3f dv;
	Vector3f a;
	Vector3f ds;
		

public:
	Move(Vector3f init_pos, Vector3f init_rot) {
		pos = init_pos;
		rotation = init_rot;
		rot_bias = Vector3f(0.0f, 0.0f, 0.0f);

		v = Vector3f(3.0f, 3.0f, 3.0f);
		dv = Vector3f(0.0f, 0.0f, 0.0f);
		a = Vector3f(0.8f, 0.8f, 0.8f);
		ds = Vector3f(0.0f, 0.0f, 0.0f);
		
	}

	void setA(Vector3f aa) {
		a.x = abs(aa.x) < 2 ? aa.x : 2;
		a.y = abs(aa.y) < 2 ? aa.y : 2;
		a.z = abs(aa.z) < 2 ? aa.z : 2;
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
	
	
	Vector3f CalPos(Vector3f p, Vector3f direction);
	Vector3f CalRotate();

	Vector3f ConstantPos(Vector3f p, Vector3f tar);
	Vector3f ConstantRotate();

	Vector3f LimitPos(Vector3f p, Vector3f direction, float limitV);

	
};

