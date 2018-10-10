 #version 330

//in vec3 vertColor;
//in vec4 Color;
//out vec4 FragColor;
//out vec4 color;

in vec2 TexCoord0;

out vec4 FragColor;

uniform sampler2D gSampler;

void main()
{
    //color = vec4(vertColor, 1.0);
	
    //FragColor = Color; // 颜色值作为片段着色器fs的输出

	FragColor = texture2D(gSampler, TexCoord0.xy);
}