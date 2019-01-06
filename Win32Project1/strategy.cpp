#include "strategy.h"


const Vector3f EMPTY = Vector3f(0.0f, 0.0f, 0.0f);
Vector3f genRandPos() {
	return Vector3f(
		0.01f*((rand() % 3 - 1)*rand() % 2000),
		0.01f*((rand() % 2)*rand() % 1500),
		0.01f*((rand() % 3 - 1)*rand() % 1000));
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
	m_move->setV(Vector3f(2.0, 0.0, 0.0));
}

void HumanStrategy::init() {
	Strategy::init();
	mypos = Vector3f(
		0.01f*((rand() % 3 - 1)*rand() % 500),
		-6,
		0.01f*((rand() % 3 - 1)*rand() % 200));

	home = Vector3f(-4.5, -6, 13);//home pos
	//mypos = home;
	
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
	home = Vector3f(20, 0, 18);//home pos
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
		//printf("oops!error\n");
		break;
	}
	
	//printf("\n");
	
}

void Strategy::chase() {
	//printf("chase\n");
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
	//printf("approach\n");
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
	//printf("goback\n");
	float xd = protect.x > mypos.x ? 1.0 : -1.0;
	float yd = protect.y > mypos.y ? 1.0 : -1.0;
	float zd = protect.z > mypos.z ? 1.0 : -1.0;
	mydir = Vector3f(xd, yd, zd);

	m_move->setA(abs(protect - mypos)*0.2);
	checkBoundry();
	mypos = m_move->CalPos(mypos, mydir);
	myrot = m_move->CalRotate();


}

void Strategy::search() {
	//printf("search\n");

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
	
	m_move->setA(Vector3f(0.4,0.4, 0.4));

	checkBoundry();

	mypos = m_move->LimitPos(mypos, mydir,2);
	myrot = m_move->CalRotate();
	inScope();	
}

void Strategy::wait() {
	//printf("wait\n");

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
	//printf("escape\n");
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

void AntiFKRStrategy::getDisturbed() {
	//printf("disturbed\n");

	float xd = disturb.x > mypos.x ? -1.0 : 1.0;
	float yd = disturb.y > mypos.y ? -1.0 : 1.0;
	float zd = disturb.z > mypos.z ? -1.0 : 1.0;
	mydir = Vector3f(xd, yd, zd);

	m_move->setA(reverse(abs(disturb - enemy)) * 2);
	checkBoundry();
	mypos = m_move->CalPos(mypos, mydir);
	myrot = m_move->CalRotate();


}


void FKRStrategy::update() {
	
	//遇到敌人-escape
	if (enemy && abs(enemy.x - mypos.x)<3 && abs(enemy.y - mypos.y)<3 && abs(enemy.z - mypos.z)<3) {
		state = ESCAPE;
	}
	else if (enemy0 && abs(enemy0.x - mypos.x)<3 && abs(enemy0.y - mypos.y)<3 && abs(enemy0.z - mypos.z)<3) {
		state = ESCAPE;
	}
	//目标离自己距离太远-search
	else if (!(abs(target.x - mypos.x)<15
		&& abs(target.y - mypos.y)<15
		&& abs(target.z - mypos.z)<10)) {
		state = SEARCH;
	}
	//离目标距离不远-approach
	else if (abs(target.x - mypos.x) < 3 && abs(target.y - mypos.y) < 8 && abs(target.z - mypos.z) < 3) {
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

	//printf("\n");

}

void AntiFKRStrategy::update() {
	
	//遇到干扰-escape
	if (disturbed && abs(disturb.x - mypos.x)<2 && abs(disturb.y - mypos.y)<2 && abs(disturb.z - mypos.z)<2) {
		getDisturbed();
		return;
	}
	//目标距离被保护对象太近-chase
	if ((abs(target.x - protect.x)<5 && abs(target.z - protect.z)<5)
		|| abs(target.x - mypos.x)<3 && abs(target.y - mypos.y)<3 && abs(target.z - mypos.z)<3) {
		if (state != CHASE)
			m_move->setV(0.1*(target - mypos));
		state = CHASE;
		//printf("chase\n");
		//homesick = false;
	}	
	//在被保护对象周围待命
	else if (abs(protect.x - mypos.x) < 5 && abs(protect.y - mypos.y) < 6 && abs(protect.z - mypos.z) < 5) {
		//printf("wait\n");
		m_move->setV(EMPTY);
		state = WAIT;
	}
	//返回待命点
	else if (homesick) {
		//printf("gohome\n");
		gohome();
		state = HOME;
		//return;
	}
	//回到被保护对象周围
	else {
		//printf("goback\n");
		state = GOBACK;
	}

	Strategy::update();
	inScope();

	//printf("\n");

}

void  AntiFKRStrategy::gohome() {
	if (abs(home.x - mypos.x) < 4 && abs(home.z - mypos.z) < 4) {
		mydir = Vector3f(0, 0, 0);
		//m_move->setV(Vector3f(0, 0, 0));
		return;
	}

	float xd = home.x > mypos.x ? 1.0 : -1.0;
	float yd = home.y > mypos.y ? 1.0 : -1.0;
	float zd = home.z > mypos.z ? 1.0 : -1.0;
	mydir = Vector3f(xd, yd, zd);

	m_move->setA(abs(home - mypos)*0.5);
	//checkBoundry();
	mypos = m_move->LimitPos(mypos, mydir, 2);
	myrot = m_move->ConstantRotate();
}

void HumanStrategy::walk_out() {
	mydir = Vector3f(1, 0, -1);
	m_move->setA(Vector3f(0.2, 0, 0.26));

	checkBoundry();

	mypos = m_move->LimitPos(mypos, mydir, 1.3);
	myrot = m_move->ConstantRotate() * 2;
	//printf("walkout\trot : (%f, %f, %f)\n", myrot.x, myrot.y, myrot.z);
	inScope();
}

void HumanStrategy::human_escape() {
	//printf("escape\n");

	if (abs(home.x - mypos.x) < 4 && abs(home.z - mypos.z) < 4) {
		mydir = Vector3f(rand() % 3 - 1, 0, rand() % 3 - 1);
		return;
	}
	else {
		float xd = home.x > mypos.x ? 1.0 : -1.0;
		float yd = 0;
		float zd = home.z > mypos.z ? 1.0 : -1.0;
		mydir = Vector3f(xd, yd, zd);
	}	//checkBoundry();

	m_move->setA(abs(enemy - mypos)*0.5);
	mypos = m_move->LimitPos(mypos, mydir, 2);
	//myrot = m_move->ConstantRotate() * 2;
	myrot = m_move->targetRotate(home, mypos) + Vector3f(0,5.0f,0);
	//printf("escape\trot : (%f, %f, %f)\n", myrot.x, myrot.y, myrot.z);

}
	
void HumanStrategy::rand_walk() {
		if (GetTickCount() - mark > 1000 && GetTickCount() - mark < 1005) {
			mydir = mydir * -1;
		}

		//随机方向	
		if (GetTickCount() - mark > 2000) {
			mydir = Vector3f(rand() % 3 - 1, 0, rand() % 3 - 1);
			mark = GetTickCount();
		}

		m_move->setA(Vector3f(0.22, 0, 0.20));

		checkBoundry();

		mypos = m_move->LimitPos(mypos, mydir, 1.8);
		myrot = m_move->ConstantRotate() * 2;
		//printf("walk\trot : (%f, %f, %f)\n", myrot.x, myrot.y, myrot.z);
		inScope();
	}

void HumanStrategy::update() {
	//printf("Human\n");
	//printf("mypos %f %f %f\n", mypos.x, mypos.y, mypos.z);

	//FKR太近-escape
	if (enemy && (abs(enemy.x - mypos.x)<2 && abs(enemy.z - mypos.z)<2 && abs(enemy.y - mypos.y)<8)){
		//printf("escape\n");
		if (!escape) {
			m_move->setV(0.4*(home - mypos));
		}
		escape = true;
		human_escape();
	}
	//离开home
	else if (abs(home.x - mypos.x) <2 || abs(home.z - mypos.z) < 2) {
		//printf("walk out\n");
		escape = false;
		walk_out();
	}
	//逛逛
	else {
		//printf("walk around\n");
		escape = false;
		rand_walk();
	}

	
	//inScope();

	//printf("\n");

}