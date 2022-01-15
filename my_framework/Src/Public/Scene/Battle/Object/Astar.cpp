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
void Astar::ShowMoveRange(noDel_ptr<Square> startSquare, int range) {
	//������
	Reset();
	//���s
	vCloseList.emplace_back(startSquare);
	SearchEnableSquare(startSquare, range);
}
//�ړ��\�}�X��o�^����
void Astar::SearchEnableSquare(noDel_ptr<Square> startSquare, int range) {
	//�ׂ̃}�X��T�����ăI�[�v�����X�g�ɓo�^
	for (int i = 0; i < (int)eNeighborDir::Max; i++) {
		//�ׂ̃}�X
		noDel_ptr<Square> _neigh = startSquare->GetNeighbor(static_cast<eNeighborDir>(i));
		if (_neigh == NULL) continue;
		//��Q���͖���
		if (_neigh->GetMoveCost() <= 0) continue;
		//�I�[�v�����X�g�ɂ���΃X���[
		if (_neigh->searchInfo.isOpen) continue;
		if (_neigh->searchInfo.isClose) continue;

		//�v�Z�ς݃}�X�̓o�^�i��ŏ������Z�b�g���邽�߁j
		vCulcList.emplace_back(_neigh);

		//�ׂ̃}�X�̏���ݒ�
		_neigh->searchInfo.pParent = startSquare;//�e�ݒ�
		_neigh->searchInfo.curCost = _neigh->searchInfo.pParent->searchInfo.curCost + _neigh->searchInfo.move_cost; //���R�X�g
		//�}�X�̃R�X�g���s�������ȓ��Ȃ�I�[�v�����X�g�ɒǉ�
		if (_neigh->searchInfo.curCost <= range) {
			//�ړ��s�}�X
			if (_neigh->GetIsUnMove()) {
				_neigh->ShowSelectSquare(true, false);
			}
			//�ړ��\�}�X
			else {
				vOpenList.emplace_back(_neigh);
				_neigh->searchInfo.isOpen = true;
				_neigh->ShowSelectSquare(true);
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
			_pNext->searchInfo.isOpen = false;
			break;
		}
	}
	vCloseList.emplace_back(_pNext);
	_pNext->searchInfo.isClose = true;

	SearchEnableSquare(_pNext, range);
}

//�ړI�n����X�^�[�g�n�_�܂ł̍ŒZ���������߁A�X�^�[�g�n�_��Ԃ�
void Astar::GetCulcedStartSquare(noDel_ptr<Square> start, noDel_ptr<Square> goal) {
	//������
	Reset();
	//���s
	vCloseList.emplace_back(start);
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
		//�I�[�v�����X�g�ɂ���΃X���[
		if (_neigh->searchInfo.isOpen) continue;
		if (_neigh->searchInfo.isClose) continue;

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
		_neigh->searchInfo.isOpen = true;
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
			_pNext->searchInfo.isOpen = false;
			break;
		}
	}
	vCloseList.emplace_back(_pNext);
	_pNext->searchInfo.isClose = true;

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