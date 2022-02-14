#include "../../../../../../framework.h"
#include "../../../../../../environment.h"
#include "StatusPanel.h"

using namespace nsBattle;

void StatusPanel::Awake() {
	gameObject->GetComponent<ImageRenderer>()->SetRenderPriority(-10);

	//パネルの上下とサイズ
	float _sizeX = gameObject->GetComponent<ImageRenderer>()->sizeX;
	float _sizeY = gameObject->GetComponent<ImageRenderer>()->sizeY;
	float _top = transform->position.y - (_sizeY / 2);
	float _left = transform->position.x - (_sizeX / 2);

	//HP,MPの画像
	noDel_ptr<Sprite> pHPBar_sp = CreateSprite(new Sprite(L"Data/Image/Battle/hp_bar.spr"));
	noDel_ptr<Sprite> pFrame_sp = CreateSprite(new Sprite(L"Data/Image/Battle/hp_frame.spr"));
	noDel_ptr<Sprite> pMPBar_sp = CreateSprite(new Sprite(L"Data/Image/Battle/mp_bar.spr"));

	//各オブジェクトの表示位置
	float _x, _y;

	//上部
	_x = _left + (_sizeX * 0.1f);
	_y = _top + (_sizeY * 0.2f);
	pNameText = gameObject->CreateObject(0, 0, 0, transform);
	pNameText->transform->SetLocalPosition(_x, _y);
	pNameText->AddComponent<Font>();
	pNameText->GetComponent<Font>()->SetFontSize(20.0f);
	_x = _left + (_sizeX * 0.7f);
	_y = _top + (_sizeY * 0.25f);
	pAttrObj = gameObject->CreateImageObject(0,0, 10.0f, 10.0f,NULL, transform);
	pAttrObj->transform->SetLocalPosition(_x, _y);

	//下部
	//HP
	_x = _left + (_sizeX * 0.4f);
	_y = _top + (_sizeY * 0.5f);
	pHPBar = gameObject->CreateImageObject(0, 0, _sizeX * 0.7f, 30.0f, pHPBar_sp, transform);
	pHPBar->transform->SetLocalPosition(_x, _y);
	pHPFrame = gameObject->CreateImageObject(0, 0, _sizeX * 0.7f, 30.0f, pFrame_sp, transform);
	pHPFrame->transform->SetLocalPosition(_x, _y);
	_x = _left + (_sizeX * 0.8f);
	_y = _top + (_sizeY * 0.45f);
	pHPText = gameObject->CreateObject(0, 0, 0, transform);
	pHPText->transform->SetLocalPosition(_x, _y);
	pHPText->AddComponent<Font>();
	
	//MP
	_x = _left + (_sizeX * 0.4f);
	_y = _top + (_sizeY * 0.8f);
	pMPBar = gameObject->CreateImageObject(0, 0, _sizeX*0.7f, 30.0f, pMPBar_sp, transform);
	pMPBar->transform->SetLocalPosition(_x, _y);
	pMPFrame = gameObject->CreateImageObject(0, 0, _sizeX * 0.7f, 30.0f, pFrame_sp, transform);
	pMPFrame->transform->SetLocalPosition(_x, _y);
	_x = _left + (_sizeX * 0.8f);
	_y = _top + (_sizeY * 0.75f);
	pMPText = gameObject->CreateObject(0, 0, 0, transform);
	pMPText->transform->SetLocalPosition(_x, _y);
	pMPText->AddComponent<Font>();

	//初期は非表示
	gameObject->SetObjEnable(false);
}

void StatusPanel::Open(noDel_ptr<BattleChara>  battleChara) {
	Panel::Open();
	//位置設定
	//ワールドの中央
	float _center = (SpriteRenderer::WorldWHPos[1] + SpriteRenderer::WorldWHPos[0]) / 2;
	if (battleChara->transform->position.x > _center) transform->SetPosition(250, 150);
	else transform->SetPosition(SCREEN_WIDTH - 250, 150);

	//キャラ情報取得
	noDel_ptr<Charactor> chara = battleChara->pCharaInfo;

	//テキスト/属性
	pNameText->GetComponent<Font>()->Print(L"%s", chara->GetName().c_str());
	pHPText->GetComponent<Font>()->Print(L"%d/%d", chara->hp, chara->maxHp);
	pMPText->GetComponent<Font>()->Print(L"%d/%d", chara->mp, chara->maxMp);
	pAttrObj->GetComponent<ImageRenderer>()->SetUpRenderer2D(32.0f, 32.0f, chara->GetAttr()->GetSprite());

	//HP,MPバーのスケーリングと調整
	float _barSize = pHPBar->GetComponent<ImageRenderer>()->sizeX;
	float _scale = (float)chara->hp / (float)chara->maxHp;
	pHPBar->transform->scale.x = _scale;
	pHPBar->transform->position.x = pHPFrame->transform->position.x - ((_barSize - (_barSize * _scale)) / 2);
	_scale = (float)chara->mp / (float)chara->maxMp;
	pMPBar->transform->scale.x = _scale;
	pMPBar->transform->position.x = pMPFrame->transform->position.x - ((_barSize - (_barSize * _scale)) / 2);
}