#include "../../../../../../framework.h"
#include "../../../../../../environment.h"

using namespace nsStrategy;

void BattlePanel::Awake() {
	//�T�E���h�}�l�[�W���[
	pSoundManager = gameObject->FindGameObject("soundManager")->GetComponent<SoundManager>();

	//�e�L�X�g�쐬
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
	pStartText->GetComponent<Font>()->Print(L"�퓬�J�n");

	for (int i = 0; i < OWN_CHARACTOR_NUM; i++) {
		pPlayerCharaText[i] = gameObject->CreateObject(0, 0, 0, transform);
		pEnemyCharaText[i] = gameObject->CreateObject(0, 0, 0, transform);
		pPlayerCharaText[i]->AddComponent<Font>();
		pEnemyCharaText[i]->AddComponent<Font>();
		pPlayerCharaText[i]->GetComponent<Font>()->SetRenderPriority((int)eRenderOrder::FrontUI);
		pEnemyCharaText[i]->GetComponent<Font>()->SetRenderPriority((int)eRenderOrder::FrontUI);
	}

	//�Z���N�g�J�[�\��
	pSelectCursor = gameObject->CreateImageObject(0, 0, 100, 30, CreateSprite(
		new Sprite(L"Data/Image/Common/square.spr")), transform);
	pSelectCursor->GetComponent<ImageRenderer>()->SetRenderPriority((int)eRenderOrder::FrontObject);

	//������ԂŉB���Ă���
	gameObject->SetObjEnable(false);
}
void BattlePanel::Update() {
	if (isSwitchingScene) {
		if (SceneManager::GetScene((int)eSceneTable::Battle)->isInitialized) {
			//�V�[���J��
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
		//�݂��ɃL���������݂��邩�m�F
		if (CheckNoChara()) return;
		//��
		pSoundManager->Play("decide");
		if(cancelEnable) pPlayerCity->IncActCount(); //�s�������񐔂𑝉�������

		//�o�g���V�[����p��
		SceneManager::CreateReserveScene(eSceneTable::Battle, false);

		nsBattle::SceneBattle* _nextScene = dynamic_cast<nsBattle::SceneBattle*>(SceneManager::GetScene((int)eSceneTable::Battle));
		
		//���̃V�[���փL�����N�^�[����n��
		for (auto& chara : pPlayerCity->vOwnChara) {
			Charactor* _pCharactor = new Charactor(*chara.get());
			_nextScene->vPlayerCharaBace.emplace_back(noDel_ptr<Charactor>(_pCharactor));
		}
		for (auto& chara : pEnemyCity->vOwnChara) {
			Charactor* _pCharactor = new Charactor(*chara.get());
			_nextScene->vEnemyCharaBace.emplace_back(noDel_ptr<Charactor>(_pCharactor));
		}

		//�Z�[�u
		gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->
			SaveBattleInfo(pPlayerCity->GetID(), pEnemyCity->GetID());

		//���̃V�[���̏�����
		SceneManager::InitializeReserveScene();

		isSwitchingScene = true;
	}
}

void BattlePanel::Open(noDel_ptr<City> p_city, noDel_ptr<City> e_city, bool cancel) {
	pPlayerCity = p_city;
	pEnemyCity = e_city;

	cancelEnable = cancel;

	//��������e�L�X�g�ύX
	noDel_ptr<Operation> _opr = gameObject->FindGameObject("operation")->GetComponent<Operation>();
	_opr->ResetOperation();
	_opr->AddOperation("decide", L"�I��");
	_opr->AddOperation("cancel", L"�߂�");

	Panel::Open();

	pSelectCursor->transform->SetPosition(pStartText->transform->position.x, pStartText->transform->position.y + 12.0f);

	//�e�L�X�g
	float _textPadding = 30.0f;
	float _localTop = -(gameObject->GetComponent<ImageRenderer>()->sizeY / 2);
	float _localLeft = -(gameObject->GetComponent<ImageRenderer>()->sizeX / 2);
	//���O
	pPlayerCityNameText->GetComponent<Font>()->Print(L"%s", pPlayerCity->GetName().c_str());
	pPlayerCityNameText->transform->SetLocalPosition(_localLeft + 30.0f, _localTop + 30.0f);
	pEnemyCityNameText->GetComponent<Font>()->Print(L"%s", pEnemyCity->GetName().c_str());
	pEnemyCityNameText->transform->SetLocalPosition(30.0f, _localTop + 30.0f);

	//�L�����N�^�[��
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

