#include "../../../../../framework.h"
#include "../../../../../environment.h"
#include "Cursor.h"

using namespace nsBattle;

void Cursor::Start() {
	transform->rotation.z = 6.0f;
	speed = 0.1f;
}

void Cursor::Update() {
	/// ƒJ[ƒ\ƒ‹‚ÌˆÚ“®
	if (Input::GetDX(InputConfig::input["moveX"]) != 0) {
		transform->position.x += Input::GetDX(InputConfig::input["moveX"]) * speed;
	}
	if (Input::GetDY(InputConfig::input["moveY"]) != 0) {
		transform->position.y -= Input::GetDY(InputConfig::input["moveY"]) * speed;
	}

	if (transform->position.x < SpriteRenderer::WorldWHPos[0]) transform->position.x = SpriteRenderer::WorldWHPos[0];
	if (transform->position.x > SpriteRenderer::WorldWHPos[1]) transform->position.x = SpriteRenderer::WorldWHPos[1];
	if (transform->position.y > SpriteRenderer::WorldWHPos[2]) transform->position.y = SpriteRenderer::WorldWHPos[2];
	if (transform->position.y < SpriteRenderer::WorldWHPos[3]) transform->position.y = SpriteRenderer::WorldWHPos[3];
}

