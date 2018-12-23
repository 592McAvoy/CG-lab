#include "staticModel.h"

#include "ogldev_pipeline.h"
#include "ogldev_util.h"

StaticModel::StaticModel(const Camera* pCamera,
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


StaticModel::~StaticModel()
{
	SAFE_DELETE(m_pShadowMapEffect);
	SAFE_DELETE(m_pLightingTechnique);
	SAFE_DELETE(m_pMesh);
}


bool StaticModel::Init(const string& modelPathName, const string& texturePathName)
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
	m_pLightingTechnique->SetMatSpecularIntensity(0.5f);
	m_pLightingTechnique->SetMatSpecularPower(4);
	m_pLightingTechnique->SetSpotLights(1, &m_spotLight);
	m_pLightingTechnique->SetDirectionalLight(m_dirLight);
	m_pLightingTechnique->SetShadowMapTextureUnit(1);

	m_pMesh = new Mesh();
	if (!m_pMesh->LoadMesh(modelPathName, texturePathName)) {
		return false;
	}

}

void StaticModel::ShadowRender()
{

	m_pShadowMapEffect->Enable();

	Pipeline p;
	p.Scale(scale, scale, scale);
	p.WorldPos(positon.x, positon.y, positon.z);
	p.Rotate(rotation.x, rotation.y, rotation.z);

	p.SetCamera(m_spotLight.Position, m_spotLight.Direction, Vector3f(0.0f, 1.0f, 0.0f));
	p.SetPerspectiveProj(m_persProjInfo);
	m_pShadowMapEffect->SetWVP(p.GetWVPTrans());
	m_pMesh->Render();

}

void StaticModel::Render()
{
	m_pLightingTechnique->Enable();
	m_pLightingTechnique->SetEyeWorldPos(m_pCamera->GetPos());

	Pipeline p;
	p.Scale(scale, scale, scale);
	p.WorldPos(positon.x, positon.y, positon.z);
	p.Rotate(rotation.x, rotation.y, rotation.z);
	
	p.SetCamera(m_pCamera->GetPos(), m_pCamera->GetTarget(), m_pCamera->GetUp());
	p.SetPerspectiveProj(m_persProjInfo);

	
	m_pLightingTechnique->SetWVP(p.GetWVPTrans());	
	m_pLightingTechnique->SetWorldMatrix(p.GetWorldTrans());
	

	p.SetCamera(m_spotLight.Position, m_spotLight.Direction, Vector3f(0.0f, 1.0f, 0.0f));
	m_pLightingTechnique->SetLightWVP(p.GetWVPTrans());


	m_pMesh->Render();


}