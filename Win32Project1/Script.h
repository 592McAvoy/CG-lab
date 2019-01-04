#pragma once
#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "ogldev_app.h"
#include "ogldev_util.h"
#include "ogldev_pipeline.h"
#include "ogldev_camera.h"
#include "ogldev_glut_backend.h"

#include<vector>

//#include "ogldev_basic_lighting.h"
#include "ogldev_shadow_map_fbo.h"
#include "shadow_map_technique.h"

#include "skybox.h"
#include "dynamicModel.h"
#include "staticModel.h"


enum Mode{STORY=0, MULTIFKR, MULTIANTI,};
class Script {
public:
	Script(Mode m) {
		m_mode = m;
		m_fkr = NULL;
		m_antifkr = NULL;
		m_human = NULL;

	}
	~Script(){
		SAFE_DELETE(m_fkr);
		SAFE_DELETE(m_antifkr);
		SAFE_DELETE(m_human);
	}
	
	bool Init(Camera* pCamera, const PersProjInfo& p, const DirectionalLight& l, const SpotLight& s);
	void ShadowRender();
	void Render();



private:
	Mode m_mode;

	Camera* m_pCamera;

	FKRModel* m_fkr;
	AntiFKRModel* m_antifkr;
	HumanModel* m_human;
	
	//multi
	AntiFKRModel* m_antifkr0;
	FKRModel* m_fkr0;
};