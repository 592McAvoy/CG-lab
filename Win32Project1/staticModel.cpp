#include "staticModel.h"

#include "ogldev_pipeline.h"
#include "ogldev_util.h"

StaticModel::StaticModel(const Camera* pCamera,
	const PersProjInfo& p,
	const DirectionalLight& l)
{
	m_pCamera = pCamera;
	m_persProjInfo = p;
	m_dirLight = l;

	m_pLightingTechnique = NULL;
	m_pMesh = NULL;
}


StaticModel::~StaticModel()
{
	SAFE_DELETE(m_pLightingTechnique);
	SAFE_DELETE(m_pMesh);
}


bool StaticModel::Init(const string& modelPathName, const string& texturePathName)
{

	m_pLightingTechnique = new BasicLightingTechnique();

	if (!m_pLightingTechnique->Init()) {
		printf("Error initializing the lighting technique\n");
		return false;
	}

	m_pLightingTechnique->Enable();
	m_pLightingTechnique->SetDirectionalLight(m_dirLight);
	m_pLightingTechnique->SetColorTextureUnit(0);
	m_pLightingTechnique->SetMatSpecularIntensity(0.5f);
	m_pLightingTechnique->SetMatSpecularPower(4);

	m_pMesh = new Mesh();
	if (!m_pMesh->LoadMesh(modelPathName, texturePathName)) {
		return false;
	}

}


void StaticModel::Render()
{
	m_pLightingTechnique->Enable();

	Pipeline p;
	p.Scale(scale, scale, scale);
	p.WorldPos(positon.x, positon.y, positon.z);
	p.Rotate(rotation.x, rotation.y, rotation.z);
	
	p.SetCamera(m_pCamera->GetPos(), m_pCamera->GetTarget(), m_pCamera->GetUp());
	p.SetPerspectiveProj(m_persProjInfo);

	m_pLightingTechnique->SetWVP(p.GetWVPTrans());
	m_pLightingTechnique->SetWorldMatrix(p.GetWorldTrans());
	m_pLightingTechnique->SetEyeWorldPos(m_pCamera->GetPos());

	m_pMesh->Render();


}