#pragma once

#include "geometry.h"

struct sceneParameters
{
	Vec3f directLight;

	//camera 
	Vec3f eyepos;
	Vec3f lookatcenter;
	Vec3f topdir;

	//projection
	float znear;
	float zfar;
	float fov;
	float aspect;

	//view

};