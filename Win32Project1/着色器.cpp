#include <iostream>
#include <vector>


// 包含着色器加载库
#include "shader.h"
#include <GL/glut.h>  // GLUT图形库

/*GLuint VBO1;
const char* pVSFileName = "shader.vertex";
const char* pFSFileName = "shader.frag";

static void RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		6 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		6 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);

	// 依然还是绘制一个三角形
	glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);

	glutSwapBuffers();
}


static void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCB);
}

static void CreateVertexBuffer()
{
	GLfloat Vertices[] = {
	-0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.5f, 0.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f};

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

	// 必须在glut初始化后！
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	printf("GL version: %s\n", glGetString(GL_VERSION));

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	CreateVertexBuffer();
	
	
	//准备着色器程序
	Shader shader(pVSFileName,pFSFileName);
	shader.use();

	glutMainLoop();
	
	return 0;
}*/
