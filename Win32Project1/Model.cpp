#include "Model.h"

#include "ogldev_pipeline.h"
#include "ogldev_util.h"

Model::Model(const Camera* pCamera,
	const PersProjInfo& p,
	const DirectionalLight& l)
{
	m_pCamera = pCamera;
	m_persProjInfo = p;
	m_dirLight = l;

	m_pLightingTechnique = NULL;
	m_pMesh = NULL;
	m_scale = 0.0f;
	m_move = NULL;
}


Model::~Model()
{
	SAFE_DELETE(m_pLightingTechnique);
	SAFE_DELETE(m_move);
	SAFE_DELETE(m_pMesh);
}


bool Model::Init(const string& modelPathName)
{
	m_move = new Move(m_pCamera->GetTarget(), Vector3f(-10.0f, 90.0f, 0.0f));

	m_pLightingTechnique = new BasicLightingTechnique();

	if (!m_pLightingTechnique->Init()) {
		printf("Error initializing the lighting technique\n");
		return false;
	}

	m_pLightingTechnique->Enable();
	m_pLightingTechnique->SetDirectionalLight(m_dirLight);
	m_pLightingTechnique->SetColorTextureUnit(0);
	
	m_pMesh = new Mesh();
	if (!m_pMesh->LoadMesh(modelPathName)) {
		return false;
	}
	
}


void Model::Render(int mode)
{
	m_scale += 0.01f;

	m_pLightingTechnique->Enable();

	m_move->setMode(mode);
	

	Pipeline p;
	/*p.Scale(0.1f, 0.1f, 0.1f);
	p.WorldPos(m_move->getPos(m_scale).x, m_move->getPos(m_scale).y, m_move->getPos(m_scale).z);
	p.Rotate(m_move->getRotation(m_scale).x, m_move->getRotation(m_scale).y, m_move->getRotation(m_scale).z);*/
	p.Scale(0.1f, 0.1f, 0.1f);
	p.WorldPos(0.0f,-7.0f, 0.0f);

	p.SetCamera(m_pCamera->GetPos(), m_pCamera->GetTarget(), m_pCamera->GetUp());
	p.SetPerspectiveProj(m_persProjInfo);

	m_pLightingTechnique->SetWVP(p.GetWVPTrans());
	m_pLightingTechnique->SetWorldMatrix(p.GetWorldTrans());
	m_pMesh->Render();

	
}