varying vec4 col;
varying vec3 N;
varying vec3 V;
varying vec4 pos;
uniform float textureWidth;
uniform float textureHeight;
varying float v_textureWidth;
varying float v_textureHeight;
attribute vec3 position;
attribute vec3 color;
attribute vec3 normal;

void main(void)
{
  gl_Position = gl_ModelViewProjectionMatrix * vec4(position, 1);
  V = vec3(gl_ModelViewMatrix * vec4(position, 1));       
  N = normalize(gl_NormalMatrix * normal);
  col = vec4(color, 1);
  pos = vec4(position, 1);
  v_textureWidth = textureWidth;
  v_textureHeight = textureHeight;
}

