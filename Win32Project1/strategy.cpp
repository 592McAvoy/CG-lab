#include "strategy.h"

Vector3f genRandPos() {
	return Vector3f(
		0.01f*((rand() % 3 - 1)*rand() % 1000),
		0.01f*((rand() % 2)*rand() % 600),
		0.01f*((rand() % 3 - 1)*rand() % 500));
}


void Strategy::init() {
	mypos = genRandPos();
	myrot = Vector3f(0.0, 0.0, 0.0);

	target = EMPTY;
	enemy = EMPTY;
	protect = EMPTY;

	temp = mypos;
	time = 0.0;
	mark = GetTickCount();
	m_move = new Move(mypos, myrot);
	m_move->setV(Vector3f(0.0, 0.0, 0.0));
}

void HumanStrategy::init() {
	Strategy::init();
	mypos = Vector3f(
		0.01f*((rand() % 3 - 1)*rand() % 500),
		-5,
		0.01f*((rand() % 3 - 1)*rand() % 200));
	
}

void FKRStrategy::init() {
	Strategy::init();
	target = genRandPos();//人
	enemy = genRandPos();//anti FKR
	state = SEARCH;
}

void AntiFKRStrategy::init() {
	Strategy::init();
	target = genRandPos();//FKR
	protect = genRandPos();//人
	state = WAIT;
}

void Strategy::update() {
	time += 0.01;

	switch (state) {
	case CHASE://追踪
		chase();
		break;
	case SEARCH://目标不在攻击范围内-》搜索
		search();
		break;
	case WAIT://待命（保持小范围随机运动）
		wait();
		break;
	case ESCAPE://躲避
		escape();
		break;
	case GOBACK://回归
		goback();
		break;
	case APPROACH://靠近
		approach();
		break;
	default:
		printf("oops!error\n");
	}
	
	printf("\n");
	
}

void Strategy::chase() {
	printf("chase\n");
	float xd = target.x > mypos.x ? 1.0 : -1.0;
	float yd = target.y > mypos.y ? 1.0 : -1.0;
	float zd = target.z > mypos.z ? 1.0 : -1.0;
	mydir = Vector3f(xd, yd, zd);

	m_move->setA(abs(target-mypos)*0.5);
	checkBoundry();
	mypos = m_move->CalPos(mypos, mydir);
	myrot = m_move->CalRotate();

	
}

void Strategy::approach() {
	printf("approach\n");
	float xd = target.x > mypos.x ? -1.0 : 1.0;
	float yd = target.y > mypos.y ? -1.0 : 1.0;
	float zd = target.z > mypos.z ? -1.0 : 1.0;
	mydir = Vector3f(xd, yd, zd);

	m_move->setA(abs(target - mypos)*0.5);
	checkBoundry();
	mypos = m_move->CalPos(mypos, mydir);
	myrot = m_move->CalRotate();


}

void Strategy::goback() {
	printf("goback\n");
	float xd = protect.x > mypos.x ? 1.0 : -1.0;
	float yd = protect.y > mypos.y ? 1.0 : -1.0;
	float zd = protect.z > mypos.z ? 1.0 : -1.0;
	mydir = Vector3f(xd, yd, zd);

	m_move->setA(abs(protect - mypos)*0.5);
	checkBoundry();
	mypos = m_move->CalPos(mypos, mydir);
	myrot = m_move->CalRotate();


}

void Strategy::search() {
	printf("search\n");

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
		mark = GetTickCount();
	}
	
	m_move->setA(Vector3f(1.0, 1.0, 1.0));

	checkBoundry();

	mypos = m_move->LimitPos(mypos, mydir,3.5);
	myrot = m_move->CalRotate();
	inScope();	
}

void Strategy::wait() {
	printf("wait\n");

	if (GetTickCount() - mark > 750 && GetTickCount() - mark < 755) {
		mydir = mydir * -1;
		m_move->setA(Vector3f(0.8, 0.8, 0.8));
	}

	//随机方向	
	if (GetTickCount() - mark > 1500) {
		temp = protect + Vector3f(
			0.01f*((rand() % 3 - 1)*rand() % 300),
			0.01f*(rand() % 600),
			0.01f*((rand() % 3 - 1)*rand() % 300));
		mark = GetTickCount();
	}
	

	mypos = m_move->ConstantPos(mypos, temp);
	inScope();

}

void Strategy::escape() {
	printf("escape\n");
	if (abs(enemy.x - mypos.x)>10 && abs(enemy.y - mypos.y)>7 && abs(enemy.z - mypos.z)>4) {
		state = SEARCH;
		m_move->setV(Vector3f(0.0, 0.0, 0.0));
		return;
	}
	

	float xd = enemy.x > mypos.x ? -1.0 : 1.0;
	float yd = enemy.y > mypos.y ? -1.0 : 1.0;
	float zd = enemy.z > mypos.z ? -1.0 : 1.0;
	mydir = Vector3f(xd, yd, zd);
	
	m_move->setA(reverse(abs(mypos - enemy))*2);
	checkBoundry();
	mypos = m_move->CalPos(mypos, mydir);
	myrot = m_move->CalRotate();


}

void FKRStrategy::update() {
	printf("FKR\n");
	printf("target %f %f %f\n", target.x, target.y, target.z);
	printf("enemy %f %f %f\n", enemy.x, enemy.y, enemy.z);
	printf("mypos %f %f %f\n", mypos.x, mypos.y, mypos.z);

	//遇到敌人-escape
	if (abs(enemy.x - mypos.x)<3 && abs(enemy.y - mypos.y)<3 && abs(enemy.z - mypos.z)<3) {
		state = ESCAPE;
	}
	//目标离自己距离太远-search
	else if (!(abs(target.x - mypos.x)<15
		&& abs(target.y - mypos.y)<10
		&& abs(target.z - mypos.z)<10)) {
		state = SEARCH;
	}
	//离目标距离不远-approach
	else if (abs(target.x - mypos.x) < 5 && abs(target.y - mypos.y) < 5 && abs(target.z - mypos.z) < 5) {
		state = APPROACH;
	}
	//追逐目标
	else {
		if (state != CHASE)
			m_move->setV(0.1*(target - mypos));
		state = CHASE;
	}


	Strategy::update();
	inScope();

	printf("\n");

}

void AntiFKRStrategy::update() {
	printf("AntiFKR\n");
	printf("target %f %f %f\n", target.x, target.y, target.z);
	printf("protect %f %f %f\n", protect.x, protect.y, protect.z);
	printf("mypos %f %f %f\n", mypos.x, mypos.y, mypos.z);

	//目标距离被保护对象太近-chase
	if ((abs(target.x - protect.x)<5 && abs(target.y - protect.y)<5 && abs(target.z - protect.z)<5)
		|| abs(target.x - mypos.x)<3 && abs(target.y - mypos.y)<3 && abs(target.z - mypos.z)<3) {
		if (state != CHASE)
			m_move->setV(0.1*(target - mypos));
		state = CHASE;
	}
	//在被保护对象周围待命
	else if (abs(protect.x - mypos.x) < 5 && abs(protect.y - mypos.y) < 6 && abs(protect.z - mypos.z) < 5) {
		m_move->setV(EMPTY);
		state = WAIT;
	}
	//回到被保护对象周围
	else {
		state = GOBACK;
	}

	Strategy::update();
	inScope();

	printf("\n");

}

void HumanStrategy::update() {
	printf("Human\n");
	printf("mypos %f %f %f\n", mypos.x, mypos.y, mypos.z);

	if (GetTickCount() - mark > 1500 && GetTickCount() - mark < 1505) {
		mydir = mydir * -1;
	}

	//随机方向	
	if (GetTickCount() - mark > 3000) {
		mydir = Vector3f(rand() % 3 - 1, 0, rand() % 3 - 1);
		mark = GetTickCount();
	}

	m_move->setA(Vector3f(0.5, 0.5, 0.5));

	checkBoundry();

	mypos = m_move->LimitPos(mypos, mydir, 1.6);
	myrot = m_move->ConstantRotate();
	inScope();

	printf("\n");

}