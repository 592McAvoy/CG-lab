#include <iostream>
#include <vector>
#include <math.h>


// ������ɫ�����ؿ�
#include "shader.h"


//���㻺�����
GLuint VBO1;

// �����������ľ��
GLuint IBO;

// ƽ�Ʊ任һ�±����ľ������
GLuint gWorldLocation;


const char* pVSFileName = "shader.vertex";
const char* pFSFileName = "shader.frag";

//��ά����
struct Matrix4f {
	float m[4][4];
};

static void RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// ά��һ��������������ľ�̬������
	static float Scale = 0.0f;
	Scale += 0.001f;
	// 4x4��ƽ�Ʊ任����
	Matrix4f World;

	World.m[0][0] = cosf(Scale); World.m[0][1] = 0.0f; World.m[0][2] = -sinf(Scale); World.m[0][3] = 0.0f;
	World.m[1][0] = 0.0;         World.m[1][1] = 1.0f; World.m[1][2] = 0.0f; World.m[1][3] = 0.0f;
	World.m[2][0] = sinf(Scale); World.m[2][1] = 0.0f; World.m[2][2] = cosf(Scale); World.m[2][3] = 0.0f;
	World.m[3][0] = 0.0f;        World.m[3][1] = 0.0f; World.m[3][2] = 0.0f; World.m[3][3] = 1.0f;



	// ���������ݼ��ص�shader��
	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, &World.m[0][0]);


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


static void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCB);

	// ����Ⱦ�ص�ע��Ϊȫ�����ûص�
	glutIdleFunc(RenderSceneCB);
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
	glutInitWindowSize(500, 400);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Tutorial 04");

	InitializeGlutCallbacks();

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
