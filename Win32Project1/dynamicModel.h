#pragma once
#include "ogldev_camera.h"
#include "mesh.h"
#include "ogldev_basic_lighting.h"
#include "strategy.h"

class DynamicModel
{
public:
	DynamicModel(const Camera* pCamera, const PersProjInfo& p, const DirectionalLight& l);

	~DynamicModel();

	bool Init(const string& modlePathName, const string& texturePathName);

	void Render();

	Vector3f getPos() {
		return m_pos;
	}

	void setScale(float ss) {
		m_scale = ss;
	}
	

protected:
	BasicLightingTechnique* m_pLightingTechnique;
	const Camera* m_pCamera;
	DirectionalLight m_dirLight;
	Mesh* m_pMesh;
	PersProjInfo m_persProjInfo;
	float m_scale = 0.02;
	Vector3f m_pos;
};

class FKRModel :public DynamicModel
{
public:
	FKRModel(const Camera* pCamera, const PersProjInfo& p, const DirectionalLight& l):
		DynamicModel(pCamera, p, l)
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
	AntiFKRModel(const Camera* pCamera, const PersProjInfo& p, const DirectionalLight& l):
		DynamicModel(pCamera, p, l)
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
	HumanModel(const Camera* pCamera, const PersProjInfo& p, const DirectionalLight& l) :
		DynamicModel(pCamera, p, l)
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