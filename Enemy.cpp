#include "Enemy.hpp"

#include <utility>
#include "glm/gtx/quaternion.hpp"
#include "data_path.hpp"
#include <iostream>

GLuint _meshes_for_lit_color_texture_program = 0;
Load< MeshBuffer > _main_meshes(LoadTagDefault, []() -> MeshBuffer const * {
    MeshBuffer const *ret = new MeshBuffer(data_path("main.pnct"));
    _meshes_for_lit_color_texture_program = ret->make_vao_for_program(lit_color_texture_program->program);
    return ret;
});

Enemy::Enemy(Scene::Transform *_transform, Scene::Transform *_target, uint32_t _id, EnemyManager *_manager) :
transform(_transform), target(_target),  manager(_manager), id(_id)
{
}

bool Enemy::update(float elapsed) {
    glm::vec3 vec_to_player = glm::normalize(target->position - transform->position);
    transform->position += elapsed * ENEMY_SPEED * vec_to_player;

    //rotate to face the player
    glm::quat rotation = glm::rotation(glm::rotate(transform->rotation, glm::vec3(0, 1, 0)), vec_to_player);
    transform->rotation = rotation * transform->rotation;
    //check for bullets?
    //if(bullet) die();
    bool collision = check_collision_with_object(target, PLAYER_HURTBOX_RADIUS);
    std::cout << std::to_string(collision) << "\n";
    return collision;
}

//checks sphere collision with transform collision with radius radius
bool Enemy::check_collision_with_object(Scene::Transform *collision, float radius) {
    glm::vec3 vec_to_player = collision->position - transform->position;
//    std::cout << "distance: " << std::to_string(glm::length(vec_to_player)) << ", total: " << std::to_string(radius + ENEMY_HURTBOX_RADIUS) << "\n";
    bool res = glm::length(vec_to_player) < radius + ENEMY_HURTBOX_RADIUS;
//    std::cout << std::to_string(res) << "\n";
    return res;
}

//enemy manager
EnemyManager::EnemyManager(Scene::Transform *_player, std::vector<Scene::Transform *> &_spawnpoints, Scene &_scene) :
    spawnpoints(std::move(_spawnpoints)), player(_player), scene(_scene) {
    spawn_enemy();
}

bool EnemyManager::update(float elapsed) {
    time_elapsed += elapsed;
    spawn_timer += elapsed;
    spawn_time = MIN_SPAWN_TIMER + (50 / (time_elapsed + 11));

    if(spawn_timer > spawn_time) {
        std::cout << std::to_string(spawn_time) << ", " << std::to_string(spawn_timer) << ", " << std::to_string(time_elapsed) << "\n";
        spawn_timer = 0;
        spawn_enemy();
    }

    //checking if hitting player
    bool hit = false;
    for(std::pair<const unsigned int, std::shared_ptr<Enemy>> &pair : enemies) {
        bool was_hit = pair.second->update(elapsed);
        std::cout << std::to_string(was_hit) <<"\n";
        hit = hit && was_hit;
    }
    return hit;
}

void EnemyManager::delete_enemy(uint32_t id) {
    std::shared_ptr<Enemy> enemy = enemies[id];
    scene.drawables.erase(std::find_if(scene.drawables.begin(), scene.drawables.end(), [&](const Scene::Drawable &d) {
        return d.transform->name == enemy->transform->name;
    }));
    scene.transforms.erase(std::find_if(scene.transforms.begin(), scene.transforms.end(), [&](const Scene::Transform &x) {
        return x.name == enemy->transform->name;
    }));
    enemies.erase(id); //should delete the enemy too cuz it's a shared pointer
}

void EnemyManager::reset() {
    std::for_each(enemies.begin(), enemies.end(), [&](std::pair<const unsigned int, std::shared_ptr<Enemy>> p) {
        delete_enemy(p.first);
    });
    time_elapsed = 0;
    spawn_timer = 0;
}

void EnemyManager::spawn_enemy() {
    //I really hope this all runs atomically, multiple enemies shouldn't spawn in on the same frame
    //so it should be fine
    //get spawn point
    size_t spawn_index = rand() % spawnpoints.size();
    Scene::Transform *spawn_point = spawnpoints.at(spawn_index);
    //make transform
    scene.transforms.emplace_back();
    scene.transforms.back().name = "Enemy" + std::to_string(current_id);
    scene.transforms.back().position = spawn_point->position;
    scene.transforms.back().rotation = spawn_point->rotation;
    scene.transforms.back().scale = spawn_point->scale;
//    scene.transforms.back().parent = spawn_point;

    Mesh const &mesh = _main_meshes->lookup("Enemy");

    scene.drawables.emplace_back(&scene.transforms.back());
    Scene::Drawable &drawable = scene.drawables.back();

    drawable.pipeline = lit_color_texture_program_pipeline;

    drawable.pipeline.vao = _meshes_for_lit_color_texture_program;
    drawable.pipeline.type = mesh.type;
    drawable.pipeline.start = mesh.start;
    drawable.pipeline.count = mesh.count;
//    Mesh const &mesh = main_meshes->lookup(mesh_name);

    std::shared_ptr<Enemy> new_enemy = std::make_shared<Enemy>(&scene.transforms.back(),player, current_id, this);
    enemies.insert(std::pair<uint32_t, std::shared_ptr<Enemy>>(current_id, new_enemy));
    current_id++;
}


