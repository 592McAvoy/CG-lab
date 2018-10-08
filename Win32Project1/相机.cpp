#include <iostream>
#include <vector>
#include <math.h>

#include <GL/glew.h>
#include <GL/glut.h>  // GLUT图形库
#include <GL/freeglut.h>
// 包含着色器加载库
#include "shader.h"
#include "ogldev_util.h"
#include "ogldev_glut_backend.h"
#include "ogldev_pipeline.h"


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


//顶点缓冲对象
GLuint VBO1;

// 索引缓冲对象的句柄
GLuint IBO;

// 平移变换一致变量的句柄引用
GLuint gWorldLocation;

// 透视变换配置参数数据结构
PersProjInfo gPersProjInfo;

// 相机对象
Camera* pGameCamera = NULL;

const char* pVSFileName = "shader.vertex";
const char* pFSFileName = "shader.frag";


static void RenderSceneCB()
{
	glClear(GL_COLOR_BUFFER_BIT);

	// 维护一个不断慢慢增大的静态浮点数
	static float Scale = 0.0f;
	Scale += 0.01f;


	static Pipeline p;
	p.Rotate(0.0f, Scale, 0.0f);
	p.WorldPos(0.0f, 0.0f, 6.0f);

	// 相机变换
	Vector3f CameraPos(3.0f, 0.0f, 0.0f);
	Vector3f CameraTarget(0.0f, -1.0f, 2.0f);
	Vector3f CameraUp(-1.0f, 1.0f, 2.0f);
	
	//创建相机
	
	p.SetCamera(*pGameCamera);


	// 设置投影变换的参数
	p.SetPerspectiveProj(gPersProjInfo);

	glUniformMatrix4fv(gWorldLocation, 1, GL_TRUE, (const GLfloat*)p.GetWVPTrans());


	// 如何绘制数据-位置
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
		6 * sizeof(GL_FLOAT), (GLvoid*)0);
	glEnableVertexAttribArray(0);
	// 如何绘制数据-颜色
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,
		6 * sizeof(GL_FLOAT), (GLvoid*)(3 * sizeof(GL_FLOAT)));
	glEnableVertexAttribArray(1);


	// 每次在绘制之前绑定索引缓冲
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	// 索引绘制图形
	glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, 0);

	//glDrawArrays(GL_TRIANGLES, 0, 3);

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glutSwapBuffers();
}

// 传递键盘事件
static void SpecialKeyboardCB(int Key, int x, int y)
{
	OGLDEV_KEY OgldevKey = GLUTKeyToOGLDEVKey(Key);
	pGameCamera->OnKeyboard(OgldevKey);
}

//鼠标事件
static void OnMouse(int x, int y)
{
	pGameCamera->OnMouse(x, y);
}

static void InitializeGlutCallbacks()
{
	glutDisplayFunc(RenderSceneCB);

	// 将渲染回调注册为全局闲置回调
	glutIdleFunc(RenderSceneCB);

	// 注册方向键事件
	glutSpecialFunc(SpecialKeyboardCB);

	// 注册鼠标事件
	glutMotionFunc(OnMouse);
}

static void CreateVertexBuffer()
{
	// 金字塔的四个顶点
	GLfloat Vertices[] = {
		-1.0f, -1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
		1.0f, 1.0f, 0.0f ,1.0f, 0.0f, 0.0f };

	glGenBuffers(1, &VBO1);
	glBindBuffer(GL_ARRAY_BUFFER, VBO1);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);
}

// 创建索引缓冲器
static void CreateIndexBuffer()
{
	// 四个三角形面的顶点索引集
	unsigned int Indices[] = {
		0, 3, 1,
		1, 3, 2,
		2, 3, 0,
		0, 1, 2 };

	// 创建缓冲区
	glGenBuffers(1, &IBO);
	// 绑定缓冲区
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	// 添加缓冲区数据
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

	// 初始化透视变换配置参数
	gPersProjInfo.FOV = 45.0f;
	gPersProjInfo.Height = WINDOW_HEIGHT;
	gPersProjInfo.Width = WINDOW_WIDTH;
	gPersProjInfo.zNear = 1.0f;
	gPersProjInfo.zFar = 100.0f;

	// 必须在glut初始化后！
	GLenum res = glewInit();
	if (res != GLEW_OK) {
		fprintf(stderr, "Error: '%s'\n", glewGetErrorString(res));
		return 1;
	}

	printf("GL version: %s\n", glGetString(GL_VERSION));

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	CreateVertexBuffer();
	CreateIndexBuffer();


	//准备着色器程序
	Shader shader(pVSFileName, pFSFileName);
	shader.use();
	shader.getPos(gWorldLocation, "gWorld");

	glutMainLoop();

	return 0;
}
