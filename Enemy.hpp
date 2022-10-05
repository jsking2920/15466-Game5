#ifndef INC_15466_GAME5_ENEMY_HPP
#define INC_15466_GAME5_ENEMY_HPP

#include "Scene.hpp"
#include "LitColorTextureProgram.hpp"
#include "Mesh.hpp"

#define ENEMY_HURTBOX_RADIUS 1.f
#define PLAYER_HURTBOX_RADIUS 1.f
#define ENEMY_SPEED 10.0f
#define MIN_SPAWN_TIMER 0.5f
#define ENEMY_SPAWN_DISTANCE 10.f

struct EnemyManager;

struct Enemy {
    Scene::Transform *transform;
    Scene::Transform *target;
    EnemyManager *manager;
    uint32_t id;


    Enemy(Scene::Transform *_transform, Scene::Transform *_target, uint32_t id, EnemyManager *_manager);

    bool update(float elapsed); //returns true iff colliding w player
    bool check_collision_with_object(Scene::Transform *collision, float radius);
};

struct EnemyManager {
    std::vector<Scene::Transform *> spawnpoints;
    Scene::Transform *player;
    Scene &scene;
    const Mesh *mesh;
    GLuint vao;

    std::unordered_map<uint32_t, std::shared_ptr<Enemy>> enemies; //ID to Enemy
    uint32_t current_id = 0;
    float spawn_time = 5.f;
    float time_elapsed = 0;
    float spawn_timer = 0;

    EnemyManager(Scene::Transform *_player, std::vector<Scene::Transform *> &_spawnpoints, Scene &_scene, const Mesh *_mesh, GLuint _vao);

    bool update(float elapsed); //returns true iff an enemy is colliding w player
    //most likely we should be calling for each bullet and each enemy check collision with object
    //and if correct then delete the enemy
    void delete_enemy(uint32_t id, bool edit_list);
    void spawn_enemy();
    void reset();
};

#endif //INC_15466_GAME5_ENEMY_HPP
