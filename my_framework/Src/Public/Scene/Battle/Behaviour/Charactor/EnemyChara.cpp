#include "../../../../../../framework.h"
#include "../../../../../../environment.h"
#include "EnemyChara.h"

using namespace nsBattle;

//������
void EnemyChara::Start() {
	fMoveFunc = &EnemyChara::StartMove;
}

//�͈͓�����X�L����I������
void EnemyChara::ChooseSkill(eSkillType type) {
	int _range = pSearcher.ReturnDistance(pTargetChara->GetCurrentSquare(), pCurSquare);
	pSearcher.Reset();
	rangeToTarget = _range;
	for (auto& skill : pCharaInfo->GetSkills()) {
		//����MP������Ȃ���Ώ������Ȃ�
		if (pCharaInfo->mp < skill->GetConsumeMP()) continue;

		//�����ȊO�̃X�L����Ȃ珈�����Ȃ�
		if (skill->GetSkillType() != type) continue;
		//�K�������Ȃ�X�L����Ԃ�
		if (skill->GetMaxDis() >= _range && skill->GetMinDis() <= _range) {
			//�񕜃^�C�v�̏ꍇMP��HP�𕪂���
			if (type == eSkillType::Heal) {
				int _maxHp = pTargetChara->pCharaInfo->maxHp;
				int _maxMp = pTargetChara->pCharaInfo->maxMp;
				int _hp = pTargetChara->pCharaInfo->hp;
				int _mp = pTargetChara->pCharaInfo->mp;
				if (dynamic_noDel_cast<HealSkill>(skill)->GetHealType() == eHealType::HP) {
					//HP�������ȏ゠��Ή񕜂��Ȃ�
					if (_hp >= _maxHp / 2) continue;
					pSelectSkill = skill;
				}
				else {
					//MP�������ȏ゠��Ή񕜂��Ȃ�
					if (_mp >= _maxMp / 2)  continue;
					pSelectSkill = skill;
				}
			}
			//�ق��̃X�L����̏ꍇ���̂܂܃X�L���ݒ�
			else {
				pSelectSkill = skill;
			}
			return;
		}
	}

	pSelectSkill = NULL;
}

//�U������
bool EnemyChara::Skill() {
	//�X�L�����ݒ肳��Ă��Ȃ���Ή������Ȃ�
	if (pSelectSkill == NULL) return false;

	//�U����ԂȂ�^�[����Ԃ��X�L���Z���N�g�ɐݒ�
	if (pSelectSkill->GetSkillType() == eSkillType::Attack) {
		gameObject->FindGameObject("fieldManager")->
			GetComponent<FieldManager>()->SetTurnState(eTurnState::SelectSkill);
	}
	//�񕜏���
	else if (pSelectSkill->GetSkillType() == eSkillType::Heal) {
		Heal();
	}
	//�o�t����
	else if (pSelectSkill->GetSkillType() == eSkillType::Buff) {
		Buff();
	}

	//�L�����̍s���I���ݒ�
	SetActionEnable(false);

	//�T����񏉊���
	pSearcher.Reset();

	return true;
}

//�ړ�����
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
	//�ړ��悪�Ȃ��ꍇ��false
	if (pSearcher.vCloseList.size() == 0) return false;

	//�ړ���̃X�^�[�g�}�X��n���Ĉړ�
	if (!BattleChara::Move(pSearcher.vCloseList.back())) {
		fMoveFunc = &EnemyChara::StartMove;
		return false;
	}

	return true;
}