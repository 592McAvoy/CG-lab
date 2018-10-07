 #version 330

//in vec3 vertColor;
in vec4 Color;
out vec4 FragColor;

void main()
{
    //color = vec4(vertColor, 1.0);
	
    FragColor = Color; // 颜色值作为片段着色器fs的输出
}