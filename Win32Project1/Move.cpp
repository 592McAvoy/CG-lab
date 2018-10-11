#include "Move.h"


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
	rotation.x = 0.5*sinf(2.0f*time);//模拟飞行时的小震动
	rotation.z = -10.0f;
	return rotation;
}

//搜索
Vector3f Move::SearchPos(float time) {
	if (pos.y > 2 || pos.y <-3) {
		direct = -direct;
	}

	if (cosf(time/2) <= 0) {
		pos.x = pos.x;
		pos.y = pos.y;
	}
	else {
		posTime += 0.01;
		pos.x = 8 * sinf(0.07*posTime);
		pos.y += direct*0.001;
		
	}
	return pos;
}

Vector3f Move::SearchRotate(float time) {
	if (cosf(time/2) <= 0.5) {
		rotateTime += 0.0035;
		rotation.y = -50.0f + 45 * sinf(rotateTime);
		rotation.z = -10.0f;
		rotation.x = sinf(2.0f*time);//
	}
	else {
		rotation.x = 3*sinf(2.0f*time);//模拟飞行时的小震动
		rotation.z = -10.0f;
	}

	return rotation;
}

// 随机运动
Vector3f Move::RandomPos(float time) {
	//到达时停顿500ms
	if (GetTickCount() - arrive_time < 400.0f) {
		return pos;
	}
	if (arrive) {
		//生成新的目的地
		rand_target = Vector3f(
			0.01f*((rand() % 3 - 1)*rand() % 300 + 350),
			0.01f*((rand() % 3 - 1)*rand() % 300),
			0.01f*(rand() % 300));
		arrive = false;
		
	}
	//向目的地前进
	pos.x += (rand_target.x > pos.x) ? 0.005f : -0.005f;
	pos.y += (rand_target.y > pos.y) ? 0.005f : -0.005f;
	pos.z += (rand_target.z > pos.z) ? 0.005f : -0.005f;

	//一定误差范围内，认为到达目的地
	if (abs(rand_target.x - pos.x)<0.1&&abs(rand_target.y - pos.y)<0.1&&abs(rand_target.z - pos.z)<0.1) {
		arrive = true;
		arrive_time = GetTickCount();
		
	}

	return pos;
}
Vector3f Move::RandomRotate(float time) {
	
	rotation.y = -50.0f;
	rotation.x = 3*sinf(2*time);//模拟飞行时的小震动
	rotation.z = -10.0f;
	
	
	return rotation;
}

// 躲避运动
Vector3f Move::EscapePos(float time) {
	if (GetTickCount() - timer < 350.0f) {
		return pos;
	}
	if (!escape) {
		//生成敌人坐标
		enemy_pos = Vector3f(
			(rand() % 3 - 1)*rand() % 3 + pos.x,
			(rand() % 3 - 1)*rand() % 3 + pos.y,
			(rand() % 3 - 1)*rand() % 3 + pos.z);
		escape = true;
		//escapeTime = 0;
	}
	
	//不在危险区域
	if (abs(enemy_pos.x - pos.x)>2 && abs(enemy_pos.y - pos.y)>2 && abs(enemy_pos.z - pos.z)>1) {
		escape = false;
		arrive_time = GetTickCount();
		//escapeTime = 0;
		//printf("safe\n");
		
	}
	else {
		//向目的地前进
		if (pos.y >3 || pos.y <-3) {
			directY = -directY;
		}
		if (pos.x > 5 || pos.x <-3) {
			directX = -directX;
		}
		if (pos.z > 10 || pos.z <-1) {
			directZ = -directZ;
		}
		pos.x += directX*0.006;
		
		pos.y += directY*0.006;
		
		pos.z += directZ*0.006;
		//escapeTime += 0.01;
	}
	return pos;
}
Vector3f Move::EscapeRotate(float time) {
	rotation.y = -50.0f;
	rotation.x = 10*sinf(2 * time);//模拟飞行时的小震动
	rotation.z = -10.0f;
	if (GetTickCount() - timer < 350) {
		return rotation;
	}
	escapeTime += 0.0035;
	rotation.x = direct * 45 * abs(sinf(escapeTime));
	//printf("rr %f\n", abs(sinf(escapeTime)));
	if (abs(sinf(escapeTime))<0.01 ) {
		timer = GetTickCount();
		escapeTime += 0.1;
		direct = -direct;
	}
	rotation.x = direct * 45 * abs(sinf(escapeTime));
	
	return rotation;
}