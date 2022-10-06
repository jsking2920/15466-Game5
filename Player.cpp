#include "Player.hpp"

#include <iostream>

/******** Bullets ***********/

Bullet::~Bullet() {
}

Bullet::Bullet(Scene::Transform* _transform, float _lifetime, glm::vec3 _velocity) {

	transform = _transform;
	lifetime = _lifetime;
	velocity = _velocity;
}

bool Bullet::CheckForCollision(std::shared_ptr<EnemyManager> enemy_manager, uint32_t* out_id) {
	
	for (auto e : enemy_manager->enemies) {
		if (e.second->check_collision_with_object(transform, bullet_radius)) {
			*out_id = e.first;
			return true;
		}
	}
	return false;
}

// TODO: Figure out how to avoid passing enemy manager around
// will note: maybe it would be better to just pass in a transform and radius, and just do the collision in the Gun/EnemyManager
bool Bullet::Update(float elapsed, std::shared_ptr<EnemyManager> enemy_manager) {
	
	lifetime -= elapsed;

	if (lifetime <= 0) {
		return true;
	}
	else {
		transform->position += velocity * elapsed;

		if (CheckForCollision(enemy_manager, &out)) {
			enemy_manager->delete_enemy(out, true);
			return true;
		}
	}

	return false;
}

/******** Guns ***********/

Gun::~Gun() {
}

Gun::Gun(Scene& _scene, const Mesh* _bullet_mesh, GLuint _bullet_vao, Scene::Transform* player_transform, Scene::Transform* _gun_transform, Scene::Transform* _fire_point, int16_t _max_ammo, float _muzzle_velocity, float _range, float _fire_rate_delay, float _reload_time) :
scene(_scene)
{

	bullet_mesh = _bullet_mesh;
	bullet_vao = _bullet_vao;

	// Heiarchy: Player -> Gun -> Fire Point
	//                  -> Camera
	// yaw comes from player, pitch applied seperately to both cam and gun
	fire_point = _fire_point;
	transform = _gun_transform;
	fire_point->parent = transform;
	transform->parent = player_transform;

	reload_time = _reload_time;
	max_ammo = _max_ammo;
	cur_ammo = _max_ammo;
	fire_rate_delay = _fire_rate_delay;
	internal_timer = _fire_rate_delay;
	muzzle_velocity = _muzzle_velocity;
	range = _range;
}

// TODO: Figure out how to avoid passing enemy manager around
void Gun::Update(float elapsed, bool shoot_button_held, std::shared_ptr<EnemyManager> enemy_manager) {

	if (cur_state == reloading) {
		reload_timer -= elapsed;

		if (reload_timer <= 0.0f) {
			cur_state = idle;
			cur_ammo = max_ammo;
		}
	}
	else if (shoot_button_held) {
		internal_timer = internal_timer - elapsed;
	}
	else {
		internal_timer = 0.0f; // means that you can tap fire at infinite fire rate
	}


    std::list<std::string> deletion_list;
	for (auto b = bullets.begin(); b != bullets.end(); b++) {
		if (b->Update(elapsed, enemy_manager)) {
			// TODO: implement scoring
			// TODO: play enemy hit sound

			// Delete bullet from scene lists and gun list
			scene.drawables.erase(std::find_if(scene.drawables.begin(), scene.drawables.end(), [&](const Scene::Drawable& d) {
				return d.transform->name == b->transform->name;
			}));
			scene.transforms.erase(std::find_if(scene.transforms.begin(), scene.transforms.end(), [&](const Scene::Transform& t) {
				return t.name == b->transform->name;
			}));
//            test.push_back(b);
            deletion_list.push_back(b->transform->name);
		}
	}
    std::for_each(deletion_list.begin(), deletion_list.end(), [&](std::string s) {
        bullets.erase(std::find_if(bullets.begin(), bullets.end(), [&](Bullet b) {
            return b.transform->name == s;
        }));
    });
    deletion_list.clear();
}

Bullet Gun::SpawnBullet(float _lifetime, glm::vec3 _velocity) {
    bullet_count++;

	//make transform
	scene.transforms.emplace_back();
	scene.transforms.back().name = "Bullet" + std::to_string(bullet_count);
	scene.transforms.back().position = fire_point->make_local_to_world() * glm::vec4(fire_point->position, 1);
	scene.transforms.back().rotation = transform->parent->rotation * transform->rotation * fire_point->rotation ;
	scene.transforms.back().scale = fire_point->scale;

	scene.drawables.emplace_back(&scene.transforms.back());
	Scene::Drawable& drawable = scene.drawables.back();

	drawable.pipeline = lit_color_texture_program_pipeline;

	drawable.pipeline.vao = bullet_vao;
	drawable.pipeline.type = bullet_mesh->type;
	drawable.pipeline.start = bullet_mesh->start;
	drawable.pipeline.count = bullet_mesh->count;

    return Bullet(&scene.transforms.back(), _lifetime, _velocity);
}

bool Gun::Shoot(glm::vec3 dir) {

	if (internal_timer <= 0.0f && cur_ammo > 0 && cur_state != reloading) {
		internal_timer = fire_rate_delay;
		cur_state = shooting;

		// Actually shoot bullet
        glm::vec3 bullet_vel = muzzle_velocity * glm::normalize(dir);
		bullets.push_back(SpawnBullet(range / muzzle_velocity, bullet_vel));

		cur_ammo = glm::clamp(--cur_ammo, int16_t(0), max_ammo);

		return true;
	}

	return false;
}

bool Gun::Reload() {
	
	if (cur_ammo == max_ammo || cur_state == reloading) {
		return false;
	}

	reload_timer = reload_time;
	cur_state = reloading;
	return true;
}

void Gun::Reset() {
	cur_ammo = max_ammo;
	internal_timer = fire_rate_delay;
	cur_state = idle;
}