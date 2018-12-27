#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "ogldev_app.h"
#include "ogldev_util.h"
#include "ogldev_pipeline.h"
#include "ogldev_camera.h"
#include "ogldev_glut_backend.h"


//#include "ogldev_basic_lighting.h"
#include "ogldev_shadow_map_fbo.h"
#include "shadow_map_technique.h"

#include "skybox.h"
#include "dynamicModel.h"
#include "staticModel.h"
#include "Script.h"

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600


class Game : public ICallbacks, public OgldevApp
{
public:

	Game()
	{   
		m_pGameCamera = NULL;
	
		m_pSkyBox = NULL;
		
		m_house = NULL;
		m_house1 = NULL;
		m_pQuad = NULL;

		m_script = NULL;

		m_spotLight.AmbientIntensity = 0.1f;
		m_spotLight.DiffuseIntensity = 0.9f;
		m_spotLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
		m_spotLight.Attenuation.Linear = 0.00001f;
		m_spotLight.Position = Vector3f(-10.0, 20.0, 25.0f);
		m_spotLight.Direction = Vector3f(0.0f, -1.0f, -1.0f);
		m_spotLight.Cutoff = 180.0f;

		m_dirLight.AmbientIntensity = 0.3f;
		m_dirLight.DiffuseIntensity = 0.8f;
		m_dirLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
		m_dirLight.Direction = Vector3f(-3.0f, -2.0f, 5.0f);

		m_persProjInfo.FOV = 60.0f;
		m_persProjInfo.Height = WINDOW_HEIGHT;
		m_persProjInfo.Width = WINDOW_WIDTH;
		m_persProjInfo.zNear = 1.0f;
		m_persProjInfo.zFar = 50.0f;
	}


	virtual ~Game()
	{		
		
		SAFE_DELETE(m_pGameCamera);        
		SAFE_DELETE(m_pSkyBox);
		
		SAFE_DELETE(m_house1);
		SAFE_DELETE(m_house);
		SAFE_DELETE(m_script);

	}


	bool Init()
	{
		srand(time(0));

		if (!m_shadowMapFBO.Init(WINDOW_WIDTH, WINDOW_HEIGHT)) {
			return false;
		}

		

		Vector3f Pos(0.0f, 10.0f, -20.0f);
		Vector3f Target(0.0f, -7.0f, 20.0f);
		Vector3f Up(0.0, 1.0f, 0.0f);

		m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

		m_script = new Script(STORY);
		if (!m_script->Init(m_pGameCamera, m_persProjInfo, m_dirLight, m_spotLight)) {
			return false;
		}

		m_cloud = new CloudModel(m_pGameCamera, m_persProjInfo, m_dirLight, m_spotLight);
		m_cloud->setInfo(0.4, Vector3f(0.0, 10, -5), Vector3f(0, 150, 0));
		m_cloud->setChangeScale(0.0002);
		if (!m_cloud->Init("../Content/cloud/file.obj", "../Content/white.png")) {
			return false;
		}
		m_cloud0 = new CloudModel(m_pGameCamera, m_persProjInfo, m_dirLight, m_spotLight);
		m_cloud0->setInfo(2.0, Vector3f(-15.0, 8, 12), Vector3f(0, 0, 0));
		m_cloud0->setChangeScale(0.00001);
		if (!m_cloud0->Init("../Content/cloud/file.obj", "../Content/white.png")) {
			return false;
		}
		m_cloud1= new CloudModel(m_pGameCamera, m_persProjInfo, m_dirLight, m_spotLight);
		m_cloud1->setInfo(1.15, Vector3f(10.0, 5, 4), Vector3f(0, -20, 0));
		m_cloud1->setChangeScale(0.00003);
		if (!m_cloud1->Init("../Content/cloud/file.obj", "../Content/white.png")) {
			return false;
		}
		

		m_house = new StaticModel(m_pGameCamera, m_persProjInfo, m_dirLight, m_spotLight);
		m_house->setInfo(0.05, Vector3f(-7.0, -6, 17), Vector3f(0, 40, 0));
		if (!m_house->Init("../Content/h/h1.obj", "../Content/h/h1.jpg")) {
			return false;
		}

		m_house1 = new StaticModel(m_pGameCamera, m_persProjInfo, m_dirLight, m_spotLight);
		m_house1->setInfo(0.05, Vector3f(20, -6, 18), Vector3f(0, 145, 0));
		if (!m_house1->Init("../Content/h/h2.obj", "../Content/h/h2.jpg")) {
			return false;
		}

		m_pQuad = new StaticModel(m_pGameCamera, m_persProjInfo, m_dirLight, m_spotLight);
		m_pQuad->setInfo(25.0f, Vector3f(0.0f, -6, 5), Vector3f(90.0f, 75.0f, 0.0f));
		if (!m_pQuad->Init("../Content/quad.obj", "../Content/grass1.jpg")) {
			return false;
		}

		/*m_pSkyBox = new SkyBox(m_pGameCamera, m_persProjInfo);
		if (!m_pSkyBox->Init(".",
			"../Content/1.jpg",
			"../Content/1.jpg",
			"../Content/1.jpg",
			"../Content/1.jpg",
			"../Content/1.jpg",
			"../Content/1.jpg")) {
			return false;
		}*/

		return true;
	}


	void Run()
	{
		GLUTBackendRun(this);
	}


	virtual void RenderSceneCB()
	{	
		m_pGameCamera->OnRender();
		
		ShadowMapPass();
		RenderPass();

		//m_pSkyBox->Render();
		
		glutSwapBuffers();
	}

	void ShadowMapPass()
	{
		m_shadowMapFBO.BindForWriting();

		glClear(GL_DEPTH_BUFFER_BIT);
		
		m_script->ShadowRender();

		m_house->ShadowRender();
		m_house1->ShadowRender();

		m_cloud->ShadowRender();
		m_cloud0->ShadowRender();
		m_cloud1->ShadowRender();

		

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderPass()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		m_shadowMapFBO.BindForReading(GL_TEXTURE1);

		
		m_script->Render();

		m_house->Render();
		m_house1->Render();
		

		m_cloud->Render();
		m_cloud0->Render();
		m_cloud1->Render();

		m_pQuad->Render();	

	}


	void KeyboardCB(OGLDEV_KEY OgldevKey, OGLDEV_KEY_STATE State)
	{
		switch (OgldevKey) {
		case OGLDEV_KEY_ESCAPE:
		case OGLDEV_KEY_q:
			GLUTBackendLeaveMainLoop();
			break;
		case OGLDEV_KEY_a:
			changeAmbLight(0.05f);
			break;
		case OGLDEV_KEY_s:
			changeAmbLight(-0.05f);
			break;
		case OGLDEV_KEY_z:
			changeDiffLight(0.05f);
			break;
		case OGLDEV_KEY_x:
			changeDiffLight(-0.05f);
		default:
			m_pGameCamera->OnKeyboard(OgldevKey);
		}
	}

	void MouseCB(OGLDEV_MOUSE OgldevMouse, OGLDEV_KEY_STATE OgldevKeyState, int x, int y) {

		Vector3f nn = m_pGameCamera->GetClickPos(x, y);
		nn.Normalize();
		Vector3f tar = m_pGameCamera->GetTarget();
		printf("clickPos(%f,%f) target(%f,%f)\n", nn.x, nn.y, tar.x, tar.y);
	}


	virtual void PassiveMouseCB(int x, int y)
	{
		m_pGameCamera->OnMouse(x, y);
	}

private:
	ShadowMapFBO m_shadowMapFBO;
	DirectionalLight m_dirLight;
	SpotLight m_spotLight;

	Camera* m_pGameCamera;  
	PersProjInfo m_persProjInfo;
	
	/*FKRModel* m_fkr;
	AntiFKRModel* m_antifkr;
	HumanModel* m_human;*/

	StaticModel* m_house;
	StaticModel* m_house1;
	StaticModel* m_pQuad;

	CloudModel* m_cloud;
	CloudModel* m_cloud0;
	CloudModel* m_cloud1;

	SkyBox* m_pSkyBox;

	Script* m_script;

	void changeAmbLight(float ff) {
		/*m_fkr->changeAmbLight(ff);
		m_antifkr->changeAmbLight(ff);
		m_human->changeAmbLight(ff);*/
		m_house->changeAmbLight(ff);
		m_house1->changeAmbLight(ff);
	}
	void changeDiffLight(float ff) {
		/*m_fkr->changeDiffLight(ff);
		m_antifkr->changeDiffLight(ff);
		m_human->changeDiffLight(ff);*/
		m_house->changeDiffLight(ff);
		m_house1->changeDiffLight(ff);
	}

};


int main(int argc, char** argv)
{
	GLUTBackendInit(argc, argv, true, false);

	if (!GLUTBackendCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, false, "ÎÞÈË»ú")) {
		return 1;
	}

	Game* pApp = new Game();

	if (!pApp->Init()) {
		return 1;
	}

	pApp->Run();

	delete pApp;

	return 0;
}