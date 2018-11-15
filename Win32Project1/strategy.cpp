#include "strategy.h"

Vector3f genRandPos() {
	return Vector3f(
		0.01f*((rand() % 3 - 1)*rand() % 2500),
		0.01f*((rand() % 3 - 1)*rand() % 1000),
		0.01f*((rand() % 3 - 1)*rand() % 500));
}

void Strategy::init() {
	mypos = genRandPos();
	myrot = Vector3f(1.0, -1.0, 0.0);

	target = EMPTY;
	enemy = EMPTY;
	protect = EMPTY;

	temp = mypos;
	time = 0.0;
	mark = GetTickCount();
	m_move = new Move(mypos, myrot);
	m_move->setV(Vector3f(0.0, 0.0, 0.0));
}

void FKRStrategy::init() {
	Strategy::init();
	target = genRandPos();//��
	enemy = genRandPos();//anti FKR
	state = SEARCH;
}

void AntiFKRStrategy::init() {
	Strategy::init();
	target = genRandPos();//FKR
	protect = genRandPos();//��
	state = WAIT;
}

void Strategy::update() {
	time += 0.01;

	switch (state) {
	case CHASE://׷��
		chase();
		break;
	case SEARCH://Ŀ�겻�ڹ�����Χ��-������
		search();
		break;
	case WAIT://����������С��Χ����˶���
		wait();
		break;
	case ESCAPE://���
		escape();
		break;
	case GOBACK://�ع�
		escape();
		break;
	case APPROACH://����
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

	mypos = m_move->CalPos(mypos, mydir);
	myrot = m_move->CalRotate();


}

void Strategy::search() {
	printf("search\n");

	if (GetTickCount() - mark > 3500 && GetTickCount() - mark < 3505) {
		mydir = mydir * -1;
	}
	
	//�������	
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
	
	m_move->setA(Vector3f(0.7, 0.7, 0.7));

	mypos = m_move->LimitPos(mypos, mydir);
	myrot = m_move->CalRotate();
	inScope();	
}

void Strategy::wait() {
	printf("wait\n");

	if (GetTickCount() - mark > 500 && GetTickCount() - mark < 505) {
		mydir = mydir * -1;
		m_move->setA(Vector3f(0.8, 0.8, 0.8));
	}

	//�������	
	if (GetTickCount() - mark > 1000) {
		temp = protect + Vector3f(
			0.01f*((rand() % 3 - 1)*rand() % 300),
			0.01f*((rand() % 3 - 1)*rand() % 300),
			0.01f*((rand() % 3 - 1)*rand() % 200));
		mark = GetTickCount();
	}
	

	mypos = m_move->ConstantPos(mypos, temp);
	inScope();

}

void Strategy::escape() {
	printf("escape\n");
	if (abs(enemy.x - mypos.x)>10 && abs(enemy.y - mypos.y)>7 && abs(enemy.z - mypos.z)>4) {
		enemy = genRandPos();
		state = SEARCH;
		m_move->setV(Vector3f(0.0, 0.0, 0.0));
		return;
	}
	

	float xd = enemy.x > mypos.x ? -1.0 : 1.0;
	float yd = enemy.y > mypos.y ? -1.0 : 1.0;
	float zd = enemy.z > mypos.z ? -1.0 : 1.0;
	mydir = Vector3f(xd, yd, zd);
	
	m_move->setA(reverse(mypos - enemy)*2);

	mypos = m_move->CalPos(mypos, mydir);
	myrot = m_move->CalRotate();


}

void FKRStrategy::update() {
	printf("FKR\n");
	printf("target %f %f %f\n", target.x, target.y, target.z);
	printf("enemy %f %f %f\n", enemy.x, enemy.y, enemy.z);
	printf("mypos %f %f %f\n", mypos.x, mypos.y, mypos.z);

	//��������-escape
	if (abs(enemy.x - mypos.x)<3 && abs(enemy.y - mypos.y)<3 && abs(enemy.z - mypos.z)<3) {
		state = ESCAPE;
	}
	//Ŀ�����Լ�����̫Զ-search
	else if (!(abs(target.x - mypos.x)<10
		&& abs(target.y - mypos.y)<7
		&& abs(target.z - mypos.z)<7)) {
		state = SEARCH;
	}
	//��Ŀ����벻Զ-approach
	else if (abs(target.x - mypos.x) < 5 && abs(target.y - mypos.y) < 5 && abs(target.z - mypos.z) < 5) {
		state = APPROACH;
	}
	//׷��Ŀ��
	else {
		state = CHASE;
	}


	Strategy::update();

	printf("\n");

}

void AntiFKRStrategy::update() {
	printf("AntiFKR\n");
	printf("target %f %f %f\n", target.x, target.y, target.z);
	printf("protect %f %f %f\n", protect.x, protect.y, protect.z);
	printf("mypos %f %f %f\n", mypos.x, mypos.y, mypos.z);

	//Ŀ����뱻��������̫��-chase
	if (abs(target.x - protect.x)<5 && abs(target.y - protect.y)<5 && abs(target.z - protect.z)<5) {
		state = CHASE;
	}
	//�ڱ�����������Χ����
	else if (abs(protect.x - mypos.x) < 5 && abs(protect.y - mypos.y) < 5 && abs(protect.z - mypos.z) < 5) {
		m_move->setV(Vector3f(0.0, 0.0, 0.0));
		state = WAIT;
	}
	//�ص�������������Χ
	else {
		state = GOBACK;
	}


	Strategy::update();

	printf("\n");

}