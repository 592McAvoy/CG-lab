#pragma once

#include "ogldev_camera.h"
#include "mesh.h"
//#include "ogldev_basic_lighting.h"
#include "lighting_technique.h"

class StaticModel
{
public:
	StaticModel(const Camera* pCamera, const PersProjInfo& p, const DirectionalLight& l, const SpotLight& s);

	~StaticModel();

	bool Init(const string& modlePathName, const string& texturePathName);
	void ShadowRender();
	void Render();

	void setInfo(float s, Vector3f &pos, Vector3f &rot) {
		scale = s;
		positon = pos;
		rotation = rot;
	}

	void changeAmbLight(float ff) {
		m_dirLight.AmbientIntensity += ff;
	}
	void changeDiffLight(float ff) {
		m_dirLight.DiffuseIntensity += ff;
	}

private:
	LightingTechnique* m_pLightingTechnique;
	ShadowMapTechnique* m_pShadowMapEffect;
	
	const Camera* m_pCamera;
	DirectionalLight m_dirLight;
	SpotLight m_spotLight;
	Mesh* m_pMesh;
	PersProjInfo m_persProjInfo;

	float scale = 0.01f;
	Vector3f positon = Vector3f(0.0f, -8.0f, 0.0f);
	Vector3f rotation = Vector3f(20.0f, 180.0f, 0.0f);
};
