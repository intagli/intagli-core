#ifndef _intagli_intagli_types_h_
#define _intagli_intagli_types_h_

typedef struct vec4
{
	float x;
	float y;
	float z;
	float w;
} vec4;

typedef struct vec3
{
	float x;
	float y;
	float z;
} vec3;

typedef struct vec2
{
	float x;
	float y;
} vec2;

vec4 Vec4(float x, float y, float z, float w);
vec3 Vec3(float x, float y, float z);
vec2 Vec2(float x, float y);

#endif
