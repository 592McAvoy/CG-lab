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
	
}


DynamicModel::~DynamicModel()
{
	SAFE_DELETE(m_pLightingTechnique);
	SAFE_DELETE(m_pMesh);

}


bool DynamicModel::Init(const string& modelPathName, const string& texturePathName)
{
	
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

	return true;
	
}


void DynamicModel::Render()
{
}

void FKRModel::Render()
{
	m_pLightingTechnique->Enable();
	updateInfo();//更新与本模型有交互的模型的位置信息
	m_strategy->update();
	m_pos = m_strategy->getPos();

	Pipeline p;
	p.Scale(m_scale, m_scale, m_scale);
	p.WorldPos(m_pos.x, m_pos.y, m_pos.z);
	p.Rotate(m_strategy->getRot().x, m_strategy->getRot().y, m_strategy->getRot().z);

	p.SetCamera(m_pCamera->GetPos(), m_pCamera->GetTarget(), m_pCamera->GetUp());
	p.SetPerspectiveProj(m_persProjInfo);

	m_pLightingTechnique->SetWVP(p.GetWVPTrans());
	m_pLightingTechnique->SetWorldMatrix(p.GetWorldTrans());
	m_pMesh->Render();
}

void AntiFKRModel::Render()
{
	m_pLightingTechnique->Enable();
	updateInfo();//更新与本模型有交互的模型的位置信息
	m_strategy->update();
	m_pos = m_strategy->getPos();

	Pipeline p;
	p.Scale(m_scale, m_scale, m_scale);
	p.WorldPos(m_pos.x, m_pos.y, m_pos.z);
	p.Rotate(m_strategy->getRot().x, m_strategy->getRot().y, m_strategy->getRot().z);

	p.SetCamera(m_pCamera->GetPos(), m_pCamera->GetTarget(), m_pCamera->GetUp());
	p.SetPerspectiveProj(m_persProjInfo);

	m_pLightingTechnique->SetWVP(p.GetWVPTrans());
	m_pLightingTechnique->SetWorldMatrix(p.GetWorldTrans());
	
	m_pMesh->Render();
}

void HumanModel::Render()
{
	m_pLightingTechnique->Enable();
	
	m_strategy->update();
	m_pos = m_strategy->getPos();

	Pipeline p;
	p.Scale(m_scale, m_scale, m_scale);
	p.WorldPos(m_pos.x, m_pos.y, m_pos.z);
	p.Rotate(m_strategy->getRot().x, m_strategy->getRot().y, m_strategy->getRot().z);

	p.SetCamera(m_pCamera->GetPos(), m_pCamera->GetTarget(), m_pCamera->GetUp());
	p.SetPerspectiveProj(m_persProjInfo);

	m_pLightingTechnique->SetWVP(p.GetWVPTrans());
	m_pLightingTechnique->SetWorldMatrix(p.GetWorldTrans());
	m_pMesh->Render();
}