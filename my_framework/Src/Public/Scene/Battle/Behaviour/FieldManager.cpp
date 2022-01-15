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

void FieldManager::Start() {
	//�X�e�[�W�E�L�����N�^�[�쐬
	InitStage();
	InitCharactor();

	//�J�����̏����ʒu�ݒ�
	Camera::main->transform->position.x = umStageRows[SquareNum_Y/2]->umRow[SquareNum_X/2]->transform->position.x;
	Camera::main->transform->position.y = umStageRows[SquareNum_Y/2]->umRow[SquareNum_X/2]->transform->position.y;

	//�o�g���L�����̃R���|�[�l���g�擾
	for (auto& chara : vPlayerChara) {
		vBattleChara.emplace_back(chara->GetComponent<BattleChara>());
	}

	for (auto& chara : vEnemyChara) {
		vBattleChara.emplace_back(chara->GetComponent<BattleChara>());
	}

	//�^�[���}�l�[�W���[�̍쐬
	pPlayerTurn = gameObject->CreateObject(0, 0, 0);
	pPlayerTurn->AddComponent<PlayerTurn>();
	pPlayerTurn->GetComponent<PlayerTurn>()->vBattleChara = vBattleChara;
	pEnemyTurn = gameObject->CreateObject(0, 0, 0);
	pEnemyTurn->AddComponent<EnemyTurn>();
	pEnemyTurn->GetComponent<EnemyTurn>()->vBattleChara = vBattleChara;
}

void FieldManager::Update() {
	
}

//Init�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[�[
void FieldManager::InitStage() {
	FILE* fp;

	fopen_s(&fp, "Data/Status/Battle/stage1.csv", "r");

	if (fp == NULL) return;

	//�t�B�[���h�摜
	noDel_ptr<Sprite> fieldSprite = CreateSprite(new Sprite(L"Data/Image/Battle/map03.spr", L"grass"));
	//��Q���摜
	noDel_ptr<Sprite> obstacleSprite = CreateSprite(new Sprite(L"Data/Image/Battle/map02.spr", L"water"));
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
			_pSquareObj = gameObject->CreateObject(_posX, _posY, 0, StageSize, StageSize, obstacleSprite);
			_pSelectSquareObj = gameObject->CreateObject(_posX, _posY, 0, StageSize, StageSize, selectSquare);
			_pSquare = new Square(_pSquareObj, _pSelectSquareObj);
			//�ړ��R�X�g�̐ݒ�
			_pSquare->SetMoveCost(0);
		}
		//��
		else if (_type == 1) {
			//�I�u�W�F�N�g�ƃ}�X�N���X�쐬
			_pSquareObj = gameObject->CreateObject(_posX, _posY, 0, StageSize, StageSize, fieldSprite);
			_pSelectSquareObj = gameObject->CreateObject(_posX, _posY, 0, StageSize, StageSize, selectSquare);
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

void FieldManager::InitCharactor() {
	//�v���C���[�L������ݒ�
	int _x = 0;
	int _y = 0;
	noDel_ptr<Square> _pTargetSquare = noDel_ptr<Square>(umStageRows[_y]->umRow[_x]);
	for (auto& chara : vPlayerCharaBace) {
		//�z�u�}�X�ݒ�
		_pTargetSquare = noDel_ptr<Square>(umStageRows[_y]->umRow[_x]);

		noDel_ptr<GameObject> _pChara = gameObject->CreateObject(_pTargetSquare->transform->position.x,
			_pTargetSquare->transform->position.y, 0, 0.9f, 0.9f, chara->GetSprite());
		_pChara->GetComponent<SpriteRenderer>()->SetRenderPriority((int)eRenderOrder::Object);
		_pChara->AddComponent<Collider2D>();
		_pChara->GetComponent<Collider2D>()->SetUpCollider2D(StageSize * 0.5f, StageSize * 0.5f, false);
		_pChara->AddComponent<PlayerChara>();
		_pChara->GetComponent<PlayerChara>()->pCharaInfo = chara;
		_pChara->GetComponent<PlayerChara>()->SetCurrentSquare(noDel_ptr<Square>(_pTargetSquare));

		//�v���C���[�L�����͔��]X
		_pChara->transform->scale.x *= -1;

		//�o�^
		vPlayerChara.emplace_back(_pChara);
		_y++;
	}

	//���W�����ݒ�
	_x = SquareNum_X - 1;
	_y = 0;

	//�G�L������ݒ�
	for (auto& chara : vEnemyCharaBace) {
		//�z�u�}�X�ݒ�
		_pTargetSquare = noDel_ptr<Square>(umStageRows[_y]->umRow[_x]);

		noDel_ptr<GameObject> _pChara = gameObject->CreateObject(_pTargetSquare->transform->position.x,
			_pTargetSquare->transform->position.y, 0, 0.9f, 0.9f, chara->GetSprite());
		_pChara->GetComponent<SpriteRenderer>()->SetRenderPriority((int)eRenderOrder::Object);
		_pChara->AddComponent<Collider2D>();
		_pChara->GetComponent<Collider2D>()->SetUpCollider2D(StageSize * 0.5f, StageSize * 0.5f, false);
		_pChara->AddComponent<EnemyChara>();
		_pChara->GetComponent<EnemyChara>()->pCharaInfo = chara;
		_pChara->GetComponent<EnemyChara>()->SetCurrentSquare(noDel_ptr<Square>(_pTargetSquare));

		//�o�^
		vEnemyChara.emplace_back(_pChara);
		_y++;
	}
}


