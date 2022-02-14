#include "../../../../../../framework.h"
#include "../../../../../../environment.h"

using namespace nsStrategy;
//初期化
void CharaEnhancePanel::Awake() {
	//サウンドマネージャー
	pSoundManager = gameObject->FindGameObject("soundManager")->GetComponent<SoundManager>();

	noDel_ptr<ImageRenderer> _renderer = gameObject->GetComponent<ImageRenderer>();
	float _sizeX = _renderer->sizeX;
	float _sizeY = _renderer->sizeY;
	float _top = transform->position.y - _sizeY / 2;
	float _left = transform->position.x - _sizeX / 2;

	float _textPosX = 0;
	float _textPosY = 0;
	
	//キャラ名
	_textPosX = _left + _sizeX * 0.1f;
	_textPosY = _top + _sizeY * 0.2f;
	for (int i = 0; i < OWN_CHARACTOR_NUM; i++) {
		const float _padding = 40.0f;
		float _y = _textPosY + (i * _padding);
		CreateText(pNameText[i], _textPosX, _y);
	}
	//題目
	_textPosY = _top + _sizeY * 0.1f;
	CreateText(pTopText, _textPosX, _textPosY);
	pTopText->Print(L"強化するキャラを選択してください");
	//必要金額テキスト
	float _Rate = 0.2f;
	float _RatePad = 0.085f;
	_textPosX = _left + _sizeX * 0.5f;
	_textPosY = _top + _sizeY * _Rate;
	CreateText(pNeedMoneyText, _textPosX, _textPosY);
	//属性
	_Rate += _RatePad;
	_textPosY = _top + _sizeY * _Rate;
	CreateText(pAttrText,_textPosX, _textPosY);
	//レベル
	_Rate += _RatePad;
	_textPosY = _top + _sizeY * _Rate;
	CreateText(pLevelText, _textPosX, _textPosY);
	//個体値
	_Rate += _RatePad;
	_textPosY = _top + _sizeY * _Rate;
	CreateText(pPowerText, _textPosX, _textPosY);
	//HP
	_Rate += _RatePad;
	_textPosY = _top + _sizeY * _Rate;
	CreateText(pHpText, _textPosX, _textPosY);
	//MP
	_Rate += _RatePad;
	_textPosY = _top + _sizeY * _Rate;
	CreateText(pMpText, _textPosX, _textPosY);
	//防御力
	_Rate += _RatePad;
	_textPosY = _top + _sizeY * _Rate;
	CreateText(pDefText, _textPosX, _textPosY);
	//回避率
	_Rate += _RatePad;
	_textPosY = _top + _sizeY * _Rate;
	CreateText(pEvdText, _textPosX, _textPosY);
	//移動量
	_Rate += _RatePad;
	_textPosY = _top + _sizeY * _Rate;
	CreateText(pMoveText, _textPosX, _textPosY);

	//カーソル
	noDel_ptr<GameObject> _pCursorObj = gameObject->CreateImageObject(0, 0, 150, 30, CreateSprite(
		new Sprite(L"Data/Image/Common/square.spr")), transform);
	pCursor = _pCursorObj->GetComponent<ImageRenderer>();
	pCursor->SetRenderPriority((int)eRenderOrder::FrontUI - 1);

	//キャラ画像
	_textPosX = _left + _sizeX * 0.2f;
	_textPosY = _top + _sizeY * 0.8f;
	noDel_ptr<GameObject> _pCharaObj = gameObject->CreateImageObject(_textPosX, _textPosY, 150, 150, NULL, transform);
	_pCharaObj->transform->scale.x *= -1;
	pCharaImage = _pCharaObj->GetComponent<ImageRenderer>();
	pCharaImage->SetRenderPriority((int)eRenderOrder::FrontUI - 1);
	pCharaImage->SetColor(1, 1, 1, 1);

	//ボーダー
	_textPosX = _left + _sizeX * 0.4f;
	_textPosY = _top + _sizeY * 0.55f;
	noDel_ptr<GameObject> _pBorderObj = gameObject->CreateImageObject(_textPosX, _textPosY, 300, _sizeY - 150.0f, CreateSprite(
		new Sprite(L"Data/Image/Common/border.spr")), transform);
	pBorder = _pBorderObj->GetComponent<ImageRenderer>();
	pBorder->SetRenderPriority((int)eRenderOrder::FrontUI + 1);

	//パネル作成
	pConfirmPanel = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER, 250, 150,
		_renderer->pRenderSprite, transform);
	pConfirmPanel->GetComponent<ImageRenderer>()->SetRenderPriority((int)eRenderOrder::FrontUI+ 10);
	//確認テキスト
	noDel_ptr<GameObject> _tempObj = gameObject->CreateObject(0, 0, 0, pConfirmPanel->transform);
	_tempObj->transform->SetLocalPosition(0, -30.0f);
	_tempObj->AddComponent<Font>();
	pConfirmText = _tempObj->GetComponent<Font>();
	pConfirmText->SetTextAlignment(eTextAlignment::Center);
	pConfirmText->Print(L"キャラを強化しますか？");
	pConfirmText->SetRenderPriority((int)eRenderOrder::FrontUI + 11);
	//確認テキスト２
	_tempObj = gameObject->CreateObject(0, 0, 0, pConfirmPanel->transform);
	_tempObj->transform->SetLocalPosition(0, 10.0f);
	_tempObj->AddComponent<Font>();
	pYesNoText = _tempObj->GetComponent<Font>();
	pYesNoText->SetTextAlignment(eTextAlignment::Center);
	pYesNoText->Print(L"〇：はい　　×：いいえ");
	pYesNoText->SetRenderPriority((int)eRenderOrder::FrontUI + 11);

	pConfirmPanel->SetObjEnable(false);

	Close();
}

//テキスト作成
void CharaEnhancePanel::CreateText(noDel_ptr<Font>& target, float posX, float posY) {
	noDel_ptr<GameObject> _tempObj = gameObject->CreateObject(posX, posY, 0, transform);
	_tempObj->AddComponent<Font>();
	target = _tempObj->GetComponent<Font>();
	target->SetRenderPriority((int)eRenderOrder::FrontUI);
}

//処理
void CharaEnhancePanel::Update() {
	//確認画面が開いた状態
	if (pConfirmPanel->IsObjEnable()) {
		if (Input::Trg(InputConfig::input["decide"])) {
			Enhance();
			pConfirmPanel->SetObjEnable(false);
		}
		if (Input::Trg(InputConfig::input["cancel"])) {
			pConfirmPanel->SetObjEnable(false);
			pSoundManager->Play("cancel");
		}
		return;
	}

	//確認画面を開く
	if (Input::Trg(InputConfig::input["decide"])) {
		if (IsEnhance(pSelectCity->vOwnChara[cursorNum])) {
			pConfirmPanel->SetObjEnable(true);
			pSoundManager->Play("decide");
		}
	}

	//戻る
	if (Input::Trg(InputConfig::input["cancel"])) {
		gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->SetTurnState(eTurnState::Back);
		pSoundManager->Play("cancel");
		Close();
	}

	//カーソル移動
	MoveCursor();
}

//カーソル移動
void CharaEnhancePanel::MoveCursor() {
	int _bef = cursorNum;

	if (Input::Trg(InputConfig::input["down"])) cursorNum++;
	if (Input::Trg(InputConfig::input["up"])) cursorNum--;

	if (cursorNum < 0) cursorNum = 0;
	if (cursorNum >= (int)pSelectCity->vOwnChara.size()) cursorNum = (int)pSelectCity->vOwnChara.size() - 1;

	if (_bef == cursorNum) return;

	//カーソル位置設定
	pCursor->transform->SetPosition(pNameText[cursorNum]->transform->position.x + 60.0f,
		pNameText[cursorNum]->transform->position.y + 12.5f);
	//ステータステキスト更新
	SetStatusText(pSelectCity->vOwnChara[cursorNum]);
	//画像更新
	pCharaImage->SetUpRenderer2D(150, 150, pSelectCity->vOwnChara[cursorNum]->GetSprite());
}

//開く
void CharaEnhancePanel::Open(noDel_ptr<City> city) {
	//キャラがいなければ閉じる
	if (city->vOwnChara.size() == 0) {
		gameObject->FindGameObject("gameManager")->
			GetComponent<GameManager>()->SetTurnState(eTurnState::Command);
		return;
	}

	//金がなければ閉じる
	if (city->money < 1000) {
		gameObject->FindGameObject("gameManager")->
			GetComponent<GameManager>()->SetTurnState(eTurnState::Command);
		return;
	}

	//初期化
	cursorNum = 0;
	pSelectCity = city;


	//テキスト設定
	//キャラテキスト
	for (auto& text : pNameText) {
		text->Print(L"");
	}
	int _count = 0;
	for (auto& chara : city->vOwnChara) {
		//強化負荷の場合色を薄める
		if (IsEnhance(chara) == false) pNameText[_count]->SetColor(0x55ffffff);
		pNameText[_count]->Print(L"%s", chara->GetName().c_str());
		_count++;
	}

	//カーソル
	pCursor->transform->SetPosition(pNameText[cursorNum]->transform->position.x + 60.0f,
		pNameText[cursorNum]->transform->position.y + 12.5f);

	//初めに表示するキャラのテキストセット
	SetStatusText(city->vOwnChara[cursorNum]);
	pCharaImage->SetUpRenderer2D(150, 150,city->vOwnChara[cursorNum]->GetSprite());

	Panel::Open();
}

//ステータステキストセット
void CharaEnhancePanel::SetStatusText(noDel_ptr<Charactor> chara) {
	//必要金額
	int _money = GetNeedMoney(chara->level);
	pNeedMoneyText->Print(L"強化金額： %d", _money);
	//属性
	pAttrText->Print(L"属性： %s", chara->GetAttr()->GetName().c_str());
	//レベル
	pLevelText->Print(L"レベル： %d  >>> %d", chara->level, chara->level + upValue.level);
	//個体値
	pPowerText->Print(L"個体値： %d  >>> %d", chara->power, chara->power + upValue.power);
	//HP
	pHpText->Print(L"HP： %d  >>> %d", chara->maxHp, chara->maxHp + upValue.hp);
	//MP
	pMpText->Print(L"MP： %d  >>> %d", chara->maxMp, chara->maxMp + upValue.mp);
	//防御力
	pDefText->Print(L"防御力： %d  >>> %d", chara->def, chara->def + upValue.def);
	//回避率
	pEvdText->Print(L"回避率： %d%%  >>> %d%%", chara->evd, chara->evd + upValue.evd);
	//移動量
	if(chara->level % 3 == 0)
		pMoveText->Print(L"移動量： %d  >>> %d", chara->move, chara->move + upValue.move);
	else
		pMoveText->Print(L"移動量： %d", chara->move);
}

//強化
void CharaEnhancePanel::Enhance() {
	
	noDel_ptr<Charactor> _chara = pSelectCity->vOwnChara[cursorNum];

	pSoundManager->Play("enhance");

	//街の資金を減らす
	int _needMoney = GetNeedMoney(_chara->level);
	pSelectCity->money -= _needMoney;

	//レベル
	_chara->level += upValue.level;
	//個体値
	_chara->power += upValue.power;
	//HP
	_chara->maxHp += upValue.hp;
	//MP
	_chara->maxMp += upValue.mp;
	//防御力
	_chara->def += upValue.def;
	//回避率
	_chara->evd += upValue.evd;
	//移動量
	if (_chara->level % 3 == 0) _chara->move += upValue.move;

	//テキスト更新
	SetStatusText(_chara);
	int _count = 0;
	for (auto& chara : pSelectCity->vOwnChara) {
		//強化不可の場合色を薄める
		if (IsEnhance(chara) == false) pNameText[_count]->SetColor(0x55ffffff);
		_count++;
	}
}

//強化可能か
bool CharaEnhancePanel::IsEnhance(noDel_ptr<Charactor> chara) {
	int _needMoney = 0;
	//レベルが１0以上なら強化不可
	if (chara->level >= 10) return false;

	//必要な金額計算
	_needMoney = GetNeedMoney(chara->level);
	//金額が足りなければ強化不可
	if (pSelectCity->money < _needMoney) return false;
	
	return true;
}

//必要金額計算
int CharaEnhancePanel::GetNeedMoney(int level) {
	int _need = 1000;
	for (int i = 1; i < level; i++) {
		_need = (int)((float)_need * 1.4f);
	}

	return _need;
}
