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

#define WINDOW_WIDTH  800
#define WINDOW_HEIGHT 600


class Game : public ICallbacks, public OgldevApp
{
public:

	Game()
	{   
		m_pGameCamera = NULL;
	
		m_pSkyBox = NULL;
		
		m_fkr = NULL;
		m_antifkr = NULL;
		m_house = NULL;
		m_human = NULL;
		m_house1 = NULL;
		m_pQuad = NULL;

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
		SAFE_DELETE(m_fkr);
		SAFE_DELETE(m_antifkr);
		SAFE_DELETE(m_house);
		SAFE_DELETE(m_human);
		SAFE_DELETE(m_house1);

	}


	bool Init()
	{
		srand(time(0));

		if (!m_shadowMapFBO.Init(WINDOW_WIDTH, WINDOW_HEIGHT)) {
			return false;
		}

		

		Vector3f Pos(0.0f, 10.0f, -22.0f);
		Vector3f Target(0.0f, -1.0f, 1.0f);
		Vector3f Up(0.0, 1.0f, 0.0f);

		m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);


		m_fkr = new FKRModel(m_pGameCamera, m_persProjInfo, m_dirLight, m_spotLight);
		m_antifkr = new AntiFKRModel(m_pGameCamera, m_persProjInfo, m_dirLight, m_spotLight);
		m_human = new HumanModel(m_pGameCamera, m_persProjInfo, m_dirLight, m_spotLight);

		m_fkr->setScale(0.1);
		m_fkr->setEnemy(m_antifkr);
		m_fkr->setTarget(m_human);
		if (!m_fkr->Init("../Content/starship/Starship.obj", "../Content/starship/1.png")) {
			return false;
		}
		
		
		m_antifkr->setScale(0.02);
		m_antifkr->setTarget(m_fkr);
		m_antifkr->setProtect(m_human);
		if (!m_antifkr->Init("../Content/BomberDrone/BomberDrone.obj", "../Content/BomberDrone/drone_d.tga")) {
			return false;
		}
				
		m_human->setScale(1);
		if (!m_human->Init("../Content/pig/pig.obj", "../Content/pig/TexturePig.png")) {
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
		if (!m_pQuad->Init("../Content/quad.obj", "../Content/grass.jpg")) {
			return false;
		}

		/*m_pSkyBox = new SkyBox(m_pGameCamera, m_persProjInfo);
		if (!m_pSkyBox->Init(".",
			"../Content/nanji/nanji_L.jpg",
			"../Content/nanji/nanji_R.jpg",
			"../Content/nanji/nanji_U.jpg",
			"../Content/nanji/nanji_D.jpg",
			"../Content/nanji/nanji_F.jpg",
			"../Content/nanji/nanji_B.jpg")) {
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
		
		glutSwapBuffers();
	}

	void ShadowMapPass()
	{
		m_shadowMapFBO.BindForWriting();

		glClear(GL_DEPTH_BUFFER_BIT);
		
		m_fkr->ShadowRender();

		m_antifkr->ShadowRender();

		m_house->ShadowRender();

		m_house1->ShadowRender();

		m_human->ShadowRender();

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	}

	void RenderPass()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		m_shadowMapFBO.BindForReading(GL_TEXTURE1);

		m_fkr->Render();

		m_antifkr->Render();

		m_house->Render();

		m_house1->Render();

		m_human->Render();

		
		m_pQuad->Render();

		//m_pSkyBox->Render();

		Vector3f pos = m_fkr->getPos();
		//printf("Pos(%f,%f,%f)\n", pos.x, pos.y, pos.z);
		m_pGameCamera->Move(pos);
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
	
	FKRModel* m_fkr;
	AntiFKRModel* m_antifkr;
	HumanModel* m_human;

	StaticModel* m_house;
	StaticModel* m_house1;
	StaticModel* m_pQuad;

	SkyBox* m_pSkyBox;

	void changeAmbLight(float ff) {
		m_fkr->changeAmbLight(ff);
		m_antifkr->changeAmbLight(ff);
		m_human->changeAmbLight(ff);
		m_house->changeAmbLight(ff);
		m_house1->changeAmbLight(ff);
	}
	void changeDiffLight(float ff) {
		m_fkr->changeDiffLight(ff);
		m_antifkr->changeDiffLight(ff);
		m_human->changeDiffLight(ff);
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