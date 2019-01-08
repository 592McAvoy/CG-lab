# CG大作业报告

516030910101 罗宇辰

---

## 本次迭代主要成果 ##

 - 实现了脚本调度，通过Script类制订FKR，AntFKR和Human模型的行动策略
 - 完善了模型的行动策略，提高了模型运动的仿真度
 - 实现了Camera调度和跟踪
 - 实现了模型的阴影效果
 - 重新进行了场景建模，统一了场景风格 
 
----------
## 具体实现介绍 ##
### 一、脚本调度 ###
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

### 二、运动策略完善 ###
- 代码文件：strategy.h/cpp 、Move.h
1. 为了实现多FKR和多AntiFKR，给AntiFKR的策略加入了干扰对象disturb，给FKR的策略增加一个enemy对象

            class AntiFKRStrategy :public Strategy
            {
            ...
            private:
            	Vector3f disturb;
            };
            
            class FKRStrategy :public Strategy
            {
            ...
            private:
            	Vector3f enemy0;
            };

2. 为Human的运动策略加入了escape方法

            class HumanStrategy :public Strategy
            {
            ...
            private:
            
            	void walk_out();
            	void human_escape();
            	void rand_walk();
            
            	bool escape = false;
            ...
            };
    当FKR距离Human太近时，Human会朝home逃跑：
            
            void HumanStrategy::update() {
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
            		escape = false;
            		walk_out();
            	}
            	//逛逛
            	else {
            		escape = false;
            		rand_walk();
            	}
            }

### 三、Camera调度和跟踪 ###
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
    
    2.跟踪
    定义了相机的Move方法：
    
        void Camera::Move(Vector3f pos);
    通过传入模型的实时位置就可以使用相机跟踪模型的运动：
    
        m_pCamera->Move(m_antifkr->getPos());
    

### 四、 阴影效果 ###
根据教程[阴影贴图（一）][1]和[阴影贴图（二）][2]实现了阴影效果，效果如图：   
![阴影][3]   

### 五、 场景建模 ###

 - 弃用了之前的天空盒子和人物模型
 - 新增了草地、小猪和云朵模型


----------


## 效果展示 ##
![demo][4]


  [1]: http://wiki.jikexueyuan.com/project/modern-opengl-tutorial/tutorial23.html
  [2]: http://wiki.jikexueyuan.com/project/modern-opengl-tutorial/tutorial24.html
  [3]: ./assets/shadow.png
  [4]: ./assets/pig.gif
