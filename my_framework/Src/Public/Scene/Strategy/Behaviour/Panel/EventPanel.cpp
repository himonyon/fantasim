#include "../../../../../../framework.h"
#include "../../../../../../environment.h"

using namespace nsStrategy;

void EventPanel::Awake() {
	//サウンドマネージャー
	pSoundManager = gameObject->FindGameObject("soundManager")->GetComponent<SoundManager>();
	pGM = gameObject->FindGameObject("gameManager")->GetComponent<GameManager>();


	noDel_ptr<ImageRenderer> _renderer = gameObject->GetComponent<ImageRenderer>();
	float _sizeX = _renderer->size.x;
	float _sizeY = _renderer->size.y;
	float _top = transform->position.y - _sizeY / 2;
	float _left = transform->position.x - _sizeX / 2;

	float _textPosX = 0;
	float _textPosY = 0;

	_textPosX = _left + _sizeX * 0.1f;
	_textPosY = _top + _sizeY * 0.1f;

	//タイトルテキスト作成
	CreateText(pTitleText, _textPosX, _textPosY);
	pTitleText->Print(L"EVENT");
	pTitleText->SetFontSize(20.0f);
	//テキスト
	for (int i = 0; i < EventMax; i++) {
		_textPosY = _top + _sizeY * (0.2f + 0.07f * i);
		CreateText(pTexts[i].pText, _textPosX, _textPosY);
	}

	Close();
}
void EventPanel::Update() {
	if (Input::Trg(InputConfig::input["decide"])) {
		pSoundManager->Play("cancel");
		Close();
	}
}

void EventPanel::Open() {
	//初期化
	for (auto& text : pTexts) {
		text.flag = false;
		text.pText->Print(L"");
	}

	//イベント発生
	Event();

	Panel::Open();
}
void EventPanel::Close() {
	Panel::Close();
}

//空いているテキストに書き込む
noDel_ptr<Font> EventPanel::GetEventText() {
	for (auto& text : pTexts) {
		if (text.flag) continue;
		text.flag = true;
		return text.pText;
	}
	return NULL;
}

//イベント処理
void EventPanel::Event() {
	//年月更新
	pGM->UpdateMonth();
	int _year = pGM->GetYear();
	int _month = pGM->GetMonth();
	pTitleText->Print(L"EVENT 　%d年 %d月", _year, _month);

	//収入時テキスト
	if (_month == 4 || _month == 10) {
		noDel_ptr<Font> _text = GetEventText();
		_text->Print(L"各街に収入が入りました");
	}

	//イベント処理
	EventCulc(_month);
}
//毎ターン処理するイベント(支持率・災害耐性減少)＋収入
void EventPanel::EventCulc(int month) {
	srand((unsigned int)time(NULL));
	int _rand = 0;
	int _curID = 0;
	while (true) {
		//街の取得
		noDel_ptr<City> _city = pGM->GetCity(_curID);
		if (_city == NULL) break;

		//災害耐性減少（毎ターンランダムで0~3）
		DecDisaster(_city);

		//支持率減少（毎ターンランダムで0~3）
		DecSupport(_city);

		//収入処理
		if (month == 4 || month == 10) Income(_city);

		//台風
		Typhoon(_city);

		//暴動
		Riot(_city);

		//更新
		_curID++;
	}
}
//収入
void EventPanel::Income(noDel_ptr<City> city) {
	//収入処理(経済力*景気)
	city->money += (int)((float)city->economic * city->bussiness);
}
//支持率減少
void EventPanel::DecSupport(noDel_ptr<City> city) {
	int _rand = rand() % 4;
	city->support -= _rand;
	if (city->support < 0) city->support = 0;
}
//災害耐性減少
void EventPanel::DecDisaster(noDel_ptr<City> city) {
	int _rand = rand() % 4;
	city->disaster -= _rand;
	if (city->disaster < 0) city->disaster = 0;
}
//台風
void EventPanel::Typhoon(noDel_ptr<City> city) {
	//3%の確率
	int _rand = rand() % 100;
	if (_rand >= 5) return;
	//災害耐性に応じてステータス減少
	int _decMoney= 100;
	int _decVal = rand() % 500;
	//１０％ごとに減少割合上昇
	_decMoney *= 10 - city->disaster / 10;
	_decVal *= 10 - city->disaster / 10;
	///反映
	city->money -= _decMoney;
	city->economic -= _decVal;
	if (city->money < 0) city->money = 0;
	if (city->economic < 0) city->economic = 0;

	//テキスト設定
	noDel_ptr<Font> _text = GetEventText();
	if (_text == NULL) return;
	_text->Print(L"%sで台風が起きました", city->GetName().c_str());
}
//暴動
void EventPanel::Riot(noDel_ptr<City> city) {
	//支持率が低いと起きる
	if (city->support > 50) return;
	//3%の確率
	int _rand = rand() % 100;
	if (_rand >= 5) return;
	//ステータス減少
	int _decMoney = 3000;
	int _decVal = 1000;
	///反映
	city->money -= _decMoney;
	city->political -= _decVal;
	if (city->money < 0) city->money = 0;
	if (city->political < 0) city->political = 0;

	//テキスト設定
	noDel_ptr<Font> _text = GetEventText();
	if (_text == NULL) return;
	_text->Print(L"%sで暴動が起きました", city->GetName().c_str());
}
