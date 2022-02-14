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
		//範囲の表示
		Astar();

		//範囲の表示
		void ShowRange(noDel_ptr<Square> startSquare, int range, eAstarFunc type);

		//スタート地点から目的地までの最短距離を求め、スタート地点を返す
		void GetCulcedStartSquare(noDel_ptr<Square> start, noDel_ptr<Square> goal);

		//目的地までの距離(マスの数)を返す
		int ReturnDistance(noDel_ptr<Square> start, noDel_ptr<Square> goal);

		//初期化
		void Reset();

	private:
		//範囲の表示
		void SearchEnableSquare(noDel_ptr<Square> startSquare, int range, eAstarFunc type);
		void HideMoveRange();

		//スタート地点から目的地までの最短距離を求め、スタート地点を返す
		void SearchShortPath(noDel_ptr<Square> start, noDel_ptr<Square> goal);
		int CulcHeuristicCost(float x, float y, float x2, float y2);
		noDel_ptr<Square> DecideSearchSquare();

		//目的地までの距離(マスの数)を返す
		void GetDistanceToGoal(noDel_ptr<Square> start, noDel_ptr<Square> goal);

		//リストにあるか確認する関数
		bool CheckOpenList(noDel_ptr<Square>);
		bool CheckCloseList(noDel_ptr<Square>);
	};
}