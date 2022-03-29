/*-----------------------------------------------------------

	Astarクラス
	経路探索クラス

-------------------------------------------------------------*/
#pragma once
namespace nsBattle {
	enum class eAstarFunc {
		MoveRange,
		FindEnemyRange,
		FindFriendRange,
		SearchShortPass,
	};

	class Astar {
	public:
		std::vector<noDel_ptr<Square>> vCulcList = {};
		std::vector<noDel_ptr<Square>> vOpenList = {};
		std::vector<noDel_ptr<Square>> vCloseList = {};

	public:
		Astar();

		//範囲の表示
		void ShowRange(noDel_ptr<Square> startSquare, int range, eAstarFunc type);

		//スタート地点から目的地までの最短距離を求め、親マスを持ったスタート地点マスクラスを返す
		noDel_ptr<Square> GetCulcedStartSquare(noDel_ptr<Square> goal, noDel_ptr<Square> start);
		noDel_ptr<Square> GetCulcedStartSquare(noDel_ptr<Square> goal, noDel_ptr<Square> start, int minSkillRange);

		//目的地までの距離(マスの数)を返す
		int ReturnDistance(noDel_ptr<Square> goal, noDel_ptr<Square> start);

		//初期化
		void Reset();

	private:
		//範囲の表示
		void SearchEnableSquare(noDel_ptr<Square> startSquare, int range, eAstarFunc type);
		void HideMoveRange();

		//スタート地点から目的地までの最短距離を求め、スタート地点を返す
		void SearchShortPath(noDel_ptr<Square> search, noDel_ptr<Square> start, bool unMoveObj);

		//マスから引数distance離れたマスをtargetから近いものを返す
		noDel_ptr<Square> GetSpcDisSquare(noDel_ptr<Square> target, noDel_ptr<Square> start, int distance);

		//周辺マスをOpenリストに格納（unMoveObj : キャラが乗っているマスを検索に含めるか）
		void PushNeighSquare(noDel_ptr<Square> search, noDel_ptr<Square> start, bool unMoveObj);
		int CulcHeuristicCost(float x, float y, float x2, float y2);
		noDel_ptr<Square> DecideSearchSquare();

		//リストにあるか確認する関数
		bool CheckOpenList(noDel_ptr<Square>);
		bool CheckCloseList(noDel_ptr<Square>);
	};
}