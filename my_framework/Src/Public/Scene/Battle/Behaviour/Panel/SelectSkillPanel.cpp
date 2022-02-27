#include "../../../../../../framework.h"
#include "../../../../../../environment.h"
#include "SelectSkillPanel.h"

using namespace nsBattle;

void  SelectSkillPanel::Awake() {
	//�T�E���h�}�l�[�W���[
	pSoundManager = gameObject->FindGameObject("soundManager")->GetComponent<SoundManager>();

	pSelecter = gameObject->CreateImageObject(0, 0, 150.0f, 30.0f,
		CreateSprite(new Sprite(L"Data/Image/Common/square.spr")), transform);
	pSelecter->GetComponent<ImageRenderer>()->SetRenderPriority(5);
	pSelecter->GetComponent<ImageRenderer>()->SetColor(1, 1, 1, 0.6f);

	float _sizeX = gameObject->GetComponent<ImageRenderer>()->sizeX;
	float _sizeY = gameObject->GetComponent<ImageRenderer>()->sizeY;
	float _top = -_sizeY / 2;
	float _left = -_sizeX / 2;
	float _x = 0, _y = 0;

	//�^�C�g��
	pTitleText = gameObject->CreateObject(0, 0, 0, transform);
	pTitleText->AddComponent<Font>();
	pTitleText->GetComponent<Font>()->Print(L"�X�L����I�����Ă�������");
	pTitleText->GetComponent<Font>()->SetRenderPriority(10);
	pTitleText->transform->SetLocalPosition(_left + _sizeX * 0.1f, _top + _sizeY * 0.05f);

	//�X�L����
	for (int i = 0; i < MAX_OWN_SKILL; i++) {
		_x = i < 2 ? _left + _sizeX * 0.1f : _top + _sizeX * 0.4f;
		_y = i % 2 == 0 ? _top + _sizeY * 0.2f : _top + _sizeY * 0.3f;

		noDel_ptr<GameObject> _temp = gameObject->CreateObject(0, 0, 0, transform);
		_temp->transform->SetLocalPosition(_x, _y);
		_temp->AddComponent<Font>();
		_temp->GetComponent<Font>()->SetRenderPriority(10);
		pSkillTexts[i] = _temp->GetComponent<Font>();
	}

	//������
	pBorder = gameObject->CreateObject(0, 0, 0, transform);
	pBorder->AddComponent<Font>();
	pBorder->GetComponent<Font>()->SetRenderPriority(10);
	pBorder->GetComponent<Font>()->Print(L"----------------------------------------------------");
	pBorder->transform->SetLocalPosition(_left + _sizeX * 0.02f, _top + _sizeY * 0.45f);


	//�X�L����
	for (int i = 0; i < DescNum; i++) {
		noDel_ptr<GameObject> _temp = gameObject->CreateObject(0, 0, 0, transform);
		_temp->AddComponent<Font>();
		_temp->GetComponent<Font>()->SetRenderPriority(10);
		pDescText[i] = _temp->GetComponent<Font>();
	}

	//�����\���͂��Ȃ�
	gameObject->SetObjEnable(false);
}

void  SelectSkillPanel::Update() {
	if (Input::Trg(InputConfig::input["cancel"])) {
		pSoundManager->Play("cancel"); //�L�����Z����
		gameObject->FindGameObject("fieldManager")->
			GetComponent<FieldManager>()->SetTurnState(eTurnState::Back);
		Close();
	}

	MoveCursor();

	if (Input::Trg(InputConfig::input["decide"])) {
		if (DecideSkill()) {
			pSoundManager->Play("decide"); //���艹
			gameObject->FindGameObject("fieldManager")->
				GetComponent<FieldManager>()->SetTurnState(eTurnState::Skill);
			Close();
		}
	}
}

void SelectSkillPanel::MoveCursor() {
	//�X�L���̍ő吔
	int maxNum = (int)pSelectChara->pCharaInfo->GetSkills().size();

	bool _flag = false;

	if (Input::Trg(InputConfig::input["up"])) {
		selectNum = CountDownInRange(selectNum, maxNum - 1);
		_flag = true;
	}

	if (Input::Trg(InputConfig::input["down"])) {
		selectNum = CountUpInRange(selectNum, maxNum - 1);
		_flag = true;
	}

	if (_flag) {
		//�I���ʒu�ݒ�
		pSelecter->transform->position.x = pSkillTexts[selectNum]->transform->position.x + 70.0f;
		pSelecter->transform->position.y = pSkillTexts[selectNum]->transform->position.y + 12.0f;
		//�������Z�b�g
		SetDescText(pSelectChara->pCharaInfo->GetSkills()[selectNum]);
	}
}

bool SelectSkillPanel::DecideSkill() {
	noDel_ptr<Skill> _skill = pSelectChara->pCharaInfo->GetSkills()[selectNum];
	//MP������Ȃ��ꍇ�������Ȃ�
	if (pSelectChara->pCharaInfo->mp < _skill->GetConsumeMP()) {
		return false;
	}
	//�X�L���ݒ�
	pSelectChara->pSelectSkill = _skill;
	return true;
}

void SelectSkillPanel::Open(noDel_ptr<PlayerChara> chara) {
	//������
	pSelectChara = NULL;
	selectNum = 0;
	for (int i = 0; i < MAX_OWN_SKILL; i++) pSkillTexts[i]->Print(L"");

	//��������e�L�X�g�ύX
	noDel_ptr<Operation> _opr = gameObject->FindGameObject("operation")->GetComponent<Operation>();
	_opr->ResetOperation();
	_opr->AddOperation("decide", L"�I��");
	_opr->AddOperation("cancel", L"�߂�");

	//Open����
	Panel::Open();
	pSelectChara = chara;

	int _count = 0;
	for (auto& skill : chara->pCharaInfo->GetSkills()) {
		pSkillTexts[_count]->Print(L"%s", skill->GetName().c_str());
		//���p�ł��Ȃ��X�L���͔�������
		if (pSelectChara->pCharaInfo->mp < skill->GetConsumeMP())
			pSkillTexts[_count]->SetColor(0x88ffffff);
		_count++;
	}

	pSelecter->transform->position.x = pSkillTexts[0]->transform->position.x + 70.0f;
	pSelecter->transform->position.y = pSkillTexts[0]->transform->position.y + 12.0f;

	SetDescText(chara->pCharaInfo->GetSkills()[selectNum]);
}

void SelectSkillPanel::SetDescText(noDel_ptr<Skill> skill) {
	//������
	for (int i = 0; i < DescNum; i++) pDescText[i]->Print(L"");

	//�e�����ֈڍs
	if (skill->GetSkillType() == eSkillType::Attack) SetAttackDescText(skill);
	else if (skill->GetSkillType() == eSkillType::Heal) SetHealDescText(skill);
	else if (skill->GetSkillType() == eSkillType::Buff) SetBuffDescText(skill);
}
void SelectSkillPanel::SetAttackDescText(noDel_ptr<Skill> skill) {
	float _sizeX = gameObject->GetComponent<ImageRenderer>()->sizeX;
	float _sizeY = gameObject->GetComponent<ImageRenderer>()->sizeY;
	float _top = -_sizeY / 2;
	float _left = -_sizeX / 2;
	float _x = 0, _y = 0;

	_x = _left + _sizeX * 0.1f;
	_y = _top + _sizeY * 0.6f;
	pDescText[0]->transform->SetLocalPosition(_x, _y);
	pDescText[0]->Print(L"%s : %s�����̍U���X�L��", skill->GetName().c_str(), skill->GetAttr()->GetName().c_str());

	_y = _top + _sizeY * 0.7f;
	int at = dynamic_noDel_cast<AttackSkill>(skill)->GetAt();
	pDescText[1]->transform->SetLocalPosition(_x, _y);
	pDescText[1]->Print(L"����MP : %d		��b�U���� : %d", skill->GetConsumeMP(), at);

	_y = _top + _sizeY * 0.8f;
	int hit = dynamic_noDel_cast<AttackSkill>(skill)->GetHit();
	pDescText[2]->transform->SetLocalPosition(_x, _y);
	pDescText[2]->Print(L"���� : %d�`%d		������ : %d", skill->GetMinDis(), skill->GetMaxDis(), hit);
}
void SelectSkillPanel::SetHealDescText(noDel_ptr<Skill> skill) {
	float _sizeX = gameObject->GetComponent<ImageRenderer>()->sizeX;
	float _sizeY = gameObject->GetComponent<ImageRenderer>()->sizeY;
	float _top = -_sizeY / 2;
	float _left = -_sizeX / 2;
	float _x = 0, _y = 0;

	_x = _left + _sizeX * 0.1f;
	_y = _top + _sizeY * 0.6f;

	std::wstring _desc = L"";
	noDel_ptr<HealSkill> _healS = dynamic_noDel_cast<HealSkill>(skill);
	if (_healS == NULL) return;

	if (_healS->GetHealType() == eHealType::HP && _healS->IsAllRange()) _desc = L"�S�̂�HP";
	else if (_healS->GetHealType() == eHealType::MP && _healS->IsAllRange()) _desc = L"�S�̂�MP";
	else if (_healS->GetHealType() == eHealType::HP) _desc = L"�P�̂�HP";
	else if (_healS->GetHealType() == eHealType::MP) _desc = L"�P�̂�MP";

	pDescText[0]->transform->SetLocalPosition(_x, _y);
	pDescText[0]->Print(L"%s : ����%s���񕜂���", skill->GetName().c_str(), _desc.c_str());

	_y = _top + _sizeY * 0.7f;
	int heal = dynamic_noDel_cast<HealSkill>(skill)->GetHeal();
	pDescText[1]->transform->SetLocalPosition(_x, _y);
	pDescText[1]->Print(L"����MP : %d		��b�񕜗� : %d", skill->GetConsumeMP(), heal);

	_y = _top + _sizeY * 0.8f;
	pDescText[2]->transform->SetLocalPosition(_x, _y);
	pDescText[2]->Print(L"���� : %d�`%d", skill->GetMinDis(), skill->GetMaxDis());
}
void SelectSkillPanel::SetBuffDescText(noDel_ptr<Skill> skill) {
	float _sizeX = gameObject->GetComponent<ImageRenderer>()->sizeX;
	float _sizeY = gameObject->GetComponent<ImageRenderer>()->sizeY;
	float _top = -_sizeY / 2;
	float _left = -_sizeX / 2;
	float _x = 0, _y = 0;

	_x = _left + _sizeX * 0.1f;
	_y = _top + _sizeY * 0.6f;

	std::wstring _desc = L"";
	if (dynamic_noDel_cast<BuffSkill>(skill)->GetBuff().type == eBuffType::AT) _desc = L"�U����";
	else if (dynamic_noDel_cast<BuffSkill>(skill)->GetBuff().type == eBuffType::DEF) _desc = L"�h���";
	else if (dynamic_noDel_cast<BuffSkill>(skill)->GetBuff().type == eBuffType::EVD) _desc = L"���";

	pDescText[0]->transform->SetLocalPosition(_x, _y);
	pDescText[0]->Print(L"%s : ������%s�����コ����", skill->GetName().c_str(), _desc.c_str());

	_y = _top + _sizeY * 0.7f;
	float val = dynamic_noDel_cast<BuffSkill>(skill)->GetBuff().value;
	int turn = dynamic_noDel_cast<BuffSkill>(skill)->GetBuff().count;
	pDescText[1]->transform->SetLocalPosition(_x, _y);
	pDescText[1]->Print(L"����MP : %d		�{�� : %.0f", val);

	_y = _top + _sizeY * 0.8f;
	pDescText[2]->transform->SetLocalPosition(_x, _y);
	pDescText[2]->Print(L"���� : %d�`%d		�����^�[���� : %d", skill->GetMinDis(), skill->GetMaxDis(), turn);
}