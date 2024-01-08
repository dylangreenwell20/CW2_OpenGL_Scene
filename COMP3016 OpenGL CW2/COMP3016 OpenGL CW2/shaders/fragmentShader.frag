#version 460
//Colour value to send to next stage
out vec4 FragColor;

in vec2 texCoordFrag;
uniform sampler2D textureIn;

void main()
{
    //RGBA values
    FragColor = texture(textureIn,texCoordFrag);
}