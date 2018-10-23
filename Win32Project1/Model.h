#pragma once
#include "ogldev_camera.h"
#include "mesh.h"
#include "ogldev_basic_lighting.h"
#include "Move.h"

class Model
{
public:
	Model(const Camera* pCamera, const PersProjInfo& p, const DirectionalLight& l);

	~Model();

	bool Init(const string& modlePathName);

	void Render(int mode);

private:
	BasicLightingTechnique* m_pLightingTechnique;
	const Camera* m_pCamera;
	DirectionalLight m_dirLight;
	float m_scale;
	Move* m_move;
	Mesh* m_pMesh;
	PersProjInfo m_persProjInfo;
};