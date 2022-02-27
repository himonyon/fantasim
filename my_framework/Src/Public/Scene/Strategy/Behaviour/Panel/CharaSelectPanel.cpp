#include "../../../../../../framework.h"
#include "../../../../../../environment.h"

using namespace nsStrategy;

void CharaSelectPanel::Awake() {
	//�T�E���h�}�l�[�W���[
	pSoundManager = gameObject->FindGameObject("soundManager")->GetComponent<SoundManager>();

	noDel_ptr<Font> _font; //�R���|�[�l���g�擾�p

	float _localTop = -(gameObject->GetComponent<ImageRenderer>()->sizeY / 2);

	//�����e�L�X�g
	pDescText = gameObject->CreateObject(0, 0, 0, transform);
	pDescText->transform->SetLocalPosition(0, _localTop + 30.0f);
	pDescText->AddComponent<Font>();
	_font = pDescText->GetComponent<Font>();
	_font->SetTextAlignment(eTextAlignment::Center);
	_font->Print(L"�E�ړ�����L�����N�^�[");
	_font->SetRenderPriority((int)eRenderOrder::FrontUI);

	pBorderText = gameObject->CreateObject(0, 0, 0, transform);
	pBorderText->transform->SetLocalPosition(0, _localTop + 50.0f);
	pBorderText->AddComponent<Font>();
	_font = pBorderText->GetComponent<Font>();
	_font->SetTextAlignment(eTextAlignment::Center);
	_font->Print(L"""--------------------------------");
	_font->SetRenderPriority((int)eRenderOrder::FrontUI);

	//�L�����N�^�[�e�L�X�g
	for (int i = 0; i < OWN_CHARACTOR_NUM; i++) {
		float _textPadding = 30.0f;
		pCharaText[i] = gameObject->CreateObject(0, 0, 0, transform);
		pCharaText[i]->transform->SetLocalPosition(0, _localTop + 80.0f + (_textPadding * i));
		pCharaText[i]->AddComponent<Font>();
		_font = pCharaText[i]->GetComponent<Font>();
		_font->SetTextAlignment(eTextAlignment::Center);
		_font->SetRenderPriority((int)eRenderOrder::FrontUI);
	}

	//����e�L�X�g�{�^��
	pDecideText = gameObject->CreateObject(0, 0, 0, transform);
	pDecideText->transform->SetLocalPosition(0, -_localTop - 50.0f);
	pDecideText->AddComponent<Font>();
	_font = pDecideText->GetComponent<Font>();
	_font->SetTextAlignment(eTextAlignment::Center);
	_font->Print(L"�m��");
	_font->SetRenderPriority((int)eRenderOrder::FrontUI);

	//�Z���N�g�J�[�\��
	pSelectCursor = gameObject->CreateImageObject(0, 0, 1, 25, CreateSprite(
		new Sprite(L"Data/Image/Common/square.spr")), transform);
	pSelectCursor->GetComponent<ImageRenderer>()->SetRenderPriority((int)eRenderOrder::FrontObject);

	//������ԂŉB���Ă���
	gameObject->SetObjEnable(false);
}

void CharaSelectPanel::Update() {\
	//�J�[�\���ړ�
	if (Input::Trg(InputConfig::input["up"]) || Input::Trg(InputConfig::input["down"])) {
		//�J�E���g�A�b�v
		Input::Trg(InputConfig::input["up"]) ? cursorNum-- : cursorNum++;
		if (cursorNum > maxCursorNum) cursorNum = 0;
		else if (cursorNum < 0) cursorNum = maxCursorNum;

		if (cursorNum == maxCursorNum) {
			pSelectCursor->transform->SetPosition(pDecideText->transform->position.x, pDecideText->transform->position.y + 12.0f);
			pSelectCursor->transform->scale.x = pDecideText->GetComponent<Font>()->GetTextLength() + 10.0f;
		}
		else {
			pSelectCursor->transform->SetPosition(pCharaText[cursorNum]->transform->position.x,
				pCharaText[cursorNum]->transform->position.y + 12.0f);
			pSelectCursor->transform->scale.x = pCharaText[cursorNum]->GetComponent<Font>()->GetTextLength() + 20.0f;
		}
	}

	if (Input::Trg(InputConfig::input["cancel"])) {
		pSoundManager->Play("cancel"); //���艹
		Close();
		gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->SetTurnState(eTurnState::Back);
	}

	///�L�����I���Ǝ��s
	if (Input::Trg(InputConfig::input["decide"])) {
		pSoundManager->Play("decide"); //���艹
		if (cursorNum == maxCursorNum) 	DecideMoveChara();
		else AddMoveChara();
	}
}

void CharaSelectPanel::Open(noDel_ptr<City> city, noDel_ptr<City> city2) {
	pCity = city;
	pCity2 = city2;

	//��������e�L�X�g�ύX
	noDel_ptr<Operation> _opr = gameObject->FindGameObject("operation")->GetComponent<Operation>();
	_opr->ResetOperation();
	_opr->AddOperation("decide", L"�I��");
	_opr->AddOperation("cancel", L"�߂�");

	Panel::Open();

	cursorNum = 0;
	maxCursorNum = (int)pCity->vOwnChara.size();

	//�L�����N�^�[�e�L�X�g
	for (int i = 0; i < OWN_CHARACTOR_NUM; i++) {
		pCharaText[i]->GetComponent<Font>()->SetColor(0xffffffff);
		if(i < pCity->vOwnChara.size()) 
			pCharaText[i]->GetComponent<Font>()->Print(L"%s", pCity->vOwnChara[i]->GetName().c_str());
		else
			pCharaText[i]->GetComponent<Font>()->Print(L"");
	}

	pSelectCursor->transform->SetPosition(pCharaText[0]->transform->position.x, pCharaText[0]->transform->position.y + 12.0f);
	pSelectCursor->transform->scale.x = pCharaText[0]->GetComponent<Font>()->GetTextLength() + 10.0f;
}

void CharaSelectPanel::Close() {
	vMoveCharas.clear();
	Panel::Close();
}

void CharaSelectPanel::AddMoveChara() {
	auto it = pCity->vOwnChara.begin() + cursorNum;
	bool _flag = false;
	//���łɂ���ꍇ�O��
	for (auto itr = vMoveCharas.begin(); itr != vMoveCharas.end();) {
		if (itr->get() == it->get()) {
			_flag = true;
			itr = vMoveCharas.erase(itr);
			break;
		}
		else {
			itr++;
		}
	}

	//�F�ύX�ƒǉ�����
	if (_flag) pCharaText[cursorNum]->GetComponent<Font>()->SetColor(0xffffffff);
	else {
		//�ő吔�𒴂���ꍇ�͏������Ȃ�
		if ((int)pCity2->vOwnChara.size() + (int)vMoveCharas.size() == OWN_CHARACTOR_NUM) return;
		pCharaText[cursorNum]->GetComponent<Font>()->SetColor(0xffff00ff);
		vMoveCharas.emplace_back(pCity->vOwnChara[cursorNum]);
	}
}
void CharaSelectPanel::DecideMoveChara() {
	//�Ώۂ𑼂̒��ֈړ�������
	for (auto itr = vMoveCharas.begin(); itr != vMoveCharas.end(); itr++) {
		int _count = 0;
		for (auto itr2 = pCity->vOwnChara.begin(); itr2 != pCity->vOwnChara.end();) {
			if (itr->get() == itr2->get()) {
				pCity2->vOwnChara.emplace_back(pCity->vOwnChara[_count]);
				itr2 = pCity->vOwnChara.erase(itr2);
				break;
			}
			else {
				itr2++;
				_count++;
			}
		}
	}

	pCity->IncActCount(); //�s�������񐔂𑝉�������
	Close();
	gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->SetTurnState(eTurnState::Command);
}

