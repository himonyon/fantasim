#include "../../../../../framework.h"
#include "../../../../../environment.h"

using namespace nsStrategy;

GameManager::~GameManager() {
	//�L�����N�^�[�폜
	for (auto& chara : umCharactors) {
		delete chara.second;
	}
	umCharactors.clear();
}

void GameManager::Awake() {
	//�Z�[�u�f�[�^�����邩�m�F--------------------
	bool _saveData = false;
	if (LoadBaceDate()) _saveData = true;
	//-------------------------------------------
	
	//�N���e�L�X�g
	noDel_ptr<GameObject> _pDateObj = gameObject->CreateObject(50, 50, 0);
	_pDateObj->AddComponent<Font>();
	pDateText = _pDateObj->GetComponent<Font>();
	pDateText->gameObject->SetRenderOrder((int)eRenderOrder::UI - 1);
	pDateText->SetFontSize(20.0f);
	pDateText->Print(L"%d�N %d��", year, month);
	
	//�T�E���h
	CreateSound();

	//����������
	noDel_ptr<GameObject> _pOprObj = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT - 25, SCREEN_WIDTH, 50.0f,
		CreateSprite(new Sprite(L"Data/Image/Common/cover.spr")), nullptr, "operation");
	_pOprObj->SetRenderOrder((int)eRenderOrder::FrontUI);
	_pOprObj->GetComponent<ImageRenderer>()->SetColor(1,1,1,0.5f);
	_pOprObj->AddComponent<Operation>();
	pOperation = _pOprObj->GetComponent<Operation>();

	//�J�[�\��
	pCursor = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER, 32.0f, 32.0f,
		CreateSprite(new Sprite(L"Data/Image/Strategy/cursor.spr")), nullptr, "cursor");
	pCursor->SetRenderOrder((int)eRenderOrder::Object + 2);
	pCursor->AddComponent<Physics2D>();
	pCursor->AddComponent<Collider2D>();
	pCursor->GetComponent<Collider2D>()->SetUpCollider2D(false);
	pCursor->AddComponent<Cursor>();
	//��\��
	pCursor->SetObjEnable(false);

	//�L�����A�̒n�쐬
	CreateCharactor(_saveData);
	CreateTerritory();

	//�f�o�b�O�ݒ�---------------------------------------------------------------------------
	Debug::SetStrategy(pCities);
	Debug::scene = (int) eSceneTable::Strategy;

	//�Z�[�u�}�l�[�W���[�쐬
	saveManager = std::make_unique<Save>();
	for (int i = 0; i < TERRITORY_NUM; i++) saveManager->pCities[i] = pCities[i];
	saveManager->umCharactors = umCharactors;

	//�p�l���쐬�[�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|�|
	noDel_ptr<Sprite> panel_sp = CreateSprite(new Sprite(L"Data/Image/Common/menu_button.spr", L"panel"));
	noDel_ptr<GameObject> _pCityPanel = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER, 700, 480, panel_sp, nullptr, "cityPanel");
	_pCityPanel->SetRenderOrder((int)eRenderOrder::UI);
	_pCityPanel->AddComponent<CityPanel>();

	//�^�[���쐬
	noDel_ptr<GameObject> _pPlayerTemp = gameObject->CreateObject(0, 0, 0);
	_pPlayerTemp->AddComponent<PlayerTurn>();
	pPlayerTurn = _pPlayerTemp->GetComponent<PlayerTurn>();
	noDel_ptr<GameObject> _pEnemyTemp = gameObject->CreateObject(0, 0, 0);
	_pEnemyTemp->AddComponent<EnemyTurn>();
	pEnemyTurn = _pEnemyTemp->GetComponent<EnemyTurn>();
	
	//�Z�[�u�����p��----------------------------------------
	//�퓬���ʃf�[�^���[�h
	if (LoadBattleInfo()) _saveData = true;
	//�Z�[�u�f�[�^���Q�[���ĊJ
	if (_saveData) {
		GameStart();
		return;
	}
	//-------------------------------------------------------

	//�񏈗�
	pPlayerTurn->SetEnable(false);
	pEnemyTurn->SetEnable(false);

	//�Z�[�u�f�[�^���Ȃ���΁A���삷�鍑��I������p�l���\��
	pSoundManager->Play("bgm");
	noDel_ptr<GameObject> _pSelCountryPanel = gameObject->CreateImageObject(400, 200, 300, 280, panel_sp);
	_pSelCountryPanel->SetRenderOrder((int)eRenderOrder::UI);
	_pSelCountryPanel->AddComponent<SelectCountryPanel>();
	pSelectCountryPanel = _pSelCountryPanel->GetComponent<SelectCountryPanel>();
}

void GameManager::GameStart() {
	if(pSoundManager->IsPlaying("bgm") == false) pSoundManager->Play("bgm");
	pCursor->SetObjEnable(true);
	pEnemyTurn->SetEnable(false);
	pPlayerTurn->SetEnable(true);
	//�o�g���f�[�^���p����
	if (battleData.data == false) return;
	//�G�̃V�~�����[�g�̓r���̏ꍇ�A�ĊJ����
	if ( battleData.isPlayerTurn == false) {
		pPlayerTurn->SetEnable(false);
		pEnemyTurn->SetEnable(true);
		pEnemyTurn->TurnInit(battleData.pE_City->GetID());
	}
	//���ʂ��p�l���ɕ\��
	noDel_ptr<InfoPanel> _infoPanel = gameObject->FindGameObject("infoPanel")->GetComponent<InfoPanel>();
	if (_infoPanel->IsOpen()) return;
	if (battleData.isWin) _infoPanel->Open(L"�X���l�����܂����B");
	else _infoPanel->Open(L"�X��D���܂����B");

	//�o�g���f�[�^�폜
	battleData.data = false;
}

//�^�[���؂�ւ�
void GameManager::ChangeTurn() {
	//�s���񐔃��Z�b�g
	for (auto& city : pCities) {
		city->ResetActCount();
	}

	//�^�[���؂�ւ�
	if (pPlayerTurn->IsEnable()) {
		pPlayerTurn->SetEnable(false);
		pEnemyTurn->SetEnable(true);
		pEnemyTurn->TurnInit();
	}
	else {
		pEnemyTurn->SetEnable(false);
		pPlayerTurn->SetEnable(true);
		pPlayerTurn->TurnInit();
	}
}

//�Q�[���I��
void GameManager::EndGame(bool isClear) {
	if (pPlayerTurn->IsEnable()) pPlayerTurn->EndGame(isClear);
	else pEnemyTurn->EndGame(isClear);

	isGameEnd = true;
}
bool GameManager::IsEndGame() {
	return isGameEnd;
}
void GameManager::CheckClear() {
	bool _clear = false;
	for (int i = 0; i < COUNTRY_NUM; i++) {
		if (pCities[i]->pCountry == NULL) return;
		if (pCities[i]->pCountry->vOwnCities.size() >= TERRITORY_NUM - 1) {
			EndGame(true);
		}
	}
}

//�I�����ꂽ�X���^�[���ɓn��
void GameManager::SetFocusCity(noDel_ptr<City> city) {
	if (pPlayerTurn->IsEnable()) pPlayerTurn->SetFocusCity(city);
	else pPlayerTurn->SetFocusCity(city);
}
//�^�[�Q�b�g�ɂ���X�̃Z�b�g
void GameManager::SetTargetCity(noDel_ptr<City> city) {
	if (pPlayerTurn->IsEnable()) pPlayerTurn->SetTargetCity(city);
	else pPlayerTurn->SetTargetCity(city);
}

//�^�[����ԃZ�b�g
void GameManager::SetTurnState(eTurnState state, int backNum) {
	if (pPlayerTurn->IsEnable()) pPlayerTurn->SetTurnState(state, backNum);
	else pPlayerTurn->SetTurnState(state, backNum);
}

//���̎擾
noDel_ptr<City> GameManager::GetCity(int id) {
	for (auto& city : pCities) {
		if (city->GetID() == id) return city;
	}

	return NULL;
}

//�Z�[�u
void GameManager::SaveGame() {
	saveManager->SaveBaceData(year, month);
	saveManager->SaveCharactor();
	saveManager->SaveCities();
}
//�퓬����XID�̕ۑ�
void GameManager::SaveBattleInfo(int p_id, int e_id) {
	bool _playerTurn = false;
	if (pPlayerTurn->IsEnable()) _playerTurn = true;
	else _playerTurn = false;
	saveManager->SaveBattleCities(year, month, p_id, e_id, _playerTurn);
}

//�N���X�V
void GameManager::UpdateMonth() {
	month++; 
	if (month > 12) {
		year++;
		month = 1;
	}
	pDateText->Print(L"%d�N %d��", year, month);
}

//������
void GameManager::CreateCharactor(bool saveData) {
	std::string file;
	//�t�@�C���I��
	if (saveData) file = "Data/SaveData/chara.txt";
	else file = "Data/Status/Common/chara.txt";

	//�e�L�X�g�t�@�C����ID�ƈ�v����L�����̐���
	for (int i = 0; i < CHARACTOR_NUM; i++) {
		Charactor* chara = new Charactor(i, file);
		umCharactors[chara->GetID()] = chara;
	}
}

void GameManager::CreateTerritory() {
	//�w�i�̍쐬
	pBg = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER, SCREEN_WIDTH, SCREEN_HEIGHT,
		CreateSprite(new Sprite(L"Data/Image/Strategy/bg.spr")));
	pBg->SetRenderOrder((int)eRenderOrder::Background);

	//���̐F
	stColor4 _colors[COUNTRY_NUM] = {
		{0,0,0,1},
		{1,0.5f,1,1},
		{0,0.5f,1,1},
		{1,1,0.3f,1},
		{1,0,0,1},
		{1,0.6f,0,1},
	};

	//�S�̒n���ʂŗ��p����摜
	noDel_ptr<Sprite> pCountry_sp = CreateSprite(new Sprite(L"Data/Image/Strategy/country.spr"));
	noDel_ptr<Sprite> pCity_sp = CreateSprite(new Sprite(L"Data/Image/Strategy/circle_icon.spr"));
	noDel_ptr<Sprite> pCityFrame_sp = CreateSprite(new Sprite(L"Data/Image/Strategy/circle_frame.spr"));
	noDel_ptr<Sprite> pRoad_sp = CreateSprite(new Sprite(L"Data/Image/Strategy/road.spr"));

	//���ƊX�̃Q�[���I�u�W�F�N�g�̍쐬
	for (int i = 0; i < TERRITORY_NUM; i++) {
		std::string _name = "city" + std::to_string(i); //����
		noDel_ptr<GameObject> _pCityObj = gameObject->CreateObject(0, 0, 0, nullptr, _name);
		_pCityObj->AddComponent<ImageRenderer>();
		noDel_ptr<ImageRenderer> spr = _pCityObj->GetComponent<ImageRenderer>();
		spr->gameObject->SetRenderOrder((int)eRenderOrder::Object);
		if (i < COUNTRY_NUM) {
			spr->SetColor(_colors[i]);
			_pCityObj->AddComponent<Country>();
			pCities[i] = static_noDel_cast<City>(_pCityObj->GetComponent<Country>());
			dynamic_noDel_cast<Country>(pCities[i])->SetSprite(pCountry_sp, pCity_sp, pCityFrame_sp);
		}
		else {
			_pCityObj->AddComponent<City>();
			pCities[i] = _pCityObj->GetComponent<City>();
			pCities[i]->SetSprite(pCity_sp, pCityFrame_sp);
		}
	}

	//�Z�[�u�f�[�^�����邩�m�F
	FILE* fp;
	char _key[256] = { 0 };
	std::string _fileName = "Data/Status/Strategy/baseData.txt";
	fopen_s(&fp, "Data/SaveData/base.txt", "r");
	if (fp != NULL) {
		fscanf_s(fp, "%s", _key, (int)sizeof(_key));
		if (strcmp(_key, "save") == 0) _fileName = "Data/SaveData/city.txt";
	}
	
	//�t�@�C������e�X�̃f�[�^��ǂݍ���
	fopen_s(&fp, _fileName.c_str(), "r");
	if (fp == NULL) return;
	for (int i = 0; i < TERRITORY_NUM; i++) {
		//�X�̃R���|�[�l���g�擾
		noDel_ptr<City> city = pCities[i];
		//�ח��ς݂��m�F����g���K�[
		bool _isFallen = false;

		//�e�X�e�[�^�X�ݒ�
		while (true)
		{
			//�L�[���[�h�ǂݍ���
			fscanf_s(fp, "%s", _key, (int)sizeof(_key));

			if (strcmp(_key, "name") == 0) {
				fscanf_s(fp, "%s", _key, (int)sizeof(_key));
				WCHAR temp[32] = L"";
				//���O��ϊ����Ċi�[
				MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCCH)_key, _countof(_key), temp, 32);
				city->SetName(temp);
			}
			else if (strcmp(_key, "id") == 0) {
				int _id = 0;
				fscanf_s(fp, "%d", &_id);
				city->SetID(_id);
			}
			else if (strcmp(_key, "fallen") == 0) {
				_isFallen = true;
			}
			else if (strcmp(_key, "country") == 0) {
				int _country = 0;
				fscanf_s(fp, "%d", &_country);
				if (i < COUNTRY_NUM) {
					if(_isFallen)
						city->ChangeBelongCountry(dynamic_noDel_cast<Country>(pCities[_country]));
					else
						city->pCountry = dynamic_noDel_cast<Country>(pCities[_country]);
				}
				else city->ChangeBelongCountry(dynamic_noDel_cast<Country>(pCities[_country]));
			}
			else if (strcmp(_key, "position") == 0) {
				float posX = 0, posY = 0;
				fscanf_s(fp, "%f %f", &city->transform->position.x, &city->transform->position.y);
			}
			else if (strcmp(_key, "player") == 0) {
				if (i < COUNTRY_NUM) dynamic_noDel_cast<Country>(city)->SetIsPlayer();
			}
			else if (strcmp(_key, "neighbor") == 0) {
				while (true) {
					fscanf_s(fp, "%s", _key, (int)sizeof(_key));
					if (strcmp(_key, "money") == 0) {
						fscanf_s(fp, "%d", &city->money);
						break;
					}
					else city->SetNeighbor(pCities[atoi(_key)]);
				}
			}
			else if (strcmp(_key, "economic") == 0) {
				fscanf_s(fp, "%d", &city->economic);
			}
			else if (strcmp(_key, "political") == 0) {
				fscanf_s(fp, "%d", &city->political);
			}
			else if (strcmp(_key, "disaster") == 0) {
				fscanf_s(fp, "%d", &city->disaster);
			}
			else if (strcmp(_key, "support") == 0) {
				fscanf_s(fp, "%d", &city->support);
			}
			else if (strcmp(_key, "action") == 0) {
				fscanf_s(fp, "%d", &city->actionCount);
			}
			else if (strcmp(_key, "business") == 0) {
				fscanf_s(fp, "%f", &city->bussiness);
			}
			else if (strcmp(_key, "chara") == 0) {
				while (!feof(fp)) {
					fscanf_s(fp, "%s", _key, (int)sizeof(_key));
					if (strcmp(_key, "new") == 0) break;
					int _charaID = atoi(_key);
					city->vOwnChara.emplace_back(noDel_ptr<Charactor>(umCharactors[_charaID]));
				}
				break;
			}
		}
	}
	fclose(fp);

	//���̕\���ݒ�
	for (int i = 0; i < TERRITORY_NUM; i++) {
		noDel_ptr<City> city1 = pCities[i];
		for (int k = 0; k < city1->GetNeighbor().size(); k++) {
			noDel_ptr<City> city2 = city1->GetNeighbor()[k];
			if (city1->GetID() > city2->GetID()) continue;
			float _x = city1->transform->position.x; float _x2 = city2->transform->position.x;
			float _y = city1->transform->position.y; float _y2 = city2->transform->position.y;

			float _roadPosX = _x - (_x - _x2) / 2;
			float _roadPosY = _y - (_y - _y2) / 2;
			float _roadSizeX = sqrtf(powf(_x2 - _x, 2) + powf(_y2 - _y, 2));
			float _roadRot = atan2f(_y2 - _y, _x2 - _x);
			noDel_ptr<GameObject> road = gameObject->CreateImageObject(_roadPosX, _roadPosY, _roadSizeX, 12.0f, pRoad_sp);
			noDel_ptr<ImageRenderer> spr = road->GetComponent<ImageRenderer>();
			spr->gameObject->SetRenderOrder((int)eRenderOrder::BackObject);;
			spr->SetColor(0.6f, 0.3f, 0.0f, 0.8f);
			road->transform->rotation.z = _roadRot;
			vpRoad.emplace_back(road);
		}
	}
}

//���̍쐬
void GameManager::CreateSound() {
	noDel_ptr<GameObject> _soundObj = gameObject->CreateObject(0, 0, 0, nullptr, "soundManager");
	_soundObj->AddComponent<SoundManager>();
	pSoundManager = _soundObj->GetComponent<SoundManager>();
	//BGM
	pSoundManager->AddSound("bgm", L"Data/Sound/Strategy/bgm.wav");
	//���艹
	pSoundManager->AddSound("decide", L"Data/Sound/Common/circle.wav");
	//cancel��
	pSoundManager->AddSound("cancel", L"Data/Sound/Common/cross.wav");
	//������
	pSoundManager->AddSound("enhance", L"Data/Sound/Strategy/statusUp.wav");

	pSoundManager->SetVolume(0.1f);

	///���[�U�[��񔽉f
	if (UserSetting::sound == false) pSoundManager->SetVolume(0);
}

//���[�h
bool GameManager::LoadBattleInfo() {
	FILE* fp;
	bool _isWin = false;
	bool _isPlayerTurn = false;
	char _key[256] = { 0 };

	//�퓬���ʃf�[�^�����邩�m�F
	fopen_s(&fp, "Data/SaveData/result.txt", "r");
	//�t�@�C�����Ȃ����False��Ԃ�
	if (fp == NULL) return false;
	//���ʂ�ێ�
	fscanf_s(fp, "%s", _key, (int)sizeof(_key));
	if (strcmp(_key, "win") == 0) _isWin = true;
	fclose(fp);

	//�Z�[�u�f�[�^����퓬�����X��ǂݍ���
	noDel_ptr<City> _pPlayerCity, _pEnemyCity;
	int _p_id = 0, _e_id = 0; //�ۑ�����Ă������ꂼ���ID
	fp = NULL;
	fopen_s(&fp, "Data/SaveData/battleCity.txt", "r");
	if (fp == NULL) return false;
	while (!feof(fp)) {
		fscanf_s(fp, "%s", _key, (int)sizeof(_key));
		if (strcmp(_key, "turn") == 0) {
			fscanf_s(fp, "%s", _key, (int)sizeof(_key));
			if (strcmp(_key, "player") == 0) _isPlayerTurn = true;
			else if (strcmp(_key, "enemy") == 0) _isPlayerTurn = false;
		}
		if (strcmp(_key, "p_city") == 0) {
			fscanf_s(fp, "%s", _key, (int)sizeof(_key));
			_p_id = atoi(_key);
		}
		if (strcmp(_key, "e_city") == 0) {
			fscanf_s(fp, "%s", _key, (int)sizeof(_key));
			_e_id = atoi(_key);
		}
	}
	fclose(fp);

	//ID����X��T��
	for (int i = 0; i < TERRITORY_NUM; i++) {
		if (pCities[i]->GetID() == _p_id) _pPlayerCity = pCities[i];
		else if (pCities[i]->GetID() == _e_id) _pEnemyCity = pCities[i];

		if (_pPlayerCity != NULL && _pEnemyCity != NULL) break;
	}

	//���ʂ𔽉f
	if (_isWin) _pEnemyCity->ChangeBelongCountry(_pPlayerCity->pCountry);
	else _pPlayerCity->ChangeBelongCountry(_pEnemyCity->pCountry);

	//�o�g���f�[�^�쐬
	battleData.data = true;
	battleData.isPlayerTurn = _isPlayerTurn;
	battleData.isWin = _isWin;
	battleData.pP_City = _pPlayerCity;
	battleData.pE_City = _pEnemyCity;

	//�t�@�C���폜
	remove("Data/SaveData/result.txt");

	return true;
}

bool GameManager::LoadBaceDate() {
	bool _flag = false; //�Z�[�u�f�[�^�����邩
	char _key[256] = { 0 };
	FILE* fp = NULL;
	fopen_s(&fp, "Data/SaveData/base.txt", "r");
	if (fp == NULL) return false;
	while (!feof(fp)) {
		fscanf_s(fp, "%s", _key, (int)sizeof(_key));
		if (strcmp(_key, "save") == 0) _flag = true;
		if (strcmp(_key, "date") == 0) {
			fscanf_s(fp, "%d %d", &year, &month);
		}
	}
	if (fp != NULL) fclose(fp);
	if (_flag == false) {
		year = 1000;
		month = 1;
		return false;
	}
	return true;
}