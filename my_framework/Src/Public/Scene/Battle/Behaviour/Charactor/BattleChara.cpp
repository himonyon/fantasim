#include "../../../../../../framework.h"
#include "../../../../../../environment.h"
#include "BattleChara.h"

using namespace nsBattle;

noDel_ptr<BattleChara> BattleChara::pCollisionChara = NULL;

Astar BattleChara::pSearcher = Astar();
bool BattleChara::OnHitFunc = false;

void BattleChara::Awake() {
	//回復オブジェクト
	noDel_ptr<GameObject> _pHealHP_obj = gameObject->CreateObject(transform->position.x,transform->position.y, 0, 0.9f, 0.9f, 
		CreateSprite(new Sprite(L"Data/Image/Battle/healHP.spr")));
	_pHealHP_obj->GetComponent<SpriteRenderer>()->SetRenderPriority(30);
	_pHealHP_obj->GetComponent<SpriteRenderer>()->SetColor(1,1,1,0);
	_pHealHP_obj->AddComponent<Animator>();
	pHealAnimator = _pHealHP_obj->GetComponent<Animator>();
	pHealAnimator->AddAnimation("hp", new SpriteAnimation(L"Data/Animation/healHP.anim"));
	pHealAnimator->AddAnimation("mp", new SpriteAnimation(L"Data/Animation/healMP.anim"));
	pHealAnimator->AddAnimation("buff", new SpriteAnimation(L"Data/Animation/buff.anim"));
}

void BattleChara::OnTriggerEnter2D(noDel_ptr<Collider2D> hitCollider) {
	isCursorHovered = true;
}

void BattleChara::OnTrigger2D(noDel_ptr<Collider2D> hitCollider) {
	//カーソルとの当たり判定を取得
	if (OnHitFunc) {
		pCollisionChara = noDel_ptr<BattleChara>(this);
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
		if(pTarget->searchInfo.pParent != NULL && pTarget->searchInfo.pParent->GetIsUnMove() == false)
			pTarget = pTarget->searchInfo.pParent;
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

void BattleChara::Heal() {
	noDel_ptr<HealSkill> _healS = dynamic_noDel_cast<HealSkill>(pSelectSkill);
	if (_healS == NULL) return; //エラーチェック

	//回復量計算(固定値でなければキャラパワー/100をかける)
	int _healVal = _healS->GetHeal();
	if (_healS->IsFixed() == false) _healVal = (int)(_healVal * ((float)pCharaInfo->power / 100.0f));

	//MP消費
	pCharaInfo->mp -= _healS->GetConsumeMP();

	//各処理
	if (_healS->GetHealType() == eHealType::HP) {
		//アニメーション
		pHealAnimator->transform->position = pTargetChara->transform->position;
		pHealAnimator->PlayAnim("hp");
		//処理
		pTargetChara->pCharaInfo->hp += _healVal;
		if(pTargetChara->pCharaInfo->hp >= pTargetChara->pCharaInfo->maxHp)
			pTargetChara->pCharaInfo->hp = pTargetChara->pCharaInfo->maxHp;
	}
	else if (_healS->GetHealType() == eHealType::MP) {
		//アニメーション
		pHealAnimator->transform->position = pTargetChara->transform->position;
		pHealAnimator->PlayAnim("mp");
		//処理
		pTargetChara->pCharaInfo->mp += _healVal;
		if (pTargetChara->pCharaInfo->mp >= pTargetChara->pCharaInfo->maxMp)
			pTargetChara->pCharaInfo->mp = pTargetChara->pCharaInfo->maxMp;
	}
}

void BattleChara::Buff() {
	noDel_ptr<BuffSkill> _buffS = dynamic_noDel_cast<BuffSkill>(pSelectSkill);
	if (_buffS == NULL) return; //エラーチェック

	//アニメーション
	pHealAnimator->transform->position = pTargetChara->transform->position;
	pHealAnimator->PlayAnim("buff");

	//MP消費
	pCharaInfo->mp -= _buffS->GetConsumeMP();

	//バフ付与
	stBuff _buf = _buffS->GetBuff();
	pTargetChara->pCharaInfo->SetBuff(_buf);
}

void BattleChara::SetActionEnable(bool flag) {
	actionEnable = flag;
	if (flag) {
		gameObject->GetComponent<SpriteRenderer>()->SetColor(1, 1, 1, 1);
	}
	else {
		gameObject->GetComponent<SpriteRenderer>()->SetColor(0.3f, 0.3f, 0.3f, 1);
	}
}

void BattleChara::Death() {
	gameObject->SetObjEnable(false);
	pCurSquare->SetUnMove(false, noDel_ptr<BattleChara>(this));
	pCurSquare = NULL;
}

void BattleChara::SetCurrentSquare(noDel_ptr<Square> square) {
	if (pCurSquare != NULL) pCurSquare->SetUnMove(false, NULL);
	pCurSquare = square;
	pCurSquare->SetUnMove(true, noDel_ptr<BattleChara>(this));
}

void BattleChara::DecreaseBuffCount() {
	for (auto& buff : pCharaInfo->GetBuff()) {
		buff.second->count--;
		if (buff.second->count <= 0) pCharaInfo->DeleteBuff(buff.second->type);
	}
}