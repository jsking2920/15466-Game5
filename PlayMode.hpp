#include "Mode.hpp"

#include "Scene.hpp"
#include "Player.hpp"
#include "TextRenderer.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "data_path.hpp"
#include "LitColorTextureProgram.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

GLuint meshes_for_lit_color_texture_program = 0;
Load< MeshBuffer > main_meshes(LoadTagDefault, []() -> MeshBuffer const * {
    MeshBuffer const *ret = new MeshBuffer(data_path("main.pnct"));
    meshes_for_lit_color_texture_program = ret->make_vao_for_program(lit_color_texture_program->program);
    return ret;
});;

struct PlayMode : Mode {
	PlayMode();
	virtual ~PlayMode();

	// Functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;

	//----- game state -----

	// Input tracking
	struct Button {
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up, lmb, r;

	// Local copy of the game scene (so code can change it during gameplay)
	Scene scene;

	// Player info
	Player player = Player();

	// Text Rendering
	TextRenderer* hud_text = nullptr;
	uint8_t hud_font_size = 48;
};
