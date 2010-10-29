uniform float focusDistance;
uniform float aperture;
uniform float pointBrightness;
uniform float rgbBrightness;
uniform float maxPointSize;
uniform float sideAA;
attribute vec3 nextVertex;

const float PI = 3.14159265;

void main() {



  //
	// get the homogeneous 2d position
 
	// use the distance from the camera and aperture to determine the radius
	// the +.5 is because point sizes <1 are rendered differently than those >1
	
	vec4 v0 = vec4(gl_Vertex);
	vec3 v1 = gl_TexCoord[0].xyz;
	gl_Position = gl_ModelViewProjectionMatrix * v0;
	vec4 newPosition = gl_ModelViewProjectionMatrix * vec4(v1,1);
 
	vec3 dx = newPosition.xyz - gl_Position.xyz;
	vec3 diffNormalized = normalize(dx);
// 
	float size = abs(gl_Position.z - focusDistance) * aperture + 1.;
	float endSize = min(size,maxPointSize);

   
	if (gl_Normal.x > 0.5){
		//gl_Position.x += 10.0 * (endSize*3.0 + 1.0);
		//gl_Position.x += -diffNormalized.y*(1.0 + endSize*10.0);
		gl_Position.y += 1.0 + (endSize * 3.0) ;
	} 

	
	gl_FrontColor = gl_Color;
	float radius = endSize / 8.;
////	// divide the color alpha by the area
	gl_FrontColor.a /= PI * radius * radius;
	gl_FrontColor.a *= pointBrightness;
	gl_FrontColor.rgb *= rgbBrightness;
}
