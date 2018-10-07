#include <stdio.h>
#include <GL/glew.h>        // GLEW扩展库
#include <GL/glut.h>  // freeGLUT图形库
#include "math3d.h" // 用于OpenGL的3d数学库,这里主要用到了顶点这个数据结构，下载原作者的源码可以找到这个头文件，

GLuint VBO; //顶点缓冲器对象

/**
* 渲染回调函数
*/


static void RenderScenceCB() {
	// 清空颜色缓存
	glClear(GL_COLOR_BUFFER_BIT);

	// 开启顶点属性
	glEnableVertexAttribArray(0);
	// 绑定GL_ARRAY_BUFFER缓冲器
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 告诉管线怎样解析bufer中的数据
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);

	// 开始绘制几何图形(绘制一个点)
	//glDrawArrays(GL_POINTS, 0, 1);

	// 开始绘制几何图形(绘制一个三角形，三个顶点)
	glDrawArrays(GL_TRIANGLES, 0, 3);

	//  禁用顶点数据
	glDisableVertexAttribArray(0);

	// 交换前后缓存
	glutSwapBuffers();
}

/**
* 创建顶点缓冲器
*/
static void CreateVertexBuffer()
{
	// 创建含有一个顶点的顶点数组
	//M3DVector3f Vertices[1];
	// 将点置于屏幕中央
	//m3dLoadVector3(Vertices[0], 0.0f, 0.0f, 0.0f);

	// 创建含有3个顶点的顶点数组
	GLfloat vertices[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		0.0f,  0.5f, 0.0f
	};


	// 创建缓冲器
	glGenBuffers(1, &VBO);
	// 绑定GL_ARRAY_BUFFER缓冲器
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// 绑定顶点数据
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
}

/**
* 主函数
*/
/*int main(int argc, char ** argv) {

	// 初始化GLUT
	glutInit(&argc, argv);

	// 显示模式：双缓冲、RGBA
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);

	// 窗口设置
	glutInitWindowSize(480, 320);      // 窗口尺寸
	glutInitWindowPosition(100, 100);  // 窗口位置
	glutCreateWindow("Tutorial 02");   // 窗口标题
										   
	glRotatef(45.0, 1.0, 0.0, 0.0);
	glutDisplayFunc(RenderScenceCB);	// 开始渲染

	// 检查GLEW是否就绪，必须要在GLUT初始化之后！
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	// 缓存清空后的颜色值
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	// 创建顶点缓冲器
	CreateVertexBuffer();


	// 通知开始GLUT的内部循环
	glutMainLoop();

	return 0;
}*/

