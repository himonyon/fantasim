#include "../../../../../../framework.h"
#include "../../../../../../environment.h"
#include "PlayerTurn.h"

using namespace nsBattle;

void PlayerTurn::Awake() {
	//�^�[���؂�ւ����摜obj
	noDel_ptr<GameObject> _startAnimObj = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER, 800, 300,
		CreateSprite(new Sprite(L"Data/Image/Battle/player_turn.spr")));
	_startAnimObj->GetComponent<ImageRenderer>()->SetRenderPriority(100);
	_startAnimObj->GetComponent<ImageRenderer>()->SetColor(1, 1, 1, 0);
	_startAnimObj->AddComponent<Animator>();
	pTurnStartAnim = _startAnimObj->GetComponent<Animator>();
	pTurnStartAnim->AddAnimation("turn",
		new SpriteAnimation(L"Data/Animation/playerTurn.anim"));
}

void PlayerTurn::Start() {
	Turn::Start();

	//�X�e�[�^�X�p�l���̌���
	pStatusPanel = gameObject->FindGameObject("statusPanel");
	//�J�[�\���̌���
	pCursor = gameObject->FindGameObject("cursor")->GetComponent<Cursor>();

	//�R�}���h�p�l��
	noDel_ptr<GameObject> _comPanel = gameObject->CreateObject(0, 0, 0);
	_comPanel->AddComponent<CommandPanel>();
	pCommandPanel = _comPanel->GetComponent<CommandPanel>();

	//�X�L���Z���N�g�p�l��
	noDel_ptr<GameObject> _skillPanel = gameObject->CreateImageObject(300, 200, 500, 300,
		CreateSprite(new Sprite(L"Data/Image/Battle/skill_select_bg.spr")));
	_skillPanel->AddComponent<SelectSkillPanel>();
	pSkillSelectPanel = _skillPanel->GetComponent<SelectSkillPanel>();
}

void PlayerTurn::Update() {
	//�^�[���J�n�A�j���[�V�������͏������Ȃ�
	if (pTurnStartAnim->IsPlayAnim("turn")) return;

	//�I���m�F
	CheckGameEnd();
	if (isGameEnd) {
		TransScene();
		return;
	}

	if (Keyboard::Trg(DIK_Q)) vEnemy[0]->pCharaInfo->hp -= 10;
	if (Keyboard::Trg(DIK_W)) vEnemy[1]->pCharaInfo->hp -= 10;
	if (Keyboard::Trg(DIK_E)) vEnemy[2]->pCharaInfo->hp -= 10;
	if (Keyboard::Trg(DIK_A)) vPlayer[0]->pCharaInfo->hp -= 10;
	if (Keyboard::Trg(DIK_S)) vPlayer[1]->pCharaInfo->hp -= 10;
	if (Keyboard::Trg(DIK_D)) vPlayer[2]->pCharaInfo->hp -= 10;

	//�ʏ펞�R�}���h
	if (curState->state == eTurnState::Field) FieldFunc();
	//�ʏ펞�ȊO�̓X�e�[�^�X�p�l�����B��
	if (curState->state != eTurnState::Field) pStatusPanel->GetComponent<StatusPanel>()->Close();

	//�L�����I�����ꂽ��R�}���h���J��
	if (curState->state == eTurnState::BeforeCommand) BeforeCommandFunc();

	//�L�����ړ���̃R�}���h���J��
	if (curState->state == eTurnState::AfterCommand) AfterCommandFunc();

	//�ړ�����
	if (curState->state == eTurnState::Move) MoveFunc();

	//�X�L���I������
	if (curState->state == eTurnState::SelectSkill) SelectSkillFunc();

	//�X�L������
	if (curState->state == eTurnState::Skill) SkillFunc();

	//�o�g������
	if (curState->state == eTurnState::Battle) BattleFunc();

	//�J�����ړ�
	if (curState->state == eTurnState::Field || curState->state == eTurnState::Move ||
		curState->state == eTurnState::Skill) {
		MoveCamera();
	}
}

void PlayerTurn::TurnInit() {
	Turn::TurnInit();
	pTurnStartAnim->PlayAnim("turn");
}

void PlayerTurn::SelectChara() {
	for (auto& chara : vBattleChara) {
		if (chara->isCursorHovered && chara->IsActionEnable()) {
			//�v���C���[�L�����Ȃ�R�}���h���J��
			noDel_ptr<PlayerChara> _playerC = dynamic_noDel_cast<PlayerChara>(chara);
			if (_playerC != NULL) {
				SetTurnState(eTurnState::BeforeCommand);
				pSelectChara = chara;
			}
			break;
		}
	}
}

void PlayerTurn::ShowCharaInfo() {
	for (auto& chara : vBattleChara) {
		if (chara->isCursorHovered && chara->IsDeath() == false) {
			if (pStatusPanel->IsObjEnable() == false) {
				pStatusPanel->GetComponent<StatusPanel>()->Open(chara);
			}
			return;
		}
	}

	if (pStatusPanel->IsObjEnable()) {
		pStatusPanel->GetComponent<StatusPanel>()->Close();
	}
}

//�e��Ԃł̏���
void PlayerTurn::FieldFunc() {
	//�J�[�\��������
	if (pCursor->IsEnable() == false) pCursor->SetEnable(true);

	//�L�����̏����o��
	ShowCharaInfo();

	if (Input::Trg(InputConfig::input["decide"])) {
		SelectChara();
	}

	//�^�[���؂�ւ�
	if (Input::Trg(InputConfig::input["option"])) {
		//�X�e�[�^�X�p�l���͕���
		pStatusPanel->GetComponent<StatusPanel>()->Close();
		gameObject->FindGameObject("fieldManager")->
			GetComponent<FieldManager>()->ChangeTurn();
	}
}
void PlayerTurn::BeforeCommandFunc() {
	//���łɊJ���ē���ΏI��
	if (pCommandPanel->IsOpen()) return;
	//�X�e�[�^�X�p�l���͕��ăR�}���h�p�l����\��
	pStatusPanel->GetComponent<StatusPanel>()->Close();
	pCommandPanel->Open(dynamic_noDel_cast<PlayerChara>(pSelectChara), eCommandPanelType::MoveBefore);
}
void PlayerTurn::AfterCommandFunc() {
	//���łɊJ���ē���ΏI��
	if (pCommandPanel->IsOpen()) return;
	//�X�e�[�^�X�p�l���͕��ăR�}���h�p�l����\��
	pStatusPanel->GetComponent<StatusPanel>()->Close();
	pCommandPanel->Open(dynamic_noDel_cast<PlayerChara>(pSelectChara), eCommandPanelType::MoveAfter);
}
void PlayerTurn::MoveFunc() {
	dynamic_noDel_cast<PlayerChara>(pSelectChara)->Move();
}
void PlayerTurn::SkillFunc() {
	dynamic_noDel_cast<PlayerChara>(pSelectChara)->SkillFunc();
}
void PlayerTurn::SelectSkillFunc() {
	//���łɊJ���ē���ΏI��
	if (pSkillSelectPanel->IsOpen()) return;
	//�X�e�[�^�X�p�l���͕���
	pStatusPanel->GetComponent<StatusPanel>()->Close();
	//�X�L���Z���N�g�p�l���\��
	pSkillSelectPanel->Open(dynamic_noDel_cast<PlayerChara>(pSelectChara));
}
void PlayerTurn::BattleFunc() {
	//���łɊJ���ē���ΏI��
	noDel_ptr<BattlePanel> _pBattlePanel =
		gameObject->FindGameObject("battlePanel")->GetComponent<BattlePanel>();
	if (_pBattlePanel->IsOpen()) return;
	//�X�e�[�^�X�p�l���͕���
	pStatusPanel->GetComponent<StatusPanel>()->Close();
	//�o�g����ʂɈڍs
	noDel_ptr<PlayerChara> _p = dynamic_noDel_cast<PlayerChara>(pSelectChara);
	noDel_ptr<EnemyChara> _e = dynamic_noDel_cast<EnemyChara>(pSelectChara->pTargetChara);
	_pBattlePanel->Open(_p,_e, true);
}
