#include "../../../../../../framework.h"
#include "../../../../../../environment.h"
#include "SelectCounterPanel.h"

using namespace nsBattle;

void  SelectCounterPanel::Awake() {
	//サウンドマネージャー
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

	//タイトル
	pTitleText = gameObject->CreateObject(0, 0, 0, transform);
	pTitleText->AddComponent<Font>();
	pTitleText->GetComponent<Font>()->Print(L"反撃スキルを選択");
	pTitleText->SetRenderOrder(10);
	pTitleText->transform->SetLocalPosition(_left + _sizeX * 0.1f, _top + _sizeY * 0.05f);

	//スキル名
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

	//説明文
	pBorder = gameObject->CreateObject(0, 0, 0, transform);
	pBorder->AddComponent<Font>();
	pBorder->SetRenderOrder(10);
	pBorder->GetComponent<Font>()->Print(L"----------------------------------------------------");
	pBorder->transform->SetLocalPosition(_left + _sizeX * 0.02f, _top + _sizeY * 0.5f);


	//スキル名
	for (int i = 0; i < DescNum; i++) {
		noDel_ptr<GameObject> _temp = gameObject->CreateObject(0, 0, 0, transform);
		_temp->AddComponent<Font>();
		_temp->SetRenderOrder(10);
		pDescText[i] = _temp->GetComponent<Font>();
	}

	//初期表示はしない
	gameObject->SetObjEnable(false);
}

void  SelectCounterPanel::Update() {
	MoveCursor();

	if (Input::Trg(InputConfig::input["decide"])) {
		if (DecideSkill()) {
			pSoundManager->Play("decide"); //決定音
			gameObject->FindGameObject("fieldManager")->
				GetComponent<FieldManager>()->SetTurnState(eTurnState::Battle);
			Close();
		}
	}
}

void SelectCounterPanel::MoveCursor() {
	//選択項目の最大数
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
		//選択位置設定
		pSelecter->transform->position.x = pSkillTexts[selectNum]->transform->position.x + 70.0f;
		pSelecter->transform->position.y = pSkillTexts[selectNum]->transform->position.y + 12.0f;
		//説明文セット
		if(selectNum < (int)vAtSkills.size()) SetDescText(vAtSkills[selectNum]);
	}
}

bool SelectCounterPanel::DecideSkill() {
	//スキルを選択しなかった場合
	if (selectNum == (int)vAtSkills.size()) {
		pSelectChara->pSelectSkill = NULL;
		return true;
	}

	//スキルを選択した場合
	noDel_ptr<Skill> _skill = vAtSkills[selectNum];
	//MPが足りない場合処理しない
	if (pSelectChara->pCharaInfo->mp < _skill->GetConsumeMP() ||
				range < _skill->GetMinDis() || range > _skill->GetMaxDis()) {
		return false;
	}
	//スキル設定
	pSelectChara->pSelectSkill = _skill;
	return true;
}

void SelectCounterPanel::Open(noDel_ptr<PlayerChara> chara, int range) {
	//初期化
	pSelectChara = NULL;
	selectNum = 0;
	vAtSkills.clear();
	this->range = range;
	for (int i = 0; i < MAX_OWN_SKILL; i++) {
		pSkillTexts[i]->Print(L"");
		pSkillTexts[i]->SetColor(0xffffffff);
	}

	//操作説明テキスト変更
	noDel_ptr<Operation> _opr = gameObject->FindGameObject("operation")->GetComponent<Operation>();
	_opr->ResetOperation();
	_opr->AddOperation("decide", L"選択");

	//Open処理
	Panel::Open();
	pSelectChara = chara;

	int _count = 0;
	for (auto& skill : chara->pCharaInfo->GetSkills()) {
		if (skill->GetSkillType() == eSkillType::Attack) {
			vAtSkills.emplace_back(skill);
			pSkillTexts[_count]->Print(L"%s", skill->GetName().c_str());
			//MPもしくは距離が足りなければ色を半透明に
			if (pSelectChara->pCharaInfo->mp < skill->GetConsumeMP() ||
				range < skill->GetMinDis() || range > skill->GetMaxDis())
				pSkillTexts[_count]->SetColor(0x88ffffff);
			_count++;
		}
	}
	//反撃しない項目
	pSkillTexts[_count]->Print(L"反撃しない");

	pSelecter->transform->position.x = pSkillTexts[0]->transform->position.x + 70.0f;
	pSelecter->transform->position.y = pSkillTexts[0]->transform->position.y + 12.0f;

	SetDescText(chara->pCharaInfo->GetSkills()[selectNum]);
}

void SelectCounterPanel::SetDescText(noDel_ptr<Skill> skill) {
	//初期化
	for (int i = 0; i < DescNum; i++) pDescText[i]->Print(L"");

	//スキルがNULLの場合
	pDescText[0]->Print(L"反撃せず戦闘開始");

	//各処理へ移行
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