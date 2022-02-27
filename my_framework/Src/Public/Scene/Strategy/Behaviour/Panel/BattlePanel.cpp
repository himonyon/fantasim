#include "../../../../../../framework.h"
#include "../../../../../../environment.h"

using namespace nsStrategy;

void BattlePanel::Awake() {
	//サウンドマネージャー
	pSoundManager = gameObject->FindGameObject("soundManager")->GetComponent<SoundManager>();

	//テキスト作成
	float _localTop = -(gameObject->GetComponent<ImageRenderer>()->sizeY / 2);

	pPlayerCityNameText = gameObject->CreateObject(0, 0, 0, transform);
	pPlayerCityNameText->AddComponent<Font>();
	pPlayerCityNameText->GetComponent<Font>()->SetRenderPriority((int)eRenderOrder::FrontUI);

	pEnemyCityNameText = gameObject->CreateObject(0, 0, 0, transform);
	pEnemyCityNameText->AddComponent<Font>();
	pEnemyCityNameText->GetComponent<Font>()->SetRenderPriority((int)eRenderOrder::FrontUI);

	pStartText = gameObject->CreateObject(0, 0, 0, transform);
	pStartText->transform->SetLocalPosition(0, -_localTop - 50.0f);
	pStartText->AddComponent<Font>();
	pStartText->GetComponent<Font>()->SetTextAlignment(eTextAlignment::Center);
	pStartText->GetComponent<Font>()->SetRenderPriority((int)eRenderOrder::FrontUI);
	pStartText->GetComponent<Font>()->Print(L"戦闘開始");

	for (int i = 0; i < OWN_CHARACTOR_NUM; i++) {
		pPlayerCharaText[i] = gameObject->CreateObject(0, 0, 0, transform);
		pEnemyCharaText[i] = gameObject->CreateObject(0, 0, 0, transform);
		pPlayerCharaText[i]->AddComponent<Font>();
		pEnemyCharaText[i]->AddComponent<Font>();
		pPlayerCharaText[i]->GetComponent<Font>()->SetRenderPriority((int)eRenderOrder::FrontUI);
		pEnemyCharaText[i]->GetComponent<Font>()->SetRenderPriority((int)eRenderOrder::FrontUI);
	}

	//セレクトカーソル
	pSelectCursor = gameObject->CreateImageObject(0, 0, 100, 30, CreateSprite(
		new Sprite(L"Data/Image/Common/square.spr")), transform);
	pSelectCursor->GetComponent<ImageRenderer>()->SetRenderPriority((int)eRenderOrder::FrontObject);

	//初期状態で隠しておく
	gameObject->SetObjEnable(false);
}
void BattlePanel::Update() {
	if (isSwitchingScene) {
		if (SceneManager::GetScene((int)eSceneTable::Battle)->isInitialized) {
			//シーン遷移
			SceneManager::SwitchScene(eSceneTable::Battle);
		}
		return;
	}

	if (cancelEnable && Input::Trg(InputConfig::input["cancel"])) {
		pSoundManager->Play("cancel");
		Close();
		gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->SetTurnState(eTurnState::Back);
	}

	if (Input::Trg(InputConfig::input["decide"])) {
		//互いにキャラが存在するか確認
		if (CheckNoChara()) return;
		//音
		pSoundManager->Play("decide");
		if(cancelEnable) pPlayerCity->IncActCount(); //行動した回数を増加させる

		//バトルシーンを用意
		SceneManager::CreateReserveScene(eSceneTable::Battle, false);

		nsBattle::SceneBattle* _nextScene = dynamic_cast<nsBattle::SceneBattle*>(SceneManager::GetScene((int)eSceneTable::Battle));
		
		//次のシーンへキャラクター情報を渡す
		for (auto& chara : pPlayerCity->vOwnChara) {
			Charactor* _pCharactor = new Charactor(*chara.get());
			_nextScene->vPlayerCharaBace.emplace_back(noDel_ptr<Charactor>(_pCharactor));
		}
		for (auto& chara : pEnemyCity->vOwnChara) {
			Charactor* _pCharactor = new Charactor(*chara.get());
			_nextScene->vEnemyCharaBace.emplace_back(noDel_ptr<Charactor>(_pCharactor));
		}

		//セーブ
		gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->
			SaveBattleInfo(pPlayerCity->GetID(), pEnemyCity->GetID());

		//次のシーンの初期化
		SceneManager::InitializeReserveScene();

		isSwitchingScene = true;
	}
}

void BattlePanel::Open(noDel_ptr<City> p_city, noDel_ptr<City> e_city, bool cancel) {
	pPlayerCity = p_city;
	pEnemyCity = e_city;

	cancelEnable = cancel;

	//操作説明テキスト変更
	noDel_ptr<Operation> _opr = gameObject->FindGameObject("operation")->GetComponent<Operation>();
	_opr->ResetOperation();
	_opr->AddOperation("decide", L"選択");
	_opr->AddOperation("cancel", L"戻る");

	Panel::Open();

	pSelectCursor->transform->SetPosition(pStartText->transform->position.x, pStartText->transform->position.y + 12.0f);

	//テキスト
	float _textPadding = 30.0f;
	float _localTop = -(gameObject->GetComponent<ImageRenderer>()->sizeY / 2);
	float _localLeft = -(gameObject->GetComponent<ImageRenderer>()->sizeX / 2);
	//名前
	pPlayerCityNameText->GetComponent<Font>()->Print(L"%s", pPlayerCity->GetName().c_str());
	pPlayerCityNameText->transform->SetLocalPosition(_localLeft + 30.0f, _localTop + 30.0f);
	pEnemyCityNameText->GetComponent<Font>()->Print(L"%s", pEnemyCity->GetName().c_str());
	pEnemyCityNameText->transform->SetLocalPosition(30.0f, _localTop + 30.0f);

	//キャラクター名
	for (int i = 0; i < OWN_CHARACTOR_NUM; i++) {
		if (i < pPlayerCity->vOwnChara.size())
			pPlayerCharaText[i]->GetComponent<Font>()->Print(L"%s", pPlayerCity->vOwnChara[i]->GetName().c_str());
		else
			pPlayerCharaText[i]->GetComponent<Font>()->Print(L"");

		if (i < pEnemyCity->vOwnChara.size())
			pEnemyCharaText[i]->GetComponent<Font>()->Print(L"%s", pEnemyCity->vOwnChara[i]->GetName().c_str());
		else
			pEnemyCharaText[i]->GetComponent<Font>()->Print(L"");

		pPlayerCharaText[i]->transform->SetLocalPosition(_localLeft + 30.0f, _localTop + 80.0f + (_textPadding * i));
		pEnemyCharaText[i]->transform->SetLocalPosition(30.0f, _localTop + 80.0f + (_textPadding * i));
	}
}

void BattlePanel::Close() {
	Panel::Close();
}

bool BattlePanel::CheckNoChara() {
	bool _flag = false;

	if (pPlayerCity->vOwnChara.size() == 0) {
		pPlayerCity->ChangeBelongCountry(pEnemyCity->pCountry);
		Close();
		return true;
	}
	if (pEnemyCity->vOwnChara.size() == 0) {
		pEnemyCity->ChangeBelongCountry(pPlayerCity->pCountry);
		Close();
		return true;
	}

	return false;
}

