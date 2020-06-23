# version 400

layout (location = 0) in vec3 vPos;
layout (location = 1) in vec3 vNorm;
layout (location = 2) in vec2 vTexCoord;

uniform mat4 vTransf;
uniform mat4 vProj;
uniform vec3 lPos;
uniform vec4 la;
uniform vec4 ld;
uniform vec4 ls;
uniform vec4 ka;
uniform vec4 kd;
uniform vec4 ks;
uniform float sh;

out vec4 Color;
out vec2 TexCoord;

vec4 shade ( vec4 p )  {
  vec3 n = normalize ( vNorm*mat3(vTransf) );
  vec3 l = normalize (lPos-p.xyz);
  vec3 r = reflect(-l, n);
  vec3 v = vec3(0, 0, 1.0);

  vec4 amb = la*ka;
  vec4 dif = ld*kd*max(dot(l,n),0.0);
  vec4 spe = ls*ks*pow(max(r.z,0.0),sh);

  if ( dot(l,n)<0 ) spe=vec4(0.0,0.0,0.0,1.0);
    return amb + dif + spe;
}

void main(){
  vec4 p = vec4(vPos,1.0) * vTransf;
  Color = shade ( p );
  TexCoord = vTexCoord;
  gl_Position = p * vProj;
}