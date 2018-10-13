#include "Move.h"


//普通前进
Vector3f Move::NormalPos(float time) {
	posTime += 0.01;
	
	if (pos.y > 1 || pos.y <-5) {
		directY = -directY;
	}
	if (pos.z > 10 || pos.y <0) {
		directZ = -directZ;
	}
	directX = 12 * sinf(0.1*posTime)-pos.x > 0?1:-1;
	pos.y += directY *  0.0005*(rand() % 6);
	pos.x = 12 * sinf(0.1*posTime);
	pos.z += directZ * 0.001*(rand() % 6);
	return pos;
}

Vector3f Move::NormalRotate(float time) {
	rotation.x = 0.5*sinf(2.0f*time);//模拟飞行时的小震动
	rotation.y = 10.0f + atan(0.001/cos(pos.x));
	
	if ((directX>0) && (directY>0) && rotation.z < 30) {
		rotation.z += 0.05;
	}
	if (directX<0 && directY>0 && rotation.z > -30) {
		rotation.z -= 0.05;
	}
	if (directX>0 && directY<0 && rotation.z > -30) {
		rotation.z -= 0.05;
	}
	if (directX<0 && directY<0 && rotation.z <30) {
		rotation.z += 0.05;
	}
	return rotation;
}

//搜索
Vector3f Move::SearchPos(float time) {
	if (pos.y > 1 || pos.y <-5) {
		directY = -directY;
	}

	if (cosf(time/2) <= 0) {
		pos.x = pos.x;
		pos.y = pos.y;
	}
	else {
		posTime += 0.01;
		directX = 8 * sinf(0.07*posTime) - pos.x > 0 ? 1 : -1;
		
		pos.x = 8 * sinf(0.07*posTime);
		pos.z = 8 * sinf(0.07*posTime);
		pos.y += directY*0.001;
		
	}
	return pos;
}

Vector3f Move::SearchRotate(float time) {
	if (cosf(time/2) <= 0.5) {
		rotateTime += 0.0035;
		rotation.y = 10.0f + 45 * sinf(rotateTime);
		//rotation.x = sinf(2.0f*time);//
	}
	else {
		rotation.x = 3*sinf(2.0f*time);//模拟飞行时的小震动
	}

	if ((directX>0) && (directY>0) && rotation.z < 30) {
		rotation.z += 0.05;
	}
	if (directX<0 && directY>0 && rotation.z > -30) {
		rotation.z -= 0.05;
	}
	if (directX>0 && directY<0 && rotation.z > -30) {
		rotation.z -= 0.05;
	}
	if (directX<0 && directY<0 && rotation.z <30) {
		rotation.z += 0.05;
	}

	return rotation;
}

// 随机运动
Vector3f Move::RandomPos(float time) {
	//到达时停顿
	if (GetTickCount() - arrive_time < 300.0f) {
		return pos;
	}
	if (arrive) {
		//生成新的目的地
		rand_target = Vector3f(
			0.01f*((rand() % 3 - 1)*rand() % 1700),
			0.01f*((rand() % 3 - 1)*rand() % 400),
			0.01f*((rand() % 3 - 1)*rand() % 700));
		arrive = false;
		
	}
	directX = (rand_target.x > pos.x) ? 1 : -1;
	directY = (rand_target.y > pos.y) ? 1 : -1;
	directZ = (rand_target.z > pos.z) ? 1 : -1;

	//向目的地前进
	pos.x += directX * 0.002 * (rand() % 8);
	pos.y += directY * 0.002 * (rand() % 8);
	pos.z += directZ * 0.002 * (rand() % 8);

	//一定误差范围内，认为到达目的地
	if (abs(rand_target.x - pos.x)<0.1&&abs(rand_target.y - pos.y)<0.1&&abs(rand_target.z - pos.z)<0.1) {
		arrive = true;
		arrive_time = GetTickCount();
		
	}

	return pos;
}
Vector3f Move::RandomRotate(float time) {
	
	rotation.y = -50.0f+ atan(pos.z / pos.x);
	rotation.x = 3*sinf(time);//模拟飞行时的小震动
	rotation.z = 3 * sinf(2 * time) ;

	if ((directX>0) && (directY>0) && rotation.x < 30) {
		rotation.z += 0.05;
		rotation.x += 0.05;
	}
	if (directX<0 && directY>0 && rotation.x > -30) {
		rotation.x -= 0.05;
		rotation.z -= 0.05;
	}
	if (directX>0 && directY<0 && rotation.x > -30) {
		rotation.x -= 0.05;
		rotation.z -= 0.05;
	}
	if (directX<0 && directY<0 && rotation.x <30) {
		rotation.z += 0.05;
		rotation.x += 0.05;
	}
	
	
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
	
		
	}
	else {
		//向目的地前进
		if (pos.y >3 || pos.y <-4) {
			directY = -directY;
		}
		if (pos.x > 6 || pos.x <-3) {
			directX = -directX;
		}
		if (pos.z > 10 || pos.z <-2) {
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

//测试随机轨迹飞行效果
Vector3f Move::TestPos(float time) {
	if (GetTickCount() - arrive_time < 200.0f) {
		return pos;
	}
	if (arrive) {
		//生成新的目的地
		rand_target = Vector3f(
			0.01f*((rand() % 3 - 1)*rand() % 2000 ),
			0.01f*((rand() % 3 - 1)*rand() % 500),
			0.01f*((rand() % 3 - 1)*rand() % 1000));
		Vector3f rand_mid = Vector3f(
			0.01f*((rand() % 3 - 1)*rand() % 2000),
			0.01f*((rand() % 3 - 1)*rand() % 500),
			0.01f*((rand() % 3 - 1)*rand() % 1000));
		arrive = false;
		road.Init(pos, rand_mid, rand_target);

	}

	//向目的地前进
	pos = road.getPoint();

	//一定误差范围内，认为到达目的地
	if (abs(rand_target.x - pos.x)<0.1&&abs(rand_target.y - pos.y)<0.1&&abs(rand_target.z - pos.z)<0.1) {
		arrive = true;
		arrive_time = GetTickCount();

	}

	return pos;
}
Vector3f Move::TestRotate(float time) {
	rotation = road.getRotation();
	return rotation;
}