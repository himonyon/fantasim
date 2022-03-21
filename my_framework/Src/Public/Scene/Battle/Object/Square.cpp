#include "../../../../../framework.h"
#include "../../../../../environment.h"
#include "Square.h"

using namespace nsBattle;

int Square::ID_Count = 0;

Square::Square(noDel_ptr<GameObject> square, noDel_ptr<GameObject> selectSquare) {
	//マス
	this->pSquare = square;
	transform = pSquare->transform;

	//選択マス
	this->pSelectSquare = selectSquare;
	pSelectSquare->AddComponent<SelectSquare>();
	pSelectSquare->AddComponent<Collider2D>();
	pSelectSquare->GetComponent<Collider2D>()->SetUpCollider2D(0.9f, 0.9f, false);
	pSelectSquare->GetComponent<SpriteRenderer>()->SetColor(1, 1, 1, 0.8f);
	pSelectSquare->SetRenderOrder(1);

	for (int i = 0; i < (int)eNeighborDir::Max; i++) pNeighbor[i] = NULL;

	//選択時のオブジェクトは非表示
	pSelectSquare->SetObjEnable(false);
	
	id = ID_Count;
	ID_Count++;
}

void Square::SetNeighbor(noDel_ptr<Square> next, bool isHorizon) {
	if (isHorizon) {
		pNeighbor[(int)eNeighborDir::Left] = next;
		next->pNeighbor[(int)eNeighborDir::Right] = noDel_ptr<Square>(this);
	}
	else {
		pNeighbor[(int)eNeighborDir::Top] = next;
		next->pNeighbor[(int)eNeighborDir::Bottom] = noDel_ptr<Square>(this);
	}
}

void Square::ShowSelectSquare(bool flag, bool isEnable) {
	pSelectSquare->SetObjEnable(flag);
	if (flag == false) {
		pSelectSquare->GetComponent<SelectSquare>()->Reset();
	}
	else {
		pSelectSquare->GetComponent<SelectSquare>()->SetSelectEnable(isEnable);
	}
}

void Square::SetUnMove(bool flag, noDel_ptr<BattleChara> chara){
	OnUnMoveObj = flag; pOnBattleChara = chara;
}

//SeerchInfo---------------------------------------------------------
void Square::stSearchInfo::Reset() {
	//親のマス
	pParent = NULL;
	//実コスト
	curCost = 0;
	//推定コスト
	heuristicCost = 0;
	//このマスの合計コスト
	totalCost = 0;
}

//SquareRowクラス------------------------------------------------
SquareRow::~SquareRow() {
	for (auto& sq : umRow) {
		delete sq.second;
	}
	umRow.clear();
}
