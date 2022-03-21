#include "../../../../../../framework.h"
#include "../../../../../../environment.h"

using namespace nsStrategy;

void CityPanel::Awake() {
	//�T�E���h�}�l�[�W���[
	pSoundManager = gameObject->FindGameObject("soundManager")->GetComponent<SoundManager>();

	//�}�b�v�J�[�\��
	pMapCursor = gameObject->FindGameObject("cursor");

	//�X�v���C�g���擾
	noDel_ptr<ImageRenderer> spr = gameObject->GetComponent<ImageRenderer>();

	//�J�ڐ�p�l��
	//�����p�l��
	noDel_ptr<GameObject> _pInvestPanel = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER, 400, 150, 
		spr->pRenderSprite,	nullptr, "investPanel");
	_pInvestPanel->AddComponent<InvestPanel>();
	_pInvestPanel->SetRenderOrder((int)eRenderOrder::UI);
	//�o�g���p�l��
	noDel_ptr<GameObject> _pBattlePanel = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER, 400, 300, spr->pRenderSprite,
		nullptr, "battlePanel");
	_pBattlePanel->AddComponent<BattlePanel>();
	_pBattlePanel->SetRenderOrder((int)eRenderOrder::UI);
	//�L�����I���p�l��
	noDel_ptr<GameObject> _pCharaSelectPanel = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER, 300, 300,
		spr->pRenderSprite, nullptr, "charaSelectPanel");
	_pCharaSelectPanel->AddComponent<CharaSelectPanel>();
	_pCharaSelectPanel->SetRenderOrder((int)eRenderOrder::UI);
	//�ߗבI���p�l��
	noDel_ptr<GameObject> _pSelectNeighborPanel = gameObject->CreateImageObject(200, 70, 350, 100, spr->pRenderSprite,
		nullptr, "selectNeighborPanel");
	_pSelectNeighborPanel->AddComponent<SelectNeighborPanel>();
	_pSelectNeighborPanel->SetRenderOrder((int)eRenderOrder::UI);
	//�L���������p�l��
	noDel_ptr<GameObject> _pCharaEnhancePanel = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER,
		600, 500, spr->pRenderSprite,nullptr, "charaEnhancePanel");
	_pCharaEnhancePanel->AddComponent<CharaEnhancePanel>();
	_pCharaEnhancePanel->SetRenderOrder((int)eRenderOrder::UI);
	//���\���p�l��
	noDel_ptr<GameObject> _pInfoPanel = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER,
		300, 150, spr->pRenderSprite, nullptr, "infoPanel");
	_pInfoPanel->AddComponent<InfoPanel>();
	_pInfoPanel->SetRenderOrder((int)eRenderOrder::UI);
	//�I�v�V�������j���[
	noDel_ptr<GameObject> _pOptionMenu = gameObject->CreateObject(0,0,0, nullptr, "optionMenu");
	_pOptionMenu->AddComponent<OptionMenu>();
	//�I�v�V�����p�l��
	noDel_ptr<GameObject> _pOptionPanel = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER,
		300, 150, spr->pRenderSprite, nullptr, "optionPanel");
	_pOptionPanel->AddComponent<OptionPanel>();
	_pOptionPanel->SetRenderOrder((int)eRenderOrder::UI);
	//�C�x���g�p�l��
	noDel_ptr<GameObject> _pEventPanel = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER,
		700, 600, spr->pRenderSprite, nullptr, "eventPanel");
	_pEventPanel->AddComponent<EventPanel>();
	_pEventPanel->SetRenderOrder((int)eRenderOrder::UI);

	//�X���e�L�X�g
	pCityInfoPanel = gameObject->CreateObject(0, 0, 0, transform);

	pCityNameText = gameObject->CreateObject(0, 0, 0, transform);
	pCityNameText->AddComponent<Font>();
	pCityNameText->SetRenderOrder((int)eRenderOrder::FrontUI);

	pCityStatusText = gameObject->CreateObject(0, 0, 0, pCityInfoPanel->transform);
	pCityStatusText->AddComponent<Font>();
	pCityStatusText->SetRenderOrder((int)eRenderOrder::FrontUI);

	//�L�����N�^�[�ꗗ�\���e�L�X�g
	pCharaInfoPanel = gameObject->CreateObject(0, 0, 0, transform);
	for (int i = 0; i < OWN_CHARACTOR_NUM; i++) {
		pCharaInfoText[i] = gameObject->CreateObject(0, 0, 0, pCharaInfoPanel->transform);
		pCharaInfoText[i]->AddComponent<Font>();
		pCharaInfoText[i]->SetRenderOrder((int)eRenderOrder::FrontUI);

		pAttrObj[i] = gameObject->CreateObject(0, 0, 0, pCharaInfoText[i]->transform);
		pAttrObj[i]->AddComponent<ImageRenderer>();
		pAttrObj[i]->SetRenderOrder((int)eRenderOrder::FrontObject);
	}

	//�s���ꗗ�e�L�X�g
	pCommandPanel = gameObject->CreateObject(0, 0, 0, transform);

	//�Z���N�g�J�[�\��
	pSelectCursor = gameObject->CreateImageObject(0, 0, 100, 30, CreateSprite(
		new Sprite(L"Data/Image/Common/square.spr")),pCommandPanel->transform);
	pSelectCursor->SetRenderOrder((int)eRenderOrder::FrontObject);

	//�{�[�_�[
	for (int i = 0; i < 2; i++) {
		pBorderText[i] = gameObject->CreateObject(0, 0, 0, pCommandPanel->transform);
		pBorderText[i]->AddComponent<Font>();
		pBorderText[i]->SetRenderOrder((int)eRenderOrder::FrontUI);
		pBorderText[i]->GetComponent<Font>()->Print(L"---------------------------------------------------------------------\n");
	}

	//�R�}���h���R�}���h�����e�L�X�g
	pCommandTitleText = gameObject->CreateObject(0, 0, 0, pCommandPanel->transform);
	pCommandTitleText->AddComponent<Font>();
	pCommandTitleText->SetRenderOrder((int)eRenderOrder::FrontUI);
	pCommandTitleText->GetComponent<Font>()->Print(L"�E�R�}���h");

	for (int i = 0; i < (int)eCommand::Max; i++) {
		pCommand[i] = gameObject->CreateObject(0, 0, 0, pCommandPanel->transform);
		pCommandTexts[i] = gameObject->CreateObject(0, 0, 0, pCommandPanel->transform);
		pCommand[i]->AddComponent<Font>();
		pCommandTexts[i]->AddComponent<Font>();
		pCommand[i]->SetRenderOrder((int)eRenderOrder::FrontUI);
		pCommandTexts[i]->SetRenderOrder((int)eRenderOrder::FrontUI);
	}

	pCommand[(int)eCommand::Economic]->GetComponent<Font>()->Print(L"���Y�͌���");
	pCommandTexts[(int)eCommand::Economic]->GetComponent<Font>()->Print(L"�o�ϗ͂��㏸�B4��/10���̎����z�ɉe��");
	pCommand[(int)eCommand::Political]->GetComponent<Font>()->Print(L"����");
	pCommandTexts[(int)eCommand::Political]->GetComponent<Font>()->Print(L"�����͂��㏸�B�R�}���h���s������ɉe��");
	pCommand[(int)eCommand::Disaster]->GetComponent<Font>()->Print(L"�ЊQ�΍�");
	pCommandTexts[(int)eCommand::Disaster]->GetComponent<Font>()->Print(L"�ЊQ�ϐ����㏸�B�ЊQ���̊e�X�e�[�^�X�����l�ɉe��");
	pCommand[(int)eCommand::Support]->GetComponent<Font>()->Print(L"����");
	pCommandTexts[(int)eCommand::Support]->GetComponent<Font>()->Print(L"�x�����㏸�B�\�����̊e�X�e�[�^�X�����l�ɉe��");
	pCommand[(int)eCommand::MoneyMove]->GetComponent<Font>()->Print(L"�����ړ�");
	pCommandTexts[(int)eCommand::MoneyMove]->GetComponent<Font>()->Print(L"�ߐڂ��Ă��鎩���A�܂��͊X�Ɏ������ړ�����");
	pCommand[(int)eCommand::CharaMove]->GetComponent<Font>()->Print(L"�L�����h��");
	pCommandTexts[(int)eCommand::CharaMove]->GetComponent<Font>()->Print(L"�ߐڂ��Ă��鎩���A�܂��͊X�֎w�肵���L�����N�^�[���ړ�������");
	pCommand[(int)eCommand::Battle]->GetComponent<Font>()->Print(L"�퓬");
	pCommandTexts[(int)eCommand::Battle]->GetComponent<Font>()->Print(L"���̍��A�܂��͊X�֐퓬���d�|����");
	pCommand[(int)eCommand::CharaEnhance]->GetComponent<Font>()->Print(L"�L��������");
	pCommandTexts[(int)eCommand::CharaEnhance]->GetComponent<Font>()->Print(L"���̍��A�X�ɂ���L�����N�^�[����������");

	//������ԂŉB���Ă���
	gameObject->SetObjEnable(false);
}

void CityPanel::Update() {
	if (Input::Trg(InputConfig::input["cancel"])) {
		pSoundManager->Play("cancel"); //cancel��
		Close();
		gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->SetTurnState(eTurnState::Field);
		pMapCursor->SetObjEnable(true);
	}
	if (Input::Trg(InputConfig::input["changeTab"])) {
		pCityInfoPanel->SetObjEnable(pCityInfoPanel->IsObjEnable() ? false : true);
		pCharaInfoPanel->SetObjEnable(pCharaInfoPanel->IsObjEnable() ? false : true);
	}

	if (isPlayerCity) {
		PlayerCityFunc();
	}
}

void CityPanel::Close() {
	Panel::Close();
}

void CityPanel::Open(noDel_ptr<City> city) {
	pMapCursor->SetObjEnable(false);

	pCity = city;

	//��������e�L�X�g�ύX
	noDel_ptr<Operation> _opr = gameObject->FindGameObject("operation")->GetComponent<Operation>();
	_opr->ResetOperation();
	_opr->AddOperation("decide", L"�I��");
	_opr->AddOperation("cancel", L"�߂�");
	_opr->AddOperation("changeTab", L"�\���ؑ�");

	noDel_ptr<ImageRenderer> spr = gameObject->GetComponent<ImageRenderer>();

	if (pCity->pCountry->IsPlayer()) {
		spr->size.y = 450.0f;
		isPlayerCity = true;
	}
	else {
		spr->size.y = 180.0f;
		isPlayerCity = false;
	}

	float _top = transform->position.y - spr->size.y / 2;
	float _left = transform->position.x - spr->size.x / 2;

	SetCityStatus(_top, _left);

	Panel::Open();
}

void CityPanel::PlayerCityFunc() {
	MoveSelectCursor();
	TransOtherState();
}

void CityPanel::SetCityStatus(float top, float left) {
	float _leftPadding = 40.0f;
	float _topPadding = isPlayerCity ? 50.0f : 30.0f;

	//�e�L�X�g���
	float _textPadding = 40.0f;
	//�X�̖��O
	pCityNameText->transform->SetPosition(left + _leftPadding, top + _topPadding);
	noDel_ptr<Font> _cityName_font = pCityNameText->GetComponent<Font>();
	_cityName_font->Print(L"%s : %s", pCity->pCountry->GetName().c_str(), pCity->GetName().c_str());
	_cityName_font->SetColor(ColorConvert16(pCity->gameObject->GetComponent<ImageRenderer>()->GetColor()));

	//�X�̃X�e�[�^�X
	pCityStatusText->transform->SetPosition(left + _leftPadding, top + _topPadding + _textPadding);
	pCityStatusText->GetComponent<Font>()->Print(L"���L�z: %d  �o�ϗ�: %d  ������: %d\n"
		"�ЊQ�ϐ�: %d%%  �x����: %d%%  �i�C: %s", pCity->money, pCity->economic, pCity->political,
		pCity->disaster, pCity->support, pCity->GetBussinessText().c_str());

	//�����L�����̕\��
	const int _rowNum = 2;
	for (int i = 0; i < OWN_CHARACTOR_NUM; i++) {
		//�����L���������Ȃ���Ε\�������Ȃ�
		if (i >= pCity->vOwnChara.size()) {
			pCharaInfoText[i]->SetObjEnable(false);
			pAttrObj[i]->SetObjEnable(false);
			continue;
		}

		pCharaInfoText[i]->SetObjEnable(true);
		pAttrObj[i]->SetObjEnable(true);
		_textPadding = 30.0f;
		const float _x = left + 30.0f + (_leftPadding + (i % _rowNum) * 300.0f);
		const float _y = top + (_topPadding + ((i / _rowNum) + 1) * _textPadding);
		pCharaInfoText[i]->transform->SetPosition(_x, _y);
		pAttrObj[i]->transform->SetLocalPosition(-20.0f, 12.0f);
		pAttrObj[i]->GetComponent<ImageRenderer>()->SetUpImageRenderer(25.0f,25.0f,
			noDel_ptr<Sprite>(pCity->vOwnChara[i]->GetAttr()->GetSprite()));
		pCharaInfoText[i]->GetComponent<Font>()->Print(L"%s �퓬��:%d", pCity->vOwnChara[i]->GetName().c_str(),
			pCity->vOwnChara[i]->power);
	}

	pCityInfoPanel->SetObjEnable(true);
	pCharaInfoPanel->SetObjEnable(false);

	//�v���C���[�̏��L�n�Ȃ�R�}���h��\������
	if (pCity->vOwnChara.size() > _rowNum * 2) top = pCharaInfoText[_rowNum*2]->transform->position.y;
	else top = pCityStatusText->transform->position.y + 20.0f;

	if (isPlayerCity) SetCommandPanel(top, left);
	else pCommandPanel->SetObjEnable(false);
}

void CityPanel::SetCommandPanel(float top, float left) {
	float _leftPadding = 40.0f;
	float _topPadding = 40.0f;

	pBorderText[0]->transform->SetPosition(left + _leftPadding, top + _topPadding);
	pCommandTitleText->transform->SetPosition(left + _leftPadding, top + _topPadding + 30.0f);
	pCommandTitleText->GetComponent<Font>()->Print(L"�E�R�}���h  �c�s����: %d/%d", 
		pCity->GetActEnableNum() - pCity->GetActCount(), pCity->GetActEnableNum());

	_topPadding = 60.0f;

	//�s���\���ǂ���
	bool _actEnable = true;
	if (pCity->GetActCount() >= pCity->GetActEnableNum()) _actEnable = false;

	//�R�}���h
	const int _rowNum = 4;
	const float _textPadding = 50.0f;
	for (int i = 0; i < sizeof(pCommand) / sizeof(pCommand[0]); i++) {
		const float _x = left + (_leftPadding + (i % _rowNum) * 150.0f);
		const float _y = top + (_topPadding + ((i / _rowNum) + 1) * _textPadding);
		pCommand[i]->transform->SetPosition(_x, _y);
		if (_actEnable) pCommand[i]->GetComponent<Font>()->SetColor(0xffffffff);
		else pCommand[i]->GetComponent<Font>()->SetColor(0x22ffffff);
	}

	//�Z���N�g�J�[�\��
	cursorNum = 0;
	pSelectCursor->transform->SetPosition(pCommand[(int)eCommand::Economic]->transform->position.x + 45.0f,
		pCommand[(int)eCommand::Economic]->transform->position.y + 12.5f);

	//�R�}���h�e�L�X�g
 	pBorderText[1]->transform->SetPosition(left + _leftPadding, 
		pCommand[_rowNum]->transform->position.y + _textPadding);
	for (int i = 0; i < sizeof(pCommand) / sizeof(pCommand[0]); i++) {
		if(i == (int)eCommand::Economic) pCommandTexts[i]->SetObjEnable(true);
		else pCommandTexts[i]->SetObjEnable(false);
		pCommandTexts[i]->transform->SetPosition(left + _leftPadding, pBorderText[1]->transform->position.y + 40.0f);
	}

	pCommandPanel->SetObjEnable(true);
}

//�Z���N�g�J�[�\���ړ�
void CityPanel::MoveSelectCursor() {
	//�R�}���h�̑I��
	const int _col = 2; //�R�}���h�̍s��
	const int _row = 4; //�R�}���h�̗�

	int _cur = cursorNum;

	if (Input::Trg(InputConfig::input["down"])) if (cursorNum + _row < (int)eCommand::Max) cursorNum += _row;
	if (Input::Trg(InputConfig::input["up"])) if (cursorNum - _row >= 0) cursorNum -= _row;
	if (Input::Trg(InputConfig::input["right"])) cursorNum = CountUpInRange(cursorNum, (int)eCommand::Max - 1);
	if (Input::Trg(InputConfig::input["left"]))	cursorNum = CountDownInRange(cursorNum, (int)eCommand::Max - 1);

	//�ύX������΁�
	if (_cur != cursorNum) {
		//�Z���N�g�J�[�\���ʒu�ύX
		pSelectCursor->transform->SetPosition(pCommand[cursorNum]->transform->position.x + 45.0f,
			pCommand[cursorNum]->transform->position.y + 12.5f);
		//�e�L�X�g�؂�ւ�
		pCommandTexts[_cur]->SetObjEnable(false);
		pCommandTexts[cursorNum]->SetObjEnable(true);
	}
}

//�^�[����ԑJ��
void CityPanel::TransOtherState() {
	if (pCity->GetActCount() >= pCity->GetActEnableNum()) return;
	if (Input::Trg(InputConfig::input["decide"])) {
		pSoundManager->Play("decide"); //���艹
		//�J��
		noDel_ptr<GameManager> _pGM = gameObject->FindGameObject("gameManager")->GetComponent<GameManager>();
		if (cursorNum == (int)eCommand::MoneyMove) _pGM->SetTurnState(eTurnState::SelMoneyMoveToCity);
		else if (cursorNum == (int)eCommand::CharaMove) _pGM->SetTurnState(eTurnState::SelCharaMoveToCity);
		else if (cursorNum == (int)eCommand::Battle) _pGM->SetTurnState(eTurnState::SelBattleCity);
		else if (cursorNum == (int)eCommand::CharaEnhance) _pGM->SetTurnState(eTurnState::CharaEnhance);
		else if (cursorNum == (int)eCommand::Economic) _pGM->SetTurnState(eTurnState::InvEconomic);
		else if (cursorNum == (int)eCommand::Political) _pGM->SetTurnState(eTurnState::InvPolitical);
		else if (cursorNum == (int)eCommand::Disaster) _pGM->SetTurnState(eTurnState::InvDisaster);
		else if (cursorNum == (int)eCommand::Support) _pGM->SetTurnState(eTurnState::InvSupport);
		Close();
	}
}