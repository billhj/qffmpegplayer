uniform float eyeseparation;
uniform sampler2D tex;
uniform int mode;

void main()
{
	float up = (1 - eyeseparation) + eyeseparation;
	float down = (1 - eyeseparation);

	if(mode == 0)
	{
		if(gl_TexCoord[0].t < 0.5)
		{
				vec4 color = texture2D(tex, vec2(gl_TexCoord[0].s  * down, gl_TexCoord[0].t));
				gl_FragColor = vec4(color);
		}else
		{
				vec4 color = texture2D(tex, vec2(gl_TexCoord[0].s * up,  gl_TexCoord[0].t));
				gl_FragColor = vec4(color);
		}
	}
	else
	{
		if(gl_TexCoord[0].t < 0.5)
		{
				vec4 color = texture2D(tex, vec2( (gl_TexCoord[0].s * down * 0.5 + 0.5), gl_TexCoord[0].t * 2));
				gl_FragColor = vec4(color);
		}else
		{
				vec4 color = texture2D(tex, vec2((gl_TexCoord[0].s * up * 0.5) ,  (gl_TexCoord[0].t - 0.5) * 2));
				gl_FragColor = vec4(color);
		}

	}
    //gl_FragColor = vec4(color * eyeseparation);
}
