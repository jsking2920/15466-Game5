#pragma once

#include "WalkMesh.hpp"
#include "Scene.hpp"
#include "Enemy.hpp"

#include <glm/glm.hpp>

struct Bullet {

	Bullet(Scene::Transform* _transform, float _lifetime, glm::vec3 _velocity);
	Bullet() = default;
	~Bullet();

	bool Update(float elapsed, std::unordered_map<uint32_t, std::shared_ptr<Enemy>> enemies); // returns true iff bullet should be destroyed (past lifetime or hit enemy)
	bool CheckForCollision(std::unordered_map<uint32_t, std::shared_ptr<Enemy>> enemies, uint32_t* out_id); // returns true if bullet is colliding with enemy

	Scene::Transform* transform = nullptr;
	float lifetime = 2.0f;
	glm::vec3 velocity = glm::vec3(0.0f);
	float bullet_radius = 0.1f;

	uint32_t out; // used to return id of enemy hit in collision checking
};

struct Gun {
	
	Gun(Scene& _scene, const Mesh* _bullet_mesh, GLuint _bullet_vao, Scene::Transform* player_transform, Scene::Transform* _gun_transform, Scene::Transform* _fire_point, int16_t _max_ammo, float _muzzle_velocity, float _range, float _fire_rate_delay, float _reload_time);
	~Gun();

	void Update(float elapsed, bool shoot_button_held); // call every frame with elapsed seconds
	bool Shoot(glm::vec3 dir); // returns if bullet was actually shot
	bool Reload(); // returns false if gun fails to reload (only happens if it was already fully loaded or is already being reloaded)


	Scene& scene; // TODO: figure out compiler errors with this
	// Transform of gun mesh
	Scene::Transform* transform = nullptr;
	// Transform of bullet spawning location
	Scene::Transform* fire_point = nullptr;
	int16_t cur_ammo = 24;
	int16_t max_ammo = 24;
	float fire_rate_delay = 0.25f; // seconds between shots
	float reload_time = 2.5f;
	float muzzle_velocity = 20.0f; // units / sec
	float range = 50.0f; // Bullet lifetime = range / muzzle_velocity 

	const Mesh* bullet_mesh;
	GLuint bullet_vao;
	std::vector<Bullet> bullets = std::vector<Bullet>();

	enum GunState {
		shooting, reloading, idle
	};

	GunState cur_state = idle;

private:
	Bullet SpawnBullet(float _lifetime, glm::vec3 _velocity);

	float internal_timer = fire_rate_delay; // used for fire rate; gun shoots on 0
	float reload_timer = reload_time; // used for reloading
};

class Player {

public:
	WalkPoint at;
	// Transform of the player mesh in the scene; will be yawed by mouse left/right motion:
	Scene::Transform* transform = nullptr;
	// Camera in scene, at head of player; will be pitched by mouse up/down motion:
	Scene::Camera* camera = nullptr;

	std::shared_ptr<Gun> cur_gun;

private:

};

