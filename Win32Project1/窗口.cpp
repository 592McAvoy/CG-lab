#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h>


void display(){
	// �����ɫ����
	glClear(GL_COLOR_BUFFER_BIT);
	// ����ǰ�󻺴�
	glutSwapBuffers();
} 

/*int main(int argc, char* argv[]){
	glutInit(&argc, argv);

	// GLUT_RGBA:��ɫ����  
	// GLUT_DOUBLE:˫�������
	
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);

	glutInitWindowPosition(100, 100);	//�ߴ�
	glutInitWindowSize(800, 480);		//λ��
	glutCreateWindow("opengl");			//����

	glutDisplayFunc(display);

	// ��ɫֵ���ĸ�ͨ����RGBA��,ʹ�õ�λ����ֵ0.0-1.0����ʾ 
	glClearColor(0.6f, 0.7f, 0.3f, 0.0f);

	glutMainLoop();
			
	return 0;
}*/
