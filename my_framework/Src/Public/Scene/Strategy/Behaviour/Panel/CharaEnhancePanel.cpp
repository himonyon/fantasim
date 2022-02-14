#include "../../../../../../framework.h"
#include "../../../../../../environment.h"

using namespace nsStrategy;
//������
void CharaEnhancePanel::Awake() {
	//�T�E���h�}�l�[�W���[
	pSoundManager = gameObject->FindGameObject("soundManager")->GetComponent<SoundManager>();

	noDel_ptr<ImageRenderer> _renderer = gameObject->GetComponent<ImageRenderer>();
	float _sizeX = _renderer->sizeX;
	float _sizeY = _renderer->sizeY;
	float _top = transform->position.y - _sizeY / 2;
	float _left = transform->position.x - _sizeX / 2;

	float _textPosX = 0;
	float _textPosY = 0;
	
	//�L������
	_textPosX = _left + _sizeX * 0.1f;
	_textPosY = _top + _sizeY * 0.2f;
	for (int i = 0; i < OWN_CHARACTOR_NUM; i++) {
		const float _padding = 40.0f;
		float _y = _textPosY + (i * _padding);
		CreateText(pNameText[i], _textPosX, _y);
	}
	//���
	_textPosY = _top + _sizeY * 0.1f;
	CreateText(pTopText, _textPosX, _textPosY);
	pTopText->Print(L"��������L������I�����Ă�������");
	//�K�v���z�e�L�X�g
	float _Rate = 0.2f;
	float _RatePad = 0.085f;
	_textPosX = _left + _sizeX * 0.5f;
	_textPosY = _top + _sizeY * _Rate;
	CreateText(pNeedMoneyText, _textPosX, _textPosY);
	//����
	_Rate += _RatePad;
	_textPosY = _top + _sizeY * _Rate;
	CreateText(pAttrText,_textPosX, _textPosY);
	//���x��
	_Rate += _RatePad;
	_textPosY = _top + _sizeY * _Rate;
	CreateText(pLevelText, _textPosX, _textPosY);
	//�̒l
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
	//�h���
	_Rate += _RatePad;
	_textPosY = _top + _sizeY * _Rate;
	CreateText(pDefText, _textPosX, _textPosY);
	//���
	_Rate += _RatePad;
	_textPosY = _top + _sizeY * _Rate;
	CreateText(pEvdText, _textPosX, _textPosY);
	//�ړ���
	_Rate += _RatePad;
	_textPosY = _top + _sizeY * _Rate;
	CreateText(pMoveText, _textPosX, _textPosY);

	//�J�[�\��
	noDel_ptr<GameObject> _pCursorObj = gameObject->CreateImageObject(0, 0, 150, 30, CreateSprite(
		new Sprite(L"Data/Image/Common/square.spr")), transform);
	pCursor = _pCursorObj->GetComponent<ImageRenderer>();
	pCursor->SetRenderPriority((int)eRenderOrder::FrontUI - 1);

	//�L�����摜
	_textPosX = _left + _sizeX * 0.2f;
	_textPosY = _top + _sizeY * 0.8f;
	noDel_ptr<GameObject> _pCharaObj = gameObject->CreateImageObject(_textPosX, _textPosY, 150, 150, NULL, transform);
	_pCharaObj->transform->scale.x *= -1;
	pCharaImage = _pCharaObj->GetComponent<ImageRenderer>();
	pCharaImage->SetRenderPriority((int)eRenderOrder::FrontUI - 1);
	pCharaImage->SetColor(1, 1, 1, 1);

	//�{�[�_�[
	_textPosX = _left + _sizeX * 0.4f;
	_textPosY = _top + _sizeY * 0.55f;
	noDel_ptr<GameObject> _pBorderObj = gameObject->CreateImageObject(_textPosX, _textPosY, 300, _sizeY - 150.0f, CreateSprite(
		new Sprite(L"Data/Image/Common/border.spr")), transform);
	pBorder = _pBorderObj->GetComponent<ImageRenderer>();
	pBorder->SetRenderPriority((int)eRenderOrder::FrontUI + 1);

	//�p�l���쐬
	pConfirmPanel = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER, 250, 150,
		_renderer->pRenderSprite, transform);
	pConfirmPanel->GetComponent<ImageRenderer>()->SetRenderPriority((int)eRenderOrder::FrontUI+ 10);
	//�m�F�e�L�X�g
	noDel_ptr<GameObject> _tempObj = gameObject->CreateObject(0, 0, 0, pConfirmPanel->transform);
	_tempObj->transform->SetLocalPosition(0, -30.0f);
	_tempObj->AddComponent<Font>();
	pConfirmText = _tempObj->GetComponent<Font>();
	pConfirmText->SetTextAlignment(eTextAlignment::Center);
	pConfirmText->Print(L"�L�������������܂����H");
	pConfirmText->SetRenderPriority((int)eRenderOrder::FrontUI + 11);
	//�m�F�e�L�X�g�Q
	_tempObj = gameObject->CreateObject(0, 0, 0, pConfirmPanel->transform);
	_tempObj->transform->SetLocalPosition(0, 10.0f);
	_tempObj->AddComponent<Font>();
	pYesNoText = _tempObj->GetComponent<Font>();
	pYesNoText->SetTextAlignment(eTextAlignment::Center);
	pYesNoText->Print(L"�Z�F�͂��@�@�~�F������");
	pYesNoText->SetRenderPriority((int)eRenderOrder::FrontUI + 11);

	pConfirmPanel->SetObjEnable(false);

	Close();
}

//�e�L�X�g�쐬
void CharaEnhancePanel::CreateText(noDel_ptr<Font>& target, float posX, float posY) {
	noDel_ptr<GameObject> _tempObj = gameObject->CreateObject(posX, posY, 0, transform);
	_tempObj->AddComponent<Font>();
	target = _tempObj->GetComponent<Font>();
	target->SetRenderPriority((int)eRenderOrder::FrontUI);
}

//����
void CharaEnhancePanel::Update() {
	//�m�F��ʂ��J�������
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

	//�m�F��ʂ��J��
	if (Input::Trg(InputConfig::input["decide"])) {
		if (IsEnhance(pSelectCity->vOwnChara[cursorNum])) {
			pConfirmPanel->SetObjEnable(true);
			pSoundManager->Play("decide");
		}
	}

	//�߂�
	if (Input::Trg(InputConfig::input["cancel"])) {
		gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->SetTurnState(eTurnState::Back);
		pSoundManager->Play("cancel");
		Close();
	}

	//�J�[�\���ړ�
	MoveCursor();
}

//�J�[�\���ړ�
void CharaEnhancePanel::MoveCursor() {
	int _bef = cursorNum;

	if (Input::Trg(InputConfig::input["down"])) cursorNum++;
	if (Input::Trg(InputConfig::input["up"])) cursorNum--;

	if (cursorNum < 0) cursorNum = 0;
	if (cursorNum >= (int)pSelectCity->vOwnChara.size()) cursorNum = (int)pSelectCity->vOwnChara.size() - 1;

	if (_bef == cursorNum) return;

	//�J�[�\���ʒu�ݒ�
	pCursor->transform->SetPosition(pNameText[cursorNum]->transform->position.x + 60.0f,
		pNameText[cursorNum]->transform->position.y + 12.5f);
	//�X�e�[�^�X�e�L�X�g�X�V
	SetStatusText(pSelectCity->vOwnChara[cursorNum]);
	//�摜�X�V
	pCharaImage->SetUpRenderer2D(150, 150, pSelectCity->vOwnChara[cursorNum]->GetSprite());
}

//�J��
void CharaEnhancePanel::Open(noDel_ptr<City> city) {
	//�L���������Ȃ���Ε���
	if (city->vOwnChara.size() == 0) {
		gameObject->FindGameObject("gameManager")->
			GetComponent<GameManager>()->SetTurnState(eTurnState::Command);
		return;
	}

	//�����Ȃ���Ε���
	if (city->money < 1000) {
		gameObject->FindGameObject("gameManager")->
			GetComponent<GameManager>()->SetTurnState(eTurnState::Command);
		return;
	}

	//������
	cursorNum = 0;
	pSelectCity = city;


	//�e�L�X�g�ݒ�
	//�L�����e�L�X�g
	for (auto& text : pNameText) {
		text->Print(L"");
	}
	int _count = 0;
	for (auto& chara : city->vOwnChara) {
		//�������ׂ̏ꍇ�F�𔖂߂�
		if (IsEnhance(chara) == false) pNameText[_count]->SetColor(0x55ffffff);
		pNameText[_count]->Print(L"%s", chara->GetName().c_str());
		_count++;
	}

	//�J�[�\��
	pCursor->transform->SetPosition(pNameText[cursorNum]->transform->position.x + 60.0f,
		pNameText[cursorNum]->transform->position.y + 12.5f);

	//���߂ɕ\������L�����̃e�L�X�g�Z�b�g
	SetStatusText(city->vOwnChara[cursorNum]);
	pCharaImage->SetUpRenderer2D(150, 150,city->vOwnChara[cursorNum]->GetSprite());

	Panel::Open();
}

//�X�e�[�^�X�e�L�X�g�Z�b�g
void CharaEnhancePanel::SetStatusText(noDel_ptr<Charactor> chara) {
	//�K�v���z
	int _money = GetNeedMoney(chara->level);
	pNeedMoneyText->Print(L"�������z�F %d", _money);
	//����
	pAttrText->Print(L"�����F %s", chara->GetAttr()->GetName().c_str());
	//���x��
	pLevelText->Print(L"���x���F %d  >>> %d", chara->level, chara->level + upValue.level);
	//�̒l
	pPowerText->Print(L"�̒l�F %d  >>> %d", chara->power, chara->power + upValue.power);
	//HP
	pHpText->Print(L"HP�F %d  >>> %d", chara->maxHp, chara->maxHp + upValue.hp);
	//MP
	pMpText->Print(L"MP�F %d  >>> %d", chara->maxMp, chara->maxMp + upValue.mp);
	//�h���
	pDefText->Print(L"�h��́F %d  >>> %d", chara->def, chara->def + upValue.def);
	//���
	pEvdText->Print(L"��𗦁F %d%%  >>> %d%%", chara->evd, chara->evd + upValue.evd);
	//�ړ���
	if(chara->level % 3 == 0)
		pMoveText->Print(L"�ړ��ʁF %d  >>> %d", chara->move, chara->move + upValue.move);
	else
		pMoveText->Print(L"�ړ��ʁF %d", chara->move);
}

//����
void CharaEnhancePanel::Enhance() {
	
	noDel_ptr<Charactor> _chara = pSelectCity->vOwnChara[cursorNum];

	pSoundManager->Play("enhance");

	//�X�̎��������炷
	int _needMoney = GetNeedMoney(_chara->level);
	pSelectCity->money -= _needMoney;

	//���x��
	_chara->level += upValue.level;
	//�̒l
	_chara->power += upValue.power;
	//HP
	_chara->maxHp += upValue.hp;
	//MP
	_chara->maxMp += upValue.mp;
	//�h���
	_chara->def += upValue.def;
	//���
	_chara->evd += upValue.evd;
	//�ړ���
	if (_chara->level % 3 == 0) _chara->move += upValue.move;

	//�e�L�X�g�X�V
	SetStatusText(_chara);
	int _count = 0;
	for (auto& chara : pSelectCity->vOwnChara) {
		//�����s�̏ꍇ�F�𔖂߂�
		if (IsEnhance(chara) == false) pNameText[_count]->SetColor(0x55ffffff);
		_count++;
	}
}

//�����\��
bool CharaEnhancePanel::IsEnhance(noDel_ptr<Charactor> chara) {
	int _needMoney = 0;
	//���x�����P0�ȏ�Ȃ狭���s��
	if (chara->level >= 10) return false;

	//�K�v�ȋ��z�v�Z
	_needMoney = GetNeedMoney(chara->level);
	//���z������Ȃ���΋����s��
	if (pSelectCity->money < _needMoney) return false;
	
	return true;
}

//�K�v���z�v�Z
int CharaEnhancePanel::GetNeedMoney(int level) {
	int _need = 1000;
	for (int i = 1; i < level; i++) {
		_need = (int)((float)_need * 1.4f);
	}

	return _need;
}
