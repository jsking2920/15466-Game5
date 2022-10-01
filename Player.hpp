#pragma once

#include "WalkMesh.hpp"
#include "Scene.hpp"

class Player {

private:


public:
	WalkPoint at;
	// Transform of the player mesh in the scene; will be yawed by mouse left/right motion:
	Scene::Transform* transform = nullptr;
	// Transform of bullet spawning location
	Scene::Transform* fire_point = nullptr;
	// Camera in scene, at head of player; will be pitched by mouse up/down motion:
	Scene::Camera* camera = nullptr;
};