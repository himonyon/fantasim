#include "../../../../../framework.h"
#include "../../../../../environment.h"
#include "Cursor.h"

using namespace nsStrategy;

void Cursor::Start() {
	transform->rotation.z = 6.0f;
	gameObject->SetObjEnable(false);
}

void Cursor::Update() {
	/// ƒJ[ƒ\ƒ‹‚ÌˆÚ“®
	if (Input::IsMouseMove()) {
		if (Mouse::GetX() < SCREEN_WIDTH && Mouse::GetX() > 0) transform->position.x = (float)Mouse::GetX();
		if (Mouse::GetY() < SCREEN_HEIGHT && Mouse::GetY() > 0) transform->position.y = (float)Mouse::GetY();
	}
	else {
		if (Input::GetDX(InputConfig::input["moveX"]) != 0) {
			transform->position.x += Input::GetDX(InputConfig::input["moveX"]) * speed;
		}
		if (Input::GetDY(InputConfig::input["moveY"]) != 0) {
			transform->position.y += Input::GetDY(InputConfig::input["moveY"]) * speed;
		}
	}
}

