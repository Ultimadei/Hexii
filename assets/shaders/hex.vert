void main()
{
    // Transform the vertex position
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;

    // Transform the texture coordinates
    gl_TexCoord[0] = gl_TextureMatrix[0] * gl_MultiTexCoord0;

    // Forward the vertex color
    gl_FrontColor = gl_Color;
}