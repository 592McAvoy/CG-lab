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

	Vector3f getPrevPos() {
		return m_prevpos;
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
	Vector3f m_prevpos = Vector3f(0.0, 0.0, 0.0);
	Vector3f m_rot;
};

class FKRModel :public DynamicModel
{
public:
	FKRModel(const Camera* pCamera, const PersProjInfo& p, const DirectionalLight& l, const SpotLight& s):
		DynamicModel(pCamera, p, l, s)
	{		
		m_strategy = new FKRStrategy();
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
		m_enemy = enemy;
	}
	
private:
	FKRStrategy* m_strategy;
	DynamicModel* m_enemy;
	DynamicModel* m_target;

	void updateInfo() {
		if (m_enemy)
			m_strategy->setEnemy(m_enemy->getPos());
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
	

private:
	HumanStrategy* m_strategy;
	
};