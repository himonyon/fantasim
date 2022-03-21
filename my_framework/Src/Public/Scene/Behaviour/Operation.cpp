#include "../../../../framework.h"
#include "../../../../environment.h"

void Operation::Awake() {
	//ボタンの作成
	CreateKeyMouButton();
	CreateJoyButton();
	
	//操作説明オブジェクト作成
	CreateOperation();
}


void Operation::Open() {
	gameObject->SetObjEnable(true);
}
void Operation::Hide() {
	gameObject->SetObjEnable(false);
}

void Operation::CreateOperation() {
	//説明オブジェクトを入れる箱
	for (int i = 0; i < OprNum; i++) {
		noDel_ptr<GameObject> _pTextObj = gameObject->CreateObject(0, 0, 0, transform);
		_pTextObj->AddComponent<Font>();
		oprText[i].trigger = false;
		oprText[i].pText = _pTextObj->GetComponent<Font>();
		oprText[i].pText->gameObject->SetRenderOrder((int)eRenderOrder::FrontUI + 1);
		oprText[i].pText->SetFontSize(24.0f);
		oprText[i].pText->SetTextAlignment(eTextAlignment::Right);
	}
}

void Operation::AddOperation(std::string type, std::wstring text) {
	//y軸
	float _pivotY = transform->position.y - 12.0f;
	//x軸
	//テキストごとのX余白
	float _padding = 30;
	float _pivotX = SCREEN_WIDTH - 100;

	std::wstring _button;

	if (UserSetting::device == (int)eDeviceType::Keyboard)_button = umKeyMouButton[type];
	else _button = umJoyButton[type];
	

	//空いている場所にテキスト設定
	for (int i = 0; i < OprNum; i++) {
		//使用済みならスキップ
		if (oprText[i].trigger) continue;
		//空いていればテキスト設定
		oprText[i].trigger = true;
		oprText[i].pText->Print(L"(%s) : %s", _button.c_str(), text.c_str());
		oprText[i].pText->transform->position.y = _pivotY;
		//一つ前のテキストがあらばその長さ分＋余白分ずらす
		oprText[i].pText->transform->position.x = _pivotX; //0個目はここで終了
		if (i == 0) return;
		float _befPosX = oprText[i - 1].pText->transform->position.x;
		float _befTextLen = oprText[i - 1].pText->GetTextLength();
		oprText[i].pText->transform->position.x = _befPosX - (_befTextLen + _padding);
		return;
	}
}

void Operation::ResetOperation() {
	for (auto& opr : oprText) {
		opr.trigger = false;
		opr.pText->Print(L"");
	}
	
}

//操作ボタン一覧作成（コントローラー）
void Operation::CreateJoyButton() {
	umJoyButton["decide"] = L"〇";
	umJoyButton["cancel"] = L"×";
	umJoyButton["option"] = L"オプション";
	umJoyButton["changeTab"] = L"△";
}
//操作ボタン一覧作成（キーマウス)
void Operation::CreateKeyMouButton() {
	umKeyMouButton["decide"] = L"SPACE";
	umKeyMouButton["cancel"] = L"Q";
	umKeyMouButton["option"] = L"P";
	umKeyMouButton["changeTab"] = L"/";
}