/*-----------------------------------------------------------

	デバッグ用

-------------------------------------------------------------*/
#pragma once

class Debug {
public:
	static noDel_ptr<nsStrategy::City> pCities[TERRITORY_NUM];
	static int scene;

	//キャラクターステータス（前のシーンから引き継ぐ）
	static std::vector<noDel_ptr<Charactor>> vChara;

	static bool open;

	static Sprite* pBg;

	//カーソル
	static int rowCursor;
	static int colCursor;

public:
	//デバッグ処理
	static void Execute();

	//ストラテジーでのデバッグ画面
	static void Strategy();
	static void SetStrategy(noDel_ptr<nsStrategy::City>* cities);

	//バトルシーンでのデバッグ画面
	static void Battle();
	static void SetBattle(std::vector<noDel_ptr<Charactor>> p, std::vector<noDel_ptr<Charactor>> e);
};