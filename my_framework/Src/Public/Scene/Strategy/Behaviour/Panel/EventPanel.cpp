#include "../../../../../../framework.h"
#include "../../../../../../environment.h"

using namespace nsStrategy;

void EventPanel::Awake() {
	//�T�E���h�}�l�[�W���[
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

	//�^�C�g���e�L�X�g�쐬
	CreateText(pTitleText, _textPosX, _textPosY);
	pTitleText->Print(L"EVENT");
	pTitleText->SetFontSize(20.0f);
	//�e�L�X�g
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
	//������
	for (auto& text : pTexts) {
		text.flag = false;
		text.pText->Print(L"");
	}

	//�C�x���g����
	Event();

	Panel::Open();
}
void EventPanel::Close() {
	Panel::Close();
}

//�󂢂Ă���e�L�X�g�ɏ�������
noDel_ptr<Font> EventPanel::GetEventText() {
	for (auto& text : pTexts) {
		if (text.flag) continue;
		text.flag = true;
		return text.pText;
	}
	return NULL;
}

//�C�x���g����
void EventPanel::Event() {
	//�N���X�V
	pGM->UpdateMonth();
	int _year = pGM->GetYear();
	int _month = pGM->GetMonth();
	pTitleText->Print(L"EVENT �@%d�N %d��", _year, _month);

	//�������e�L�X�g
	if (_month == 4 || _month == 10) {
		noDel_ptr<Font> _text = GetEventText();
		_text->Print(L"�e�X�Ɏ���������܂���");
	}

	//�C�x���g����
	EventCulc(_month);
}
//���^�[����������C�x���g(�x�����E�ЊQ�ϐ�����)�{����
void EventPanel::EventCulc(int month) {
	srand((unsigned int)time(NULL));
	int _rand = 0;
	int _curID = 0;
	while (true) {
		//�X�̎擾
		noDel_ptr<City> _city = pGM->GetCity(_curID);
		if (_city == NULL) break;

		//�ЊQ�ϐ������i���^�[�������_����0~3�j
		DecDisaster(_city);

		//�x���������i���^�[�������_����0~3�j
		DecSupport(_city);

		//��������
		if (month == 4 || month == 10) Income(_city);

		//�䕗
		Typhoon(_city);

		//�\��
		Riot(_city);

		//�X�V
		_curID++;
	}
}
//����
void EventPanel::Income(noDel_ptr<City> city) {
	//��������(�o�ϗ�*�i�C)
	city->money += (int)((float)city->economic * city->bussiness);
}
//�x��������
void EventPanel::DecSupport(noDel_ptr<City> city) {
	int _rand = rand() % 4;
	city->support -= _rand;
	if (city->support < 0) city->support = 0;
}
//�ЊQ�ϐ�����
void EventPanel::DecDisaster(noDel_ptr<City> city) {
	int _rand = rand() % 4;
	city->disaster -= _rand;
	if (city->disaster < 0) city->disaster = 0;
}
//�䕗
void EventPanel::Typhoon(noDel_ptr<City> city) {
	//3%�̊m��
	int _rand = rand() % 100;
	if (_rand >= 5) return;
	//�ЊQ�ϐ��ɉ����ăX�e�[�^�X����
	int _decMoney= 100;
	int _decVal = rand() % 500;
	//�P�O�����ƂɌ��������㏸
	_decMoney *= 10 - city->disaster / 10;
	_decVal *= 10 - city->disaster / 10;
	///���f
	city->money -= _decMoney;
	city->economic -= _decVal;
	if (city->money < 0) city->money = 0;
	if (city->economic < 0) city->economic = 0;

	//�e�L�X�g�ݒ�
	noDel_ptr<Font> _text = GetEventText();
	if (_text == NULL) return;
	_text->Print(L"%s�ő䕗���N���܂���", city->GetName().c_str());
}
//�\��
void EventPanel::Riot(noDel_ptr<City> city) {
	//�x�������Ⴂ�ƋN����
	if (city->support > 50) return;
	//3%�̊m��
	int _rand = rand() % 100;
	if (_rand >= 5) return;
	//�X�e�[�^�X����
	int _decMoney = 3000;
	int _decVal = 1000;
	///���f
	city->money -= _decMoney;
	city->political -= _decVal;
	if (city->money < 0) city->money = 0;
	if (city->political < 0) city->political = 0;

	//�e�L�X�g�ݒ�
	noDel_ptr<Font> _text = GetEventText();
	if (_text == NULL) return;
	_text->Print(L"%s�Ŗ\�����N���܂���", city->GetName().c_str());
}
