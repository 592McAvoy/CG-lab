#pragma once
#include "ogldev_camera.h"
#include "mesh.h"
//#include "ogldev_basic_lighting.h"
#include "lighting_technique.h"
#include "strategy.h"

class DynamicModel
{
public:
	DynamicModel(const Camera* pCamera, const PersProjInfo& p, const DirectionalLight& l, const SpotLight& s);

	~DynamicModel();

	bool Init(const string& modlePathName, const string& texturePathName);

	void Render();

	void ShadowRender();

	Vector3f getPos() {
		return m_pos;
	}
	
	Vector3f getRot() {
		return m_rot;
	}
	void setScale(float ss) {
		m_scale = ss;
	}

	void changeAmbLight(float ff) {
		m_dirLight.AmbientIntensity += ff;
	}
	void changeDiffLight(float ff) {
		m_dirLight.DiffuseIntensity += ff;
	}
	

protected:
	LightingTechnique *m_pLightingTechnique;
	ShadowMapTechnique* m_pShadowMapEffect;
	//BasicLightingTechnique* m_pLightingTechnique;
	
	DirectionalLight m_dirLight;
	SpotLight m_spotLight;

	const Camera* m_pCamera;
	Mesh* m_pMesh;
	PersProjInfo m_persProjInfo;
	float m_scale = 0.02;
	Vector3f m_pos;
	Vector3f m_rot;
};

class FKRModel :public DynamicModel
{
public:
	FKRModel(const Camera* pCamera, const PersProjInfo& p, const DirectionalLight& l, const SpotLight& s):
		DynamicModel(pCamera, p, l, s)
	{		
		m_strategy = new FKRStrategy();
		m_enemy = NULL;
		m_enemy0 = NULL;
		m_target = NULL;
	}
	~FKRModel() {
		SAFE_DELETE(m_strategy);
	}

	bool Init(const string& modlePathName, const string& texturePathName) {
		if (!DynamicModel::Init(modlePathName, texturePathName))
			return false;
		m_strategy->init();
		return true;
	}
	void Render();
	void setTarget(DynamicModel* target) {
		m_target = target;
	}
	void setEnemy(DynamicModel* enemy) {
		if (!m_enemy)
			m_enemy = enemy;
		else
			m_enemy0 = enemy;
	}
	void setInitPos(Vector3f init) {
		m_strategy->setPos(init);
	}
	
private:
	FKRStrategy* m_strategy;
	DynamicModel* m_enemy;
	DynamicModel* m_enemy0;
	DynamicModel* m_target;

	void updateInfo() {
		if (m_enemy0)
			m_strategy->setEnemy(m_enemy->getPos(), m_enemy0->getPos());
		else if(m_enemy)
		{
			m_strategy->setEnemy(m_enemy->getPos());
		}

		if (m_target)
			m_strategy->setTarget(m_target->getPos());
	}
};

class AntiFKRModel :public DynamicModel
{
public:
	AntiFKRModel(const Camera* pCamera, const PersProjInfo& p, const DirectionalLight& l, const SpotLight& s):
		DynamicModel(pCamera, p, l,s)
	{		
		m_strategy = new AntiFKRStrategy();
	}
	~AntiFKRModel() {
		SAFE_DELETE(m_strategy);
	}
	bool Init(const string& modlePathName, const string& texturePathName) {
		if (!DynamicModel::Init(modlePathName, texturePathName))
			return false;
		m_strategy->init();
		return true;
	}
	void Render();
	void setTarget(DynamicModel* target) {
		m_target = target;
	}
	void setProtect(DynamicModel* protect) {
		m_protect = protect;
	}
	void setDisturb(DynamicModel* disturb) {
		m_disturb = disturb;
		m_strategy->disturbed = true;
	}

	void setInitPos(Vector3f init) {
		m_strategy->setPos(init);
	}
	void likeHome() {
		m_strategy->likeHome();
	}
private:
	AntiFKRStrategy* m_strategy;
	DynamicModel* m_protect;
	DynamicModel* m_target;
	DynamicModel* m_disturb = NULL;

	void updateInfo() {
		if (m_protect)
			m_strategy->setProtect(m_protect->getPos());
		if (m_target)
			m_strategy->setTarget(m_target->getPos());
		if (m_disturb)
			m_strategy->setDisturb(m_disturb->getPos());
	}
};

class HumanModel :public DynamicModel
{
public:
	HumanModel(const Camera* pCamera, const PersProjInfo& p, const DirectionalLight& l, const SpotLight& s) :
		DynamicModel(pCamera, p, l,s)
	{
		m_strategy = new HumanStrategy();
	}
	~HumanModel() {
		SAFE_DELETE(m_strategy);
	}
	bool Init(const string& modlePathName, const string& texturePathName) {
		if (!DynamicModel::Init(modlePathName, texturePathName))
			return false;
		m_strategy->init();
		return true;
	}
	
	void Render();
	void setEnemy(DynamicModel* enemy) {		
		m_enemy = enemy;		
	}
	void setInitPos(Vector3f init) {
		m_strategy->setPos(init);
	}
	

private:
	HumanStrategy* m_strategy;
	DynamicModel* m_enemy;//FKR

	void updateInfo() {
		if (m_enemy)
			m_strategy->setEnemy(m_enemy->getPos());
	}
	
};