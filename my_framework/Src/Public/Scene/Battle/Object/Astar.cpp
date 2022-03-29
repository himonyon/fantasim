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
void Astar::ShowRange(noDel_ptr<Square> startSquare, int range, eAstarFunc type) {
	//初期化
	Reset();
	//実行
	vCloseList.emplace_back(startSquare);
	SearchEnableSquare(startSquare, range, type);
}
//移動可能マスを登録する
void Astar::SearchEnableSquare(noDel_ptr<Square> startSquare, int range, eAstarFunc type) {
	//隣のマスを探索してオープンリストに登録
	for (int i = 0; i < (int)eNeighborDir::Max; i++) {
		//隣のマス
		noDel_ptr<Square> _neigh = startSquare->GetNeighbor(static_cast<eNeighborDir>(i));
		if (_neigh == NULL) continue;
		//障害物は無視
		if (_neigh->GetMoveCost() <= 0) continue;
		//リストにあればスルー
		if (CheckOpenList(_neigh)) continue;
		if (CheckCloseList(_neigh)) continue;

		//計算済みマスの登録（後で情報をリセットするため）
		vCulcList.emplace_back(_neigh);

		//隣のマスの情報を設定
		_neigh->searchInfo.pParent = startSquare;//親設定
		_neigh->searchInfo.curCost = _neigh->searchInfo.pParent->searchInfo.curCost + _neigh->searchInfo.move_cost; //実コスト
		//マスのコストが行動距離以内ならオープンリストに追加
		if (_neigh->searchInfo.curCost <= range) {
			//種類に応じて移動可能マスを切り替える
			if (type == eAstarFunc::MoveRange) {
				//移動不可マス
				if (_neigh->GetIsUnMove()) {
					_neigh->ShowSelectSquare(true, false);
				}
				//移動可能マス
				else {
					vOpenList.emplace_back(_neigh);
					_neigh->ShowSelectSquare(true);
				}
			}
			else if (type == eAstarFunc::FindEnemyRange) {
				//選択可能マス
				if (_neigh->GetIsUnMove() && dynamic_noDel_cast<EnemyChara>(_neigh->GetOnChara()) != NULL) {
					_neigh->ShowSelectSquare(true);
				}
				//選択不可マス
				else {
					_neigh->ShowSelectSquare(true, false);
				}
				vOpenList.emplace_back(_neigh);
			}
			else if (type == eAstarFunc::FindFriendRange) {
				//選択可能マス
				if (_neigh->GetIsUnMove() && dynamic_noDel_cast<PlayerChara>(_neigh->GetOnChara()) != NULL) {
					_neigh->ShowSelectSquare(true);
				}
				//選択不可マス
				else {
					_neigh->ShowSelectSquare(true, false);
				}
				vOpenList.emplace_back(_neigh);
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
			break;
		}
	}
	vCloseList.emplace_back(_pNext);

	SearchEnableSquare(_pNext, range, type);
}

//目的地からスタート地点までの最短距離を求める
noDel_ptr<Square> Astar::GetCulcedStartSquare(noDel_ptr<Square> goal, noDel_ptr<Square> start) {
	//初期化
	Reset();
	//最初のマスをクローズにする
	vCloseList.emplace_back(goal);
	//ゴール地点からスタート地点までの最短を探す
	SearchShortPath(goal, start, true);

	//成功したらスターt地点マスクラスを返す
	if (vOpenList.size() != 0) {
		if (vCloseList.size() != 0) return vCloseList.back();
		return NULL;
	}

	//Openリストが空ならマス上OBJは無視して最短探索
	Reset();
	vCloseList.emplace_back(goal);
	SearchShortPath(goal, start, false);

	if (vCloseList.size() != 0) return vCloseList.back();
	return NULL;
}
noDel_ptr<Square> Astar::GetCulcedStartSquare(noDel_ptr<Square> goal, noDel_ptr<Square> start, int minSkillRange) {
	//初期化
	Reset();
	//ゴールから近くスキルが届く範囲を新ゴールに設定
	noDel_ptr<Square> _newGoal = GetSpcDisSquare(goal, start, minSkillRange);
	//再初期化
	Reset();
	//最初のマスをクローズにする
	vCloseList.emplace_back(_newGoal);
	//ゴール地点からスタート地点までの最短を探す
	SearchShortPath(_newGoal, start, true);

	//成功したらスターt地点マスクラスを返す
	if (vOpenList.size() != 0) {
		if (vCloseList.size() != 0) return vCloseList.back();
		return NULL;
	}

	//Openリストが空ならマス上OBJは無視して最短探索
	Reset();
	vCloseList.emplace_back(_newGoal);
	SearchShortPath(_newGoal, start, false);

	if (vCloseList.size() != 0) return vCloseList.back();
	return NULL;
}

void Astar::SearchShortPath(noDel_ptr<Square> search, noDel_ptr<Square> start, bool unMoveObj) {
	
	//周辺マスをOpenリストに格納
	PushNeighSquare(search, start, unMoveObj);

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
			break;
		}
	}
	vCloseList.emplace_back(_pNext);

	//探索マスがスタート位置なら終了
	if (_pNext == start) {
		for (int i = 0; i < vCloseList.size(); i++) {
			if (vCloseList[i] == start) {
				vCloseList.erase(vCloseList.begin() + i);
				break;
			}
		}
		return;
	}

	SearchShortPath(_pNext, start, unMoveObj);
}

void Astar::PushNeighSquare(noDel_ptr<Square> search, noDel_ptr<Square> start, bool unMoveObj) {
	//隣のマスを探索してオープンリストに登録
	for (int i = 0; i < (int)eNeighborDir::Max; i++) {
		//隣のマス
		noDel_ptr<Square> _neigh = search->GetNeighbor(static_cast<eNeighborDir>(i));
		if (_neigh == NULL) continue;

		//障害物は無視
		if (_neigh->GetMoveCost() <= 0) continue;
		//マス上にオブジェクトがあれば無視（unMoveObj時）
		if(unMoveObj) if (_neigh->GetIsUnMove() && _neigh != start) continue;
		//リストにあればスルー
		if (CheckOpenList(_neigh)) continue;
		if (CheckCloseList(_neigh)) continue;

		//計算済みマスの登録（後で情報をリセットするため）
		vCulcList.emplace_back(_neigh);

		//隣のマスの情報を設定
		_neigh->searchInfo.pParent = search;//親設定
		_neigh->searchInfo.curCost = _neigh->searchInfo.pParent->searchInfo.curCost + _neigh->searchInfo.move_cost; //実コスト
		_neigh->searchInfo.heuristicCost = CulcHeuristicCost(_neigh->transform->position.x, _neigh->transform->position.y,
			start->transform->position.x, start->transform->position.y);
		_neigh->searchInfo.totalCost = _neigh->searchInfo.curCost + _neigh->searchInfo.heuristicCost;
		//オープンリストに追加
		vOpenList.emplace_back(_neigh);
	}
}

noDel_ptr<Square> Astar::GetSpcDisSquare(noDel_ptr<Square> target, noDel_ptr<Square> start, int distance) {
	std::vector<noDel_ptr<Square>> _vSquares = {};

	//targetマスからdistance距離離れたマスをvSquareに格納
	noDel_ptr<Square> _target = target;
	while (true) {
		for (int i = 0; i < (int)eNeighborDir::Max; i++) {
			//隣のマス
			noDel_ptr<Square> _neigh = _target->GetNeighbor(static_cast<eNeighborDir>(i));
			if (_neigh == NULL) continue;

			//障害物は無視
			if (_neigh->GetMoveCost() <= 0) continue;
			//マス上にOBJがある場合は無視
			if (_neigh->GetIsUnMove() == true) continue;
			//オーブンリストにあればContinue
			for (auto& open : vOpenList) if (_neigh == open) continue;
			//クローズリストにあればContinue
			for (auto& close : vCloseList) if (_neigh == close) continue;
			//すでにコスト計算済みならContinue
			for (auto& close : vCulcList) if (_neigh == close) continue;

			vCulcList.emplace_back(_neigh);
			_neigh->searchInfo.pParent = _target;
			//コストがdistanceなら配列に格納してcontinue
			if (_neigh->searchInfo.pParent->searchInfo.curCost + 1 == distance) {
				_neigh->searchInfo.curCost = _neigh->searchInfo.pParent->searchInfo.curCost + 1;
				_vSquares.emplace_back(_neigh);
				continue;
			}

			//コストがdistance未満ならオープンリストに格納
			if (_neigh->searchInfo.pParent->searchInfo.curCost + 1 >= distance) continue;
			_neigh->searchInfo.curCost = _neigh->searchInfo.pParent->searchInfo.curCost + 1;
			vOpenList.emplace_back(_neigh);
		}

		//探索マスをクローズに
		vCloseList.emplace_back(_target);

		//オープンリストから次の探索マスを決定
		if (vOpenList.size() != 0) _target = vOpenList.back();
		else break; //ループ終了

		//探索マスをオープンから消す
		for (int i = 0; i < vOpenList.size(); i++) {
			if (vOpenList[i] == _target) {
				vOpenList.erase(vOpenList.begin() + i);
				break;
			}
		}
	}

	if (_vSquares.size() == 0) return NULL;

	//配列の中から一番startの距離に近いマスを返す
	noDel_ptr<Square> _nearSq = NULL;
	for (auto& sq : _vSquares) {
		if (_nearSq == NULL) { _nearSq = sq; continue; }
		//比較
		float _curdis = abs(_nearSq->transform->position.x - start->transform->position.x) +
			abs(_nearSq->transform->position.y - start->transform->position.y);
		float _newdis = abs(sq->transform->position.x - start->transform->position.x) +
			abs(sq->transform->position.y - start->transform->position.y);

		if (_curdis > _newdis) _nearSq = sq;
	}

	return _nearSq;
}

//目的地までの距離を返す
int Astar::ReturnDistance(noDel_ptr<Square> goal, noDel_ptr<Square> start) {
	//初期化
	Reset();
	//最初のマスをクローズにする
	vCloseList.emplace_back(goal);
	//実行
	SearchShortPath(goal, start, false);

	if (vCloseList.size() == 0) return 1;

	noDel_ptr<Square> _target = vCloseList.back();
	int count = 1;
	while (true) {
		if (_target->searchInfo.pParent != NULL) {
			count++;
			_target = _target->searchInfo.pParent;
		}
		else {
			break;
		}
	}

	return count;
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

bool Astar::CheckOpenList(noDel_ptr<Square> sq) {
	for (auto& open : vOpenList) {
		if (sq == open) return true;
	}
	return false;
}
bool Astar::CheckCloseList(noDel_ptr<Square> sq) {
	for (auto& close : vCloseList) {
		if (sq == close) return true;
	}
	return false;
}