#include "dynamicModel.h"

#include "ogldev_pipeline.h"
#include "ogldev_util.h"


DynamicModel::DynamicModel(const Camera* pCamera,
	const PersProjInfo& p,
	const DirectionalLight& l,
	const SpotLight& s)
{
	m_pCamera = pCamera;
	m_persProjInfo = p;
	m_dirLight = l;
	m_spotLight = s;
	
	m_pShadowMapEffect = NULL;
	m_pLightingTechnique = NULL;
	m_pMesh = NULL;
	
}


DynamicModel::~DynamicModel()
{
	
	SAFE_DELETE(m_pShadowMapEffect);
	SAFE_DELETE(m_pLightingTechnique);
	SAFE_DELETE(m_pMesh);

}


bool DynamicModel::Init(const string& modelPathName, const string& texturePathName)
{
	m_pShadowMapEffect = new ShadowMapTechnique();

	if (!m_pShadowMapEffect->Init()) {
		printf("Error initializing the shadow map technique\n");
		return false;
	}

	m_pLightingTechnique = new LightingTechnique();

	if (!m_pLightingTechnique->Init()) {
		printf("Error initializing the lighting technique\n");
		return false;
	}

	m_pLightingTechnique->Enable();
	m_pLightingTechnique->SetTextureUnit(0);
	m_pLightingTechnique->SetSpotLights(1, &m_spotLight);
	m_pLightingTechnique->SetDirectionalLight(m_dirLight);
	m_pLightingTechnique->SetShadowMapTextureUnit(1);

	m_pMesh = new Mesh();
	if (!m_pMesh->LoadMesh(modelPathName, texturePathName)) {
		return false;
	}

	return true;
	
}

void DynamicModel::ShadowRender()
{
	
	m_pShadowMapEffect->Enable();

	Pipeline p;
	p.Scale(m_scale, m_scale, m_scale);
	p.WorldPos(m_pos.x, m_pos.y, m_pos.z);
	p.Rotate(m_rot.x, m_rot.y, m_rot.z);

	p.SetCamera(m_spotLight.Position, m_spotLight.Direction, Vector3f(0.0f, 1.0f, 0.0f));
	p.SetPerspectiveProj(m_persProjInfo);
	m_pShadowMapEffect->SetWVP(p.GetWVPTrans());
	m_pMesh->Render();

}

void DynamicModel::Render()
{
}

void FKRModel::Render()
{
	m_pLightingTechnique->Enable();
	m_pLightingTechnique->SetEyeWorldPos(m_pCamera->GetPos());

	updateInfo();//更新与本模型有交互的模型的位置信息
	m_strategy->update();
	m_rot = m_strategy->getRot();
	m_pos = m_strategy->getPos();
	

	Pipeline p;
	p.Scale(m_scale, m_scale, m_scale);
	p.WorldPos(m_pos.x, m_pos.y, m_pos.z);
	p.Rotate(m_rot.x, m_rot.y, m_rot.z);

	p.SetCamera(m_pCamera->GetPos(), m_pCamera->GetTarget(), m_pCamera->GetUp());
	p.SetPerspectiveProj(m_persProjInfo); 

	m_pLightingTechnique->SetWVP(p.GetWVPTrans());
	m_pLightingTechnique->SetWorldMatrix(p.GetWorldTrans());

	p.SetCamera(m_spotLight.Position, m_spotLight.Direction, Vector3f(0.0f, 1.0f, 0.0f));
	m_pLightingTechnique->SetLightWVP(p.GetWVPTrans());

	m_pMesh->Render();
}

void AntiFKRModel::Render()
{
	m_pLightingTechnique->Enable();
	m_pLightingTechnique->SetEyeWorldPos(m_pCamera->GetPos());

	updateInfo();//更新与本模型有交互的模型的位置信息
	m_strategy->update();
	m_rot = m_strategy->getRot();
	m_pos = m_strategy->getPos();
	

	Pipeline p;
	p.Scale(m_scale, m_scale, m_scale);
	p.WorldPos(m_pos.x, m_pos.y, m_pos.z);
	p.Rotate(m_rot.x, m_rot.y, m_rot.z);

	p.SetCamera(m_pCamera->GetPos(), m_pCamera->GetTarget(), m_pCamera->GetUp());
	p.SetPerspectiveProj(m_persProjInfo);

	m_pLightingTechnique->SetWVP(p.GetWVPTrans());
	m_pLightingTechnique->SetWorldMatrix(p.GetWorldTrans());

	p.SetCamera(m_spotLight.Position, m_spotLight.Direction, Vector3f(0.0f, 1.0f, 0.0f));
	m_pLightingTechnique->SetLightWVP(p.GetWVPTrans());

	m_pMesh->Render();
}

void HumanModel::Render()
{
	m_pLightingTechnique->Enable();
	m_pLightingTechnique->SetEyeWorldPos(m_pCamera->GetPos());
	
	updateInfo();
	m_strategy->update();
	m_rot = m_strategy->getRot();	
	m_pos = m_strategy->getPos();

	Pipeline p;
	p.Scale(m_scale, m_scale, m_scale);
	p.WorldPos(m_pos.x, m_pos.y, m_pos.z);
	p.Rotate(m_rot.x, m_rot.y, m_rot.z);

	p.SetCamera(m_pCamera->GetPos(), m_pCamera->GetTarget(), m_pCamera->GetUp());
	p.SetPerspectiveProj(m_persProjInfo);

	m_pLightingTechnique->SetWVP(p.GetWVPTrans());
	m_pLightingTechnique->SetWorldMatrix(p.GetWorldTrans());

	p.SetCamera(m_spotLight.Position, m_spotLight.Direction, Vector3f(0.0f, 1.0f, 0.0f));
	m_pLightingTechnique->SetLightWVP(p.GetWVPTrans());

	m_pMesh->Render();
}