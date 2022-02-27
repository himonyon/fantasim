#include "../../../../../../framework.h"
#include "../../../../../../environment.h"
#include "EnemyTurn.h"

using namespace nsBattle;

void EnemyTurn::Awake() {
	//�^�[���؂�ւ����摜obj
	noDel_ptr<GameObject> _startAnimObj = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER, 800, 300,
		CreateSprite(new Sprite(L"Data/Image/Battle/enemy_turn.spr")));
	_startAnimObj->GetComponent<ImageRenderer>()->SetRenderPriority(100);
	_startAnimObj->GetComponent<ImageRenderer>()->SetColor(1,1,1,0);
	_startAnimObj->AddComponent<Animator>();
	pTurnStartAnim = _startAnimObj->GetComponent<Animator>();
	pTurnStartAnim->AddAnimation("turn",
		new SpriteAnimation(L"Data/Animation/enemyTurn.anim"));

	//�X�L���Z���N�g�p�l��
	noDel_ptr<GameObject> _skillPanel = gameObject->CreateImageObject(300, 200, 500, 300,
		CreateSprite(new Sprite(L"Data/Image/Battle/skill_select_bg.spr")));
	_skillPanel->AddComponent<SelectCounterPanel>();
	pSelectCounterPanel = _skillPanel->GetComponent<SelectCounterPanel>();
}

void EnemyTurn::Update() {
	//�^�[���J�n�A�j���[�V�������͏������Ȃ�
	if (pTurnStartAnim->IsPlayAnim("turn")) return;

	//�I���m�F
	if (curState->state != eTurnState::Battle) {
		CheckGameEnd();
		if (isGameEnd) {
			TransScene();
			return;
		}
	}

	//�^�C�}�[����
	timer.Execute();

	//�����X�L���I������
	if (curState->state == eTurnState::SelectSkill) {
		SelectCounterFunc();
		return;
	}
	//�o�g������
	if (curState->state == eTurnState::Battle) {
		BattleFunc();
		return;
	}

	//�������s���\���̏���
	if (vEnemy[enemyNum]->IsDeath() == false && vEnemy[enemyNum]->IsActionEnable()) {
		//�U������
		if (curState->state == eTurnState::EnemyAttack) {
			BeforeSkillFunc();
			return;
		}

		//�ړ�����
		if (curState->state == eTurnState::EnemyMove) {
			MoveFunc();
			return;
		}

		//�U�������i2���
		if (curState->state == eTurnState::EnemyAfterAttack) {
			AfterSkillFunc();
			return;
		}

		//�s���I��
		if (curState->state == eTurnState::None){
			vEnemy[enemyNum]->SetActionEnable(false);
			return;
		}
	}
	//�s���I����
	else {
		if (timer.time < 1000) return;
		//���̓G��{��
		enemyNum++;
		//�^�C�}�[������
		timer.Start();
		//�S�Ă̓G�̍s�����I����ƃ^�[���I��
		if (enemyNum == vEnemy.size())
			gameObject->FindGameObject("fieldManager")->
			GetComponent<FieldManager>()->ChangeTurn();
		//�s����ԏ�����
		SetTurnState(eTurnState::EnemyAttack);
	}
}

//�^�[�Q�b�g�ɂ���G�̐ݒ�
void EnemyTurn::SetTargetEnemy() {
	//���݂̍ő�l
	float maxDir = 100000;
	//�L�����̈ʒu
	stVector3 _pos = vEnemy[enemyNum]->transform->position;
	for (auto& player : vPlayer) {
		if(player->IsDeath())continue;
		stVector3 _temp = _pos - player->transform->position;
		float _dirX = abs(_temp.x);
		float _dirY = abs(_temp.y);
		if (_dirX + _dirY < maxDir) {
			maxDir = _dirX + _dirY;
			vEnemy[enemyNum]->pTargetChara = static_noDel_cast<BattleChara>(player);
		}
	}
}

//�^�[�Q�b�g�ɂ��閡���̐ݒ�
void EnemyTurn::SetTargetFriend() {
	//���݂̍ő�l
	float maxDir = 100000;
	vEnemy[enemyNum]->pTargetChara = NULL;
	//�L�����̈ʒu
	stVector3 _pos = vEnemy[enemyNum]->transform->position;
	for (auto& enemy : vEnemy) {
		if (enemy->IsDeath())continue;
		if (enemy->pCharaInfo->GetID() == vEnemy[enemyNum]->pCharaInfo->GetID())continue;
		stVector3 _temp = _pos - enemy->transform->position;
		float _dirX = abs(_temp.x);
		float _dirY = abs(_temp.y);
		if (_dirX + _dirY < maxDir) {
			maxDir = _dirX + _dirY;
			vEnemy[enemyNum]->pTargetChara = static_noDel_cast<BattleChara>(enemy);
		}
	}
}

//�^�[���J�n������
void EnemyTurn::TurnInit() {
	Turn::TurnInit();
	pTurnStartAnim->PlayAnim("turn");
	SetTurnState(eTurnState::EnemyAttack);
	enemyNum = 0;
	//�^�C�}�[������
	timer.Start();
	//�͂��߂ɏ�������G�̐ݒ�
	for (int i = 0; i < vEnemy.size(); i++) {
		if (vEnemy[enemyNum]->IsDeath()) continue;
		enemyNum = i;
		break;
	}
}

//�X�L���I��
void EnemyTurn::SelectSkill() {
	//�^�[�Q�b�g�̗̑͂������ȉ��Ȃ�񕜃X�L���ݒ�
	//�^�[�Q�b�g(����)�ݒ�
	vEnemy[enemyNum]->pSelectSkill = NULL;
	SetTargetFriend();
	//�^�[�Q�b�g���Ȃ��ꍇ�I��
	if (vEnemy[enemyNum]->pTargetChara != NULL) {
		int _maxHp = vEnemy[enemyNum]->pTargetChara->pCharaInfo->maxHp;
		int _maxMp = vEnemy[enemyNum]->pTargetChara->pCharaInfo->maxMp;
		int _hp = vEnemy[enemyNum]->pTargetChara->pCharaInfo->hp;
		int _mp = vEnemy[enemyNum]->pTargetChara->pCharaInfo->mp;
		if (_hp < _maxHp / 2) vEnemy[enemyNum]->ChooseSkill(eSkillType::Heal);
		if (_mp < _maxMp / 2) vEnemy[enemyNum]->ChooseSkill(eSkillType::Heal);
	}
	//�X�L�����ݒ肳��Ă���ΏI��
	if (vEnemy[enemyNum]->pSelectSkill != NULL) return;

	//�U���X�L���ݒ�
	//�^�[�Q�b�g(�G)�ݒ�
	SetTargetEnemy();
	if (vEnemy[enemyNum]->pTargetChara != NULL) {
		vEnemy[enemyNum]->ChooseSkill(eSkillType::Attack);
	}
	//�X�L�����ݒ肳��Ă���ΏI��
	if (vEnemy[enemyNum]->pSelectSkill != NULL) return;

	//�o�t�X�L���ݒ�i�����Ŕ����j
	//�^�[�Q�b�g(����)�ݒ�
	srand((unsigned int)time(NULL));
	const int _rate = 20;
	int _rand = rand() & 100;
	if (_rand < _rate) {
		SetTargetFriend();
		if (vEnemy[enemyNum]->pTargetChara != NULL) {
			vEnemy[enemyNum]->ChooseSkill(eSkillType::Buff);
		}
	}
	
	return;
}

//�e����
void EnemyTurn::BeforeSkillFunc() {
	//�W�I�̃L����
	if (onceTrg == false) SelectSkill();
	//�X�L������
	if (vEnemy[enemyNum]->pTargetChara != NULL) {
		if (vEnemy[enemyNum]->Skill()) {
			onceTrg = false;
			return;
		}
	}
	//�ړ��^�[�Q�b�g�I��
	SetTargetEnemy();
	SetTurnState(eTurnState::EnemyMove);
}
void EnemyTurn::AfterSkillFunc() {
	//�W�I�̃L����
	if (onceTrg == false) SelectSkill();
	//�X�L������
	if (vEnemy[enemyNum]->pTargetChara != NULL) {
		if (vEnemy[enemyNum]->Skill()) {
			onceTrg = false;
			return;
		}
	}
	SetTurnState(eTurnState::None);
}
void EnemyTurn::MoveFunc() {
	if (vEnemy[enemyNum]->Move() == false) {
		SetTurnState(eTurnState::EnemyAfterAttack);
	}
}
void EnemyTurn::BattleFunc() {
	//���łɊJ���ē���ΏI��
	noDel_ptr<BattlePanel> _pBattlePanel =
		gameObject->FindGameObject("battlePanel")->GetComponent<BattlePanel>();
	if (_pBattlePanel->IsOpen()) return;
	//�o�g����ʂɈڍs
	noDel_ptr<PlayerChara> _p = dynamic_noDel_cast<PlayerChara>(vEnemy[enemyNum]->pTargetChara);
	noDel_ptr<EnemyChara> _e = vEnemy[enemyNum];
	_pBattlePanel->Open(_p, _e, false);
}
void EnemyTurn::SelectCounterFunc() {
	//���łɊJ���ē���ΏI��
	if (pSelectCounterPanel->IsOpen()) return;
	//�X�L���Z���N�g���
	pSelectCounterPanel->Open(dynamic_noDel_cast<PlayerChara>(vEnemy[enemyNum]->pTargetChara), vEnemy[enemyNum]->rangeToTarget);
}