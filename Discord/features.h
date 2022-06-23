#pragma once
#include <string>

namespace features
{

	bool boxes = false;
	bool chest = true;
	bool ammo = true;
	bool lines = false;
	bool distance = false;
	bool aimbot = false;
	bool crosshair = false;
	bool fovcirclee = false;
	bool randomaim = false;
	bool ads = false;
	bool infinitejump = false;
	bool menu = false;
	bool pfly = false;
	bool fovchanger = false;
	bool bigplayers = false;
	bool allan = false;
	bool aimpred = false;
	bool debug = false;

	static float smoothness = 1;
	static float fov = 100;
	float crossthick = 1.0f;
	static float randomspeed = 1000;
	float boxwidthlol = 0.5f;
	float boxthick = 2.0f;
	float fovvalue = 90.0f;

	bool instarevive = false;
	bool noweapswapdelay = false;

	static int max_options = 1;
	static int aimkey = 1;
	std::string aimkeystr;
	static int hitbox = 0;
	static int aimkeypos = 1;
	static int lineesppos = 2;
	std::string lineespposstr;
	static int hitboxpos = 0;
	std::string hitboxposstr;

	float fov_valueX;
	float fov_valueY;
	float fov_valueZ;
}