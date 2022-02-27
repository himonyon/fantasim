#include "../../../../framework.h"
#include "../../../../environment.h"

void Panel::Open() {
	openTrg = true;
	gameObject->SetObjEnable(true);
}

void Panel::Close() {
	openTrg = false;
	gameObject->SetObjEnable(false);
}

//テキスト作成
void Panel::CreateText(noDel_ptr<Font>& target, float posX, float posY) {
	noDel_ptr<GameObject> _tempObj = gameObject->CreateObject(posX, posY, 0, transform);
	_tempObj->AddComponent<Font>();
	target = _tempObj->GetComponent<Font>();
	target->SetRenderPriority((int)eRenderOrder::FrontUI);
}