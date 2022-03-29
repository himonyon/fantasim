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
void FieldManager::Awake() {
	//サウンドマネージャー
	CreateSound();
	//操作説明画面
	noDel_ptr<GameObject> _pOprObj = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT - 25, SCREEN_WIDTH, 50.0f,
		CreateSprite(new Sprite(L"Data/Image/Common/cover.spr")), nullptr, "operation");
	_pOprObj->SetRenderOrder((int)eRenderOrder::FrontUI);
	_pOprObj->GetComponent<ImageRenderer>()->SetColor(1, 1, 1, 0.5f);
	_pOprObj->AddComponent<Operation>();
	pOperation = _pOprObj->GetComponent<Operation>();
}

void FieldManager::Start() {
	//ステージ・キャラクター作成
	InitStage();
	InitCharactor();

	//カメラの初期位置設定
	Camera::main->transform->position.x = umStageRows[SquareNum_Y / 2]->umRow[SquareNum_X / 2]->transform->position.x;
	Camera::main->transform->position.y = umStageRows[SquareNum_Y / 2]->umRow[SquareNum_X / 2]->transform->position.y;

	//バトルキャラのコンポーネント取得
	for (auto& chara : vPlayerChara) {
		vBattleChara.emplace_back(chara->GetComponent<BattleChara>());
	}

	for (auto& chara : vEnemyChara) {
		vBattleChara.emplace_back(chara->GetComponent<BattleChara>());
	}

	//ターンマネージャーの作成
	noDel_ptr<GameObject> _turnP = gameObject->CreateObject(0, 0, 0);
	_turnP->AddComponent<PlayerTurn>();
	pPlayerTurn = _turnP->GetComponent<PlayerTurn>();
	pPlayerTurn->vBattleChara = vBattleChara;
	noDel_ptr<GameObject> _turnE = gameObject->CreateObject(0, 0, 0);
	_turnE->AddComponent<EnemyTurn>();
	pEnemyTurn = _turnE->GetComponent<EnemyTurn>();
	pEnemyTurn->vBattleChara = vBattleChara;

	//プレイヤーターン開始
	pEnemyTurn->SetEnable(false);
	static_noDel_cast<Turn>(pPlayerTurn)->TurnInit();

	//BGMスタート
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
		//バフカウントを減らす
		for (auto& ch : vBattleChara) {
			if (ch->IsDeath()) continue;
			ch->DecreaseBuffCount();
		}
	}
}

//ターンの状態遷移
void FieldManager::SetTurnState(eTurnState state, int backNum) {
	if (pPlayerTurn->IsEnable()) pPlayerTurn->SetTurnState(state, backNum);
	else pEnemyTurn->SetTurnState(state, backNum);
}

//Initーーーーーーーーーーーーーーーーーーーーーーーーーー
void FieldManager::InitStage() {
	FILE* fp;

	std::string _stageFile = ChooseStage();

	fopen_s(&fp, _stageFile.c_str(), "r");

	if (fp == NULL) return;

	//フィールド画像
	noDel_ptr<Sprite> fieldSprite = GetRoad();
	//障害物画像
	noDel_ptr<Sprite> obstacleSprite = GetObstacle();
	//バトル画面の背景も設定
	noDel_ptr<ImageRenderer> _pBattlePanel = gameObject->FindGameObject("battlePanel")->GetComponent<ImageRenderer>();
	std::wstring _battleBG[3] = {
		L"Data/Image/Battle/battle_bg_0.spr",
		L"Data/Image/Battle/battle_bg_1.spr",
		L"Data/Image/Battle/battle_bg_2.spr",
	};
	_pBattlePanel->SetUpImageRenderer(SCREEN_WIDTH, SCREEN_HEIGHT,
		CreateSprite(new Sprite(_battleBG[stageType - 1].c_str())));
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
			_pSquareObj = gameObject->CreateObject(_posX, _posY, 0,  obstacleSprite);
			_pSelectSquareObj = gameObject->CreateObject(_posX, _posY, 0, selectSquare);
			_pSquare = new Square(_pSquareObj, _pSelectSquareObj);
			//移動コストの設定
			_pSquare->SetMoveCost(0);
		}
		//道
		else if (_type == 1) {
			//オブジェクトとマスクラス作成
			_pSquareObj = gameObject->CreateObject(_posX, _posY, 0, fieldSprite);
			_pSelectSquareObj = gameObject->CreateObject(_posX, _posY, 0,  selectSquare);
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
	//プレイヤーキャラを設定
	int _x = 0;
	int _y = 0;
	//配置されたキャラのY座標一覧
	std::vector<int> _beSetPosY;

	//配置マス
	noDel_ptr<Square> _pTargetSquare = noDel_ptr<Square>(umStageRows[_y]->umRow[_x]);
	for (auto& chara : vPlayerCharaBace) {
		//キャラの初期化
		chara->hp = chara->maxHp;
		chara->mp = chara->maxMp;
		
		//配置マス設定
		//位置かぶり回避
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

		//プレイヤーキャラは反転X
		_pChara->transform->scale.x *= -1;

		//登録
		vPlayerChara.emplace_back(_pChara);
	}

	//座標初期設定
	_beSetPosY.clear();
	_x = SquareNum_X - 1;
	_y = 0;

	//敵キャラを設定
	for (auto& chara : vEnemyCharaBace) {
		//キャラの初期化
		chara->hp = chara->maxHp;
		chara->mp = chara->maxMp;
		//配置マス設定
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

		//登録
		vEnemyChara.emplace_back(_pChara);
	}
}

void FieldManager::CreateSound() {
	noDel_ptr<GameObject> _pSoundObj = gameObject->CreateObject(0, 0, 0, nullptr, "soundManager");
	_pSoundObj->AddComponent<SoundManager>();
	pSoundManager = _pSoundObj->GetComponent<SoundManager>();
	//BGM
	pSoundManager->AddSound("bgm", L"Data/Sound/Battle/bgm.wav");
	//決定音
	pSoundManager->AddSound("decide", L"Data/Sound/Common/circle.wav");
	//キャンセル音
	pSoundManager->AddSound("cancel", L"Data/Sound/Common/cross.wav");
	//バトル時BGM
	pSoundManager->AddSound("battle", L"Data/Sound/Battle/battle.wav");
	//ダメージ
	pSoundManager->AddSound("damage", L"Data/Sound/Battle/damage.wav");
	//死亡
	pSoundManager->AddSound("death", L"Data/Sound/Battle/death.wav");
	//攻撃
	pSoundManager->AddSound("attack", L"Data/Sound/Battle/attack.wav");

	pSoundManager->SetVolume(0.1f);

	///ユーザー情報反映
	if (UserSetting::sound == false) pSoundManager->SetVolume(0);
}

