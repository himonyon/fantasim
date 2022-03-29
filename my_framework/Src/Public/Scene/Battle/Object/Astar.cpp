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

//�ړI�n����X�^�[�g�n�_�܂ł̍ŒZ���������߂�
noDel_ptr<Square> Astar::GetCulcedStartSquare(noDel_ptr<Square> goal, noDel_ptr<Square> start) {
	//������
	Reset();
	//�ŏ��̃}�X���N���[�Y�ɂ���
	vCloseList.emplace_back(goal);
	//�S�[���n�_����X�^�[�g�n�_�܂ł̍ŒZ��T��
	SearchShortPath(goal, start, true);

	//����������X�^�[t�n�_�}�X�N���X��Ԃ�
	if (vOpenList.size() != 0) {
		if (vCloseList.size() != 0) return vCloseList.back();
		return NULL;
	}

	//Open���X�g����Ȃ�}�X��OBJ�͖������čŒZ�T��
	Reset();
	vCloseList.emplace_back(goal);
	SearchShortPath(goal, start, false);

	if (vCloseList.size() != 0) return vCloseList.back();
	return NULL;
}
noDel_ptr<Square> Astar::GetCulcedStartSquare(noDel_ptr<Square> goal, noDel_ptr<Square> start, int minSkillRange) {
	//������
	Reset();
	//�S�[������߂��X�L�����͂��͈͂�V�S�[���ɐݒ�
	noDel_ptr<Square> _newGoal = GetSpcDisSquare(goal, start, minSkillRange);
	//�ď�����
	Reset();
	//�ŏ��̃}�X���N���[�Y�ɂ���
	vCloseList.emplace_back(_newGoal);
	//�S�[���n�_����X�^�[�g�n�_�܂ł̍ŒZ��T��
	SearchShortPath(_newGoal, start, true);

	//����������X�^�[t�n�_�}�X�N���X��Ԃ�
	if (vOpenList.size() != 0) {
		if (vCloseList.size() != 0) return vCloseList.back();
		return NULL;
	}

	//Open���X�g����Ȃ�}�X��OBJ�͖������čŒZ�T��
	Reset();
	vCloseList.emplace_back(_newGoal);
	SearchShortPath(_newGoal, start, false);

	if (vCloseList.size() != 0) return vCloseList.back();
	return NULL;
}

void Astar::SearchShortPath(noDel_ptr<Square> search, noDel_ptr<Square> start, bool unMoveObj) {
	
	//���Ӄ}�X��Open���X�g�Ɋi�[
	PushNeighSquare(search, start, unMoveObj);

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

	//�T���}�X���X�^�[�g�ʒu�Ȃ�I��
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
	//�ׂ̃}�X��T�����ăI�[�v�����X�g�ɓo�^
	for (int i = 0; i < (int)eNeighborDir::Max; i++) {
		//�ׂ̃}�X
		noDel_ptr<Square> _neigh = search->GetNeighbor(static_cast<eNeighborDir>(i));
		if (_neigh == NULL) continue;

		//��Q���͖���
		if (_neigh->GetMoveCost() <= 0) continue;
		//�}�X��ɃI�u�W�F�N�g������Ζ����iunMoveObj���j
		if(unMoveObj) if (_neigh->GetIsUnMove() && _neigh != start) continue;
		//���X�g�ɂ���΃X���[
		if (CheckOpenList(_neigh)) continue;
		if (CheckCloseList(_neigh)) continue;

		//�v�Z�ς݃}�X�̓o�^�i��ŏ������Z�b�g���邽�߁j
		vCulcList.emplace_back(_neigh);

		//�ׂ̃}�X�̏���ݒ�
		_neigh->searchInfo.pParent = search;//�e�ݒ�
		_neigh->searchInfo.curCost = _neigh->searchInfo.pParent->searchInfo.curCost + _neigh->searchInfo.move_cost; //���R�X�g
		_neigh->searchInfo.heuristicCost = CulcHeuristicCost(_neigh->transform->position.x, _neigh->transform->position.y,
			start->transform->position.x, start->transform->position.y);
		_neigh->searchInfo.totalCost = _neigh->searchInfo.curCost + _neigh->searchInfo.heuristicCost;
		//�I�[�v�����X�g�ɒǉ�
		vOpenList.emplace_back(_neigh);
	}
}

noDel_ptr<Square> Astar::GetSpcDisSquare(noDel_ptr<Square> target, noDel_ptr<Square> start, int distance) {
	std::vector<noDel_ptr<Square>> _vSquares = {};

	//target�}�X����distance�������ꂽ�}�X��vSquare�Ɋi�[
	noDel_ptr<Square> _target = target;
	while (true) {
		for (int i = 0; i < (int)eNeighborDir::Max; i++) {
			//�ׂ̃}�X
			noDel_ptr<Square> _neigh = _target->GetNeighbor(static_cast<eNeighborDir>(i));
			if (_neigh == NULL) continue;

			//��Q���͖���
			if (_neigh->GetMoveCost() <= 0) continue;
			//�}�X���OBJ������ꍇ�͖���
			if (_neigh->GetIsUnMove() == true) continue;
			//�I�[�u�����X�g�ɂ����Continue
			for (auto& open : vOpenList) if (_neigh == open) continue;
			//�N���[�Y���X�g�ɂ����Continue
			for (auto& close : vCloseList) if (_neigh == close) continue;
			//���łɃR�X�g�v�Z�ς݂Ȃ�Continue
			for (auto& close : vCulcList) if (_neigh == close) continue;

			vCulcList.emplace_back(_neigh);
			_neigh->searchInfo.pParent = _target;
			//�R�X�g��distance�Ȃ�z��Ɋi�[����continue
			if (_neigh->searchInfo.pParent->searchInfo.curCost + 1 == distance) {
				_neigh->searchInfo.curCost = _neigh->searchInfo.pParent->searchInfo.curCost + 1;
				_vSquares.emplace_back(_neigh);
				continue;
			}

			//�R�X�g��distance�����Ȃ�I�[�v�����X�g�Ɋi�[
			if (_neigh->searchInfo.pParent->searchInfo.curCost + 1 >= distance) continue;
			_neigh->searchInfo.curCost = _neigh->searchInfo.pParent->searchInfo.curCost + 1;
			vOpenList.emplace_back(_neigh);
		}

		//�T���}�X���N���[�Y��
		vCloseList.emplace_back(_target);

		//�I�[�v�����X�g���玟�̒T���}�X������
		if (vOpenList.size() != 0) _target = vOpenList.back();
		else break; //���[�v�I��

		//�T���}�X���I�[�v���������
		for (int i = 0; i < vOpenList.size(); i++) {
			if (vOpenList[i] == _target) {
				vOpenList.erase(vOpenList.begin() + i);
				break;
			}
		}
	}

	if (_vSquares.size() == 0) return NULL;

	//�z��̒�������start�̋����ɋ߂��}�X��Ԃ�
	noDel_ptr<Square> _nearSq = NULL;
	for (auto& sq : _vSquares) {
		if (_nearSq == NULL) { _nearSq = sq; continue; }
		//��r
		float _curdis = abs(_nearSq->transform->position.x - start->transform->position.x) +
			abs(_nearSq->transform->position.y - start->transform->position.y);
		float _newdis = abs(sq->transform->position.x - start->transform->position.x) +
			abs(sq->transform->position.y - start->transform->position.y);

		if (_curdis > _newdis) _nearSq = sq;
	}

	return _nearSq;
}

//�ړI�n�܂ł̋�����Ԃ�
int Astar::ReturnDistance(noDel_ptr<Square> goal, noDel_ptr<Square> start) {
	//������
	Reset();
	//�ŏ��̃}�X���N���[�Y�ɂ���
	vCloseList.emplace_back(goal);
	//���s
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