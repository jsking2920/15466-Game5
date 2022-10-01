#include "Enemy.hpp"
#include "glm/gtx/quaternion.hpp"
#include "data_path.hpp"

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
    transform->position = elapsed * ENEMY_SPEED * vec_to_player;

    //rotate to face the player
    glm::quat rotation = glm::rotation(glm::rotate(transform->rotation, glm::vec3(0, 1, 0)), vec_to_player);
    transform->rotation = rotation * transform->rotation;
    //check for bullets?
    //if(bullet) die();
    return check_collision_with_object(target, PLAYER_HURTBOX_RADIUS);
}

//checks sphere collision with transform collision with radius radius
bool Enemy::check_collision_with_object(Scene::Transform *collision, float radius) {
    glm::vec3 vec_to_player = collision->position - transform->position;
    return glm::length(vec_to_player) < radius + ENEMY_HURTBOX_RADIUS;
}

//enemy manager
EnemyManager::EnemyManager(Scene::Transform *_player, std::vector<Scene::Transform *> &_spawnpoints, Scene &_scene, Scene::Drawable &_drawable) :
    spawnpoints(_spawnpoints), player(_player), scene(_scene) {

}

bool EnemyManager::update(float elapsed) {
    time_elapsed += elapsed;
    spawn_timer = MIN_SPAWN_TIMER + (50 / time_elapsed + 11);

    //checking if hitting player
    bool hit = false;
    for(std::pair<uint32_t, std::shared_ptr<Enemy>> pair : enemies) {
        hit = hit && pair.second->update(elapsed);
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
    enemies.clear();
    time_elapsed = 0;
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

}


