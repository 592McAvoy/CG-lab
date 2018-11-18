#include "Move.h"



Vector3f Move::CalPos(Vector3f p, Vector3f direction) {
	float dt = 0.005;

	prev_v = v;
	
	ds = v*dt + direction*a*0.5*dt*dt;	
	v += direction*a*dt;
	return p + ds;
}

Vector3f Move::CalRotate() {
	float rx = norm(atan2(v.z, -v.y) - atan2(prev_v.z, -prev_v.y));
	float ry = norm(atan2(v.z, v.x) - atan2(prev_v.z, prev_v.x));
	float rz = norm(atan2(-v.y, v.x) - atan2(-prev_v.y, prev_v.x));
	
	rotation += Vector3f(0, ry, 0)*(180 / PI);
	//printf("rot : (%f, %f, %f)\n", rotation.x, rotation.y, rotation.z);
	return rotation;
}

Vector3f Move::LimitPos(Vector3f p, Vector3f direction, float limitV) {
	float dt = 0.005;

	prev_v = v;
	Vector3f tt = v + direction*a*dt;
	v.x = abs(tt.x) >= limitV ? v.x : tt.x;
	v.y = abs(tt.y) >= limitV ? v.y : tt.y;
	v.z = abs(tt.z) >= limitV ? v.z : tt.z;


	//printf("direction: %f, %f, %f\n", direction.x, direction.y, direction.z);
	ds = v*dt + direction*a*0.5*dt*dt;
	//printf("ds: %f, %f, %f\n", ds.x, ds.y, ds.z);
	
	//printf("a: %f, %f, %f\n", direction.x*a.x, direction.y*a.y, direction.z*a.z);
	//printf("v: %f, %f, %f\n", v.x, v.y, v.z);

	return p + ds;
}


Vector3f Move::ConstantPos(Vector3f p, Vector3f tar) {
	prev_v = v;
	ds = 0.01*(tar - p);
	ds.x = abs(ds.x) > 0.5 ? 0.05*ds.x : ds.x;
	ds.y = abs(ds.y) > 0.5 ? 0.05*ds.y : ds.y;
	ds.z = abs(ds.z) > 0.5 ? 0.05*ds.z : ds.z;
	return p + ds;
}
Vector3f Move::ConstantRotate() {
	float rx = norm(atan2(v.z, -v.y) - atan2(prev_v.z, -prev_v.y));
	float ry = norm(atan2(v.z, v.x) - atan2(prev_v.z, prev_v.x));
	float rz = norm(atan2(-v.y, v.x) - atan2(-prev_v.y, prev_v.x));
	rotation += Vector3f(0, ry, 0)*(180 / 3.14);
	//printf("rot : (%f, %f, %f)\n", rotation.x, rotation.y, rotation.z);
	return rotation+rot_bias;
}