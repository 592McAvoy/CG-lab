#include <math.h>
#include <GL/glew.h>
#include <GL/freeglut.h>

#include "ogldev_app.h"
#include "ogldev_util.h"
#include "ogldev_pipeline.h"
#include "ogldev_camera.h"
#include "ogldev_basic_lighting.h"
#include "ogldev_glut_backend.h"
#include "skybox.h"
#include "Model.h"

#define WINDOW_WIDTH  1920
#define WINDOW_HEIGHT 1200


class Game : public ICallbacks, public OgldevApp
{
public:

	Game()
	{   
		m_pGameCamera = NULL;
	
		m_pSkyBox = NULL;
		
		m_fkr = NULL;
		m_tank = NULL;

		m_dirLight.AmbientIntensity = 0.3f;
		m_dirLight.DiffuseIntensity = 0.8f;
		m_dirLight.Color = Vector3f(1.0f, 1.0f, 1.0f);
		m_dirLight.Direction = Vector3f(3.0f, -2.0f, 5.0f);

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
		SAFE_DELETE(m_tank);
	}


	bool Init()
	{
		Vector3f Pos(3.0f, 0.0f, -15.0f);
		Vector3f Target(0.5f, -1.2f, 1.0f);
		Vector3f Up(2.0, 1.0f, 0.0f);

		m_pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT, Pos, Target, Up);

		m_mode = 0;

		/*m_fkr = new Model(m_pGameCamera, m_persProjInfo, m_dirLight);
		if (!m_fkr->Init("../Content/blacksmith/blacksmiths.obj")) {
			return false;
		}*/

		m_tank = new Model(m_pGameCamera, m_persProjInfo, m_dirLight);
		if (!m_tank->Init("../Content/BomberDrone/BomberDrone.obj")) {
			return false;
		}

		m_pSkyBox = new SkyBox(m_pGameCamera, m_persProjInfo);
		if (!m_pSkyBox->Init(".",
			"../Content/nanji/nanji_L.jpg",
			"../Content/nanji/nanji_R.jpg",
			"../Content/nanji/nanji_U.jpg",
			"../Content/nanji/nanji_D.jpg",
			"../Content/nanji/nanji_F.jpg",
			"../Content/nanji/nanji_B.jpg")) {
			return false;
		}

		return true;
	}


	void Run()
	{
		GLUTBackendRun(this);
	}


	virtual void RenderSceneCB()
	{
		m_pGameCamera->OnRender();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//m_fkr->Render(m_mode);

		m_tank->Render(0);

		m_pSkyBox->Render();


		glutSwapBuffers();
	}


	void KeyboardCB(OGLDEV_KEY OgldevKey, OGLDEV_KEY_STATE State)
	{
		switch (OgldevKey) {
		case OGLDEV_KEY_ESCAPE:
		case OGLDEV_KEY_q:
			GLUTBackendLeaveMainLoop();
			break;
		case OGLDEV_KEY_0:
			m_mode = 0;
			break;
		case OGLDEV_KEY_1:
			m_mode = 1;
			break;
		case OGLDEV_KEY_2:
			m_mode = 2;
			break;
		case OGLDEV_KEY_3:
			m_mode = 3;
			break;
		case OGLDEV_KEY_4:
			m_mode = 4;
			break;
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

	Camera* m_pGameCamera;
	DirectionalLight m_dirLight;   
	SkyBox* m_pSkyBox;
	PersProjInfo m_persProjInfo;
	int m_mode;
	Model* m_fkr;
	Model* m_tank;

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