#include <stdio.h>
#include <GL/glew.h>        // GLEW��չ��
#include <GL/glut.h>  // freeGLUTͼ�ο�
#include "math3d.h" // ����OpenGL��3d��ѧ��,������Ҫ�õ��˶���������ݽṹ������ԭ���ߵ�Դ������ҵ����ͷ�ļ���

GLuint VBO; //���㻺��������

/**
* ��Ⱦ�ص�����
*/


static void RenderScenceCB() {
	// �����ɫ����
	glClear(GL_COLOR_BUFFER_BIT);

	// ������������
	glEnableVertexAttribArray(0);
	// ��GL_ARRAY_BUFFER������
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// ���߹�����������bufer�е�����
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// ��ʼ���Ƽ���ͼ��(����һ����)
	//glDrawArrays(GL_POINTS, 0, 1);

	// ��ʼ���Ƽ���ͼ��(����һ�������Σ���������)
	glDrawArrays(GL_TRIANGLES, 0, 3);

	//  ���ö�������
	glDisableVertexAttribArray(0);

	// ����ǰ�󻺴�
	glutSwapBuffers();
}

/**
* �������㻺����
*/
static void CreateVertexBuffer()
{
	// ��������һ������Ķ�������
	//M3DVector3f Vertices[1];
	// ����������Ļ����
	//m3dLoadVector3(Vertices[0], 0.0f, 0.0f, 0.0f);

	// ��������3������Ķ�������
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};


	// ����������
	glGenBuffers(1, &VBO);
	// ��GL_ARRAY_BUFFER������
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// �󶨶�������
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

/**
* ������
*/
/*int main(int argc, char ** argv) {

	// ��ʼ��GLUT
	glutInit(&argc, argv);

	// ��ʾģʽ��˫���塢RGBA
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	// ��������
	glutInitWindowSize(480, 320);      // ���ڳߴ�
	glutInitWindowPosition(100, 100);  // ����λ��
	glutCreateWindow("Tutorial 02");   // ���ڱ���
										   
	glRotatef(45.0, 1.0, 0.0, 0.0);
	glutDisplayFunc(RenderScenceCB);	// ��ʼ��Ⱦ

	// ���GLEW�Ƿ����������Ҫ��GLUT��ʼ��֮��
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	// ������պ����ɫֵ
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// �������㻺����
	CreateVertexBuffer();


	// ֪ͨ��ʼGLUT���ڲ�ѭ��
	glutMainLoop();

	return 0;
}*/

