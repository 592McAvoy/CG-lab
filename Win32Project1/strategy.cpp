#include "strategy.h"

void Strategy::update() {
	printf("\n");
	printf("target %f %f %f\n", target.x, target.y, target.z);
	printf("mypos %f %f %f\n", mypos.x, mypos.y, mypos.z);

	//生成随机目标，以后被替换成真实目标
	if (abs(target.x - mypos.x)<5&&abs(target.y - mypos.y)<5&&abs(target.z - mypos.z)<5) {
		arrive = true;
	}

	if (arrive) {		
		target = Vector3f(
			0.01f*((rand() % 3 - 1)*rand() % 2500),
			0.01f*((rand() % 3 - 1)*rand() % 1000),
			0.01f*((rand() % 3 - 1)*rand() % 500));
		arrive = false;
		state = SEARCH;
	}


	time += 0.01;

	switch (state) {
	case CHASE://追踪
		chase();
		break;
	case SEARCH://目标不在攻击范围内-》搜索
		search();
		break;
	case WAIT://悬停（保持小范围随机运动）
		wait();
		break;
	default:
		printf("oops!error\n");
	}
	

	
}

void Strategy::chase() {
	printf("chase\n");
	float xd = target.x > mypos.x ? 1.0 : -1.0;
	float yd = target.y > mypos.y ? 1.0 : -1.0;
	float zd = target.z > mypos.z ? 1.0 : -1.0;
	//mydir = Vector3f(xd, yd, zd);
	mydir = Vector3f(1.0, 1.0, 1.0);

	m_move->setA(Vector3f(0.6,0.6,0.6)-(target-mypos)*0.5);
	
	mypos = m_move->CalPos(mypos, mydir);
	myrot = m_move->CalRotate();
}

void Strategy::search() {
	printf("search\n");

	if (abs(target.x - mypos.x)<10
		&& abs(target.y - mypos.y)<7
		&& abs(target.z - mypos.z)<7) {
		m_move->reset();
		//m_move->setV((target - mypos)*0.8);
		m_move->setRot(myrot);
		state = CHASE;
	}

	if (GetTickCount() - mark > 3500 && GetTickCount() - mark < 3505) {
		mydir = mydir * -1;
	}
	
	//随机方向	
	if (GetTickCount() - mark > 7000) {
		float xd = target.x > mypos.x ? 1.0 : -1.0;
		float yd = target.y > mypos.y ? 1.0 : -1.0;
		float zd = target.z > mypos.z ? 1.0 : -1.0;

		float rr = (rand() % 3 - 1) * 0.2;
		xd += rr;
		yd += rr;
		zd += rr;
		mydir = Vector3f(xd, yd, zd);
		//m_move->setV(2*mydir);

		mark = GetTickCount();
	}
	
	m_move->setA(Vector3f(0.7, 0.7, 0.7));

	mypos = m_move->CalPos(mypos, mydir);
	myrot = m_move->CalRotate();
	inScope();	
}

void Strategy::wait() {
	printf("wait\n");

}

void Strategy::escape() {
	printf("escape\n");

	float xd = enemy.x > mypos.x ? -1.0 : 1.0;
	float yd = enemy.y > mypos.y ? -1.0 : 1.0;
	float zd = enemy.z > mypos.z ? -1.0 : 1.0;
	//mydir = Vector3f(xd, yd, zd);
	mydir = Vector3f(1.0, 1.0, 1.0);

	m_move->setA((mypos - enemy));

	mypos = m_move->CalPos(mypos, mydir);
	myrot = m_move->CalRotate();

}