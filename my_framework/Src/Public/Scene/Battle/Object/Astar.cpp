#include "../../../../../framework.h"
#include "../../../../../environment.h"
#include "Astar.h"

using namespace nsBattle;

//�ړ��\�}�X�̕\��
Astar::Astar(){
}

void Astar::Reset() {
	HideMoveRange();
	vOpenList.clear();
	vCloseList.clear();
	vCulcList.clear();
}

void Astar::HideMoveRange() {
	//���̃��Z�b�g
	for (auto& sq : vCulcList) {
		sq->searchInfo.Reset();
		sq->ShowSelectSquare(false);
	}
}

//�ړ��\�}�X�̕\��
void Astar::ShowRange(noDel_ptr<Square> startSquare, int range, eAstarFunc type) {
	//������
	Reset();
	//���s
	vCloseList.emplace_back(startSquare);
	SearchEnableSquare(startSquare, range, type);
}
//�ړ��\�}�X��o�^����
void Astar::SearchEnableSquare(noDel_ptr<Square> startSquare, int range, eAstarFunc type) {
	//�ׂ̃}�X��T�����ăI�[�v�����X�g�ɓo�^
	for (int i = 0; i < (int)eNeighborDir::Max; i++) {
		//�ׂ̃}�X
		noDel_ptr<Square> _neigh = startSquare->GetNeighbor(static_cast<eNeighborDir>(i));
		if (_neigh == NULL) continue;
		//��Q���͖���
		if (_neigh->GetMoveCost() <= 0) continue;
		//���X�g�ɂ���΃X���[
		if (CheckOpenList(_neigh)) continue;
		if (CheckCloseList(_neigh)) continue;

		//�v�Z�ς݃}�X�̓o�^�i��ŏ������Z�b�g���邽�߁j
		vCulcList.emplace_back(_neigh);

		//�ׂ̃}�X�̏���ݒ�
		_neigh->searchInfo.pParent = startSquare;//�e�ݒ�
		_neigh->searchInfo.curCost = _neigh->searchInfo.pParent->searchInfo.curCost + _neigh->searchInfo.move_cost; //���R�X�g
		//�}�X�̃R�X�g���s�������ȓ��Ȃ�I�[�v�����X�g�ɒǉ�
		if (_neigh->searchInfo.curCost <= range) {
			//��ނɉ����Ĉړ��\�}�X��؂�ւ���
			if (type == eAstarFunc::MoveRange) {
				//�ړ��s�}�X
				if (_neigh->GetIsUnMove()) {
					_neigh->ShowSelectSquare(true, false);
				}
				//�ړ��\�}�X
				else {
					vOpenList.emplace_back(_neigh);
					_neigh->ShowSelectSquare(true);
				}
			}
			else if (type == eAstarFunc::FindEnemyRange) {
				//�I���\�}�X
				if (_neigh->GetIsUnMove() && dynamic_noDel_cast<EnemyChara>(_neigh->GetOnChara()) != NULL) {
					_neigh->ShowSelectSquare(true);
				}
				//�I��s�}�X
				else {
					_neigh->ShowSelectSquare(true, false);
				}
				vOpenList.emplace_back(_neigh);
			}
			else if (type == eAstarFunc::FindFriendRange) {
				//�I���\�}�X
				if (_neigh->GetIsUnMove() && dynamic_noDel_cast<PlayerChara>(_neigh->GetOnChara()) != NULL) {
					_neigh->ShowSelectSquare(true);
				}
				//�I��s�}�X
				else {
					_neigh->ShowSelectSquare(true, false);
				}
				vOpenList.emplace_back(_neigh);
			}
		}
	}

	//�I�[�v�����X�g����Ȃ�I��
	if (vOpenList.size() == 0) return;

	//�V���ȒT���}�X
	noDel_ptr<Square> _pNext = *vOpenList.begin();

	//�T���}�X���I�[�v�����X�g����폜���N���[�Y���X�g�ɓ����
	for (int i = 0; i < vOpenList.size(); i++) {
		if (vOpenList[i] == _pNext) {
			vOpenList.erase(vOpenList.begin() + i);
			break;
		}
	}
	vCloseList.emplace_back(_pNext);

	SearchEnableSquare(_pNext, range, type);
}

//�ړI�n����X�^�[�g�n�_�܂ł̍ŒZ���������߁A�X�^�[�g�n�_��Ԃ�
void Astar::GetCulcedStartSquare(noDel_ptr<Square> start, noDel_ptr<Square> goal) {
	//������
	Reset();
	//�ŏ��̃}�X���N���[�Y�ɂ���
	vCloseList.emplace_back(start);
	//���s
	SearchShortPath(start, goal);
}
void Astar::SearchShortPath(noDel_ptr<Square> start, noDel_ptr<Square> goal) {
	//�ׂ̃}�X��T�����ăI�[�v�����X�g�ɓo�^
	for (int i = 0; i < (int)eNeighborDir::Max; i++) {
		//�ׂ̃}�X
		noDel_ptr<Square> _neigh = start->GetNeighbor(static_cast<eNeighborDir>(i));
		if (_neigh == NULL) continue;

		//�����ړI�n(�L����������ꏊ)�̏ꍇ�I��
		if (_neigh == goal) {
			return;
		}

		//��Q���͖���
		if (_neigh->GetMoveCost() <= 0) continue;
		if (_neigh->GetIsUnMove()) continue;
		//���X�g�ɂ���΃X���[
		if (CheckOpenList(_neigh)) continue;
		if (CheckCloseList(_neigh)) continue;

		//�v�Z�ς݃}�X�̓o�^�i��ŏ������Z�b�g���邽�߁j
		vCulcList.emplace_back(_neigh);

		//�ׂ̃}�X�̏���ݒ�
		_neigh->searchInfo.pParent = start;//�e�ݒ�
		_neigh->searchInfo.curCost = _neigh->searchInfo.pParent->searchInfo.curCost + _neigh->searchInfo.move_cost; //���R�X�g
		_neigh->searchInfo.heuristicCost = CulcHeuristicCost(_neigh->transform->position.x, _neigh->transform->position.y,
			goal->transform->position.x, goal->transform->position.y);
		_neigh->searchInfo.totalCost = _neigh->searchInfo.curCost + _neigh->searchInfo.heuristicCost;
		//�I�[�v�����X�g�ɒǉ�
		vOpenList.emplace_back(_neigh);
	}

	//�I�[�v�����X�g����Ȃ�I��(���s)
	if (vOpenList.size() == 0) {
		Reset();
		return;
	}

	//�V���ȒT���}�X
	noDel_ptr<Square> _pNext = DecideSearchSquare();

	//�T���}�X���I�[�v�����X�g����폜���N���[�Y���X�g�ɓ����
	for (int i = 0; i < vOpenList.size(); i++) {
		if (vOpenList[i] == _pNext) {
			vOpenList.erase(vOpenList.begin() + i);
			break;
		}
	}
	vCloseList.emplace_back(_pNext);

	//�T���}�X���S�[���Ȃ�I��
	if (_pNext == goal) {
		return;
	}

	SearchShortPath(_pNext, goal);
}

//�ړI�n�܂ł̋�����Ԃ�
int Astar::ReturnDistance(noDel_ptr<Square> start, noDel_ptr<Square> goal) {
	//������
	Reset();
	//���s
	GetDistanceToGoal(start, goal);
	if (vCloseList.size() == 0) return 0;
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

	if (count > 1) count--;

	return count;
}
void Astar::GetDistanceToGoal(noDel_ptr<Square> start, noDel_ptr<Square> goal) {
	//�ׂ̃}�X��T�����ăI�[�v�����X�g�ɓo�^
	for (int i = 0; i < (int)eNeighborDir::Max; i++) {
		//�ׂ̃}�X
		noDel_ptr<Square> _neigh = start->GetNeighbor(static_cast<eNeighborDir>(i));
		if (_neigh == NULL) continue;

		//��Q���͖���
		if (_neigh->GetMoveCost() <= 0) continue;
		//���X�g�ɂ���΃X���[
		if (CheckOpenList(_neigh)) continue;
		if (CheckCloseList(_neigh)) continue;

		//�v�Z�ς݃}�X�̓o�^�i��ŏ������Z�b�g���邽�߁j
		vCulcList.emplace_back(_neigh);

		//�ׂ̃}�X�̏���ݒ�
		_neigh->searchInfo.pParent = start;//�e�ݒ�
		_neigh->searchInfo.curCost = _neigh->searchInfo.pParent->searchInfo.curCost + _neigh->searchInfo.move_cost; //���R�X�g
		_neigh->searchInfo.heuristicCost = CulcHeuristicCost(_neigh->transform->position.x, _neigh->transform->position.y,
			goal->transform->position.x, goal->transform->position.y);
		_neigh->searchInfo.totalCost = _neigh->searchInfo.curCost + _neigh->searchInfo.heuristicCost;
		//�I�[�v�����X�g�ɒǉ�
		vOpenList.emplace_back(_neigh);
	}

	//�I�[�v�����X�g����Ȃ�I��(���s)
	if (vOpenList.size() == 0) {
		Reset();
		return;
	}

	//�V���ȒT���}�X
	noDel_ptr<Square> _pNext = DecideSearchSquare();

	//�T���}�X���I�[�v�����X�g����폜���N���[�Y���X�g�ɓ����
	for (int i = 0; i < vOpenList.size(); i++) {
		if (vOpenList[i] == _pNext) {
			vOpenList.erase(vOpenList.begin() + i);
			break;
		}
	}
	vCloseList.emplace_back(_pNext);

	//�T���}�X���S�[���Ȃ�I��
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