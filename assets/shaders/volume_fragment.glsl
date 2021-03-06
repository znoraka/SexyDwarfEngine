varying vec4 col;
varying vec3 N;
varying vec3 V;
varying vec4 v_colAttr;
uniform sampler2D tex;
varying vec4 pos;

#define MAX_LIGHTS 1 
// #define MAX_LIGHTS 0
void main (void) 
{ 
  vec4 finalColor = v_colAttr;
   
   for (int i=0;i<MAX_LIGHTS;i++)
   {
      vec3 L = normalize(gl_LightSource[i].position.xyz - V); 
      vec3 E = normalize(-V); // we are in Eye Coordinates, so EyePos is (0,0,0) 
      vec3 R = normalize(-reflect(L,N)); 
   
      //calculate Ambient Term: 
      vec4 Iamb = gl_FrontLightProduct[i].ambient; 

      //calculate Diffuse Term: 
      vec4 Idiff = gl_FrontLightProduct[i].diffuse * max(dot(N,L), 0.0);
      Idiff = clamp(Idiff, 0.0, 1.0); 
   
      // calculate Specular Term:
      vec4 Ispec = gl_FrontLightProduct[i].specular 
             * pow(max(dot(R,E),0.0),0.3*gl_FrontMaterial.shininess);
      Ispec = clamp(Ispec, 0.0, 1.0); 
   
      finalColor += Iamb + Idiff + Ispec;
   }
   vec4 c = ((gl_FrontLightModelProduct.sceneColor + finalColor));
   if(v_colAttr.w == 0) {
     c = c + col;
   }
     gl_FragColor = c;
}
