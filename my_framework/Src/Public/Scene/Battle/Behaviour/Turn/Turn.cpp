#include "../../../../../../framework.h"
#include "../../../../../../environment.h"
#include "Turn.h"

using namespace nsBattle;


void Turn::Start() {
	//�v���C���[�L�����ƓG�L����
	for (auto& chara : vBattleChara) {
		noDel_ptr<EnemyChara> _enemy = dynamic_noDel_cast<EnemyChara>(chara);
		if (_enemy == NULL) continue;
		vEnemy.emplace_back(_enemy);
	}
	for (auto& chara : vBattleChara) {
		noDel_ptr<PlayerChara> _player = dynamic_noDel_cast<PlayerChara>(chara);
		if (_player == NULL) continue;
		vPlayer.emplace_back(_player);
	}
}

void Turn::MoveCamera() {
	//�J�����ړ�
	float _moveX = Input::GetDX(InputConfig::input["moveCamX"]) * 0.2f;
	float _moveY = -Input::GetDY(InputConfig::input["moveCamY"]) * 0.2f;
	if (_moveX != 0 || _moveY != 0) {
		//����
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

void Turn::TurnInit() {
	SetTurnState(eTurnState::Field);
	for (auto& chara : vBattleChara) {
		chara->SetActionEnable(true);
	}
}

void Turn::CheckEndGame() {
	for (auto& chara : vEnemy) {
		
	}
	for (auto& chara : vPlayer) {
	}
}

void Turn::SetTurnState(eTurnState state, int backNum) {
	//�߂��Ԃ�����ꂽ���O�̏�Ԃɖ߂�
	if (state == eTurnState::Back) {
		for (int i = 0; i < backNum; i++) {
			curState = curState->bef.lock();
		}
		return;
	}

	//�ꗗ�ɂȂ��ꍇ�A�V�K�o�^
	if (umStates.find(state) == umStates.end()) {
		std::shared_ptr<stTurnState> _temp = std::make_shared<stTurnState>(stTurnState(state));
		umStates[state] = _temp;
	}
	//���݂̏�Ԃ��Z�b�g
	if(curState != NULL) umStates[state]->bef = curState; //�ȑO�̏�Ԃ��Z�b�g
	curState = umStates[state]; //���݂̏�Ԃ��Z�b�g
}

void Turn::CheckGameEnd() {
	if (isGameEnd) return;
	//�Q�[���I���t���O
	bool _flag = true;
	//�v���C���[�����t���O
	bool _playerWin = true;

	//�v���C���[��
	for (auto& pl : vPlayer) {
		if (pl->IsDeath()) continue; //���S���X�L�b�v
		if (pl->pCharaInfo->hp <= 0) pl->Death();
		_flag = false; //���S���Ă��Ȃ���΃g���K�[�I�t
	}
	if (_flag) {
		noDel_ptr<GameObject> _temp = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER, 300, 200,
			CreateSprite(new Sprite(L"Data/Image/Battle/lose_text.spr")));
		_playerWin = false;
	}

	//�G��
	if (_flag == false) {
		_flag = true;
		for (auto& en : vEnemy) {
			if (en->IsDeath()) continue; //���S���X�L�b�v
			if (en->pCharaInfo->hp <= 0) en->Death();
			_flag = false; //���S���Ă��Ȃ���΃g���K�[�I�t
		}
		if (_flag) {
			noDel_ptr<GameObject> _temp = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER, 300, 200,
				CreateSprite(new Sprite(L"Data/Image/Battle/win_text.spr")));
		}
	}

	//�Q�[���I��������
	if (_flag) {
		isGameEnd = true;
		//���ʂ��Z�[�u
		FILE* fp;
		fopen_s(&fp, "Data/SaveData/result.txt", "w");
		if (fp == NULL) return;

		if(_playerWin) fprintf_s(fp, "win");
		else fprintf_s(fp, "lose");

		fclose(fp);
	}
	
}

void Turn::TransScene() {
	if (Input::Trg(InputConfig::input["decide"])) {
		SceneManager::SwitchScene(eSceneTable::Strategy);
	}
}