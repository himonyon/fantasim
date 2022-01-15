/*-----------------------------------------------------------

	Astarクラス
	経路探索クラス

-------------------------------------------------------------*/
#pragma once
namespace nsBattle {
	enum class eAstarFunc {
		ShowSelectSquare,
		SearchShortPass,
	};

	class Astar {
	public:
		std::vector<noDel_ptr<Square>> vCulcList = {};
		std::vector<noDel_ptr<Square>> vOpenList = {};
		std::vector<noDel_ptr<Square>> vCloseList = {};

	public:
		//移動範囲の表示
		Astar();

		//移動範囲の表示
		void ShowMoveRange(noDel_ptr<Square> startSquare, int range);

		//スタート地点から目的地までの最短距離を求め、スタート地点を返す
		void GetCulcedStartSquare(noDel_ptr<Square> start, noDel_ptr<Square> goal);

		//初期化
		void Reset();

	private:
		//移動範囲の表示
		void SearchEnableSquare(noDel_ptr<Square> startSquare, int range);
		void HideMoveRange();

		//スタート地点から目的地までの最短距離を求め、スタート地点を返す
		void SearchShortPath(noDel_ptr<Square> start, noDel_ptr<Square> goal);
		int CulcHeuristicCost(float x, float y, float x2, float y2);
		noDel_ptr<Square> DecideSearchSquare();
	};
}