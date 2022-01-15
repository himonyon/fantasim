#include "../../../../../../framework.h"
#include "../../../../../../environment.h"
#include "SelectSkillPanel.h"

using namespace nsBattle;

void  SelectSkillPanel::Awake() {
	pSelecter = gameObject->CreateImageObject(0, 0, 150.0f, 30.0f,
		CreateSprite(new Sprite(L"Data/Image/Common/square.spr")), transform);
	pSelecter->GetComponent<ImageRenderer>()->SetRenderPriority(5);
	pSelecter->GetComponent<ImageRenderer>()->SetColor(1, 1, 1, 0.6f);

	float _sizeX = gameObject->GetComponent<ImageRenderer>()->sizeX;
	float _sizeY = gameObject->GetComponent<ImageRenderer>()->sizeY;
	float _top = -_sizeY / 2;
	float _left = -_sizeX / 2;
	float _x = 0, _y = 0;

	//タイトル
	pTitleText = gameObject->CreateObject(0, 0, 0, transform);
	pTitleText->AddComponent<Font>();
	pTitleText->GetComponent<Font>()->Print(L"スキルを選択してください");
	pTitleText->GetComponent<Font>()->SetRenderPriority(10);
	pTitleText->transform->SetLocalPosition(_left + _sizeX * 0.1f, _top + _sizeY * 0.05f);

	//スキル名
	for (int i = 0; i < MAX_OWN_SKILL; i++) {
		_x = i < 2 ? _left + _sizeX * 0.1f : _top + _sizeX * 0.4f;
		_y = i % 2 == 0 ? _top + _sizeY * 0.2f : _top + _sizeY * 0.3f;

		noDel_ptr<GameObject> _temp = gameObject->CreateObject(0, 0, 0, transform);
		_temp->transform->SetLocalPosition(_x, _y);
		_temp->AddComponent<Font>();
		_temp->GetComponent<Font>()->SetRenderPriority(10);
		pSkillTexts[i] = _temp->GetComponent<Font>();
	}

	//説明文
	pBorder = gameObject->CreateObject(0, 0, 0, transform);
	pBorder->AddComponent<Font>();
	pBorder->GetComponent<Font>()->Print(L"--------------------------------------------------------");
	pBorder->transform->SetLocalPosition(_left + _sizeX * 0.02f, _top + _sizeY * 0.45f);


	//スキル名
	for (int i = 0; i < DescNum; i++) {
		noDel_ptr<GameObject> _temp = gameObject->CreateObject(0, 0, 0, transform);
		_temp->AddComponent<Font>();
		_temp->GetComponent<Font>()->SetRenderPriority(10);
		pDescText[i] = _temp->GetComponent<Font>();
	}

	//初期表示はしない
	gameObject->SetObjEnable(false);
}

void  SelectSkillPanel::Update() {
	if (Input::Trg(InputConfig::input["cancel"])) {
		Turn::curState = eFieldState::Field;
		Close();
	}

	MoveCursor();
}

void SelectSkillPanel::MoveCursor() {
	//スキルの最大数
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
		//選択位置設定
		pSelecter->transform->position.x = pSkillTexts[selectNum]->transform->position.x + 70.0f;
		pSelecter->transform->position.y = pSkillTexts[selectNum]->transform->position.y + 12.0f;
		//説明文セット
		SetDescText(pSelectChara->pCharaInfo->GetSkills()[selectNum]);
	}
}

void SelectSkillPanel::Open(noDel_ptr<PlayerChara> chara) {
	//初期化
	pSelectChara = NULL;
	selectNum = 0;
	for (int i = 0; i < MAX_OWN_SKILL; i++) pSkillTexts[i]->Print(L"");

	//Open処理
	Panel::Open();
	pSelectChara = chara;

	int _count = 0;
	for (auto& skill : chara->pCharaInfo->GetSkills()) {
		pSkillTexts[_count]->Print(L"%s", skill->GetName().c_str());
		_count++;
	}

	pSelecter->transform->position.x = pSkillTexts[0]->transform->position.x + 70.0f;
	pSelecter->transform->position.y = pSkillTexts[0]->transform->position.y + 12.0f;

	SetDescText(chara->pCharaInfo->GetSkills()[selectNum]);
}

void SelectSkillPanel::SetDescText(noDel_ptr<Skill> skill) {
	//初期化
	for (int i = 0; i < DescNum; i++) pDescText[i]->Print(L"");

	//各処理へ移行
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
	pDescText[0]->Print(L"%s : %s属性の攻撃スキル", skill->GetName().c_str(), skill->GetAttr()->GetName().c_str());

	_y = _top + _sizeY * 0.7f;
	int at = dynamic_noDel_cast<AttackSkill>(skill)->GetAt();
	pDescText[1]->transform->SetLocalPosition(_x, _y);
	pDescText[1]->Print(L"消費MP : %d		基礎攻撃力 : %d", skill->GetConsumeMP(), at);

	_y = _top + _sizeY * 0.8f;
	int hit = dynamic_noDel_cast<AttackSkill>(skill)->GetHit();
	pDescText[2]->transform->SetLocalPosition(_x, _y);
	pDescText[2]->Print(L"距離 : %d～%d		命中力 : %d", skill->GetMinDis(), skill->GetMaxDis(), hit);
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
	if (dynamic_noDel_cast<HealSkill>(skill)->GetHealType() == eHealType::HP) _desc = L"単体のHP";
	else if (dynamic_noDel_cast<HealSkill>(skill)->GetHealType() == eHealType::MP) _desc = L"単体のMP";
	else if (dynamic_noDel_cast<HealSkill>(skill)->GetHealType() == eHealType::ALL_HP) _desc = L"全体のHP";
	else if (dynamic_noDel_cast<HealSkill>(skill)->GetHealType() == eHealType::ALL_MP) _desc = L"全体のMP";

	pDescText[0]->transform->SetLocalPosition(_x, _y);
	pDescText[0]->Print(L"%s : 味方%sを回復する", skill->GetName().c_str(), _desc.c_str());

	_y = _top + _sizeY * 0.7f;
	int heal = dynamic_noDel_cast<HealSkill>(skill)->GetHeal();
	pDescText[1]->transform->SetLocalPosition(_x, _y);
	pDescText[1]->Print(L"消費MP : %d		基礎回復量 : %d", skill->GetConsumeMP(), heal);

	_y = _top + _sizeY * 0.8f;
	pDescText[2]->transform->SetLocalPosition(_x, _y);
	pDescText[2]->Print(L"距離 : %d～%d", skill->GetMinDis(), skill->GetMaxDis());
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
	if (dynamic_noDel_cast<BuffSkill>(skill)->GetBuff().type == eBuffType::AT) _desc = L"攻撃力";
	else if (dynamic_noDel_cast<BuffSkill>(skill)->GetBuff().type == eBuffType::DEF) _desc = L"防御力";
	else if (dynamic_noDel_cast<BuffSkill>(skill)->GetBuff().type == eBuffType::EVD) _desc = L"回避率";

	pDescText[0]->transform->SetLocalPosition(_x, _y);
	pDescText[0]->Print(L"%s : 味方の%sを向上させる", skill->GetName().c_str(), _desc.c_str());

	_y = _top + _sizeY * 0.7f;
	float val = dynamic_noDel_cast<BuffSkill>(skill)->GetBuff().value;
	int turn = dynamic_noDel_cast<BuffSkill>(skill)->GetBuff().count;
	pDescText[1]->transform->SetLocalPosition(_x, _y);
	pDescText[1]->Print(L"消費MP : %d		倍率 : %.0f", val);

	_y = _top + _sizeY * 0.8f;
	pDescText[2]->transform->SetLocalPosition(_x, _y);
	pDescText[2]->Print(L"距離 : %d～%d		持続ターン数 : %d", skill->GetMinDis(), skill->GetMaxDis(), turn);
}