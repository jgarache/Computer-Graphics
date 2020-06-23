# version 400

//in  vec4 Color;
in  vec2 TexCoord;
out vec4 fColor;
uniform sampler2D Tex1;

void main() 
 { 
   fColor = texture2D(Tex1,TexCoord);
 } 
