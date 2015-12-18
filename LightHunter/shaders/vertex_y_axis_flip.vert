void main()
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
	gl_Position.y *= -1.0;
	gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;
    gl_FrontColor = gl_Color;
}