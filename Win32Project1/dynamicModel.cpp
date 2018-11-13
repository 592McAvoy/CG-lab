#include "dynamicModel.h"

#include "ogldev_pipeline.h"
#include "ogldev_util.h"


DynamicModel::DynamicModel(const Camera* pCamera,
	const PersProjInfo& p,
	const DirectionalLight& l)
{
	m_pCamera = pCamera;
	m_persProjInfo = p;
	m_dirLight = l;

	m_pLightingTechnique = NULL;
	m_pMesh = NULL;
	m_strategy = NULL;
}


DynamicModel::~DynamicModel()
{
	SAFE_DELETE(m_pLightingTechnique);
	SAFE_DELETE(m_strategy);
	SAFE_DELETE(m_pMesh);
}


bool DynamicModel::Init(const string& modelPathName, const string& texturePathName)
{
	m_strategy = new Strategy();
	m_strategy->init(m_pCamera->GetTarget());

	m_pLightingTechnique = new BasicLightingTechnique();

	if (!m_pLightingTechnique->Init()) {
		printf("Error initializing the lighting technique\n");
		return false;
	}

	m_pLightingTechnique->Enable();
	m_pLightingTechnique->SetDirectionalLight(m_dirLight);
	m_pLightingTechnique->SetColorTextureUnit(0);
	
	m_pMesh = new Mesh();
	if (!m_pMesh->LoadMesh(modelPathName, texturePathName)) {
		return false;
	}
	
}


void DynamicModel::Render(int mode)
{
	m_pLightingTechnique->Enable();

	m_strategy->update();

	Pipeline p;
	p.Scale(0.02f, 0.02f, 0.02f);
	p.WorldPos(m_strategy->getPos().x, m_strategy->getPos().y, m_strategy->getPos().z);
	p.Rotate(m_strategy->getRot().x, m_strategy->getRot().y, m_strategy->getRot().z);

	p.SetCamera(m_pCamera->GetPos(), m_pCamera->GetTarget(), m_pCamera->GetUp());
	p.SetPerspectiveProj(m_persProjInfo);

	m_pLightingTechnique->SetWVP(p.GetWVPTrans());
	m_pLightingTechnique->SetWorldMatrix(p.GetWorldTrans());
	m_pMesh->Render();

	
}