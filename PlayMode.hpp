#pragma once
#include "Mode.hpp"

#include "Scene.hpp"
#include "Player.hpp"
#include "TextRenderer.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "data_path.hpp"
#include "LitColorTextureProgram.hpp"
#include "Enemy.hpp"
#include "Sound.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	// Functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;
    void reset(float time_survived);

	//----- game state -----

    std::shared_ptr<EnemyManager> enemy_manager;

    float game_timer = 0.0f;
    float best_time = 0.0f;

	// Input tracking
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up, lmb, r;

	// Local copy of the game scene (so code can change it during gameplay)
	Scene scene;

	// Player info
	Player player;

	// Text Rendering
	TextRenderer* hud_text = nullptr;
	uint8_t hud_font_size = 34;
};
