#include <iostream>
#include <vector>
#include <math.h>


// ������ɫ�����ؿ�
#include "shader.h"


GLuint VBO1;
GLuint gScaleLocation; // λ���м����

const char* pVSFileName = "shader.vertex";
const char* pFSFileName = "shader.frag";

static void RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// ά��һ��������������ľ�̬������
	static float Scale = 0.0f;
	Scale += 0.01f;
	// ��ֵ���ݸ�shader
	glUniform1f(gScaleLocation, Scale);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		6 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		6 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);

	// ��Ȼ���ǻ���һ��������
	glDrawArrays(GL_TRIANGLES, 0, 3);

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
	GLfloat Vertices[] = {
		-0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.0f, 0.5f, 0.0f, 0.0f, 0.0f, 1.0f };

	glGenBuffers(1, &VBO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}


int main(int argc, char** argv)
{

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(1024, 768);
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


	//׼����ɫ������
	Shader shader(pVSFileName, pFSFileName);
	shader.use();
	shader.changePos(gScaleLocation);

	glutMainLoop();

	return 0;
}
