uniform sampler2D texture;
uniform bool enable;

void main()
{
	vec4 texColor = texture2D(texture, gl_TexCoord[0]);

	if (enable)
		texColor.rgb = 1.0 - texColor.rgb;
	
	gl_FragColor = texColor * gl_Color;
}

/* Black and white
void main()
{
	vec4 texColor = texture2D(texture, gl_TexCoord[0]);
	float gray = (texColor.r + texColor.g + texColor.b) / 3.0;
	
	gl_FragColor = vec4(gray, gray, gray, texColor.a) * gl_Color;
}
*/