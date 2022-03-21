#include "../../../../../../framework.h"
#include "../../../../../../environment.h"
#include "EnemyChara.h"

using namespace nsBattle;

//初期化
void EnemyChara::Start() {
	fMoveFunc = &EnemyChara::StartMove;
}

//範囲内からスキルを選択する
void EnemyChara::ChooseSkill(eSkillType type) {
	int _range = pSearcher.ReturnDistance(pTargetChara->GetCurrentSquare(), pCurSquare);
	pSearcher.Reset();
	rangeToTarget = _range;
	for (auto& skill : pCharaInfo->GetSkills()) {
		//消費MPが足りなければ処理しない
		if (pCharaInfo->mp < skill->GetConsumeMP()) continue;

		//引数以外のスキル種なら処理しない
		if (skill->GetSkillType() != type) continue;
		//適正距離ならスキルを返す
		if (skill->GetMaxDis() >= _range && skill->GetMinDis() <= _range) {
			//回復タイプの場合MPとHPを分ける
			if (type == eSkillType::Heal) {
				int _maxHp = pTargetChara->pCharaInfo->maxHp;
				int _maxMp = pTargetChara->pCharaInfo->maxMp;
				int _hp = pTargetChara->pCharaInfo->hp;
				int _mp = pTargetChara->pCharaInfo->mp;
				if (dynamic_noDel_cast<HealSkill>(skill)->GetHealType() == eHealType::HP) {
					//HPが半分以上あれば回復しない
					if (_hp >= _maxHp / 2) continue;
					pSelectSkill = skill;
				}
				else {
					//MPが半分以上あれば回復しない
					if (_mp >= _maxMp / 2)  continue;
					pSelectSkill = skill;
				}
			}
			//ほかのスキル種の場合そのままスキル設定
			else {
				pSelectSkill = skill;
			}
			return;
		}
	}

	pSelectSkill = NULL;
}

//攻撃処理
bool EnemyChara::Skill() {
	//スキルが設定されていなければ何もしない
	if (pSelectSkill == NULL) return false;

	//攻撃状態ならターン状態をスキルセレクトに設定
	if (pSelectSkill->GetSkillType() == eSkillType::Attack) {
		gameObject->FindGameObject("fieldManager")->
			GetComponent<FieldManager>()->SetTurnState(eTurnState::SelectSkill);
	}
	//回復処理
	else if (pSelectSkill->GetSkillType() == eSkillType::Heal) {
		Heal();
	}
	//バフ処理
	else if (pSelectSkill->GetSkillType() == eSkillType::Buff) {
		Buff();
	}

	//キャラの行動終了設定
	SetActionEnable(false);

	//探索情報初期化
	pSearcher.Reset();

	return true;
}

//移動処理
bool EnemyChara::Move() {
	if (fMoveFunc(*this) == false) {
		return false;
	}
	return true;
}
bool EnemyChara::StartMove() {
	pSearcher.GetCulcedStartSquare(pTargetChara->GetCurrentSquare(), GetCurrentSquare());
	fMoveFunc = &EnemyChara::MoveToTarget;
	return true;
}
bool EnemyChara::MoveToTarget() {
	//移動先がない場合はfalse
	if (pSearcher.vCloseList.size() == 0) return false;

	//移動先のスタートマスを渡して移動
	if (!BattleChara::Move(pSearcher.vCloseList.back())) {
		fMoveFunc = &EnemyChara::StartMove;
		return false;
	}

	return true;
}