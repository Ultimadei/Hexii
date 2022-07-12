const float PI        = 3.1415926;
const float TWO_PI    = 6.2831852;

uniform float progress;
// Centre should be in the screen coordinate space
uniform vec2 centre;
uniform sampler2D progressBar;

// Second argument is negated in order to have the radial start on the left
float angleBetween(vec2 v1, vec2 v2)
{
    return atan( v1.y - v2.y, -(v1.x - v2.x)) + PI;
}

// Angle of the radial (where TWO_PI would mean the full thing)
float getTargetAngle() 
{
    return clamp(progress * TWO_PI, 0.0, TWO_PI);
}

bool shouldDrawProgress(vec2 fragCoord)
{
    float targetAngle = getTargetAngle();

    float angle = angleBetween(centre, fragCoord);

    return angle <= targetAngle;
}

void main()
{
    // Start with the existing color
    vec4 col = gl_Color;

    if (shouldDrawProgress(gl_FragCoord.xy)) {
        vec4 pixel = texture2D(progressBar, gl_TexCoord[0].xy);
        // The progress bar texture should act as an overlay
        if (pixel.a > 0.0) {
            col = pixel;
        }
    }

    gl_FragColor = col;
}