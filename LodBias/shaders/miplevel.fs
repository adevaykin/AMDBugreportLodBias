#version 430 core
out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;

void main()
{
	float mipmapLevel = textureQueryLod(texture1, TexCoord).x;
	vec4 mipColor = vec4(0.0);
	if (mipmapLevel <= 1.0) {
		mipColor = vec4(1., 0., 0., 1.);
	} else if (mipmapLevel <= 2.0) {
		mipColor = vec4(0., 1., 0., 1.);
	} else if (mipmapLevel <= 3.0) {
		mipColor = vec4(0., 0., 1., 1.);
	} else if (mipmapLevel <= 4.0) {
		mipColor = vec4(0., 1., 1., 1.);
	} else if (mipmapLevel <= 5.0) {
		mipColor = vec4(1., 0., 1., 1.);
	} else if (mipmapLevel <= 6.0) {
		mipColor = vec4(1., 1., 0., 1.);
	} else {
		mipColor = vec4(1., 1., 1., 1.);
	}

	FragColor = texture(texture1, TexCoord)*0.2 + mipColor*0.8;
}