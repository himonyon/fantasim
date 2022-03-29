#include "../../../../../framework.h"
#include "../../../../../environment.h"
#include "FieldManager.h"

using namespace nsBattle;

const float FieldManager::StageSize = 1;
const float FieldManager::StageLeft = -10;
const float FieldManager::StageTop = 8;
int FieldManager::SquareNum_X = 0;
int FieldManager::SquareNum_Y = 0;

FieldManager::~FieldManager() {
	for (auto& stage : umStageRows) {
		delete stage.second;
	}
	umStageRows.clear();
}

//Behaviour�֐�-----------------------------------
void FieldManager::Awake() {
	//�T�E���h�}�l�[�W���[
	CreateSound();
	//����������
	noDel_ptr<GameObject> _pOprObj = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT - 25, SCREEN_WIDTH, 50.0f,
		CreateSprite(new Sprite(L"Data/Image/Common/cover.spr")), nullptr, "operation");
	_pOprObj->SetRenderOrder((int)eRenderOrder::FrontUI);
	_pOprObj->GetComponent<ImageRenderer>()->SetColor(1, 1, 1, 0.5f);
	_pOprObj->AddComponent<Operation>();
	pOperation = _pOprObj->GetComponent<Operation>();
}

void FieldManager::Start() {
	//�X�e�[�W�E�L�����N�^�[�쐬
	InitStage();
	InitCharactor();

	//�J�����̏����ʒu�ݒ�
	Camera::main->transform->position.x = umStageRows[SquareNum_Y / 2]->umRow[SquareNum_X / 2]->transform->position.x;
	Camera::main->transform->position.y = umStageRows[SquareNum_Y / 2]->umRow[SquareNum_X / 2]->transform->position.y;

	//�o�g���L�����̃R���|�[�l���g�擾
	for (auto& chara : vPlayerChara) {
		vBattleChara.emplace_back(chara->GetComponent<BattleChara>());
	}

	for (auto& chara : vEnemyChara) {
		vBattleChara.emplace_back(chara->GetComponent<BattleChara>());
	}

	//�^�[���}�l�[�W���[�̍쐬
	noDel_ptr<GameObject> _turnP = gameObject->CreateObject(0, 0, 0);
	_turnP->AddComponent<PlayerTurn>();
	pPlayerTurn = _turnP->GetComponent<PlayerTurn>();
	pPlayerTurn->vBattleChara = vBattleChara;
	noDel_ptr<GameObject> _turnE = gameObject->CreateObject(0, 0, 0);
	_turnE->AddComponent<EnemyTurn>();
	pEnemyTurn = _turnE->GetComponent<EnemyTurn>();
	pEnemyTurn->vBattleChara = vBattleChara;

	//�v���C���[�^�[���J�n
	pEnemyTurn->SetEnable(false);
	static_noDel_cast<Turn>(pPlayerTurn)->TurnInit();

	//BGM�X�^�[�g
	pSoundManager->Play("bgm");
}

void FieldManager::ChangeTurn() {
	if (pPlayerTurn->IsEnable()) {
		pPlayerTurn->SetEnable(false);
		pEnemyTurn->SetEnable(true);
		static_noDel_cast<Turn>(pEnemyTurn)->TurnInit();
	}
	else {
		pEnemyTurn->SetEnable(false);
		pPlayerTurn->SetEnable(true);
		static_noDel_cast<Turn>(pPlayerTurn)->TurnInit();
		//�o�t�J�E���g�����炷
		for (auto& ch : vBattleChara) {
			if (ch->IsDeath()) continue;
			ch->DecreaseBuffCount();
		}
	}
}

//�^�[���̏�ԑJ��
void FieldManager::SetTurnState(eTurnState state, int backNum) {
	if (pPlayerTurn->IsEnable()) pPlayerTurn->SetTurnState(state, backNum);
	else pEnemyTurn->SetTurnState(state, backNum);
}

//Init�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
void FieldManager::InitStage() {
	FILE* fp;

	std::string _stageFile = ChooseStage();

	fopen_s(&fp, _stageFile.c_str(), "r");

	if (fp == NULL) return;

	//�t�B�[���h�摜
	noDel_ptr<Sprite> fieldSprite = GetRoad();
	//��Q���摜
	noDel_ptr<Sprite> obstacleSprite = GetObstacle();
	//�o�g����ʂ̔w�i���ݒ�
	noDel_ptr<ImageRenderer> _pBattlePanel = gameObject->FindGameObject("battlePanel")->GetComponent<ImageRenderer>();
	std::wstring _battleBG[3] = {
		L"Data/Image/Battle/battle_bg_0.spr",
		L"Data/Image/Battle/battle_bg_1.spr",
		L"Data/Image/Battle/battle_bg_2.spr",
	};
	_pBattlePanel->SetUpImageRenderer(SCREEN_WIDTH, SCREEN_HEIGHT,
		CreateSprite(new Sprite(_battleBG[stageType - 1].c_str())));
	//�I�����摜
	noDel_ptr<Sprite> selectSquare = CreateSprite(new Sprite(L"Data/Image/Battle/square.spr"));

	//�}�X�̍��W
	float _posX = StageLeft;
	float _posY = StageTop;
	//�}�X�̐�
	int _curNum = 0;
	//���݂̃}�X�̐�YX
	int _squareX_num = 0;
	int _squareY_num = 0;

	//�}�X�̍s�N���X�̃|�C���^
	SquareRow* _pCurRow = new SquareRow();

	while (!feof(fp)) {
		int _type = 100;

		//�L�[���[�h�ǂݍ���
		fscanf_s(fp, "%d,", &_type);

		if (_type == 100) break;

		Square* _pSquare = NULL;
		noDel_ptr<GameObject> _pSquareObj; //�}�X�I�u�W�F
		noDel_ptr<GameObject> _pSelectSquareObj; //�}�X�I�����I�u�W�F

		//��Q��
		if (_type == 0) {
			//�I�u�W�F�N�g�ƃ}�X�N���X�쐬
			_pSquareObj = gameObject->CreateObject(_posX, _posY, 0,  obstacleSprite);
			_pSelectSquareObj = gameObject->CreateObject(_posX, _posY, 0, selectSquare);
			_pSquare = new Square(_pSquareObj, _pSelectSquareObj);
			//�ړ��R�X�g�̐ݒ�
			_pSquare->SetMoveCost(0);
		}
		//��
		else if (_type == 1) {
			//�I�u�W�F�N�g�ƃ}�X�N���X�쐬
			_pSquareObj = gameObject->CreateObject(_posX, _posY, 0, fieldSprite);
			_pSelectSquareObj = gameObject->CreateObject(_posX, _posY, 0,  selectSquare);
			_pSquare = new Square(_pSquareObj, _pSelectSquareObj);
			//�ړ��R�X�g�̐ݒ�
			_pSquare->SetMoveCost(1);
		}

		//�X�V
		if (_type == -1) { 
			//�V���ȍs�̍쐬�Ɗ����̓o�^
			umStageRows[_squareY_num] = _pCurRow;
			_pCurRow = new SquareRow();

			//�E�[�̏ꍇ�͈ʒuX�������ʒu�ɖ߂���Y��i�߂�
			_posX = StageLeft;
			_posY -= StageSize;
			_squareY_num++;
			_squareX_num = 0;
		}
		else {
			//�s�N���X�Ƀ}�X�̓o�^
			if (_pSquare == NULL) continue;
			_pCurRow->umRow[_squareX_num] = _pSquare;

			//�׃}�X���̓o�^
			if (_squareX_num != 0) {
				_pSquare->SetNeighbor(noDel_ptr<Square>(_pCurRow->umRow[_squareX_num - 1]), true);
			}
			if (_squareY_num != 0) {
				_pSquare->SetNeighbor(noDel_ptr<Square>(umStageRows[_squareY_num - 1]->umRow[_squareX_num]), false);
			}
			
			//�}�X�̐����X�V
			_curNum++;
			_squareX_num++;
			_posX += StageSize; //1(�T�C�Y��)�X�V
		}
	}

	//�}�X�̏c���̐�
	SquareNum_X = _curNum / _squareY_num;
	SquareNum_Y = _squareY_num;
}
std::string FieldManager::ChooseStage() {
	srand((unsigned int)time(NULL));
	int _stage = rand() % 3 + 1;
	if (_stage == 1) return "Data/Status/Battle/stage1.csv";
	if (_stage == 2) return "Data/Status/Battle/stage2.csv";
	if (_stage == 3) return "Data/Status/Battle/stage3.csv";
	return "Data/Status/Battle/stage1.csv";
}
noDel_ptr<Sprite> FieldManager::GetRoad(){
	srand((unsigned int)time(NULL));
	int _stage = rand() % 3 + 1;
	stageType = _stage;
	if (stageType == 1) return CreateSprite(new Sprite(L"Data/Image/Battle/map03.spr", L"road1"));
	if (stageType == 2) return CreateSprite(new Sprite(L"Data/Image/Battle/map01.spr", L"road2"));
	if (stageType == 3) return CreateSprite(new Sprite(L"Data/Image/Battle/map01.spr", L"road3"));

	return NULL;
}
noDel_ptr<Sprite> FieldManager::GetObstacle() {
	srand((unsigned int)time(NULL));
	int _stage = rand() % 3 + 1;
	if (stageType == 1) return CreateSprite(new Sprite(L"Data/Image/Battle/map02.spr", L"obj1"));
	if (stageType == 2) return CreateSprite(new Sprite(L"Data/Image/Battle/map01.spr", L"obj2"));
	if (stageType == 3) return CreateSprite(new Sprite(L"Data/Image/Battle/map01.spr", L"obj3"));
	return NULL;
}
void FieldManager::InitCharactor() {
	srand((unsigned int)time(NULL));
	//�v���C���[�L������ݒ�
	int _x = 0;
	int _y = 0;
	//�z�u���ꂽ�L������Y���W�ꗗ
	std::vector<int> _beSetPosY;

	//�z�u�}�X
	noDel_ptr<Square> _pTargetSquare = noDel_ptr<Square>(umStageRows[_y]->umRow[_x]);
	for (auto& chara : vPlayerCharaBace) {
		//�L�����̏�����
		chara->hp = chara->maxHp;
		chara->mp = chara->maxMp;
		
		//�z�u�}�X�ݒ�
		//�ʒu���Ԃ���
		while (true) {
			bool _flag = false;
			_y = rand() % SquareNum_Y;
			for (auto& y : _beSetPosY) if (_y == y) _flag = true;
			if (_flag == false) break;
		}
		_beSetPosY.emplace_back(_y);
		_pTargetSquare = noDel_ptr<Square>(umStageRows[_y]->umRow[_x]);

		noDel_ptr<GameObject> _pChara = gameObject->CreateObject(_pTargetSquare->transform->position.x,
			_pTargetSquare->transform->position.y, 0,  chara->GetSprite());
		_pChara->SetRenderOrder((int)eRenderOrder::FrontObject);
		_pChara->AddComponent<Collider2D>();
		_pChara->GetComponent<Collider2D>()->SetUpCollider2D(StageSize * 0.5f, StageSize * 0.5f, false);
		_pChara->AddComponent<PlayerChara>();
		_pChara->GetComponent<PlayerChara>()->pCharaInfo = chara;
		_pChara->GetComponent<PlayerChara>()->SetCurrentSquare(noDel_ptr<Square>(_pTargetSquare));

		//�v���C���[�L�����͔��]X
		_pChara->transform->scale.x *= -1;

		//�o�^
		vPlayerChara.emplace_back(_pChara);
	}

	//���W�����ݒ�
	_beSetPosY.clear();
	_x = SquareNum_X - 1;
	_y = 0;

	//�G�L������ݒ�
	for (auto& chara : vEnemyCharaBace) {
		//�L�����̏�����
		chara->hp = chara->maxHp;
		chara->mp = chara->maxMp;
		//�z�u�}�X�ݒ�
		while (true) {
			bool _flag = false;
			_y = rand() % SquareNum_Y;
			for (auto& y : _beSetPosY) if (_y == y) _flag = true;
			if (_flag == false) break;
		}
		_beSetPosY.emplace_back(_y);
		_pTargetSquare = noDel_ptr<Square>(umStageRows[_y]->umRow[_x]);

		noDel_ptr<GameObject> _pChara = gameObject->CreateObject(_pTargetSquare->transform->position.x,
			_pTargetSquare->transform->position.y, 0,  chara->GetSprite());
		_pChara->SetRenderOrder((int)eRenderOrder::FrontObject);
		_pChara->AddComponent<Collider2D>();
		_pChara->GetComponent<Collider2D>()->SetUpCollider2D(StageSize * 0.5f, StageSize * 0.5f, false);
		_pChara->AddComponent<EnemyChara>();
		_pChara->GetComponent<EnemyChara>()->pCharaInfo = chara;
		_pChara->GetComponent<EnemyChara>()->SetCurrentSquare(noDel_ptr<Square>(_pTargetSquare));

		//�o�^
		vEnemyChara.emplace_back(_pChara);
	}
}

void FieldManager::CreateSound() {
	noDel_ptr<GameObject> _pSoundObj = gameObject->CreateObject(0, 0, 0, nullptr, "soundManager");
	_pSoundObj->AddComponent<SoundManager>();
	pSoundManager = _pSoundObj->GetComponent<SoundManager>();
	//BGM
	pSoundManager->AddSound("bgm", L"Data/Sound/Battle/bgm.wav");
	//���艹
	pSoundManager->AddSound("decide", L"Data/Sound/Common/circle.wav");
	//�L�����Z����
	pSoundManager->AddSound("cancel", L"Data/Sound/Common/cross.wav");
	//�o�g����BGM
	pSoundManager->AddSound("battle", L"Data/Sound/Battle/battle.wav");
	//�_���[�W
	pSoundManager->AddSound("damage", L"Data/Sound/Battle/damage.wav");
	//���S
	pSoundManager->AddSound("death", L"Data/Sound/Battle/death.wav");
	//�U��
	pSoundManager->AddSound("attack", L"Data/Sound/Battle/attack.wav");

	pSoundManager->SetVolume(0.1f);

	///���[�U�[��񔽉f
	if (UserSetting::sound == false) pSoundManager->SetVolume(0);
}

