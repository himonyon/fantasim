#include "../../../../../../framework.h"
#include "../../../../../../environment.h"
#include "BattleChara.h"

using namespace nsBattle;

noDel_ptr<BattleChara> BattleChara::pSelectChara = NULL;

Astar BattleChara::pSearcher = Astar();


void BattleChara::OnTriggerEnter2D(noDel_ptr<Collider2D> hitCollider) {
	isCursorHovered = true;
}

void BattleChara::OnTrigger2D(noDel_ptr<Collider2D> hitCollider) {
	//�ړ��I����ԂɈڍs
	if (Input::Trg(InputConfig::input["decide"])) {
		pSelectChara = noDel_ptr<BattleChara>(this);
	}
}

void BattleChara::OnTriggerExit2D(noDel_ptr<Collider2D> hitCollider) {
	isCursorHovered = false;
}

bool BattleChara::Move(noDel_ptr<Square> start) {
	noDel_ptr<Square> pTarget = start;
	float speed = 0.1f;

	//���ڂ̃}�X��ڕW�ɂ��邩��߂�
	for (int i = 0; i < moveCount; i++) {
		if(pTarget->searchInfo.pParent != NULL) pTarget = pTarget->searchInfo.pParent;
	}

	//�^�[�Q�b�g�}�X�Ɍ������Ĉړ�����
	if (transform->position.x < pTarget->transform->position.x) {  //�E�ړ�
		transform->position.x += speed;
		if (transform->position.x >= pTarget->transform->position.x) {
			transform->position.x = pTarget->transform->position.x;
			moveCount++;
		}
	}
	else if (transform->position.x > pTarget->transform->position.x) {  //���ړ�
		transform->position.x -= speed;
		if (transform->position.x <= pTarget->transform->position.x) {
			transform->position.x = pTarget->transform->position.x;
			moveCount++;
		}
	}
	else if (transform->position.y < pTarget->transform->position.y) {  //���ړ�
		transform->position.y += speed;
		if (transform->position.y >= pTarget->transform->position.y) {
			transform->position.y = pTarget->transform->position.y;
			moveCount++;
		}
	}
	else if (transform->position.y > pTarget->transform->position.y) {  //��ړ�
		transform->position.y -= speed;
		if (transform->position.y <= pTarget->transform->position.y) {
			transform->position.y = pTarget->transform->position.y;
			moveCount++;
		}
	}
	else {
		moveCount++;
	}

	//�ړ��͂��J�E���g���傫����ΏI��
	if (pCharaInfo->move <= moveCount) {
		moveCount = 0;
		//���}�X���X�V
		SetCurrentSquare(pTarget);
		return false;
	}

	return true;
}