uniform float eyeseparation;
uniform sampler2D tex;
uniform int mode;

//y axis direction is down
void main()
{
	
	if(mode == 0)
	{
	//left
		if(gl_TexCoord[0].t < 0.5)
		{
				vec4 color = texture2D(tex, vec2(gl_TexCoord[0].s  * (1 - eyeseparation), gl_TexCoord[0].t));
				gl_FragColor = vec4(color);
		}else
		{
		//right
				vec4 color = texture2D(tex, vec2(gl_TexCoord[0].s * (1 - eyeseparation) + eyeseparation,  gl_TexCoord[0].t));
				gl_FragColor = vec4(color);
		}
	}
	else
	{
		//float up = (1 - eyeseparation);
	    //float down = (1 - eyeseparation);

	//right
		if(gl_TexCoord[0].t > 0.5)
		{
				float tp = (gl_TexCoord[0].s * (1 - eyeseparation) + eyeseparation)  * 0.5  + 0.5;
				vec4 color = texture2D(tex, vec2(tp, (gl_TexCoord[0].t - 0.5) *2 ));	
				gl_FragColor = vec4(color);
			
		}else
		{
	//left
				vec4 color = texture2D(tex, vec2( (gl_TexCoord[0].s  * (1 - eyeseparation) * 0.5 ),  gl_TexCoord[0].t * 2));
				gl_FragColor = vec4(color);
		}

	}
    //gl_FragColor = vec4(color * eyeseparation);
}
