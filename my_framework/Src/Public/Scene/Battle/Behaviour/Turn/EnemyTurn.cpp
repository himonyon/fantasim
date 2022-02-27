#include "../../../../../../framework.h"
#include "../../../../../../environment.h"
#include "EnemyTurn.h"

using namespace nsBattle;

void EnemyTurn::Awake() {
	//ターン切り替え時画像obj
	noDel_ptr<GameObject> _startAnimObj = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER, 800, 300,
		CreateSprite(new Sprite(L"Data/Image/Battle/enemy_turn.spr")));
	_startAnimObj->GetComponent<ImageRenderer>()->SetRenderPriority(100);
	_startAnimObj->GetComponent<ImageRenderer>()->SetColor(1,1,1,0);
	_startAnimObj->AddComponent<Animator>();
	pTurnStartAnim = _startAnimObj->GetComponent<Animator>();
	pTurnStartAnim->AddAnimation("turn",
		new SpriteAnimation(L"Data/Animation/enemyTurn.anim"));

	//スキルセレクトパネル
	noDel_ptr<GameObject> _skillPanel = gameObject->CreateImageObject(300, 200, 500, 300,
		CreateSprite(new Sprite(L"Data/Image/Battle/skill_select_bg.spr")));
	_skillPanel->AddComponent<SelectCounterPanel>();
	pSelectCounterPanel = _skillPanel->GetComponent<SelectCounterPanel>();
}

void EnemyTurn::Update() {
	//ターン開始アニメーション中は処理しない
	if (pTurnStartAnim->IsPlayAnim("turn")) return;

	//終了確認
	if (curState->state != eTurnState::Battle) {
		CheckGameEnd();
		if (isGameEnd) {
			TransScene();
			return;
		}
	}

	//タイマー処理
	timer.Execute();

	//反撃スキル選択処理
	if (curState->state == eTurnState::SelectSkill) {
		SelectCounterFunc();
		return;
	}
	//バトル処理
	if (curState->state == eTurnState::Battle) {
		BattleFunc();
		return;
	}

	//生存かつ行動可能時の処理
	if (vEnemy[enemyNum]->IsDeath() == false && vEnemy[enemyNum]->IsActionEnable()) {
		//攻撃処理
		if (curState->state == eTurnState::EnemyAttack) {
			BeforeSkillFunc();
			return;
		}

		//移動処理
		if (curState->state == eTurnState::EnemyMove) {
			MoveFunc();
			return;
		}

		//攻撃処理（2回目
		if (curState->state == eTurnState::EnemyAfterAttack) {
			AfterSkillFunc();
			return;
		}

		//行動終了
		if (curState->state == eTurnState::None){
			vEnemy[enemyNum]->SetActionEnable(false);
			return;
		}
	}
	//行動終了時
	else {
		if (timer.time < 1000) return;
		//次の敵を捜索
		enemyNum++;
		//タイマー初期化
		timer.Start();
		//全ての敵の行動を終えるとターン終了
		if (enemyNum == vEnemy.size())
			gameObject->FindGameObject("fieldManager")->
			GetComponent<FieldManager>()->ChangeTurn();
		//行動状態初期化
		SetTurnState(eTurnState::EnemyAttack);
	}
}

//ターゲットにする敵の設定
void EnemyTurn::SetTargetEnemy() {
	//現在の最大値
	float maxDir = 100000;
	//キャラの位置
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

//ターゲットにする味方の設定
void EnemyTurn::SetTargetFriend() {
	//現在の最大値
	float maxDir = 100000;
	vEnemy[enemyNum]->pTargetChara = NULL;
	//キャラの位置
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

//ターン開始時処理
void EnemyTurn::TurnInit() {
	Turn::TurnInit();
	pTurnStartAnim->PlayAnim("turn");
	SetTurnState(eTurnState::EnemyAttack);
	enemyNum = 0;
	//タイマー初期化
	timer.Start();
	//はじめに処理する敵の設定
	for (int i = 0; i < vEnemy.size(); i++) {
		if (vEnemy[enemyNum]->IsDeath()) continue;
		enemyNum = i;
		break;
	}
}

//スキル選択
void EnemyTurn::SelectSkill() {
	//ターゲットの体力が半分以下なら回復スキル設定
	//ターゲット(味方)設定
	vEnemy[enemyNum]->pSelectSkill = NULL;
	SetTargetFriend();
	//ターゲットがない場合終了
	if (vEnemy[enemyNum]->pTargetChara != NULL) {
		int _maxHp = vEnemy[enemyNum]->pTargetChara->pCharaInfo->maxHp;
		int _maxMp = vEnemy[enemyNum]->pTargetChara->pCharaInfo->maxMp;
		int _hp = vEnemy[enemyNum]->pTargetChara->pCharaInfo->hp;
		int _mp = vEnemy[enemyNum]->pTargetChara->pCharaInfo->mp;
		if (_hp < _maxHp / 2) vEnemy[enemyNum]->ChooseSkill(eSkillType::Heal);
		if (_mp < _maxMp / 2) vEnemy[enemyNum]->ChooseSkill(eSkillType::Heal);
	}
	//スキルが設定されていれば終了
	if (vEnemy[enemyNum]->pSelectSkill != NULL) return;

	//攻撃スキル設定
	//ターゲット(敵)設定
	SetTargetEnemy();
	if (vEnemy[enemyNum]->pTargetChara != NULL) {
		vEnemy[enemyNum]->ChooseSkill(eSkillType::Attack);
	}
	//スキルが設定されていれば終了
	if (vEnemy[enemyNum]->pSelectSkill != NULL) return;

	//バフスキル設定（割合で発動）
	//ターゲット(味方)設定
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

//各処理
void EnemyTurn::BeforeSkillFunc() {
	//標的のキャラ
	if (onceTrg == false) SelectSkill();
	//スキル処理
	if (vEnemy[enemyNum]->pTargetChara != NULL) {
		if (vEnemy[enemyNum]->Skill()) {
			onceTrg = false;
			return;
		}
	}
	//移動ターゲット選定
	SetTargetEnemy();
	SetTurnState(eTurnState::EnemyMove);
}
void EnemyTurn::AfterSkillFunc() {
	//標的のキャラ
	if (onceTrg == false) SelectSkill();
	//スキル処理
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
	//すでに開いて入れば終了
	noDel_ptr<BattlePanel> _pBattlePanel =
		gameObject->FindGameObject("battlePanel")->GetComponent<BattlePanel>();
	if (_pBattlePanel->IsOpen()) return;
	//バトル画面に移行
	noDel_ptr<PlayerChara> _p = dynamic_noDel_cast<PlayerChara>(vEnemy[enemyNum]->pTargetChara);
	noDel_ptr<EnemyChara> _e = vEnemy[enemyNum];
	_pBattlePanel->Open(_p, _e, false);
}
void EnemyTurn::SelectCounterFunc() {
	//すでに開いて入れば終了
	if (pSelectCounterPanel->IsOpen()) return;
	//スキルセレクト画面
	pSelectCounterPanel->Open(dynamic_noDel_cast<PlayerChara>(vEnemy[enemyNum]->pTargetChara), vEnemy[enemyNum]->rangeToTarget);
}