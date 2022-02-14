#pragma once
/*-----------------------------------------------------------

	Saveラス
	　Strategyのセーブ管理

-------------------------------------------------------------*/

namespace nsStrategy{
	class Save {
	public:
		//領地
		noDel_ptr<City> pCities[TERRITORY_NUM];
		//キャラクター
		std::unordered_map<int, Charactor*> umCharactors;

		//プレイヤー情報保管
		void SaveBaceData();
		//街のステータスセーブ
		void SaveCities();
		//キャラのステータスセーブ
		void SaveCharactor();
		//戦闘する街のIDを保管(p_id = プレイヤーの街ID e_id = 敵の街ID)
		void SaveBattleCities(int p_id, int e_id, bool playerTurn);
	};
};