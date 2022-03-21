#include "../../../../../../framework.h"
#include "../../../../../../environment.h"
#include "SelectCounterPanel.h"

using namespace nsBattle;

void  SelectCounterPanel::Awake() {
	//�T�E���h�}�l�[�W���[
	pSoundManager = gameObject->FindGameObject("soundManager")->GetComponent<SoundManager>();

	pSelecter = gameObject->CreateImageObject(0, 0, 150.0f, 30.0f,
		CreateSprite(new Sprite(L"Data/Image/Common/square.spr")), transform);
	pSelecter->SetRenderOrder(5);
	pSelecter->GetComponent<ImageRenderer>()->SetColor(1, 1, 1, 0.6f);

	float _sizeX = gameObject->GetComponent<ImageRenderer>()->size.x;
	float _sizeY = gameObject->GetComponent<ImageRenderer>()->size.y;
	float _top = -_sizeY / 2;
	float _left = -_sizeX / 2;
	float _x = 0, _y = 0;

	//�^�C�g��
	pTitleText = gameObject->CreateObject(0, 0, 0, transform);
	pTitleText->AddComponent<Font>();
	pTitleText->GetComponent<Font>()->Print(L"�����X�L����I��");
	pTitleText->SetRenderOrder(10);
	pTitleText->transform->SetLocalPosition(_left + _sizeX * 0.1f, _top + _sizeY * 0.05f);

	//�X�L����
	for (int i = 0; i < MAX_OWN_SKILL + 1; i++) {
		_x = i < 2 ? _left + _sizeX * 0.1f : _top + _sizeX * 0.4f;
		_y = i % 2 == 0 ? _top + _sizeY * 0.2f : _top + _sizeY * 0.3f;

		noDel_ptr<GameObject> _temp = gameObject->CreateObject(0, 0, 0, transform);
		_temp->transform->SetLocalPosition(_x, _y);
		_temp->AddComponent<Font>();
		_temp->SetRenderOrder(10);
		pSkillTexts[i] = _temp->GetComponent<Font>();
	}
	pSkillTexts[MAX_OWN_SKILL]->transform->position.y = _top + _sizeY * 0.4f;

	//������
	pBorder = gameObject->CreateObject(0, 0, 0, transform);
	pBorder->AddComponent<Font>();
	pBorder->SetRenderOrder(10);
	pBorder->GetComponent<Font>()->Print(L"----------------------------------------------------");
	pBorder->transform->SetLocalPosition(_left + _sizeX * 0.02f, _top + _sizeY * 0.5f);


	//�X�L����
	for (int i = 0; i < DescNum; i++) {
		noDel_ptr<GameObject> _temp = gameObject->CreateObject(0, 0, 0, transform);
		_temp->AddComponent<Font>();
		_temp->SetRenderOrder(10);
		pDescText[i] = _temp->GetComponent<Font>();
	}

	//�����\���͂��Ȃ�
	gameObject->SetObjEnable(false);
}

void  SelectCounterPanel::Update() {
	MoveCursor();

	if (Input::Trg(InputConfig::input["decide"])) {
		if (DecideSkill()) {
			pSoundManager->Play("decide"); //���艹
			gameObject->FindGameObject("fieldManager")->
				GetComponent<FieldManager>()->SetTurnState(eTurnState::Battle);
			Close();
		}
	}
}

void SelectCounterPanel::MoveCursor() {
	//�I�����ڂ̍ő吔
	int maxNum = (int)vAtSkills.size();

	bool _flag = false;

	if (Input::Trg(InputConfig::input["up"])) {
		selectNum = CountDownInRange(selectNum, maxNum);
		_flag = true;
	}

	if (Input::Trg(InputConfig::input["down"])) {
		selectNum = CountUpInRange(selectNum, maxNum);
		_flag = true;
	}

	if (_flag) {
		//�I���ʒu�ݒ�
		pSelecter->transform->position.x = pSkillTexts[selectNum]->transform->position.x + 70.0f;
		pSelecter->transform->position.y = pSkillTexts[selectNum]->transform->position.y + 12.0f;
		//�������Z�b�g
		if(selectNum < (int)vAtSkills.size()) SetDescText(vAtSkills[selectNum]);
	}
}

bool SelectCounterPanel::DecideSkill() {
	//�X�L����I�����Ȃ������ꍇ
	if (selectNum == (int)vAtSkills.size()) {
		pSelectChara->pSelectSkill = NULL;
		return true;
	}

	//�X�L����I�������ꍇ
	noDel_ptr<Skill> _skill = vAtSkills[selectNum];
	//MP������Ȃ��ꍇ�������Ȃ�
	if (pSelectChara->pCharaInfo->mp < _skill->GetConsumeMP() ||
				range < _skill->GetMinDis() || range > _skill->GetMaxDis()) {
		return false;
	}
	//�X�L���ݒ�
	pSelectChara->pSelectSkill = _skill;
	return true;
}

void SelectCounterPanel::Open(noDel_ptr<PlayerChara> chara, int range) {
	//������
	pSelectChara = NULL;
	selectNum = 0;
	vAtSkills.clear();
	this->range = range;
	for (int i = 0; i < MAX_OWN_SKILL; i++) {
		pSkillTexts[i]->Print(L"");
		pSkillTexts[i]->SetColor(0xffffffff);
	}

	//��������e�L�X�g�ύX
	noDel_ptr<Operation> _opr = gameObject->FindGameObject("operation")->GetComponent<Operation>();
	_opr->ResetOperation();
	_opr->AddOperation("decide", L"�I��");

	//Open����
	Panel::Open();
	pSelectChara = chara;

	int _count = 0;
	for (auto& skill : chara->pCharaInfo->GetSkills()) {
		if (skill->GetSkillType() == eSkillType::Attack) {
			vAtSkills.emplace_back(skill);
			pSkillTexts[_count]->Print(L"%s", skill->GetName().c_str());
			//MP�������͋���������Ȃ���ΐF�𔼓�����
			if (pSelectChara->pCharaInfo->mp < skill->GetConsumeMP() ||
				range < skill->GetMinDis() || range > skill->GetMaxDis())
				pSkillTexts[_count]->SetColor(0x88ffffff);
			_count++;
		}
	}
	//�������Ȃ�����
	pSkillTexts[_count]->Print(L"�������Ȃ�");

	pSelecter->transform->position.x = pSkillTexts[0]->transform->position.x + 70.0f;
	pSelecter->transform->position.y = pSkillTexts[0]->transform->position.y + 12.0f;

	SetDescText(chara->pCharaInfo->GetSkills()[selectNum]);
}

void SelectCounterPanel::SetDescText(noDel_ptr<Skill> skill) {
	//������
	for (int i = 0; i < DescNum; i++) pDescText[i]->Print(L"");

	//�X�L����NULL�̏ꍇ
	pDescText[0]->Print(L"���������퓬�J�n");

	//�e�����ֈڍs
	if (skill->GetSkillType() == eSkillType::Attack) SetAttackDescText(skill);
}
void SelectCounterPanel::SetAttackDescText(noDel_ptr<Skill> skill) {
	float _sizeX = gameObject->GetComponent<ImageRenderer>()->size.x;
	float _sizeY = gameObject->GetComponent<ImageRenderer>()->size.y;
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