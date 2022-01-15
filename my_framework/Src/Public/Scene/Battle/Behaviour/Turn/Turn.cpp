#include "../../../../../../framework.h"
#include "../../../../../../environment.h"
#include "Turn.h"

using namespace nsBattle;

eFieldState Turn::curState = eFieldState::Field;


void Turn::MoveCamera() {
	//ƒJƒƒ‰ˆÚ“®
	float _moveX = Input::GetDX(InputConfig::input["moveCamX"]) * 0.2f;
	float _moveY = -Input::GetDY(InputConfig::input["moveCamY"]) * 0.2f;
	if (_moveX != 0 || _moveY != 0) {
		//§ŒÀ
		float _padding = 5.0f;
		float _left = FieldManager::StageLeft - _padding;
		float _right = FieldManager::FieldManager::StageLeft + (FieldManager::StageSize * (FieldManager::SquareNum_X - 1)) + _padding;
		float _top = FieldManager::StageTop + _padding;
		float _bottom = FieldManager::StageTop - (FieldManager::StageSize * (FieldManager::SquareNum_Y - 1)) - _padding;
		if (_moveX > 0) {
			if (SpriteRenderer::WorldWHPos[1] < _right) Camera::main->transform->position.x += _moveX;
		}
		else if (_moveX < 0) {
			if (SpriteRenderer::WorldWHPos[0] > _left) Camera::main->transform->position.x += _moveX;
		}

		if (_moveY > 0) {
			if (SpriteRenderer::WorldWHPos[2] < _top) Camera::main->transform->position.y += _moveY;
		}
		else if (_moveY < 0) {
			if (SpriteRenderer::WorldWHPos[3] > _bottom) Camera::main->transform->position.y += _moveY;
		}
	}
}