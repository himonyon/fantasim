#include "../../../../../../framework.h"
#include "../../../../../../environment.h"

using namespace nsStrategy;

void CharaSelectPanel::Awake() {
	//サウンドマネージャー
	pSoundManager = gameObject->FindGameObject("soundManager")->GetComponent<SoundManager>();

	noDel_ptr<Font> _font; //コンポーネント取得用

	float _localTop = -(gameObject->GetComponent<ImageRenderer>()->sizeY / 2);

	//説明テキスト
	pDescText = gameObject->CreateObject(0, 0, 0, transform);
	pDescText->transform->SetLocalPosition(0, _localTop + 30.0f);
	pDescText->AddComponent<Font>();
	_font = pDescText->GetComponent<Font>();
	_font->SetTextAlignment(eTextAlignment::Center);
	_font->Print(L"・移動するキャラクター");
	_font->SetRenderPriority((int)eRenderOrder::FrontUI);

	pBorderText = gameObject->CreateObject(0, 0, 0, transform);
	pBorderText->transform->SetLocalPosition(0, _localTop + 50.0f);
	pBorderText->AddComponent<Font>();
	_font = pBorderText->GetComponent<Font>();
	_font->SetTextAlignment(eTextAlignment::Center);
	_font->Print(L"""--------------------------------");
	_font->SetRenderPriority((int)eRenderOrder::FrontUI);

	//キャラクターテキスト
	for (int i = 0; i < OWN_CHARACTOR_NUM; i++) {
		float _textPadding = 30.0f;
		pCharaText[i] = gameObject->CreateObject(0, 0, 0, transform);
		pCharaText[i]->transform->SetLocalPosition(0, _localTop + 80.0f + (_textPadding * i));
		pCharaText[i]->AddComponent<Font>();
		_font = pCharaText[i]->GetComponent<Font>();
		_font->SetTextAlignment(eTextAlignment::Center);
		_font->SetRenderPriority((int)eRenderOrder::FrontUI);
	}

	//決定テキストボタン
	pDecideText = gameObject->CreateObject(0, 0, 0, transform);
	pDecideText->transform->SetLocalPosition(0, -_localTop - 50.0f);
	pDecideText->AddComponent<Font>();
	_font = pDecideText->GetComponent<Font>();
	_font->SetTextAlignment(eTextAlignment::Center);
	_font->Print(L"確定");
	_font->SetRenderPriority((int)eRenderOrder::FrontUI);

	//セレクトカーソル
	pSelectCursor = gameObject->CreateImageObject(0, 0, 1, 25, CreateSprite(
		new Sprite(L"Data/Image/Common/square.spr")), transform);
	pSelectCursor->GetComponent<ImageRenderer>()->SetRenderPriority((int)eRenderOrder::FrontObject);

	//初期状態で隠しておく
	gameObject->SetObjEnable(false);
}

void CharaSelectPanel::Update() {\
	//カーソル移動
	if (Input::Trg(InputConfig::input["up"]) || Input::Trg(InputConfig::input["down"])) {
		//カウントアップ
		Input::Trg(InputConfig::input["up"]) ? cursorNum-- : cursorNum++;
		if (cursorNum > maxCursorNum) cursorNum = 0;
		else if (cursorNum < 0) cursorNum = maxCursorNum;

		if (cursorNum == maxCursorNum) {
			pSelectCursor->transform->SetPosition(pDecideText->transform->position.x, pDecideText->transform->position.y + 12.0f);
			pSelectCursor->transform->scale.x = pDecideText->GetComponent<Font>()->GetTextLength() + 10.0f;
		}
		else {
			pSelectCursor->transform->SetPosition(pCharaText[cursorNum]->transform->position.x,
				pCharaText[cursorNum]->transform->position.y + 12.0f);
			pSelectCursor->transform->scale.x = pCharaText[cursorNum]->GetComponent<Font>()->GetTextLength() + 20.0f;
		}
	}

	if (Input::Trg(InputConfig::input["cancel"])) {
		pSoundManager->Play("cancel"); //決定音
		Close();
		gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->SetTurnState(eTurnState::Back);
	}

	///キャラ選択と実行
	if (Input::Trg(InputConfig::input["decide"])) {
		pSoundManager->Play("decide"); //決定音
		if (cursorNum == maxCursorNum) 	DecideMoveChara();
		else AddMoveChara();
	}
}

void CharaSelectPanel::Open(noDel_ptr<City> city, noDel_ptr<City> city2) {
	pCity = city;
	pCity2 = city2;

	//操作説明テキスト変更
	noDel_ptr<Operation> _opr = gameObject->FindGameObject("operation")->GetComponent<Operation>();
	_opr->ResetOperation();
	_opr->AddOperation("decide", L"選択");
	_opr->AddOperation("cancel", L"戻る");

	Panel::Open();

	cursorNum = 0;
	maxCursorNum = (int)pCity->vOwnChara.size();

	//キャラクターテキスト
	for (int i = 0; i < OWN_CHARACTOR_NUM; i++) {
		pCharaText[i]->GetComponent<Font>()->SetColor(0xffffffff);
		if(i < pCity->vOwnChara.size()) 
			pCharaText[i]->GetComponent<Font>()->Print(L"%s", pCity->vOwnChara[i]->GetName().c_str());
		else
			pCharaText[i]->GetComponent<Font>()->Print(L"");
	}

	pSelectCursor->transform->SetPosition(pCharaText[0]->transform->position.x, pCharaText[0]->transform->position.y + 12.0f);
	pSelectCursor->transform->scale.x = pCharaText[0]->GetComponent<Font>()->GetTextLength() + 10.0f;
}

void CharaSelectPanel::Close() {
	vMoveCharas.clear();
	Panel::Close();
}

void CharaSelectPanel::AddMoveChara() {
	auto it = pCity->vOwnChara.begin() + cursorNum;
	bool _flag = false;
	//すでにある場合外す
	for (auto itr = vMoveCharas.begin(); itr != vMoveCharas.end();) {
		if (itr->get() == it->get()) {
			_flag = true;
			itr = vMoveCharas.erase(itr);
			break;
		}
		else {
			itr++;
		}
	}

	//色変更と追加処理
	if (_flag) pCharaText[cursorNum]->GetComponent<Font>()->SetColor(0xffffffff);
	else {
		//最大数を超える場合は処理しない
		if ((int)pCity2->vOwnChara.size() + (int)vMoveCharas.size() == OWN_CHARACTOR_NUM) return;
		pCharaText[cursorNum]->GetComponent<Font>()->SetColor(0xffff00ff);
		vMoveCharas.emplace_back(pCity->vOwnChara[cursorNum]);
	}
}
void CharaSelectPanel::DecideMoveChara() {
	//対象を他の町へ移動させる
	for (auto itr = vMoveCharas.begin(); itr != vMoveCharas.end(); itr++) {
		int _count = 0;
		for (auto itr2 = pCity->vOwnChara.begin(); itr2 != pCity->vOwnChara.end();) {
			if (itr->get() == itr2->get()) {
				pCity2->vOwnChara.emplace_back(pCity->vOwnChara[_count]);
				itr2 = pCity->vOwnChara.erase(itr2);
				break;
			}
			else {
				itr2++;
				_count++;
			}
		}
	}

	pCity->IncActCount(); //行動した回数を増加させる
	Close();
	gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->SetTurnState(eTurnState::Command);
}

