#include "../../../../../../framework.h"
#include "../../../../../../environment.h"
#include "PlayerTurn.h"

using namespace nsBattle;

void PlayerTurn::Awake() {
	//サウンドマネージャー
	pSoundManager = gameObject->FindGameObject("soundManager")->GetComponent<SoundManager>();

	//ターン切り替え時画像obj
	noDel_ptr<GameObject> _startAnimObj = gameObject->CreateImageObject(SCREEN_WIDTH_CENTER, SCREEN_HEIGHT_CENTER, 800, 300,
		CreateSprite(new Sprite(L"Data/Image/Battle/player_turn.spr")));
	_startAnimObj->SetRenderOrder(100);
	_startAnimObj->GetComponent<ImageRenderer>()->SetColor(1, 1, 1, 0);
	_startAnimObj->AddComponent<Animator>();
	pTurnStartAnim = _startAnimObj->GetComponent<Animator>();
	pTurnStartAnim->AddAnimation("turn",
		new SpriteAnimation(L"Data/Animation/playerTurn.anim"));
}

void PlayerTurn::Start() {
	Turn::Start();

	//ステータスパネルの検索
	pStatusPanel = gameObject->FindGameObject("statusPanel");
	//カーソルの検索
	pCursor = gameObject->FindGameObject("cursor")->GetComponent<Cursor>();

	//コマンドパネル
	noDel_ptr<GameObject> _comPanel = gameObject->CreateObject(0, 0, 0);
	_comPanel->AddComponent<CommandPanel>();
	pCommandPanel = _comPanel->GetComponent<CommandPanel>();

	//スキルセレクトパネル
	noDel_ptr<GameObject> _skillPanel = gameObject->CreateImageObject(300, 200, 500, 300,
		CreateSprite(new Sprite(L"Data/Image/Battle/skill_select_bg.spr")));
	_skillPanel->AddComponent<SelectSkillPanel>();
	pSkillSelectPanel = _skillPanel->GetComponent<SelectSkillPanel>();
}

void PlayerTurn::Update() {
	if (Keyboard::Trg(DIK_F10)) Debug::open = true;

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

	//音楽のループ
	if (curState->state != eTurnState::Battle) {
		if (pSoundManager->IsPlaying("bgm") == false) pSoundManager->Play("bgm");
	}

	//通常時コマンド
	if (curState->state == eTurnState::Field) FieldFunc();
	//通常時以外はステータスパネルを隠す
	if (curState->state != eTurnState::Field) pStatusPanel->GetComponent<StatusPanel>()->Close();

	//キャラ選択されたらコマンドを開く
	if (curState->state == eTurnState::BeforeCommand) BeforeCommandFunc();

	//キャラ移動後のコマンドを開く
	if (curState->state == eTurnState::AfterCommand) AfterCommandFunc();

	//移動処理
	if (curState->state == eTurnState::Move) MoveFunc();

	//スキル選択処理
	if (curState->state == eTurnState::SelectSkill) SelectSkillFunc();

	//スキル処理
	if (curState->state == eTurnState::Skill) SkillFunc();

	//バトル処理
	if (curState->state == eTurnState::Battle) BattleFunc();

	//カメラ移動
	if (curState->state == eTurnState::Field || curState->state == eTurnState::Move ||
		curState->state == eTurnState::Skill) {
		MoveCamera();
	}

	if (curState->state != checkState) {
		checkState = curState->state;
	}
}

void PlayerTurn::TurnInit() {
	Turn::TurnInit();
	pTurnStartAnim->PlayAnim("turn");
}

void PlayerTurn::SelectChara() {
	for (auto& chara : vBattleChara) {
		if (chara->isCursorHovered && chara->IsActionEnable()) {
			//プレイヤーキャラならコマンドを開く
			noDel_ptr<PlayerChara> _playerC = dynamic_noDel_cast<PlayerChara>(chara);
			if (_playerC != NULL) {
				pSoundManager->Play("decide");
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

//各状態での処理
void PlayerTurn::FieldFunc() {
	//カーソル動かす
	if (pCursor->IsEnable() == false) pCursor->SetEnable(true);

	if (curState->state != checkState) {
		//操作説明テキスト変更
		noDel_ptr<Operation> _opr = gameObject->FindGameObject("operation")->GetComponent<Operation>();
		_opr->ResetOperation();
		_opr->AddOperation("decide", L"キャラ選択");
		_opr->AddOperation("option", L"ターン終了");
	}

	//キャラの情報を出す
	ShowCharaInfo();

	if (Input::Trg(InputConfig::input["decide"])) {
		SelectChara();
	}

	//ターン切り替え
	if (Input::Trg(InputConfig::input["option"])) {
		//ステータスパネルは閉じる
		pStatusPanel->GetComponent<StatusPanel>()->Close();
		gameObject->FindGameObject("fieldManager")->
			GetComponent<FieldManager>()->ChangeTurn();
	}
}
void PlayerTurn::BeforeCommandFunc() {
	//すでに開いて入れば終了
	if (pCommandPanel->IsOpen()) return;
	//ステータスパネルは閉じてコマンドパネルを表示
	pStatusPanel->GetComponent<StatusPanel>()->Close();
	pCommandPanel->Open(dynamic_noDel_cast<PlayerChara>(pSelectChara), eCommandPanelType::MoveBefore);
}
void PlayerTurn::AfterCommandFunc() {
	//すでに開いて入れば終了
	if (pCommandPanel->IsOpen()) return;
	//ステータスパネルは閉じてコマンドパネルを表示
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
	//すでに開いて入れば終了
	if (pSkillSelectPanel->IsOpen()) return;
	//ステータスパネルは閉じて
	pStatusPanel->GetComponent<StatusPanel>()->Close();
	//スキルセレクトパネル表示
	pSkillSelectPanel->Open(dynamic_noDel_cast<PlayerChara>(pSelectChara));
}
void PlayerTurn::BattleFunc() {
	//すでに開いて入れば終了
	noDel_ptr<BattlePanel> _pBattlePanel =
		gameObject->FindGameObject("battlePanel")->GetComponent<BattlePanel>();
	if (_pBattlePanel->IsOpen()) return;
	//ステータスパネルは閉じる
	pStatusPanel->GetComponent<StatusPanel>()->Close();
	//バトル画面に移行
	noDel_ptr<PlayerChara> _p = dynamic_noDel_cast<PlayerChara>(pSelectChara);
	noDel_ptr<EnemyChara> _e = dynamic_noDel_cast<EnemyChara>(pSelectChara->pTargetChara);
	_pBattlePanel->Open(_p,_e, true);
}
