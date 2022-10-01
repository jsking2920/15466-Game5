#ifndef INC_15466_GAME5_ENEMY_HPP
#define INC_15466_GAME5_ENEMY_HPP

#include "Scene.hpp"

#define ENEMY_HURTBOX_RADIUS 2.f
#define PLAYER_HURTBOX_RADIUS 2.f
#define ENEMY_SPEED 1.0f

struct EnemyManager;

struct Enemy {
    Scene::Transform *transform;
    Scene::Transform *target;
    EnemyManager *manager;
    uint32_t id;


    Enemy(Scene::Transform *_transform, Scene::Transform *_target, uint32_t id, EnemyManager *_manager);

    bool update(float elapsed); //returns true iff colliding w player
    bool check_collision_with_object(Scene::Transform *collision, uint32_t radius);
};

struct EnemyManager {
    std::unordered_map<uint32_t, std::shared_ptr<Enemy>> enemies; //ID to Enemy
    uint32_t current_id = 0;
    Scene::Transform *player;

    EnemyManager(Scene::Transform *_player);

    bool update(float elapsed); //returns true iff an enemy is colliding w player
    //most likely we should be calling for each bullet and each enemy check collision with object
    //and if correct then delete the enemy
    void delete_enemy(uint32_t id);
    void spawn_enemy();
};

#endif //INC_15466_GAME5_ENEMY_HPP
