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

	void Render(int mode);

private:
	BasicLightingTechnique* m_pLightingTechnique;
	const Camera* m_pCamera;
	DirectionalLight m_dirLight;
	Strategy* m_strategy;
	Mesh* m_pMesh;
	PersProjInfo m_persProjInfo;
};