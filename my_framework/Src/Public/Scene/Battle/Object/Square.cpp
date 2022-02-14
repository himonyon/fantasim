#include "../../../../../framework.h"
#include "../../../../../environment.h"
#include "Square.h"

using namespace nsBattle;

int Square::ID_Count = 0;

Square::Square(noDel_ptr<GameObject> square, noDel_ptr<GameObject> selectSquare) {
	this->pSquare = square;
	transform = pSquare->transform;
	this->pSelectSquare = selectSquare;
	this->pSelectSquare->AddComponent<SelectSquare>();
	this->pSelectSquare->AddComponent<Collider2D>();
	this->pSelectSquare->GetComponent<Collider2D>()->SetUpCollider2D(0.9f, 0.9f, false);
	this->pSelectSquare->GetComponent<SpriteRenderer>()->SetColor(1, 1, 1, 0.8f);
	this->pSelectSquare->GetComponent<SpriteRenderer>()->SetRenderPriority(1);

	for (int i = 0; i < (int)eNeighborDir::Max; i++) pNeighbor[i] = NULL;

	//�I�����̃I�u�W�F�N�g�͔�\��
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

//SeerchInfo---------------------------------------------------------
void Square::stSearchInfo::Reset() {
	//�e�̃}�X
	pParent = NULL;
	//���R�X�g
	curCost = 0;
	//����R�X�g
	heuristicCost = 0;
	//���̃}�X�̍��v�R�X�g
	totalCost = 0;
}

//SquareRow�N���X------------------------------------------------
SquareRow::~SquareRow() {
	for (auto& sq : umRow) {
		delete sq.second;
	}
	umRow.clear();
}
