#include "Script.h"

bool Script::Init(
	Camera* pCamera,
	const PersProjInfo& p,
	const DirectionalLight& l,
	const SpotLight& s) 
{

	m_pCamera = pCamera;

	m_human = new HumanModel(pCamera, p, l, s);
	m_human->setScale(1);
	if (!m_human->Init("../Content/pig/pig.obj", "../Content/pig/TexturePig.png")) {
		return false;
	}

	m_fkr = new FKRModel(pCamera, p, l, s);
	m_antifkr = new AntiFKRModel(pCamera, p, l, s);

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

	switch (m_mode)
	{
	case STORY: {
		m_human->setEnemy(m_fkr);
		m_human->setInitPos(Vector3f(-5, -6, 14));
		m_fkr->setInitPos(Vector3f(20, 16, 10));
		m_antifkr->setInitPos(Vector3f(20, 0, 16));
		m_antifkr->likeHome();

	}break;

	case NORMAL: {

	}break;

	case MULTIANTI: {
		m_antifkr0 = new AntiFKRModel(pCamera, p, l, s);

		m_antifkr0->setScale(0.02);
		m_antifkr0->setTarget(m_fkr);
		m_antifkr0->setProtect(m_human);
		if (!m_antifkr0->Init("../Content/BomberDrone/BomberDrone.obj", "../Content/BomberDrone/drone_d.tga")) {
			return false;
		}
		m_antifkr->likeHome();

		m_fkr->setEnemy(m_antifkr0);

	}break;

	default:
		return false;
	}

	return true;

}
void Script::Render() {
	m_fkr->Render();

	m_antifkr->Render();

	m_human->Render();

	switch (m_mode)
	{
	case STORY:{
		Vector3f pos = m_human->getPos();
		//printf("Pos(%f,%f,%f)\n", pos.x, pos.y, pos.z);
		m_pCamera->Move(pos); 
	}
		break;
	case NORMAL:
		break;
	case MULTIANTI: {
		m_antifkr0->Render();
	}
		break;
	default:
		break;
	}
}

void Script::ShadowRender() {
	m_fkr->ShadowRender();

	m_antifkr->ShadowRender();

	m_human->ShadowRender();

	if (m_mode == MULTIANTI) {
		m_antifkr0->ShadowRender();
	}
}