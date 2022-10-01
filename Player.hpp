#pragma once

#include "WalkMesh.hpp"
#include "Scene.hpp"

class Player {

private:


public:
	WalkPoint at;
	//transform is at player's feet and will be yawed by mouse left/right motion:
	Scene::Transform* transform = nullptr;
	//camera is at player's head and will be pitched by mouse up/down motion:
	Scene::Camera* camera = nullptr;
};