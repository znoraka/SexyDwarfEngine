varying vec4 col;
varying vec3 N;
varying vec3 V;
varying vec4 pos;
varying vec4 v_colAttr;
attribute vec3 position;
attribute vec3 color;
attribute vec3 normal;
uniform vec4 colAttr;

void main(void)
{
  gl_Position = gl_ModelViewProjectionMatrix * vec4(position, 1);
  V = vec3(gl_ModelViewMatrix * vec4(position, 1));       
  N = normalize(gl_NormalMatrix * normal);
  col = vec4(color, 1);
  v_colAttr = colAttr;
  pos = vec4(position, 1);
}

