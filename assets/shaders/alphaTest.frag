uniform sampler2D texture;
uniform float threshold;                                                  
void main()
{
        vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
        if(pixel.a < threshold) 
            discard;
        else
            gl_FragColor = gl_Color * pixel;
}