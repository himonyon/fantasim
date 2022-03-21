#include "../../../../../../framework.h"
#include "../../../../../../environment.h"

using namespace nsStrategy;

void SelectCountryPanel::Awake() {
	//�T�E���h�}�l�[�W���[
	pSoundManager = gameObject->FindGameObject("soundManager")->GetComponent<SoundManager>();

	//�J�[�\���ƍ��̎擾
	pCursor = gameObject->FindGameObject("cursor");
	for (int i = 0; i < COUNTRY_NUM; i++) {
		std::string _name = "city" + std::to_string(i);
		countries[i] = gameObject->FindGameObject(_name)->GetComponent<Country>();
	}

	//�e�L�X�g�̐ݒ�
	for (int i = 0; i < 4; i++) {
		pText[i] = gameObject->CreateObject(0, 0, 0);
		pText[i]->AddComponent<Font>();
		pText_font[i] = pText[i]->GetComponent<Font>();
		pText_font[i]->gameObject->SetRenderOrder((int)eRenderOrder::UI + 1);
		pText_font[i]->SetRectWH(
			transform->position.x,
			transform->position.y,
			gameObject->GetComponent<ImageRenderer>()->size.x,
			gameObject->GetComponent<ImageRenderer>()->size.y
		);
		pText_font[i]->SetTextAlignment(eTextAlignment::Left);
	}

	pText_font[0]->Print(30.0f, 30.0f, L"����I�����Ă�������");
	pText_font[1]->Print(30.0f, 60.0f, L"���@           �@       ��");

	Open();
	SetTextState(countries[cursor]);
}

void SelectCountryPanel::Update() {

	if (Input::Trg(InputConfig::input["right"])) {
		countries[cursor]->FocusCity(false);
		cursor = CountUpInRange(cursor, COUNTRY_NUM - 1);
		countries[cursor]->FocusCity(true);
		SetTextState(countries[cursor]);
	}
	if (Input::Trg(InputConfig::input["left"])) {
		countries[cursor]->FocusCity(false);
		cursor = CountDownInRange(cursor, COUNTRY_NUM - 1);
		countries[cursor]->FocusCity(true);
		SetTextState(countries[cursor]);
	}

	if (Input::Trg(InputConfig::input["decide"])) {
		pSoundManager->Play("decide"); //���艹
		//���삷�鍑��I��
		countries[cursor]->SetIsPlayer();
		//�p�l������
		Close();
		//�Q�[���J�n
		gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->GameStart();
		//�J�[�\���\��
		pCursor->SetObjEnable(true);
		pCursor->transform->position.x = countries[cursor]->transform->position.x;
		pCursor->transform->position.y = countries[cursor]->transform->position.y;
		//���̃p�l����j��
		gameObject->Destroy();
		for (int i = 0; i < 4; i++) {
			gameObject->Destroy(pText[i]);
		}
	}

	
	WCHAR description[COUNTRY_NUM][256] = {
		L"�̒n�������Ȃ��A������\n�����Ă��Ȃ����W�r�㍑�B\n���͂ȕ��m��������Ă���B\n�̒n�� : 3\n��Փx : HARD",
		L"�������ĕ��ϓI�ȍ��B\n�����������Ă��邽��\n�������N���ɂ����B\n�̒n�� : 5\n��Փx : NORMAL",
		L"�ЊQ�ɋ�����r�I����\n���Ă��鍑�B\n���K�ʂɂ��]�T����\n�̒n�� : 4\n��Փx : NORMAL",
		L"�ŋߗ͂����Ă������ŁA\n�̒n�������|�I�ɑ����B\n���̈ʒu���嗤�̒����̂���\n��������_���₷��\n�̒n�� : 7\n��Փx : EASY",
		L"�����A�R���Ƃ��Ƀo�����X��\n��ꂽ���B\n�׍������͂ȍ��Ɉ͂܂�Ă���B\n�̒n�� : 4\n��Փx : NORMAL",
		L"�D��I�ł͂Ȃ������Ȃ��\n���m��L���Ă���B\n���K�ʂɂ͏����s��������\n�̒n�� : 4\n��Փx : EASY",
	};

	unsigned int col = ColorConvert16(textCol);

	pText_font[2]->SetColor(col);
	pText_font[2]->Print(70.0f, 60.0f, L"%s", countries[cursor]->GetName().c_str());
	pText_font[3]->SetColor(0xffffffff);
	pText_font[3]->Print(30.0f, 80.0f, 
		L"----------------------------\n%s", description[cursor]);
}

void SelectCountryPanel::SetTextState(noDel_ptr<Country> country) {
	textCol = country->gameObject->GetComponent<ImageRenderer>()->GetColor();
}