#include <stdio.h>
#include <GL/glew.h>
#include <GL/glut.h>


void display(){
	// 清空颜色缓存
	glClear(GL_COLOR_BUFFER_BIT);
	// 交换前后缓存
	glutSwapBuffers();
} 

/*int main(int argc, char* argv[]){
	glutInit(&argc, argv);

	// GLUT_RGBA:颜色缓冲  
	// GLUT_DOUBLE:双缓冲机制
	
	glutInitDisplayMode(GLUT_RGBA|GLUT_DOUBLE);

	glutInitWindowPosition(100, 100);	//尺寸
	glutInitWindowSize(800, 480);		//位置
	glutCreateWindow("opengl");			//标题

	glutDisplayFunc(display);

	// 颜色值有四个通道（RGBA）,使用单位化的值0.0-1.0来表示 
	glClearColor(0.6f, 0.7f, 0.3f, 0.0f);

	glutMainLoop();
			
	return 0;
}*/
