#include "../../../../../../framework.h"
#include "../../../../../../environment.h"
#include "BattlePanel.h"

using namespace nsBattle;

void BattlePanel::Awake() {
	float _sizeX = gameObject->GetComponent<ImageRenderer>()->sizeX;
	float _sizeY = gameObject->GetComponent<ImageRenderer>()->sizeY;
	float _top = -_sizeY / 2;
	float _left = -_sizeX / 2;

	gameObject->GetComponent<ImageRenderer>()->SetRenderPriority(-10);

	//キャラクタ(バトル画面に表示するモデル)
	pPlayerView = gameObject->CreateImageObject(0,0, 100.0f, 100.0f, NULL, transform);
	pPlayerView->transform->scale.x *= -1;
	pPlayerView->AddComponent<Animator>();
	noDel_ptr<Animator> _tempAnim = pPlayerView->GetComponent<Animator>();
	_tempAnim->AddAnimation("evd",new SpriteAnimation(L"Data/Animation/playerEvade.anim"));
	_tempAnim->AddAnimation("death",new SpriteAnimation(L"Data/Animation/death.anim"));
	_tempAnim->AddAnimation("damage",new SpriteAnimation(L"Data/Animation/damage.anim"));
	pPlayerView->transform->SetLocalPosition(_left + _sizeX * 0.25f, _top + _sizeY * 0.25f);

	pEnemyView = gameObject->CreateImageObject(0, 0, 100.0f, 100.0f, NULL, transform);
	pEnemyView->AddComponent<Animator>();
	_tempAnim = pEnemyView->GetComponent<Animator>();
	_tempAnim->AddAnimation("evd",new SpriteAnimation(L"Data/Animation/enemyEvade.anim"));
	_tempAnim->AddAnimation("death",new SpriteAnimation(L"Data/Animation/death.anim"));
	_tempAnim->AddAnimation("damage",new SpriteAnimation(L"Data/Animation/damage.anim"));
	pEnemyView->transform->SetLocalPosition(_left + _sizeX * 0.75f, _top + _sizeY * 0.25f);

	//キャラ情報パネル
	noDel_ptr<Sprite> pPanel_sp = CreateSprite(new Sprite(L"Data/Image/Common/menu_button.spr"));
	pCharaInfoPanel[playerNum] = gameObject->CreateImageObject(0, 0, 600.0f, 190.0f, pPanel_sp, transform);
	pCharaInfoPanel[playerNum]->transform->SetLocalPosition(_left + _sizeX * 0.266f, _top + _sizeY * 0.58f);
	pCharaInfoPanel[playerNum]->GetComponent<ImageRenderer>()->SetRenderPriority(-5);
	pCharaInfoPanel[enemyNum] = gameObject->CreateImageObject(0, 0, 600.0f, 190.0f, pPanel_sp, transform);
	pCharaInfoPanel[enemyNum]->transform->SetLocalPosition(_left + _sizeX * 0.734f, _top + _sizeY * 0.58f);
	pCharaInfoPanel[enemyNum]->GetComponent<ImageRenderer>()->SetRenderPriority(-5);
	//バトルテキストパネル
	pBattleTextPanel = gameObject->CreateImageObject(0, 0, 1200.0f, 200.0f, pPanel_sp, transform);
	pBattleTextPanel->transform->SetLocalPosition(_left + _sizeX * 0.5f, _top + _sizeY * 0.84f);
	pBattleTextPanel->GetComponent<ImageRenderer>()->SetRenderPriority(-5);

	//キャラパネルの中身
	SetInfoPanel(playerNum);
	SetInfoPanel(enemyNum);

	//バトルテキスト
	_sizeX = pBattleTextPanel->GetComponent<ImageRenderer>()->sizeX;
	_sizeY = pBattleTextPanel->GetComponent<ImageRenderer>()->sizeY;
	_top = -_sizeY / 2;
	_left = -_sizeX / 2;
	noDel_ptr<GameObject> _textB = gameObject->CreateObject(0, 0, 0, pBattleTextPanel->transform);
	_textB->AddComponent<Font>();
	_textB->transform->SetLocalPosition(_left + _sizeX * 0.05f, _top + _sizeY * 0.2f);
	pBattleText = _textB->GetComponent<Font>();
	pBattleText->SetFontSize(22.0f);

	//アニメーション
	noDel_ptr<GameObject> _atAnim = gameObject->CreateImageObject(0,0, 200.0f, 200.0f, 
		CreateSprite(new Sprite(L"Data/Image/Battle/attack_effect.spr")));
	_atAnim->GetComponent<ImageRenderer>()->SetRenderPriority(100);
	_atAnim->AddComponent<Animator>();
	pAttack_anim = _atAnim->GetComponent<Animator>();
	pAttack_anim->AddAnimation("attack", new SpriteAnimation(L"Data/Animation/attack.anim"));
	pAttack_anim->gameObject->SetObjEnable(false);

	//初期では非表示
	gameObject->SetObjEnable(false);
}

void BattlePanel::Update() {
	timer.Execute();

	if (fBattleFunc(*this) == false) {
		gameObject->FindGameObject("fieldManager")->
			GetComponent<FieldManager>()->SetTurnState(eTurnState::Field);
		Close();
	}
}

void BattlePanel::Open(noDel_ptr<PlayerChara> player, noDel_ptr<EnemyChara> enemy, bool isPlayerAttack) {
	//初期化
	Panel::Open();
	counterTurn = false;
	pAttacker= NULL;
	pDefender = NULL;
	//アタッカーディフェンダー設定
	if (isPlayerAttack) {
		pAttacker = static_noDel_cast<BattleChara>(player);
		pDefender = static_noDel_cast<BattleChara>(enemy);;
	}
	else {
		pAttacker = static_noDel_cast<BattleChara>(enemy);
		pDefender = static_noDel_cast<BattleChara>(player);;
	}
	
	//タイマー開始
	timer.Start();
	onceTrg = true;

	noDel_ptr<Charactor> pPlayerInfo = player->pCharaInfo;
	noDel_ptr<Charactor> pEnemyInfo = enemy->pCharaInfo;

	//キャラ絵の設定
	float _charaSize = 250.0f;
	pPlayerView->GetComponent<ImageRenderer>()->SetUpRenderer2D(_charaSize, _charaSize, pPlayerInfo->GetSprite());
	pPlayerView->GetComponent<ImageRenderer>()->SetColor(1,1,1,1);
	pEnemyView->GetComponent<ImageRenderer>()->SetUpRenderer2D(_charaSize, _charaSize, pEnemyInfo->GetSprite());
	pEnemyView->GetComponent<ImageRenderer>()->SetColor(1,1,1,1);

	//名前/スキル
	std::wstring _skillName = player->pSelectSkill != NULL ? player->pSelectSkill->GetName() : L"攻撃不可";
	pNameText[playerNum]->Print(L"%s : %s", pPlayerInfo->GetName().c_str(), _skillName.c_str());
	_skillName = enemy->pSelectSkill != NULL ? enemy->pSelectSkill->GetName() : L"攻撃不可";
	pNameText[enemyNum]->Print(L"%s : %s", pEnemyInfo->GetName().c_str(), _skillName.c_str());

	//属性
	float _attrSize = 25.0f;
	pAttrObj[playerNum]->GetComponent<ImageRenderer>()->SetUpRenderer2D(_attrSize, _attrSize, pPlayerInfo->GetAttr()->GetSprite());
	pAttrObj[enemyNum]->GetComponent<ImageRenderer>()->SetUpRenderer2D(_attrSize, _attrSize, pEnemyInfo->GetAttr()->GetSprite());

	//HP、MPバーの設定
	SetBarPosition(pHPBar[playerNum]->GetComponent<ImageRenderer>(), pHPFrame[playerNum]->GetComponent<ImageRenderer>(),
		pPlayerInfo->hp, pPlayerInfo->maxHp);
	SetBarPosition(pMPBar[playerNum]->GetComponent<ImageRenderer>(), pMPFrame[playerNum]->GetComponent<ImageRenderer>(),
		pPlayerInfo->mp, pPlayerInfo->maxMp);
	SetBarPosition(pHPBar[enemyNum]->GetComponent<ImageRenderer>(), pHPFrame[enemyNum]->GetComponent<ImageRenderer>(),
		pEnemyInfo->hp, pEnemyInfo->maxHp);
	SetBarPosition(pMPBar[enemyNum]->GetComponent<ImageRenderer>(), pMPFrame[enemyNum]->GetComponent<ImageRenderer>(),
		pEnemyInfo->mp, pEnemyInfo->maxMp);

	pHPText[playerNum]->Print(L"%d/%d", pPlayerInfo->hp, pPlayerInfo->maxHp);
	pMPText[playerNum]->Print(L"%d/%d", pPlayerInfo->mp, pPlayerInfo->maxMp);
	pHPText[enemyNum]->Print(L"%d/%d", pEnemyInfo->hp, pEnemyInfo->maxHp);
	pMPText[enemyNum]->Print(L"%d/%d", pEnemyInfo->mp, pEnemyInfo->maxMp);

	//バトル処理を開始
	fBattleFunc = &BattlePanel::InitFunc;
}

void BattlePanel::SetInfoPanel(int index) {
	float _sizeX = pCharaInfoPanel[index]->GetComponent<ImageRenderer>()->sizeX;
	float _sizeY = pCharaInfoPanel[index]->GetComponent<ImageRenderer>()->sizeY;
	float _top = -_sizeY / 2;
	float _left = -_sizeX / 2;

	noDel_ptr<Sprite> pHP_sp = CreateSprite(new Sprite(L"Data/Image/Battle/hp_bar.spr"));
	noDel_ptr<Sprite> pMP_sp = CreateSprite(new Sprite(L"Data/Image/Battle/mp_bar.spr"));
	noDel_ptr<Sprite> pFrame_sp = CreateSprite(new Sprite(L"Data/Image/Battle/hp_frame.spr"));

	float _localBarPosX = _left + _sizeX * 0.4f;
	float _localHPPosY = _top + _sizeY * 0.5f;
	float _localMPPosY = _top + _sizeY * 0.7f;

	//名前
	noDel_ptr<GameObject> _nameObj = gameObject->CreateObject(0, 0, 0, pCharaInfoPanel[index]->transform);
	_nameObj->transform->SetLocalPosition(_left + _sizeX * 0.12f, _top + _sizeY * 0.2f);
	_nameObj->AddComponent<Font>();
	pNameText[index] = _nameObj->GetComponent<Font>();
	pNameText[index]->SetFontSize(20.0f);

	//属性
	pAttrObj[index] = gameObject->CreateImageObject(0, 0, 0, 0, NULL, pCharaInfoPanel[index]->transform);
	pAttrObj[index]->transform->SetLocalPosition(_left + _sizeX * 0.7f, _top + _sizeY * 0.25f);

	//HPバー
	pHPBar[index] = gameObject->CreateImageObject(0, 0, _sizeX * 0.6f, 18.0f, pHP_sp, pCharaInfoPanel[index]->transform);
	pHPBar[index]->transform->SetLocalPosition(_localBarPosX, _localHPPosY);

	//フレーム
	pHPFrame[index] = gameObject->CreateImageObject(0, 0, _sizeX * 0.6f, 18.0f, pFrame_sp, pCharaInfoPanel[index]->transform);
	pHPFrame[index]->transform->SetLocalPosition(_localBarPosX, _localHPPosY);
	pHPFrame[index]->GetComponent<ImageRenderer>()->SetRenderPriority(1);

	//HPテキスト
	noDel_ptr<GameObject> _HPText = gameObject->CreateObject(0, 0, 0, pCharaInfoPanel[index]->transform);
	_HPText->transform->SetLocalPosition(_left + _sizeX * 0.8f, _top + _sizeY * 0.47f);
	_HPText->AddComponent<Font>();
	pHPText[index] = _HPText->GetComponent<Font>();

	//MPバー
	pMPBar[index] = gameObject->CreateImageObject(0, 0, _sizeX * 0.6f, 18.0f, pMP_sp, pCharaInfoPanel[index]->transform);
	pMPBar[index]->transform->SetLocalPosition(_localBarPosX, _localMPPosY);

	//フレーム
	pMPFrame[index] = gameObject->CreateImageObject(0, 0, _sizeX * 0.6f, 18.0f, pFrame_sp, pCharaInfoPanel[index]->transform);
	pMPFrame[index]->transform->SetLocalPosition(_localBarPosX, _localMPPosY);
	pMPFrame[index]->GetComponent<ImageRenderer>()->SetRenderPriority(1);

	//MPテキスト
	noDel_ptr<GameObject> _MPText = gameObject->CreateObject(0, 0, 0, pCharaInfoPanel[index]->transform);
	_MPText->transform->SetLocalPosition(_left + _sizeX * 0.8f, _top + _sizeY * 0.67f);
	_MPText->AddComponent<Font>();
	pMPText[index] = _MPText->GetComponent<Font>();
}

void BattlePanel::SetBarPosition(noDel_ptr<ImageRenderer> pImage, noDel_ptr<ImageRenderer> pFrame, int curVal, int maxVal) {
	//HP,MPバーのスケーリングと調整
	float _barSize = pImage->sizeX;
	float _scale = (float)curVal / (float)maxVal;
	pImage->transform->scale.x = _scale;
	pImage->transform->position.x = pFrame->transform->position.x - ((_barSize - (_barSize * _scale)) / 2);
}

//バトル開始時処理
bool BattlePanel::InitFunc() {
	//一度だけ処理したい変更
	if (onceTrg) {
		pBattleText->Print(L"戦闘を開始します...");
		onceTrg = false;
	}

	//時間がたつと次の処理へ
	if (timer.time > 5000) {
		timer.Start();
		fBattleFunc = &BattlePanel::Attack;
		onceTrg = true;
	}

	return true;
}

//プレイヤーキャラの処理
bool BattlePanel::Attack() {
	//一度だけ処理したい変更
	if (onceTrg) {
		//テキスト変更
		pBattleText->Print(L"%sの攻撃 : %s", pAttacker->pCharaInfo->GetName().c_str(),
			pAttacker->pSelectSkill->GetName().c_str());
		//ダメージ計算
		Battle(pAttacker, pDefender);

		//アニメーション再生
		pAttack_anim->gameObject->SetObjEnable(true);

		//ディフェンダー側の描画オブジェクト
		noDel_ptr<GameObject> _pDefenderView;
		if (dynamic_noDel_cast<PlayerChara>(pDefender) != NULL) _pDefenderView = pPlayerView;
		else _pDefenderView = pEnemyView;

		pAttack_anim->transform->position = _pDefenderView->transform->position;
		pAttack_anim->PlayAnim("attack");

		if (evadeTrg) _pDefenderView->GetComponent<Animator>()->PlayAnim("evd");

		onceTrg = false;
	}

	if (pAttack_anim->IsPlayAnim("attack") == false) {
		pAttack_anim->gameObject->SetObjEnable(false);
	}

	//時間がたつと次の処理へ
	if (timer.time > 2000) {
		timer.Start();
		fBattleFunc = &BattlePanel::Damage;
		onceTrg = true;
	}
	return true;
}
bool BattlePanel::Damage() {
	//一度だけ処理したい変更
	if (onceTrg) {
		//テキスト変更
		if (pDefender->IsDeath())
			pBattleText->Print(L"%s%sに%dのダメージ\n%sは死亡した。",
				attrText.c_str(), pDefender->pCharaInfo->GetName().c_str(), damage,
				pDefender->pCharaInfo->GetName().c_str());
		else if(evadeTrg == false)
			pBattleText->Print(L"%s%sに%dのダメージ", 
				attrText.c_str(), pDefender->pCharaInfo->GetName().c_str(), damage);
		else
			pBattleText->Print(L"%sは回避した。", pDefender->pCharaInfo->GetName().c_str());

		//回避トリガーリセット
		evadeTrg = false;

		//アニメーション
		if (evadeTrg == false) {
			noDel_ptr<GameObject> _pDefenderView;
			if (dynamic_noDel_cast<PlayerChara>(pDefender) != NULL) _pDefenderView = pPlayerView;
			else _pDefenderView = pEnemyView;

			if (pDefender->IsDeath()) _pDefenderView->GetComponent<Animator>()->PlayAnim("death");
			else _pDefenderView->GetComponent<Animator>()->PlayAnim("damage");
		}

		//ステータス反映
		SetStatusBar();
		onceTrg = false;
	}

	//時間がたつと次の処理へ
	if (timer.time > 2000) {
		timer.Start();
		//死亡したら終了
		if (pDefender->IsDeath()) fBattleFunc = &BattlePanel::ResultFunc;
		//敵が先行かつ攻撃可能な場合反撃
		else if (!counterTurn && pDefender->pSelectSkill != NULL) {
			counterTurn = true;
			fBattleFunc = &BattlePanel::Attack;
		}
		//それ以外は終了
		else fBattleFunc = &BattlePanel::ResultFunc;

		//アタッカー交代
		ChangeAttacker();

		onceTrg = true;
	}
	return true;
}

//結果処理
bool BattlePanel::ResultFunc() {
	if (timer.time > 2000) return false;
	return true;
}

//ダメージ計算
void BattlePanel::Battle(noDel_ptr<BattleChara> attacker, noDel_ptr<BattleChara> defender) {
	srand((unsigned int)time(NULL));

	//初期化
	damage = 0;
	float _buffRate = 1;

	//スキル攻撃力
	int _skillAt = dynamic_noDel_cast<AttackSkill>(attacker->pSelectSkill)->GetAt();
	//総合攻撃力(スキル攻撃力＊キャラの個体値)
	int _at = _skillAt * attacker->pCharaInfo->power / 100;
	//総合攻撃力(バフ込み)
	_buffRate = 1; //初期化
	if (attacker->pCharaInfo->GetBuff(eBuffType::AT) != NULL)
		_buffRate = attacker->pCharaInfo->GetBuff(eBuffType::AT)->value;
	_at = (int)((float)_at * _buffRate);

	//命中力
	int _hitRate = dynamic_noDel_cast<AttackSkill>(attacker->pSelectSkill)->GetHit();

	//防御力
	int _def = defender->pCharaInfo->def;
	//防御力(バフ込み)
	_buffRate = 1; //初期化
	if (defender->pCharaInfo->GetBuff(eBuffType::DEF) != NULL)
		_buffRate = defender->pCharaInfo->GetBuff(eBuffType::DEF)->value;
	_def = (int)((float)_def * _buffRate);

	//回避力
	int _evd = defender->pCharaInfo->evd;
	//回避力(バフ込み)
	_buffRate = 1; //初期化
	if (defender->pCharaInfo->GetBuff(eBuffType::EVD) != NULL)
		_buffRate = defender->pCharaInfo->GetBuff(eBuffType::EVD)->value;
	_evd = (int)((float)_evd * _buffRate);

	//回避判定--------------------------------------------------
	int _battleHitRate = _hitRate - _evd;
	int _random = rand() % 100;
	if (_battleHitRate < _random) evadeTrg = true;

	//ダメージ計算-----------------------------------------------
	//属性相性
	if (evadeTrg == false) {
		float attrRate = attacker->pSelectSkill->GetAttr()->
			GetDamageRate(defender->pCharaInfo->GetAttr()->GetName());
		int _totalAt = _at - _def;
		damage = _totalAt;
		//最小は１
		if (damage < 1) damage = 1;
		defender->pCharaInfo->hp -= damage;
		if (defender->pCharaInfo->hp < 1) {
			defender->pCharaInfo->hp = 0;
			defender->Death();
		}

		//バトルテキスト
		attrText = L"";
		if (attrRate > 1) attrText = L"効果抜群だ！！\n";
		if (attrRate < 1) attrText = L"効果はいまひとつだ、、、\n";
	}

	//MP処理
	attacker->pCharaInfo->mp -= attacker->pSelectSkill->GetConsumeMP();
	if (attacker->pCharaInfo->mp < 1) defender->pCharaInfo->mp = 0;
}

//ステータスバーのセット
void BattlePanel::SetStatusBar() {
	noDel_ptr<BattleChara> _pPlayer;
	noDel_ptr<BattleChara> _pEnemy;
	if (dynamic_noDel_cast<PlayerChara>(pAttacker) != NULL) {
		_pPlayer = pAttacker;
		_pEnemy = pDefender;
	}
	else {
		_pPlayer = pDefender;
		_pEnemy = pAttacker;
	}

	pHPText[playerNum]->Print(L"%d/%d", _pPlayer->pCharaInfo->hp, _pPlayer->pCharaInfo->maxHp);
	pMPText[playerNum]->Print(L"%d/%d", _pPlayer->pCharaInfo->mp, _pPlayer->pCharaInfo->maxMp);
	pHPText[enemyNum]->Print(L"%d/%d", _pEnemy->pCharaInfo->hp, _pEnemy->pCharaInfo->maxHp);
	pMPText[enemyNum]->Print(L"%d/%d", _pEnemy->pCharaInfo->mp, _pEnemy->pCharaInfo->maxMp);

	SetBarPosition(pHPBar[playerNum]->GetComponent<ImageRenderer>(), pHPFrame[playerNum]->GetComponent<ImageRenderer>(),
		_pPlayer->pCharaInfo->hp, _pPlayer->pCharaInfo->maxHp);
	SetBarPosition(pMPBar[playerNum]->GetComponent<ImageRenderer>(), pMPFrame[playerNum]->GetComponent<ImageRenderer>(),
		_pPlayer->pCharaInfo->mp, _pPlayer->pCharaInfo->maxMp);

	SetBarPosition(pHPBar[enemyNum]->GetComponent<ImageRenderer>(), pHPFrame[enemyNum]->GetComponent<ImageRenderer>(),
		_pEnemy->pCharaInfo->hp, _pEnemy->pCharaInfo->maxHp);
	SetBarPosition(pMPBar[enemyNum]->GetComponent<ImageRenderer>(), pMPFrame[enemyNum]->GetComponent<ImageRenderer>(),
		_pEnemy->pCharaInfo->mp, _pEnemy->pCharaInfo->maxMp);
}

//アタッカー変更
void BattlePanel::ChangeAttacker() {
	noDel_ptr<BattleChara> _tempAttacker = pAttacker;
	pAttacker = pDefender;
	pDefender = _tempAttacker;
}