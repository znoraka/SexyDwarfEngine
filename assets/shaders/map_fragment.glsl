varying vec4 col;
varying vec3 N;
varying vec3 V;
uniform sampler2D tex;
varying vec4 pos;
varying float v_textureWidth;
varying float v_textureHeight;

#define MAX_LIGHTS 3 

void main (void) 
{ 
   vec4 finalColor = vec4(0.0, 0.0, 0.0, 0.0);
   
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
   
   gl_FragColor = (gl_FrontLightModelProduct.sceneColor + finalColor) * texture2D(tex, vec2(pos.x / v_textureWidth, pos.y / v_textureHeight));
}
