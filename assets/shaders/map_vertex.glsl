varying vec4 col;
varying vec3 N;
varying vec3 V;
varying vec4 pos;
uniform float textureWidth;
uniform float textureHeight;
varying float v_textureWidth;
varying float v_textureHeight;

void main(void)
{
    gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
    V = vec3(gl_ModelViewMatrix * gl_Vertex);       
    N = normalize(gl_NormalMatrix * gl_Normal);
    col = gl_Color;
    pos = gl_Vertex;
    v_textureWidth = textureWidth;
    v_textureHeight = textureHeight;
}

