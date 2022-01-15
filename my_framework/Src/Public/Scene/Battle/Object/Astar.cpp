#include "../../../../../framework.h"
#include "../../../../../environment.h"
#include "Astar.h"

using namespace nsBattle;

//移動可能マスの表示
Astar::Astar(){
}

void Astar::Reset() {
	HideMoveRange();
	vOpenList.clear();
	vCloseList.clear();
	vCulcList.clear();
}

void Astar::HideMoveRange() {
	//情報のリセット
	for (auto& sq : vCulcList) {
		sq->searchInfo.Reset();
		sq->ShowSelectSquare(false);
	}
}

//移動可能マスの表示
void Astar::ShowMoveRange(noDel_ptr<Square> startSquare, int range) {
	//初期化
	Reset();
	//実行
	vCloseList.emplace_back(startSquare);
	SearchEnableSquare(startSquare, range);
}
//移動可能マスを登録する
void Astar::SearchEnableSquare(noDel_ptr<Square> startSquare, int range) {
	//隣のマスを探索してオープンリストに登録
	for (int i = 0; i < (int)eNeighborDir::Max; i++) {
		//隣のマス
		noDel_ptr<Square> _neigh = startSquare->GetNeighbor(static_cast<eNeighborDir>(i));
		if (_neigh == NULL) continue;
		//障害物は無視
		if (_neigh->GetMoveCost() <= 0) continue;
		//オープンリストにあればスルー
		if (_neigh->searchInfo.isOpen) continue;
		if (_neigh->searchInfo.isClose) continue;

		//計算済みマスの登録（後で情報をリセットするため）
		vCulcList.emplace_back(_neigh);

		//隣のマスの情報を設定
		_neigh->searchInfo.pParent = startSquare;//親設定
		_neigh->searchInfo.curCost = _neigh->searchInfo.pParent->searchInfo.curCost + _neigh->searchInfo.move_cost; //実コスト
		//マスのコストが行動距離以内ならオープンリストに追加
		if (_neigh->searchInfo.curCost <= range) {
			//移動不可マス
			if (_neigh->GetIsUnMove()) {
				_neigh->ShowSelectSquare(true, false);
			}
			//移動可能マス
			else {
				vOpenList.emplace_back(_neigh);
				_neigh->searchInfo.isOpen = true;
				_neigh->ShowSelectSquare(true);
			}
		}
	}

	//オープンリストが空なら終了
	if (vOpenList.size() == 0) return;

	//新たな探索マス
	noDel_ptr<Square> _pNext = *vOpenList.begin();

	//探索マスをオープンリストから削除しクローズリストに入れる
	for (int i = 0; i < vOpenList.size(); i++) {
		if (vOpenList[i] == _pNext) {
			vOpenList.erase(vOpenList.begin() + i);
			_pNext->searchInfo.isOpen = false;
			break;
		}
	}
	vCloseList.emplace_back(_pNext);
	_pNext->searchInfo.isClose = true;

	SearchEnableSquare(_pNext, range);
}

//目的地からスタート地点までの最短距離を求め、スタート地点を返す
void Astar::GetCulcedStartSquare(noDel_ptr<Square> start, noDel_ptr<Square> goal) {
	//初期化
	Reset();
	//実行
	vCloseList.emplace_back(start);
	SearchShortPath(start, goal);
}
void Astar::SearchShortPath(noDel_ptr<Square> start, noDel_ptr<Square> goal) {
	//隣のマスを探索してオープンリストに登録
	for (int i = 0; i < (int)eNeighborDir::Max; i++) {
		//隣のマス
		noDel_ptr<Square> _neigh = start->GetNeighbor(static_cast<eNeighborDir>(i));
		if (_neigh == NULL) continue;

		//もし目的地(キャラがいる場所)の場合終了
		if (_neigh == goal) {
			return;
		}

		//障害物は無視
		if (_neigh->GetMoveCost() <= 0) continue;
		if (_neigh->GetIsUnMove()) continue;
		//オープンリストにあればスルー
		if (_neigh->searchInfo.isOpen) continue;
		if (_neigh->searchInfo.isClose) continue;

		//計算済みマスの登録（後で情報をリセットするため）
		vCulcList.emplace_back(_neigh);

		//隣のマスの情報を設定
		_neigh->searchInfo.pParent = start;//親設定
		_neigh->searchInfo.curCost = _neigh->searchInfo.pParent->searchInfo.curCost + _neigh->searchInfo.move_cost; //実コスト
		_neigh->searchInfo.heuristicCost = CulcHeuristicCost(_neigh->transform->position.x, _neigh->transform->position.y,
			goal->transform->position.x, goal->transform->position.y);
		_neigh->searchInfo.totalCost = _neigh->searchInfo.curCost + _neigh->searchInfo.heuristicCost;
		//オープンリストに追加
		vOpenList.emplace_back(_neigh);
		_neigh->searchInfo.isOpen = true;
	}

	//オープンリストが空なら終了(失敗)
	if (vOpenList.size() == 0) {
		Reset();
		return;
	}

	//新たな探索マス
	noDel_ptr<Square> _pNext = DecideSearchSquare();

	//探索マスをオープンリストから削除しクローズリストに入れる
	for (int i = 0; i < vOpenList.size(); i++) {
		if (vOpenList[i] == _pNext) {
			vOpenList.erase(vOpenList.begin() + i);
			_pNext->searchInfo.isOpen = false;
			break;
		}
	}
	vCloseList.emplace_back(_pNext);
	_pNext->searchInfo.isClose = true;

	//探索マスがゴールなら終了
	if (_pNext == goal) {
		return;
	}

	SearchShortPath(_pNext, goal);
}

int Astar::CulcHeuristicCost(float x, float y, float x2, float y2) {
	float dx = abs(x - x2);
	float dy = abs(y - y2);
	return (int)(dx + dy);
}
noDel_ptr<Square> Astar::DecideSearchSquare() {
	noDel_ptr<Square> pSearch_square = 0;
	int min_cost = 1000000;
	for (int i = 0; i < vOpenList.size(); i++) {
		if (vOpenList[i]->searchInfo.totalCost < min_cost) {
			min_cost = vOpenList[i]->searchInfo.totalCost;
			pSearch_square = vOpenList[i];
		}
	}
	return pSearch_square;
}