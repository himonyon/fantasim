#include "../../../../../../framework.h"
#include "../../../../../../environment.h"
#include "PlayerChara.h"

using namespace nsBattle;

void PlayerChara::Start() {
	fMoveFunc = &PlayerChara::ShowMoveArea;
	fSkillFunc = &PlayerChara::ShowSkillRange;
}

//移動処理
bool PlayerChara::Move() {
	if(fMoveFunc == NULL) fMoveFunc = &PlayerChara::ShowMoveArea;
	if (fMoveFunc(*this) == false) {
		pSearcher.Reset();
		return false;
	}
	return true;
}
//移動範囲の表示
bool PlayerChara::ShowMoveArea() {
	//現在の位置を格納
	SetBeforeSquare();
	//選択エリア表示
	pSearcher.ShowRange(pCurSquare, pCharaInfo->move, eAstarFunc::MoveRange);
	fMoveFunc = &PlayerChara::SelectMoveSquare;

	return true;
}
//移動マスの選択
bool PlayerChara::SelectMoveSquare() {
	//選択
	//移動常態を解除(セレクトオブジェクトを消す)
	if (Input::Trg(InputConfig::input["cancel"])) {
		pSearcher.Reset();
		gameObject->FindGameObject("fieldManager")->
			GetComponent<FieldManager>()->SetTurnState(eTurnState::Back);
		//ポインタ初期化
		fMoveFunc = &PlayerChara::ShowMoveArea;
		return false;
	}

	//移動選択状態に移行
	if (Input::Trg(InputConfig::input["decide"])) {
		for (auto& selectSq : pSearcher.vCloseList) {
			//選択されたマスが移動可能マスなら最短経路を計算する
			if (selectSq->GetSelectSquareHover() && selectSq->GetSelectEnable()) {
				pSearcher.GetCulcedStartSquare(selectSq, GetCurrentSquare());
				fMoveFunc = &PlayerChara::MoveToTarget;
				break;
			}
		}
	}

	return true;
}
//目的地へ移動
bool PlayerChara::MoveToTarget() {
	//移動できない場合は処理しない
	if (pSearcher.vCloseList.size() == 0) fMoveFunc = &PlayerChara::SelectMoveSquare;
	else {
		if (!BattleChara::Move(pSearcher.vCloseList.back())) {
			//ターンの状態遷移
			gameObject->FindGameObject("fieldManager")->
				GetComponent<FieldManager>()->SetTurnState(eTurnState::AfterCommand);
			//移動処理の関数ポインタを初期状態に戻す
			fMoveFunc = &PlayerChara::ShowMoveArea;
			return false;
		}
	}
	
	return true;
}


//スキル処理
bool PlayerChara::SkillFunc() {
	if (fSkillFunc == NULL) fSkillFunc = &PlayerChara::ShowSkillRange;
	if (fSkillFunc(*this) == false) {
		pSearcher.Reset();
		return false;
	}
	return true;
}
//スキル範囲の表示
bool PlayerChara::ShowSkillRange() {
	//選択エリア表示
	if (pSelectSkill->GetSkillType() == eSkillType::Attack) {
		pSearcher.ShowRange(pCurSquare, pSelectSkill->GetMaxDis(), eAstarFunc::FindEnemyRange);
		fSkillFunc = &PlayerChara::SelectEnemy;
	}
	else{
		pSearcher.ShowRange(pCurSquare, pSelectSkill->GetMaxDis(), eAstarFunc::FindFriendRange);
		fSkillFunc = &PlayerChara::SelectFriend;
	}

	//キャラの当たり判定を開始
	BattleChara::SetOnHitFunc(true);

	return true;
}
//敵の選択
bool PlayerChara::SelectEnemy() {
	//戻る処理
	if (Input::Trg(InputConfig::input["cancel"])) {
		pSearcher.Reset();
		gameObject->FindGameObject("fieldManager")->
			GetComponent<FieldManager>()->SetTurnState(eTurnState::Back);
		//ポインタ初期化
		fSkillFunc = &PlayerChara::ShowSkillRange;
		return false;
	}

	//攻撃状態に移行
	if (Input::Trg(InputConfig::input["decide"])) {
		for (auto& selectSq : pSearcher.vCloseList) {
			if (selectSq->GetSelectSquareHover() && selectSq->GetSelectEnable()) {
				if (BattleChara::pCollisionChara == NULL) break;
				//敵キャラではない場合処理しない
				noDel_ptr<EnemyChara> _enemy = dynamic_noDel_cast<EnemyChara>(BattleChara::pCollisionChara);
				if (_enemy == NULL) break;
				
				//敵のスキル選択
				_enemy->pTargetChara = noDel_ptr<BattleChara>(this);
				_enemy->ChooseSkill(eSkillType::Attack);

				//ターゲットキャラの設定
				pTargetChara = static_noDel_cast<BattleChara>(_enemy);

				//キャラの行動終了設定
				SetActionEnable(false);

				//ターンの状態をバトル状態へ遷移
				gameObject->FindGameObject("fieldManager")->
					GetComponent<FieldManager>()->SetTurnState(eTurnState::Battle);

				//当たり判定を止める
				BattleChara::SetOnHitFunc(false);

				//ポインタ初期化
				fSkillFunc = &PlayerChara::ShowSkillRange;
				return false;
			}
		}
	}
	return true;
}
//味方の選択
bool PlayerChara::SelectFriend() {
	bool _endFlag = false;

	//戻る処理
	if (Input::Trg(InputConfig::input["cancel"])) {
		pSearcher.Reset();
		gameObject->FindGameObject("fieldManager")->
			GetComponent<FieldManager>()->SetTurnState(eTurnState::Back);
		//ポインタ初期化
		fSkillFunc = &PlayerChara::ShowSkillRange;
		return false;
	}

	//移動選択状態に移行
	if (Input::Trg(InputConfig::input["decide"])) {
		for (auto& selectSq : pSearcher.vCloseList) {
			if (selectSq->GetSelectSquareHover() && selectSq->GetSelectEnable()) {
				if (BattleChara::pCollisionChara == NULL) break;
				//敵キャラではない場合処理しない
				noDel_ptr<PlayerChara> _friend = dynamic_noDel_cast<PlayerChara>(BattleChara::pCollisionChara);
				if (_friend == NULL) break;

				//ターゲットキャラの設定
				pTargetChara = static_noDel_cast<BattleChara>(_friend);

				//処理
				//全体処理
				if (pSelectSkill->IsAllRange()) {
					for (auto& selectSq : pSearcher.vCloseList) {
						//プレイヤーキャラのいるマスならそのキャラを処理
						noDel_ptr<PlayerChara> _charaP = dynamic_noDel_cast<PlayerChara>(selectSq->GetOnChara());
						if (_charaP == NULL) continue;
						pTargetChara = static_noDel_cast<BattleChara>(_charaP);;
						if (pSelectSkill->GetSkillType() == eSkillType::Heal) Heal();
						else Buff();
					}
				}
				//単体処理
				else {
					if (pSelectSkill->GetSkillType() == eSkillType::Heal) Heal();
					else Buff();
				}

				//当たり判定を止める
				BattleChara::SetOnHitFunc(false);

				//キャラの行動終了設定
				SetActionEnable(false);

				//ターンの状態をフィールド状態へ遷移
				gameObject->FindGameObject("fieldManager")->
					GetComponent<FieldManager>()->SetTurnState(eTurnState::Field);

				//ポインタ初期化
				fSkillFunc = &PlayerChara::ShowSkillRange;

				_endFlag = true;

				return false;
			}
		}
	}
	return true;
}


void PlayerChara::BackBeforePos() {
	SetCurrentSquare(pBeforeSquare);
	transform->position = pBeforeSquare->transform->position;
}

