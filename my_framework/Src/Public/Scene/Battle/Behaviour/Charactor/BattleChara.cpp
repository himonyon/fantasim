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
	//移動選択状態に移行
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

	//何個目のマスを目標にするか定める
	for (int i = 0; i < moveCount; i++) {
		if(pTarget->searchInfo.pParent != NULL) pTarget = pTarget->searchInfo.pParent;
	}

	//ターゲットマスに向かって移動する
	if (transform->position.x < pTarget->transform->position.x) {  //右移動
		transform->position.x += speed;
		if (transform->position.x >= pTarget->transform->position.x) {
			transform->position.x = pTarget->transform->position.x;
			moveCount++;
		}
	}
	else if (transform->position.x > pTarget->transform->position.x) {  //左移動
		transform->position.x -= speed;
		if (transform->position.x <= pTarget->transform->position.x) {
			transform->position.x = pTarget->transform->position.x;
			moveCount++;
		}
	}
	else if (transform->position.y < pTarget->transform->position.y) {  //下移動
		transform->position.y += speed;
		if (transform->position.y >= pTarget->transform->position.y) {
			transform->position.y = pTarget->transform->position.y;
			moveCount++;
		}
	}
	else if (transform->position.y > pTarget->transform->position.y) {  //上移動
		transform->position.y -= speed;
		if (transform->position.y <= pTarget->transform->position.y) {
			transform->position.y = pTarget->transform->position.y;
			moveCount++;
		}
	}
	else {
		moveCount++;
	}

	//移動力よりカウントが大きければ終了
	if (pCharaInfo->move <= moveCount) {
		moveCount = 0;
		//踏マスを更新
		SetCurrentSquare(pTarget);
		return false;
	}

	return true;
}