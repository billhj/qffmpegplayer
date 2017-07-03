
uniform float eyeseparation;
uniform sampler2D tex;

void main()
{
	
	if(gl_TexCoord[0].t < 0.5)
	{
		vec4 color = texture2D(tex, vec2(gl_TexCoord[0].s * (1 - eyeseparation), gl_TexCoord[0].t));
		gl_FragColor = vec4(color);
	}else
	{	
		vec4 color = texture2D(tex, vec2(gl_TexCoord[0].s * (1 - eyeseparation) + eyeseparation,  gl_TexCoord[0].t));
		gl_FragColor = vec4(color);
	}
    //gl_FragColor = vec4(color * eyeseparation);
}