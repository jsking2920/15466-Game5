#include "Enemy.hpp"

Enemy::Enemy(Scene::Transform *_transform, Scene::Transform *_target, uint32_t _id, EnemyManager *_manager) :
transform(_transform), target(_target), id(_id), manager(_manager)
{
}

bool Enemy::update(float elapsed) {
    glm::vec3 vec_to_player = glm::normalize(target->position - transform->position);
    transform->position = elapsed * ENEMY_SPEED * vec_to_player;
    //check for bullets?
    //if(bullet) die();
    return check_collision_with_object(target, PLAYER_HURTBOX_RADIUS);
}

//checks sphere collision with transform collision with radius radius
bool Enemy::check_collision_with_object(Scene::Transform *collision, uint32_t radius) {
    glm::vec3 vec_to_player = collision->position - transform->position;
    return glm::length(vec_to_player) < radius + ENEMY_HURTBOX_RADIUS;
}

//enemy manager
EnemyManager::EnemyManager(Scene::Transform *_player) : player(_player) {

}

bool EnemyManager::update(float elapsed) {
    bool hit = false;
    for(std::pair<uint16_t, std::shared_ptr<Enemy>> pair : enemies) {
        hit = hit && pair.second->update(elapsed);
    }
    return hit;
}

void EnemyManager::delete_enemy(uint32_t id) {
    enemies.erase(id); //should delete the enemy too cuz it's a shared pointer
}


