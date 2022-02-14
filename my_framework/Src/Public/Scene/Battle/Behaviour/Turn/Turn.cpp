#include "../../../../../../framework.h"
#include "../../../../../../environment.h"
#include "Turn.h"

using namespace nsBattle;


void Turn::Start() {
	//プレイヤーキャラと敵キャラ
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
	//カメラ移動
	float _moveX = Input::GetDX(InputConfig::input["moveCamX"]) * 0.2f;
	float _moveY = -Input::GetDY(InputConfig::input["moveCamY"]) * 0.2f;
	if (_moveX != 0 || _moveY != 0) {
		//制限
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
	//戻る状態が送られたら一つ前の状態に戻す
	if (state == eTurnState::Back) {
		for (int i = 0; i < backNum; i++) {
			curState = curState->bef.lock();
		}
		return;
	}

	//一覧にない場合、新規登録
	if (umStates.find(state) == umStates.end()) {
		std::shared_ptr<stTurnState> _temp = std::make_shared<stTurnState>(stTurnState(state));
		umStates[state] = _temp;
	}
	//現在の状態をセット
	if(curState != NULL) umStates[state]->bef = curState; //以前の状態をセット
	curState = umStates[state]; //現在の状態をセット
}

void Turn::CheckGameEnd() {
	if (isGameEnd) return;
	//ゲーム終了フラグ
	bool _flag = true;
	//プレイヤー勝利フラグ
	bool _playerWin = true;

	//プレイヤー側
	for (auto& pl : vPlayer) {
		if (pl->IsDeath()) continue; //死亡時スキップ
		if (pl->pCharaInfo->hp <= 0) pl->Death();
		_flag = false; //死亡していなければトリガーオフ
	}
	if (_flag) {
		noDel_ptr<GameObject> _temp = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER, 300, 200,
			CreateSprite(new Sprite(L"Data/Image/Battle/lose_text.spr")));
		_playerWin = false;
	}

	//敵側
	if (_flag == false) {
		_flag = true;
		for (auto& en : vEnemy) {
			if (en->IsDeath()) continue; //死亡時スキップ
			if (en->pCharaInfo->hp <= 0) en->Death();
			_flag = false; //死亡していなければトリガーオフ
		}
		if (_flag) {
			noDel_ptr<GameObject> _temp = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER, 300, 200,
				CreateSprite(new Sprite(L"Data/Image/Battle/win_text.spr")));
		}
	}

	//ゲーム終了時処理
	if (_flag) {
		isGameEnd = true;
		//結果をセーブ
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