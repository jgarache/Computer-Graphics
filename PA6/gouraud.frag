# version 400

in vec4 Color;
in vec2 TexCoord;
out vec4 fColor;

uniform sampler2D Tex1;
uniform float shadInfluence;
void main() 
 { 
   fColor = shadInfluence*Color + texture2D(Tex1, TexCoord);
 } 
