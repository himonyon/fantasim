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

//Behaviour関数-----------------------------------

void FieldManager::Start() {
	//ステージ・キャラクター作成
	InitStage();
	InitCharactor();

	//カメラの初期位置設定
	Camera::main->transform->position.x = umStageRows[SquareNum_Y/2]->umRow[SquareNum_X/2]->transform->position.x;
	Camera::main->transform->position.y = umStageRows[SquareNum_Y/2]->umRow[SquareNum_X/2]->transform->position.y;

	//バトルキャラのコンポーネント取得
	for (auto& chara : vPlayerChara) {
		vBattleChara.emplace_back(chara->GetComponent<BattleChara>());
	}

	for (auto& chara : vEnemyChara) {
		vBattleChara.emplace_back(chara->GetComponent<BattleChara>());
	}

	//ターンマネージャーの作成
	pPlayerTurn = gameObject->CreateObject(0, 0, 0);
	pPlayerTurn->AddComponent<PlayerTurn>();
	pPlayerTurn->GetComponent<PlayerTurn>()->vBattleChara = vBattleChara;
	pEnemyTurn = gameObject->CreateObject(0, 0, 0);
	pEnemyTurn->AddComponent<EnemyTurn>();
	pEnemyTurn->GetComponent<EnemyTurn>()->vBattleChara = vBattleChara;
}

void FieldManager::Update() {
	
}

//Initーーーーーーーーーーーーーーーーーーーーーーーーーー
void FieldManager::InitStage() {
	FILE* fp;

	fopen_s(&fp, "Data/Status/Battle/stage1.csv", "r");

	if (fp == NULL) return;

	//フィールド画像
	noDel_ptr<Sprite> fieldSprite = CreateSprite(new Sprite(L"Data/Image/Battle/map03.spr", L"grass"));
	//障害物画像
	noDel_ptr<Sprite> obstacleSprite = CreateSprite(new Sprite(L"Data/Image/Battle/map02.spr", L"water"));
	//選択時画像
	noDel_ptr<Sprite> selectSquare = CreateSprite(new Sprite(L"Data/Image/Battle/square.spr"));

	//マスの座標
	float _posX = StageLeft;
	float _posY = StageTop;
	//マスの数
	int _curNum = 0;
	//現在のマスの数YX
	int _squareX_num = 0;
	int _squareY_num = 0;

	//マスの行クラスのポインタ
	SquareRow* _pCurRow = new SquareRow();

	while (!feof(fp)) {
		int _type = 100;

		//キーワード読み込み
		fscanf_s(fp, "%d,", &_type);

		if (_type == 100) break;

		Square* _pSquare = NULL;
		noDel_ptr<GameObject> _pSquareObj; //マスオブジェ
		noDel_ptr<GameObject> _pSelectSquareObj; //マス選択時オブジェ

		//障害物
		if (_type == 0) {
			//オブジェクトとマスクラス作成
			_pSquareObj = gameObject->CreateObject(_posX, _posY, 0, StageSize, StageSize, obstacleSprite);
			_pSelectSquareObj = gameObject->CreateObject(_posX, _posY, 0, StageSize, StageSize, selectSquare);
			_pSquare = new Square(_pSquareObj, _pSelectSquareObj);
			//移動コストの設定
			_pSquare->SetMoveCost(0);
		}
		//道
		else if (_type == 1) {
			//オブジェクトとマスクラス作成
			_pSquareObj = gameObject->CreateObject(_posX, _posY, 0, StageSize, StageSize, fieldSprite);
			_pSelectSquareObj = gameObject->CreateObject(_posX, _posY, 0, StageSize, StageSize, selectSquare);
			_pSquare = new Square(_pSquareObj, _pSelectSquareObj);
			//移動コストの設定
			_pSquare->SetMoveCost(1);
		}

		//更新
		if (_type == -1) { 
			//新たな行の作成と既存の登録
			umStageRows[_squareY_num] = _pCurRow;
			_pCurRow = new SquareRow();

			//右端の場合は位置Xを初期位置に戻してYを進める
			_posX = StageLeft;
			_posY -= StageSize;
			_squareY_num++;
			_squareX_num = 0;
		}
		else {
			//行クラスにマスの登録
			if (_pSquare == NULL) continue;
			_pCurRow->umRow[_squareX_num] = _pSquare;

			//隣マス情報の登録
			if (_squareX_num != 0) {
				_pSquare->SetNeighbor(noDel_ptr<Square>(_pCurRow->umRow[_squareX_num - 1]), true);
			}
			if (_squareY_num != 0) {
				_pSquare->SetNeighbor(noDel_ptr<Square>(umStageRows[_squareY_num - 1]->umRow[_squareX_num]), false);
			}
			
			//マスの数を更新
			_curNum++;
			_squareX_num++;
			_posX += StageSize; //1(サイズ分)更新
		}
	}

	//マスの縦横の数
	SquareNum_X = _curNum / _squareY_num;
	SquareNum_Y = _squareY_num;
}

void FieldManager::InitCharactor() {
	//プレイヤーキャラを設定
	int _x = 0;
	int _y = 0;
	noDel_ptr<Square> _pTargetSquare = noDel_ptr<Square>(umStageRows[_y]->umRow[_x]);
	for (auto& chara : vPlayerCharaBace) {
		//配置マス設定
		_pTargetSquare = noDel_ptr<Square>(umStageRows[_y]->umRow[_x]);

		noDel_ptr<GameObject> _pChara = gameObject->CreateObject(_pTargetSquare->transform->position.x,
			_pTargetSquare->transform->position.y, 0, 0.9f, 0.9f, chara->GetSprite());
		_pChara->GetComponent<SpriteRenderer>()->SetRenderPriority((int)eRenderOrder::Object);
		_pChara->AddComponent<Collider2D>();
		_pChara->GetComponent<Collider2D>()->SetUpCollider2D(StageSize * 0.5f, StageSize * 0.5f, false);
		_pChara->AddComponent<PlayerChara>();
		_pChara->GetComponent<PlayerChara>()->pCharaInfo = chara;
		_pChara->GetComponent<PlayerChara>()->SetCurrentSquare(noDel_ptr<Square>(_pTargetSquare));

		//プレイヤーキャラは反転X
		_pChara->transform->scale.x *= -1;

		//登録
		vPlayerChara.emplace_back(_pChara);
		_y++;
	}

	//座標初期設定
	_x = SquareNum_X - 1;
	_y = 0;

	//敵キャラを設定
	for (auto& chara : vEnemyCharaBace) {
		//配置マス設定
		_pTargetSquare = noDel_ptr<Square>(umStageRows[_y]->umRow[_x]);

		noDel_ptr<GameObject> _pChara = gameObject->CreateObject(_pTargetSquare->transform->position.x,
			_pTargetSquare->transform->position.y, 0, 0.9f, 0.9f, chara->GetSprite());
		_pChara->GetComponent<SpriteRenderer>()->SetRenderPriority((int)eRenderOrder::Object);
		_pChara->AddComponent<Collider2D>();
		_pChara->GetComponent<Collider2D>()->SetUpCollider2D(StageSize * 0.5f, StageSize * 0.5f, false);
		_pChara->AddComponent<EnemyChara>();
		_pChara->GetComponent<EnemyChara>()->pCharaInfo = chara;
		_pChara->GetComponent<EnemyChara>()->SetCurrentSquare(noDel_ptr<Square>(_pTargetSquare));

		//登録
		vEnemyChara.emplace_back(_pChara);
		_y++;
	}
}


