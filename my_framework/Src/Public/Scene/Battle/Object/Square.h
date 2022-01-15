/*-----------------------------------------------------------

	Squareクラス
	ステージの各マスの情報

-------------------------------------------------------------*/
#pragma once
namespace nsBattle {
	enum class eNeighborDir {
		Left,
		Top,
		Right,
		Bottom,
		Max,
	};

	class Square {
	private:
		static int ID_Count;

	private:
		//マスのID
		int id = 0;

		//マスに何か移動不可オブジェクトが乗っているか
		bool OnUnMoveObj = false;
		 
		
		//マスのオブジェクト
		noDel_ptr<GameObject> pSquare;
		//選択時のマスオブジェクト
		noDel_ptr<GameObject> pSelectSquare;
		//隣のマス
		noDel_ptr<Square> pNeighbor[(int)eNeighborDir::Max];

	public:
		noDel_ptr<Transform> transform;

		struct stSearchInfo {
			//親のマス
			noDel_ptr<Square> pParent = NULL;
			//オープンクローズ情報
			bool isOpen = false;
			bool isClose = false;
			//移動にかかるコスト
			int move_cost = 0;
			//実コスト
			int curCost = 0;
			//推定コスト
			int heuristicCost = 0;
			//このマスの合計コスト
			int totalCost = 0;

			void Reset();
		};

		stSearchInfo searchInfo;

	public:
		Square(noDel_ptr<GameObject> square, noDel_ptr<GameObject> selectSquare);

		void SetUnMove(bool flag) { OnUnMoveObj = flag; }
		bool GetIsUnMove() { return OnUnMoveObj; }

		void SetMoveCost(int val) { searchInfo.move_cost = val; }
		int GetMoveCost() const { return searchInfo.move_cost; }

		noDel_ptr<Square> GetNeighbor(eNeighborDir dir) { return pNeighbor[(int)dir]; }

		void SetNeighbor(noDel_ptr<Square> next, bool isHorizon);

		//選択マスにカーソルが乗っているか
		bool GetSelectSquareHover() { return pSelectSquare->GetComponent<SelectSquare>()->GetHoverState(); }
		//選択マスが選択可能状態か
		bool GetSelectEnable() { return pSelectSquare->GetComponent<SelectSquare>()->GetSelectEnable(); }
		void ShowSelectSquare(bool flag, bool isEnable = true);
	};

	//SquareRowクラス----------------------------------------------------------------------
	class SquareRow {
	public:
		std::unordered_map<int, Square*> umRow;

	public:
		~SquareRow();
	};
}