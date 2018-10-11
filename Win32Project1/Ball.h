#pragma once
#include "ogldev_camera.h"
#include "ogldev_cubemap_texture.h"
#include "mesh.h"
#include "ogldev_pipeline.h"
#include "technique.h"
#include "ogldev_util.h"

class Ball
{
public:
	Ball(const Camera* pCamera, const PersProjInfo& p) {
		m_pCamera = pCamera;
		m_persProjInfo = p;
	}


	bool Init(Vector3f& p) {
		m_pos = p;
		return true;
	}

	void Render() {
		Pipeline p;
		p.Scale(20.0f, 20.0f, 20.0f);
		p.Rotate(0.0f, 0.0f, 0.0f);
		p.WorldPos(m_pCamera->GetPos().x, m_pCamera->GetPos().y, m_pCamera->GetPos().z);
		p.SetCamera(m_pCamera->GetPos(), m_pCamera->GetTarget(), m_pCamera->GetUp());
		p.SetPerspectiveProj(m_persProjInfo);

		glColor3f(0.9f, 0.7f, 0.7f);  /*设置显示对象的颜色*/
		glPushMatrix();
		glTranslatef(m_pos.x,m_pos.y,m_pos.z);
		//printf("pos %f,%f,%f\n", tt.x, tt.y, tt.z);
		glutWireSphere(20, 18, 18);  /*显示线框球体*/
		glPopMatrix();
	}

private:
	const Camera* m_pCamera;
	Vector3f m_pos;
	PersProjInfo m_persProjInfo;
};
