#include <stdio.h>
#include <GL/glew.h>        // GLEW扩展库
#include <GL/glut.h>  // GLUT图形库

void draw()   /*显示回调函数，设定显示内容*/
{
	// 清空颜色缓存
	glClear(GL_COLOR_BUFFER_BIT);

	glColor3f(0.9f, 0.7f, 0.7f);  /*设置显示对象的颜色*/
	glutWireSphere(0.8, 18, 18);  /*显示线框球体*/

	glutSwapBuffers();
}

/*int main(int argc, char ** argv)
{
	// 初始化GLUT
	glutInit(&argc, argv);
	
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	glutInitWindowPosition(100, 100);	//尺寸
	glutInitWindowSize(400, 400);		//位置
	glutCreateWindow("opengl");			//标题

	
	glRotatef(45.0, 1.0, 0.0, 0.0);
	glutDisplayFunc(draw);
	/*
	glutReshapeFunc(reshape);
	glutMouseFunc(movelight);
	glutMotionFunc(motion);
	glutKeyFunc(keyboard);
	*/

	
/*
	// 检查GLEW是否就绪，必须要在GLUT初始化之后！
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	// 缓存清空后的颜色值
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// 通知开始GLUT的内部循环
	glutMainLoop();

	return 0;
}*/
