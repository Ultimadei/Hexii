const float PI        = 3.1415926;
const float TWO_PI    = 6.2831852;

uniform float progress;
// Centre should be in the screen coordinate space
uniform vec2 centre;
uniform sampler2D progressBar;

float angleBetween(vec2 v1, vec2 v2)
{
    return atan( v1.y - v2.y, -(v1.x - v2.x)) + PI;
}

float getTargetAngle() 
{
    return clamp(progress * TWO_PI, 0.0, TWO_PI);
}

// OpenGL uses upper left as origin by default
bool shouldDrawFragment(vec2 fragCoord)
{
    float targetAngle = getTargetAngle();

    float angle = angleBetween(centre, fragCoord);

    return angle <= targetAngle;
}

void main()
{
    vec4 col = gl_Color;

    if (shouldDrawFragment(gl_FragCoord.xy)) {
        vec4 pixel = texture2D(progressBar, gl_TexCoord[0].xy);
        if (pixel.a > 0.0) {
            col = pixel;
        }
    }

    gl_FragColor = col;
}