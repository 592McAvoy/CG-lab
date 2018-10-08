#include <iostream>
#include <vector>
#include <math.h>

#include <GL/glew.h>
#include <GL/glut.h>  // GLUTͼ�ο�
#include <GL/freeglut.h>
// ������ɫ�����ؿ�
#include "shader.h"
#include "ogldev_util.h"
#include "ogldev_glut_backend.h"
#include "ogldev_pipeline.h"


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


//���㻺�����
GLuint VBO1;

// �����������ľ��
GLuint IBO;

// ƽ�Ʊ任һ�±����ľ������
GLuint gWorldLocation;

// ͸�ӱ任���ò������ݽṹ
PersProjInfo gPersProjInfo;

// �������
Camera* pGameCamera = NULL;

const char* pVSFileName = "shader.vertex";
const char* pFSFileName = "shader.frag";


static void RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// ά��һ��������������ľ�̬������
	static float Scale = 0.0f;
	Scale += 0.01f;


	static Pipeline p;
	p.Rotate(0.0f, Scale, 0.0f);
	p.WorldPos(0.0f, 0.0f, 6.0f);

	// ����任
	Vector3f CameraPos(3.0f, 0.0f, 0.0f);
	Vector3f CameraTarget(0.0f, -1.0f, 2.0f);
	Vector3f CameraUp(-1.0f, 1.0f, 2.0f);
	
	//�������
	
	p.SetCamera(*pGameCamera);


	// ����ͶӰ�任�Ĳ���
	p.SetPerspectiveProj(gPersProjInfo);

	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)p.GetWVPTrans());


	// ��λ�������-λ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		6 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// ��λ�������-��ɫ
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		6 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);


	// ÿ���ڻ���֮ǰ����������
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	// ��������ͼ��
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

	//glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glutSwapBuffers();
}

// ���ݼ����¼�
static void SpecialKeyboardCB(int Key, int x, int y)
{
	OGLDEV_KEY OgldevKey = GLUTKeyToOGLDEVKey(Key);
	pGameCamera->OnKeyboard(OgldevKey);
}

//����¼�
static void OnMouse(int x, int y)
{
	pGameCamera->OnMouse(x, y);
}

static void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCB);

	// ����Ⱦ�ص�ע��Ϊȫ�����ûص�
	glutIdleFunc(RenderSceneCB);

	// ע�᷽����¼�
	glutSpecialFunc(SpecialKeyboardCB);

	// ע������¼�
	glutMotionFunc(OnMouse);
}

static void CreateVertexBuffer()
{
	// ���������ĸ�����
	GLfloat Vertices[] = {
		-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f ,1.0f, 0.0f, 0.0f };

	glGenBuffers(1, &VBO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

// ��������������
static void CreateIndexBuffer()
{
	// �ĸ���������Ķ���������
	unsigned int Indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2 };

	// ����������
	glGenBuffers(1, &IBO);
	// �󶨻�����
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	// ��ӻ���������
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(Indices), Indices, GL_STATIC_DRAW);
}


int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Tutorial 04");

	InitializeGlutCallbacks();

	pGameCamera = new Camera(WINDOW_WIDTH, WINDOW_HEIGHT);

	// ��ʼ��͸�ӱ任���ò���
	gPersProjInfo.FOV = 45.0f;
	gPersProjInfo.Height = WINDOW_HEIGHT;
	gPersProjInfo.Width = WINDOW_WIDTH;
	gPersProjInfo.zNear = 1.0f;
	gPersProjInfo.zFar = 100.0f;

	// ������glut��ʼ����
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	printf("GL version: %s\n", glGetString(GL_VERSION));

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	CreateVertexBuffer();
	CreateIndexBuffer();


	//׼����ɫ������
	Shader shader(pVSFileName, pFSFileName);
	shader.use();
	shader.getPos(gWorldLocation, "gWorld");

	glutMainLoop();

	return 0;
}
