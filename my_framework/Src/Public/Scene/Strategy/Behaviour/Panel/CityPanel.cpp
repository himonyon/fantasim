#include "../../../../../../framework.h"
#include "../../../../../../environment.h"

#include "CityPanel.h"

using namespace nsStrategy;

void CityPanel::Awake() {
	//�}�b�v�J�[�\��
	pMapCursor = gameObject->FindGameObject("cursor");

	//�X�v���C�g���擾
	noDel_ptr<ImageRenderer> spr = gameObject->GetComponent<ImageRenderer>();

	//�J�ڐ�p�l��
	pInvestPanel = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER, 400, 150, spr->pRenderSprite,
		nullptr, "investPanel");
	pInvestPanel->AddComponent<InvestPanel>();
	pInvestPanel->GetComponent<ImageRenderer>()->SetRenderPriority((int)eRenderOrder::UI);

	pBattlePanel = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER, 400, 300, spr->pRenderSprite,
		nullptr, "battlePanel");
	pBattlePanel->AddComponent<BattlePanel>();
	pBattlePanel->GetComponent<ImageRenderer>()->SetRenderPriority((int)eRenderOrder::UI);

	pCharaSelectPanel = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER, 300, 300,
		spr->pRenderSprite, nullptr, "charaSelectPanel");
	pCharaSelectPanel->AddComponent<CharaSelectPanel>();
	pCharaSelectPanel->GetComponent<ImageRenderer>()->SetRenderPriority((int)eRenderOrder::UI);

	pSelectNeighborPanel = gameObject->CreateImageObject(200, 70, 350, 100, spr->pRenderSprite);
	pSelectNeighborPanel->AddComponent<SelectNeighborPanel>();
	pSelectNeighborPanel->GetComponent<ImageRenderer>()->SetRenderPriority((int)eRenderOrder::UI);

	//�X���e�L�X�g
	pCityInfoPanel = gameObject->CreateObject(0, 0, 0, transform);

	pCityNameText = gameObject->CreateObject(0, 0, 0, transform);
	pCityNameText->AddComponent<Font>();
	pCityNameText->GetComponent<Font>()->SetRenderPriority((int)eRenderOrder::FrontUI);

	pCityStatusText = gameObject->CreateObject(0, 0, 0, pCityInfoPanel->transform);
	pCityStatusText->AddComponent<Font>();
	pCityStatusText->GetComponent<Font>()->SetRenderPriority((int)eRenderOrder::FrontUI);

	//�L�����N�^�[�ꗗ�\���e�L�X�g
	pCharaInfoPanel = gameObject->CreateObject(0, 0, 0, transform);
	for (int i = 0; i < OWN_CHARACTOR_NUM; i++) {
		pCharaInfoText[i] = gameObject->CreateObject(0, 0, 0, pCharaInfoPanel->transform);
		pCharaInfoText[i]->AddComponent<Font>();
		pCharaInfoText[i]->GetComponent<Font>()->SetRenderPriority((int)eRenderOrder::FrontUI);

		pAttrObj[i] = gameObject->CreateObject(0, 0, 0, pCharaInfoText[i]->transform);
		pAttrObj[i]->AddComponent<ImageRenderer>();
		pAttrObj[i]->GetComponent<ImageRenderer>()->SetRenderPriority((int)eRenderOrder::FrontObject);
	}

	//�s���ꗗ�e�L�X�g
	pCommandPanel = gameObject->CreateObject(0, 0, 0, transform);

	//�Z���N�g�J�[�\��
	pSelectCursor = gameObject->CreateImageObject(0, 0, 100, 30, CreateSprite(
		new Sprite(L"Data/Image/Common/square.spr")),pCommandPanel->transform);
	pSelectCursor->GetComponent<ImageRenderer>()->SetRenderPriority((int)eRenderOrder::FrontObject);

	//�{�[�_�[
	for (int i = 0; i < 2; i++) {
		pBorderText[i] = gameObject->CreateObject(0, 0, 0, pCommandPanel->transform);
		pBorderText[i]->AddComponent<Font>();
		pBorderText[i]->GetComponent<Font>()->SetRenderPriority((int)eRenderOrder::FrontUI);
		pBorderText[i]->GetComponent<Font>()->Print(L"---------------------------------------------------------------------\n");
	}

	//�R�}���h���R�}���h�����e�L�X�g
	pCommandTitleText = gameObject->CreateObject(0, 0, 0, pCommandPanel->transform);
	pCommandTitleText->AddComponent<Font>();
	pCommandTitleText->GetComponent<Font>()->SetRenderPriority((int)eRenderOrder::FrontUI);
	pCommandTitleText->GetComponent<Font>()->Print(L"�E�R�}���h");

	for (int i = 0; i < (int)eCommand::Max; i++) {
		pCommand[i] = gameObject->CreateObject(0, 0, 0, pCommandPanel->transform);
		pCommandTexts[i] = gameObject->CreateObject(0, 0, 0, pCommandPanel->transform);
		pCommand[i]->AddComponent<Font>();
		pCommandTexts[i]->AddComponent<Font>();
		pCommand[i]->GetComponent<Font>()->SetRenderPriority((int)eRenderOrder::FrontUI);
		pCommandTexts[i]->GetComponent<Font>()->SetRenderPriority((int)eRenderOrder::FrontUI);
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
	if (Input::GetDX(InputConfig::input["moveX"])) {
		transform->position.x += Input::GetDX(InputConfig::input["moveX"]);
	}

	if (Input::Trg(InputConfig::input["cancel"])) {
		Close();
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

	noDel_ptr<ImageRenderer> spr = gameObject->GetComponent<ImageRenderer>();

	if (pCity->pCountry->IsPlayer()) {
		spr->sizeY = 450.0f;
		isPlayerCity = true;
	}
	else {
		spr->sizeY = 180.0f;
		isPlayerCity = false;
	}

	float _top = transform->position.y - spr->sizeY / 2;
	float _left = transform->position.x - spr->sizeX / 2;

	SetCityStatus(_top, _left);

	Panel::Open();
}

void CityPanel::PlayerCityFunc() {
	MoveSelectCursor();
	TransOtherPanel();
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
		pAttrObj[i]->GetComponent<ImageRenderer>()->SetUpRenderer2D(25.0f,25.0f,
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

	_topPadding = 60.0f;

	//�R�}���h
	const int _rowNum = 4;
	const float _textPadding = 50.0f;
	for (int i = 0; i < sizeof(pCommand) / sizeof(pCommand[0]); i++) {
		const float _x = left + (_leftPadding + (i % _rowNum) * 150.0f);
		const float _y = top + (_topPadding + ((i / _rowNum) + 1) * _textPadding);
		pCommand[i]->transform->SetPosition(_x, _y);
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

//���̃p�l���֑J��
void CityPanel::TransOtherPanel() {
	if (Input::Trg(InputConfig::input["decide"])) {
		Close();
		if (cursorNum == (int)eCommand::MoneyMove || cursorNum == (int)eCommand::CharaMove ||
			cursorNum == (int)eCommand::Battle)
			pSelectNeighborPanel->GetComponent<SelectNeighborPanel>()->Open(pCity, cursorNum);
		else pInvestPanel->GetComponent<InvestPanel>()->Open(pCity, cursorNum);
	}
}