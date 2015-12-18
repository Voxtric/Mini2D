uniform sampler2D texture;		//Set by default
uniform sampler2D shadowMap;	//Set once
uniform vec2 windowSize;		//Set once
uniform vec2 lightPosition;		//Set each render
uniform vec4 lightColor;		//Set each render

bool pixelDark(int x, int y)
{
    vec4 pixel = texture2D(shadowMap, vec2(float(x) / windowSize.x, float(y) / windowSize.y));
	return pixel.r == 0.0;
}

bool lineBlocked(int x1, int y1, int x2, int y2)
{
    int dx, dy, inx, iny, e;
	
    dx = x2 - x1;
    dy = y2 - y1;
    inx = dx > 0 ? 1 : -1;
    iny = dy > 0 ? 1 : -1;
 
    dx = int(abs(float(dx)));
    dy = int(abs(float(dy)));
     
    if(dx >= dy)
	{
        dy <<= 1;
        e = dy - dx;
        dx <<= 1;
        while (x1 != x2)
		{
			if (pixelDark(x1, y1))
			{
				return true;
			}
	
            if(e >= 0)
			{
                y1 += iny;
                e-= dx;
            }
            e += dy;
			x1 += inx;
        }
    }
	else
	{
        dx <<= 1;
        e = dx - dy;
        dy <<= 1;
        while (y1 != y2)
		{
			if (pixelDark(x1, y1))
			{
				return true;
			}
			
            if(e >= 0)
			{
                x1 += inx;
                e -= dy;
            }
            e += dx;
			y1 += iny;
        }
    }
	if (pixelDark(x1, y1))
	{
		return true;
	}
	return false;
}

void main()
{
	vec2 pixelPos = gl_TexCoord[0].xy;
	vec4 shadowMapPixel = texture2D(shadowMap, vec2(gl_FragCoord.x / windowSize.x, gl_FragCoord.y / windowSize.y));
    
	vec4 renderPixel = vec4(0.0, 0.0, 0.0, 1.0);
	if (shadowMapPixel.r > 0.0)
	{		
		vec4 lightMapPixel = texture2D(texture, pixelPos);
		if (lightMapPixel.r + lightMapPixel.g + lightMapPixel.b > 0.0)
		{
			if (!lineBlocked(int(lightPosition.x), int(lightPosition.y), int(gl_FragCoord.x), int(gl_FragCoord.y)))
			{
				renderPixel = lightMapPixel * vec4(lightColor.r , lightColor.g, lightColor.b, 0);
				renderPixel.a = lightMapPixel.a;
			}	
		}
	}
	
    gl_FragColor = renderPixel;
}