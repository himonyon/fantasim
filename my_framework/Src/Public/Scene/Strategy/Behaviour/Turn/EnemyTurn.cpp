#include "../../../../../../framework.h"
#include "../../../../../../environment.h"

using namespace nsStrategy;

void EnemyTurn::Start() {
	//ゲームマネージャー
	pGM = gameObject->FindGameObject("gameManager")->GetComponent<GameManager>();

	//バトルパネル
	pBtPanel = gameObject->FindGameObject("battlePanel")->GetComponent<BattlePanel>();
	pInfoPanel = gameObject->FindGameObject("infoPanel")->GetComponent<InfoPanel>();

	//プレイヤーの国
	for (int i = 0; i < TERRITORY_NUM; i++) {
		noDel_ptr<Country> _temp = dynamic_noDel_cast<Country>(pGM->GetCity(i));
		if (_temp == NULL) continue;
		if (_temp->IsPlayer() == false) continue;
		pPlayerCountry = _temp;
		break;
	}
}

void EnemyTurn::Update() {
	if (stopSimulate) return;
	if (pInfoPanel->IsOpen()) return;

	//タイマー処理
	timer.Execute();

	//処理が終わっていれば時間まで何もしない
	if (isSimulated) {
		//次の街をセット
		if (timer.time >= 200){
			//更新
			pTargetCity->FocusCity(false);
			isSimulated = false;
			while (true) {
				curCityID++;
				//すべて完了したらターン切り替え
				if (curCityID >= TERRITORY_NUM) {
					pGM->ChangeTurn();
					return;
				}
				//ターゲットセット
				pTargetCity = pGM->GetCity(curCityID);
				if (pTargetCity->pCountry != pPlayerCountry) break;
			}
			//タイマースタート
			timer.Start();
		}
		return;
	}

	//ターゲットの街のシミュレート
	Simulate();
}

void EnemyTurn::TurnInit() {
	if (pGM == NULL) Start();
	curCityID = 0;
	//ターゲットセット
	while (true) {
		pTargetCity = pGM->GetCity(curCityID);
		if (pTargetCity->pCountry != pPlayerCountry) break;
		curCityID++;
	}
	timer.Start();
}
void EnemyTurn::TurnInit(int id) {
	if (pGM == NULL) Start();
	//途中から
	curCityID = id + 1;
	//すべて完了したらターン切り替え
	if (curCityID >= TERRITORY_NUM) {
		pGM->ChangeTurn();
		return;
	}
	//ターゲットセット
	while (true) {
		pTargetCity = pGM->GetCity(curCityID);
		if (pTargetCity->pCountry != pPlayerCountry) break;
		curCityID++;
	}
	timer.Start();
}

////ターゲットの街のシミュレート
void EnemyTurn::Simulate() {
	//画像のフォーカス
	pTargetCity->FocusCity(true);

	//処理-----------------------------------
	//戦闘
	if (Battle()) isSimulated = true;
	if (isSimulated) return;

	//キャラ派遣
	if (CharaMove()) isSimulated = true;
	if (isSimulated) return;

	//キャラ強化
	if (CharaEnhance()) isSimulated = true;
	if (isSimulated) return;

	//資金移動
	if (MoneyMove()) isSimulated = true;
	if (isSimulated) return;

	//生産力向上
	if (EconomicUp()) isSimulated = true;
	if (isSimulated) return;

	//政策
	if (PoliticalUp()) isSimulated = true;
	if (isSimulated) return;

	//災害対策
	if (DisasterUp()) isSimulated = true;
	if (isSimulated) return;

	//演説
	if (SupportUp()) isSimulated = true;
	if (isSimulated) return;

	isSimulated = true;
}

bool EnemyTurn::Battle() {
	/*条件----------------------------------------------------
	-キャラの合計パワー差が-30より上なら戦闘を仕掛ける
	-確率は40%
	--------------------------------------------------------*/
	//キャラの合計パワー
	int _cityPower = 0;
	for (auto& chara : pTargetCity->vOwnChara) {
		_cityPower += chara->power;
	}

	//隣り合う敵領地の合計キャラパワーを比較
	for (auto& neigh : pTargetCity->GetNeighbor()) {
		//同じ国ならスキップ
		if (neigh->pCountry == pTargetCity->pCountry) continue;
		//隣国の合計キャラパワー算出
		int _neighPower = 0;
		for (auto& chara : neigh->vOwnChara) {
			_neighPower += chara->power;
		}
		//パワー差が-３０より大きいか確認
		if (_cityPower - _neighPower <= -30) continue;
		//40パーセントの確率
		srand((unsigned int) time(NULL));
		int _rand = rand() % 100;
		if (_rand >= 40) continue;
		//戦闘
		//プレイヤーへの戦闘の場合
		if (neigh->pCountry == pPlayerCountry) {
			//戦闘パネルを開く
			pBtPanel->Open(neigh, pTargetCity, false);
			stopSimulate = true;
		}
		//敵同士で戦闘を行う場合
		else {
			//キャラの合計パワーが多いほうに確率を傾ける
			int _rate = 0;
			if (_cityPower > _neighPower) _rate = 70;
			else _rate = 30;

			//戦闘結果を反映
			_rand = rand() % 100;
			//シミュレートしている国の勝利
			if (_rand < _rate) neigh->ChangeBelongCountry(pTargetCity->pCountry);
			//隣国の勝利
			else if (_rand < _rate) pTargetCity->ChangeBelongCountry(neigh->pCountry);
		}

		return true;
	}

	return false;
}
bool EnemyTurn::CharaEnhance() {
	/*条件----------------------------------------------------
	-資金が10000以上
	-確率20%
	--------------------------------------------------------*/

	return false;
}
bool EnemyTurn::CharaMove() {
	/*条件----------------------------------------------------
	-隣の領地に敵領地がない
	-隣の領地のキャラが5体未満
	-確率20%
	--------------------------------------------------------*/

	//キャラがいなければ処理しない
	if (pTargetCity->vOwnChara.size() == 0) return false;

	//隣に領地があるか確認
	for (auto& neigh : pTargetCity->GetNeighbor()) {
		if (neigh->pCountry != pTargetCity->pCountry) return false;
	}

	//隣の領地のキャラが5体未満
	for (auto& neigh : pTargetCity->GetNeighbor()) {
		int _charaNum = (int)neigh->vOwnChara.size();
		if (_charaNum >= OWN_CHARACTOR_NUM) continue;
		//確率20%
		srand((unsigned int)time(NULL));
		int _rand = rand() % 100;
		if (_rand >= 20) continue;

		//キャラ移動----------------
		//街からキャラを抜く
		noDel_ptr<Charactor> _pTempChara = pTargetCity->vOwnChara[0];
		pTargetCity->vOwnChara.erase(pTargetCity->vOwnChara.begin());
		//キャラを移動
		neigh->vOwnChara.emplace_back(_pTempChara);
		return true;
	}

	return false;
}
bool EnemyTurn::MoneyMove() {
	/*条件----------------------------------------------------
	-隣の街の資金が選択中の街の半分以下
	-確率50%
	--------------------------------------------------------*/

	//隣の街の資金が選択中の街の半分以下
	for (auto& neigh : pTargetCity->GetNeighbor()) {
		if (neigh->pCountry != pTargetCity->pCountry) continue;
		if (neigh->money > pTargetCity->money / 2) continue;
		//確率20%
		srand((unsigned int)time(NULL));
		int _rand = rand() % 100;
		if (_rand >= 50) continue;
		//資金移動
		int _moveMoney = pTargetCity->money / 4;
		pTargetCity->money -= _moveMoney;
		neigh->money += _moveMoney;
		return true;
	}
	
	return false;;
}
bool EnemyTurn::EconomicUp() {
	/*条件----------------------------------------------------
	-資金が2000以上
	-確率30%
	--------------------------------------------------------*/

	if (pTargetCity->money <= 2000) return false;
	//確率30%
	srand((unsigned int)time(NULL));
	int _rand = rand() % 100;
	if (_rand >= 30) return false;
	//経済力向上
	pTargetCity->money -= 1000;
	pTargetCity->economic += 1000 / 10;

	return true;
}
bool EnemyTurn::PoliticalUp() {
	/*条件----------------------------------------------------
	-資金が2000以上
	-確率15%
	--------------------------------------------------------*/

	if (pTargetCity->money <= 2000) return false;
	//確率15%
	srand((unsigned int)time(NULL));
	int _rand = rand() % 100;
	if (_rand >= 15) return false;
	//政策向上
	pTargetCity->money -= 1000;
	pTargetCity->political += 1000 / 10;

	return true;
}
bool EnemyTurn::DisasterUp() {
	/*条件----------------------------------------------------
	-資金が500以上
	-災害耐性60以下で50%
	-災害耐性80以下で20%
	-災害耐性90以下で10%
	--------------------------------------------------------*/

	if (pTargetCity->money <= 500) return false;
	//確率
	int _rate = 0;
	if (pTargetCity->disaster <= 60) _rate = 50;
	else if (pTargetCity->disaster <= 80) _rate = 20;
	else if (pTargetCity->disaster <= 90) _rate = 10;

	srand((unsigned int)time(NULL));
	int _rand = rand() % 100;
	if (_rand >= _rate) return false;
	//災害耐性向上
	pTargetCity->money -= 500;
	pTargetCity->disaster += 500 / 100;

	return true;
}
bool EnemyTurn::SupportUp() {
	/*条件----------------------------------------------------
	-資金が500以上
	-支持率60以下で50%
	-支持率80以下で20%
	-支持率90以下で10%
	--------------------------------------------------------*/

	if (pTargetCity->money <= 500) return false;
	//確率
	int _rate = 0;
	if (pTargetCity->support <= 60) _rate = 50;
	else if (pTargetCity->support <= 80) _rate = 20;
	else if (pTargetCity->support <= 90) _rate = 10;

	srand((unsigned int)time(NULL));
	int _rand = rand() % 100;
	if (_rand >= _rate) return false;
	//災害耐性向上
	pTargetCity->money -= 500;
	pTargetCity->support += 500 / 100;

	return true;
}