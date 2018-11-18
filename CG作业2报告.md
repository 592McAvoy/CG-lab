# CG作业2报告

*516030910101 
罗宇辰*

---

## 本次迭代实现任务简介 ##
本次迭代用到的库和上一次一样，来自教程——《[现代OpenGL教程][1]》   
这次迭代中，我完成的工作主要有：   

1. 重新封装模型load逻辑为StaticModel和DynamicModel类，用来表示静态的环境模型和动态的FKR，antiFKR及human模型
2. 创建Strategy类，构造动态模型的行动策略，实现其自主决策
3. 重写Move类，抛弃之前用函数来表示位置变化的实现，改为通过加速度、速度、方向来计算位置的变换


----------


## 具体实现说明 ##

### 1. 核心部分结构 ###
![类图][2]   

主要文件：
 - dynamicModel.h/cpp 
 - strategy.h/cpp 
 - Move.h/cpp
 
其他：
 - main.cpp #程序启动入口 
 - staticmodel.h/cpp

文件夹：
 - Content #模型和纹理 
 - shaderFile #着色器文件

### 2. 自主决策实现 ###

    enum State { CHASE = 0, SEARCH, WAIT, ESCAPE, GOBACK, APPROACH };
共定义了6种状态，每次渲染时，FKR和antiFKR将根据当前彼此的位置关系更新自己的运动状态

    void FKRStrategy::update() {
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
    	
    	Strategy::update();//switch函数-具体策略实现
    	inScope();//确保运动范围不越界
	}
不同的strategy子类有不同的update方法，以FKR为例，它每次都会判断antiFKR和human与自己的位置关系，不断变更自己的运动策略

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

以chase策略为例，将判断目标位置关系来确定运动方向，并根据距离调整加速度大小，最后将当前位置、加速度、加速度方向传递给move，得到新的位置和旋转角度

### 3. 模型交互 ###

    class AntiFKRModel :public DynamicModel
    {
    public:
	    //...
    private:
	    AntiFKRStrategy* m_strategy;
	    DynamicModel* m_protect;
	    DynamicModel* m_target;

    	void updateInfo() {
    		if (m_protect)
    			m_strategy->setProtect(m_protect->getPos());
    		if (m_target)
    			m_strategy->setTarget(m_target->getPos());
	    }
    };
以antiFKR模型为例，他持有两个dynamicModel对象，一个是目标对象FKR，一个是保护对象human，每次渲染都会先调用updateInfo来更新strategy中的信息，实现模型之间的信息交流，为自主决策提供判断数据

### 4. 场景搭建 ###
因为没有多少找到大小合适、风格统一的模型，所以场景建模主要是使用了一个天空盒子和两个静态房屋模型

    SkyBox* m_pSkyBox;
    
    StaticModel* m_house;
	StaticModel* m_house1;

效果如图
![场景建模][3]

### 5. 光照设置 ###
本次迭代中，光照部分使用了教程作者封装好的光源及光照技术的实现，全局设置了一个平行光光源

    m_dirLight.AmbientIntensity = 0.3f;
	m_dirLight.DiffuseIntensity = 0.8f;
	m_dirLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
	m_dirLight.Direction = Vector3f(-3.0f, -2.0f, 5.0f);

使用键盘按键a/s以及z/x可以对环境光强度和漫反射光强度进行调整
效果如下
![常规][4]
![亮][5]


----------
## 开发中遇到的问题 ##

 - 由于我的物理和数学知识的匮乏，尝试了很多方法（比如上一次使用二阶贝塞尔曲线来模拟运动轨迹）之后还是无法很好的仿真无人机的运动。
 - 在引入速度和加速度之后，对于运动中的偏转，以及运动策略切换前后的状态过渡我实现的也非常吃力，最后效果也不是很好
 - 由于不知道怎么进行合理的碰撞检测和碰撞运动实现，我很粗暴的选择了用限制运动范围的办法来解决静态和动态模型之间可能出现的交错问题，在交互这一点没有做好


----------
## 总结 ##
在本次迭代中对模型导入和光照效果实现有了进一步的认识，虽然有很多地方都是一知半解，但是感觉对图形学的认识稍微深了一点，也深刻地认识到了要想模拟物体的运动和搭建逼真的场景是多么难，唉，希望自己继续加油


  [1]: http://ogldev.atspace.co.uk/index.html
  [2]: http://m.qpic.cn/psb?/V13Ti98m05LW5b/Z9iMbW0oBOzWC5cOCqqt3pw0KksOkZfPXfmFwtBCjJU!/b/dDEBAAAAAAAA&bo=PAMGAwAAAAADBxg!&rf=viewer_4
  [3]: http://m.qpic.cn/psb?/V13Ti98m05LW5b/sx4PCfrqFXO4LP39DeSHH94xy3AcwjGW3qp0VDSoKUg!/b/dFMBAAAAAAAA&bo=rwQBAwAAAAADB4s!&rf=viewer_4
  [4]: http://m.qpic.cn/psb?/V13Ti98m05LW5b/5bm4c5Vw101dxPib8GvUj831teAHUwqwla2IB39qw6w!/b/dDYBAAAAAAAA&bo=2QOfAgAAAAADB2U!&rf=viewer_4
  [5]: http://m.qpic.cn/psb?/V13Ti98m05LW5b/JS9VknJqP67IoaC0kz.YQ4aVVCcZ02W25FWD7QYI3HM!/b/dDYBAAAAAAAA&bo=6wNrAgAAAAADN5M!&rf=viewer_4