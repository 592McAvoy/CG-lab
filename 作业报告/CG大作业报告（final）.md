# CG大作业报告

516030910101 罗宇辰

---

## 三次迭代主要成果 ##
 - 根据速度、加速度和运动方向来计算模型的位置和旋转角度，以实现对模型运动的仿真
 - 为不同模型的制订了不同的行动策略
 - 实现了脚本调度，通过Script类制订FKR，AntFKR和Human模型的行动脚本
 - 实现了Camera调度和跟踪
 - 实现了模型的阴影效果
 - 重新进行了场景建模，统一了场景风格 
 - 使用粒子系统和billboard技术实现了下雪特效
 
----------
## 具体实现介绍 ##
### 一、运动仿真 ###
- 代码文件：Move.h/cpp

定义了Move类，
记录运动速度和位置参数：

        Vector3f pos;
    	Vector3f rotation;
    	
    	Vector3f v;
    	Vector3f prev_v;
    	Vector3f a;
    	Vector3f ds;
对外多种提供位移和旋转角度的计算方法：

        # 默认情况
        Vector3f CalPos(Vector3f p, Vector3f direction);
    	Vector3f CalRotate();
        # 根据目标位置运动
    	Vector3f ConstantPos(Vector3f p, Vector3f tar);
    	Vector3f ConstantRotate();
        # 限制速度下运动
    	Vector3f LimitPos(Vector3f p, Vector3f direction, float limitV);
        # 调整模型方向
    	Vector3f targetRotate(Vector3f now, Vector3f target);
总体是根据如下公式计算位移、速度和偏转：
        
        
        ds = v*dt + direction*a*0.5*dt*dt;  # 位移
    	v += direction*a*dt;    # 速度
    	
    	float ry = norm(atan2(v.z, v.x) - atan2(prev_v.z, prev_v.x));
    	rotation += Vector3f(0, ry, 0)*(180 / PI);  #y轴偏转
在研究合理的运动仿真的过程中，我做过以下尝试：   
1. 使用函数模拟运动     
2. 使用生成随机点，构造贝叶斯曲线来模拟运动轨迹        
3. 在x、y、z三轴都用速度切线方向变化来模拟运动偏转    
但是这些最后效果都不好，要么是不真实，要么是对参数要求太多，要么是计算量太大，所以最后选择按照老师的要求来做运动计算

### 二、运动策略 ###
- 代码文件：strategy.h/cpp 
- 总体结构图：  
![strategy][1]

在父类Strategy中，  
定义了模型自身的属性和交互对象位置：

    State state; # 状态
	
	# 自身的位置、旋转和方向
	Vector3f mypos;
	Vector3f myrot;
	Vector3f mydir;
	
    # 可能的交互对象位置
	Vector3f target;
	Vector3f enemy;
	Vector3f protect;
定义了边界检查方法：

    void inScope();
	void checkBoundry();
在具体类中定义了不同的init和update方法，以FKR模型为例：

    void FKRStrategy::update() {
	# 遇到敌人-escape
	if (abs(enemy.x - mypos.x)<5 && abs(enemy.y - mypos.y)<5 && abs(enemy.z - mypos.z)<5) {
		state = ESCAPE;
	}
	# 有多个AntiFKR时，做两次遇敌判断
	else if (enemy0 && abs(enemy0.x - mypos.x)<5 && abs(enemy0.y - mypos.y)<5 && abs(enemy0.z - mypos.z)<5) {
		state = ESCAPE;
	}
	# 目标不在自己的视野范围内-search
	else if (!(abs(target.x - mypos.x)<15
		&& abs(target.y - mypos.y)<15
		&& abs(target.z - mypos.z)<10)) {
		state = SEARCH;
	}
	# 离目标距离不远-approach
	else if (abs(target.x - mypos.x) < 3 && abs(target.y - mypos.y) < 8 && abs(target.z - mypos.z) < 3) {
		state = APPROACH;
	}
	# 离目标很远-chase
	else {
		if (state != CHASE)
			m_move->setV(0.1*(target - mypos));
		state = CHASE;
	}


	Strategy::update();
	inScope();
    }
每种模型都有自己update方法，因此不同的模型会选择不同的运动策略。每一次渲染都会更新交互模型的信息，然后调整模型的运动策略。 

对于multi情况，会进行额外的处理，以AntiFKR为例：

    //遇到干扰-escape
	if (disturbed && abs(disturb.x - mypos.x)<2 && abs(disturb.y - mypos.y)<2 && abs(disturb.z - mypos.z)<2) {
		getDisturbed();
		return;
	}
当有两个FKR模型时，Script指定其中一个干扰AntiFKR，相对于只有一个AKR的情况，AntiFKR就会额外对干扰对象做判断，有可能采取躲避而不是追逐的策略

### 三、脚本调度 ###
 - 代码文件：Script.h/cpp

    
        class Script {
            public:
            	Script(Mode m);
            	~Script();
    	
        	bool Init(Camera* pCamera, const PersProjInfo& p, const DirectionalLight& l, const SpotLight& s);
        	void ShadowRender();
        	void Render();
    
        private:
        	Mode m_mode;    //脚本模式
        	
        	Camera* m_pCamera;
        
        	FKRModel* m_fkr;
        	AntiFKRModel* m_antifkr;
        	HumanModel* m_human;
        	
        	//multi
        	AntiFKRModel* m_antifkr0;
        	FKRModel* m_fkr0;
        };
        
封装了对DynamicModel的加载和渲染，设置了三种模式，三种模式对应不同的模型数量和运动策略：

    enum Mode{
        STORY=0,     # 故事（单个FKR & 单个AntiFKR）
        MULTIFKR,    # 两个FKR & 单个AntiFKR
        MULTIANTI    # 两个AntiFKR & 多个FKR
    };
在Script的构造函数中可以指定脚本模式：

    m_script = new Script(MULTIFKR);

渲染时，直接通过Script实例进行渲染：

    m_script->Render();


### 四、Camera调度和跟踪 ###
- 代码文件：ogldev_camera.h & camera.cpp


1. 调度
可以通过鼠标和方向键对相机进行调度：

            void KeyboardCB(OGLDEV_KEY OgldevKey, OGLDEV_KEY_STATE State)
        	{
        		switch (OgldevKey) {
        		case OGLDEV_KEY_ESCAPE:
        		case OGLDEV_KEY_q:
        			GLUTBackendLeaveMainLoop();
        			break;
        		default:
        			m_pGameCamera->OnKeyboard(OgldevKey);
        		}
        	}    
         
            void PassiveMouseCB(int x, int y)
            {
              m_pGameCamera->OnMouse(x, y);
            }
    
    2. 跟踪  
    自己定义实现了相机的Move方法：
    
            void Camera::Move(Vector3f pos)
            {
                # focusPos预定义为原点，根据pos不断变化
            	const float DeltaX = pos.x - m_focusPos.x;
            	const float DeltaY = pos.y - m_focusPos.y;
            	
            	m_focusPos = pos;
            
            	m_AngleH += (float)DeltaX * 3.0f;
            	m_AngleV += (float)DeltaY * 0.3f;
            	
            	Update();
            }
    通过传入模型的实时位置就可以使用相机跟踪模型的运动：
    
            m_pCamera->Move(m_antifkr->getPos());
    

### 五、 阴影效果 ###
- 根据教程[阴影贴图（一）][2]和[阴影贴图（二）][3]实现了阴影效果   

调用 ShadowMapPass() 函数将深度信息渲染进入 shadow map 中，接着用 RenderPass() 函数来显示阴影纹理

    void RenderSceneCB()
    {
        m_pGameCamera->OnRender();
        
        ShadowMapPass();
        RenderPass();
        
        glutSwapBuffers();
    }


效果如图：   
![阴影][4]   

### 六、 场景建模 ###

 一开始使用了天空盒子和人物模型，但是在绘制阴影时需要使用一个地面模型，与天空盒子风格难以融合，就弃用了天空盒子，后来为了场景卡通风格的统一，也弃用了人物模型，改成了小猪模型   

    # 房屋
    StaticModel* m_house;
    StaticModel* m_house1;
    # 草地
    StaticModel* m_pQuad;
    # 云朵
	CloudModel* m_cloud;
	CloudModel* m_cloud0;
	CloudModel* m_cloud1;

 为了好看还加入了云朵模型，并且为了实现云朵的飘动定义了CloudModel类

### 七、 粒子系统 ###
- 根据教程[使用 Transform Feedback 创建粒子系统][5]实现了下雪的特效

原教程实现的是从地面向上喷射散发的烟火效果，我更改了Particle的Init函数,根据传入参数Pos的x,y,z值自动计算调整LAUNCHER粒子的数量和位置：

    bool ParticleSystem::InitParticleSystem(const Vector3f& Pos)
    {   
        Particle Particles[MAX_PARTICLES*X_AMOUNT*Z_AMOUNT];
        ZERO_MEM(Particles);
    
    	for (int i = 0; i < X_AMOUNT; i++) {
    		for (int j = 0; j < Z_AMOUNT; j++) {
    			int idx = i * X_AMOUNT + j;
    			Particles[idx].Type = PARTICLE_TYPE_LAUNCHER;
    			Particles[idx].Pos = Vector3f((i - 1)*Pos.x, Pos.y, (j-Z_AMOUNT)*Pos.z);
    			Particles[idx].Vel = Vector3f(0.0f, -0.001f, 0.0f);
    			Particles[idx].LifetimeMillis = 0.0f;
    		}
    	}
    	...
    }
还更改了几何着色器文件ps_update.gs，调整了不同TYPE的粒子的生命周期和运动速度，以模拟雪花飘落的效果    
![snow][6]


----------


## 效果展示 ##
![demo][7]


----------


  [1]: ./assets/strategy.png
  [2]: http://wiki.jikexueyuan.com/project/modern-opengl-tutorial/tutorial23.html
  [3]: http://wiki.jikexueyuan.com/project/modern-opengl-tutorial/tutorial24.html
  [4]: ./assets/shadow.png
  [5]: http://wiki.jikexueyuan.com/project/modern-opengl-tutorial/tutorial28.html
  [6]: ./assets/snow.gif
  [7]: ./assets/demo.gif