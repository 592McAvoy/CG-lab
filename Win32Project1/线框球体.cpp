#include <stdio.h>
#include <GL/glew.h>        // GLEW��չ��
#include <GL/glut.h>  // GLUTͼ�ο�

void draw()   /*��ʾ�ص��������趨��ʾ����*/
{
	// �����ɫ����
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.9f, 0.7f, 0.7f);  /*������ʾ�������ɫ*/
	glutWireSphere(0.8, 18, 18);  /*��ʾ�߿�����*/

	glutSwapBuffers();
}

/*int main(int argc, char ** argv)
{
	// ��ʼ��GLUT
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowPosition(100, 100);	//�ߴ�
	glutInitWindowSize(400, 400);		//λ��
	glutCreateWindow("opengl");			//����

	
	glRotatef(45.0, 1.0, 0.0, 0.0);
	glutDisplayFunc(draw);
	/*
	glutReshapeFunc(reshape);
	glutMouseFunc(movelight);
	glutMotionFunc(motion);
	glutKeyFunc(keyboard);
	*/

	
/*
	// ���GLEW�Ƿ����������Ҫ��GLUT��ʼ��֮��
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	// ������պ����ɫֵ
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// ֪ͨ��ʼGLUT���ڲ�ѭ��
	glutMainLoop();

	return 0;
}*/
