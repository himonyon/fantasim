/*-----------------------------------------------------------

	Astar�N���X
	�o�H�T���N���X

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
		Astar();

		//�͈͂̕\��
		void ShowRange(noDel_ptr<Square> startSquare, int range, eAstarFunc type);

		//�X�^�[�g�n�_����ړI�n�܂ł̍ŒZ���������߁A�e�}�X���������X�^�[�g�n�_�}�X�N���X��Ԃ�
		noDel_ptr<Square> GetCulcedStartSquare(noDel_ptr<Square> goal, noDel_ptr<Square> start);
		noDel_ptr<Square> GetCulcedStartSquare(noDel_ptr<Square> goal, noDel_ptr<Square> start, int minSkillRange);

		//�ړI�n�܂ł̋���(�}�X�̐�)��Ԃ�
		int ReturnDistance(noDel_ptr<Square> goal, noDel_ptr<Square> start);

		//������
		void Reset();

	private:
		//�͈͂̕\��
		void SearchEnableSquare(noDel_ptr<Square> startSquare, int range, eAstarFunc type);
		void HideMoveRange();

		//�X�^�[�g�n�_����ړI�n�܂ł̍ŒZ���������߁A�X�^�[�g�n�_��Ԃ�
		void SearchShortPath(noDel_ptr<Square> search, noDel_ptr<Square> start, bool unMoveObj);

		//�}�X�������distance���ꂽ�}�X��target����߂����̂�Ԃ�
		noDel_ptr<Square> GetSpcDisSquare(noDel_ptr<Square> target, noDel_ptr<Square> start, int distance);

		//���Ӄ}�X��Open���X�g�Ɋi�[�iunMoveObj : �L����������Ă���}�X�������Ɋ܂߂邩�j
		void PushNeighSquare(noDel_ptr<Square> search, noDel_ptr<Square> start, bool unMoveObj);
		int CulcHeuristicCost(float x, float y, float x2, float y2);
		noDel_ptr<Square> DecideSearchSquare();

		//���X�g�ɂ��邩�m�F����֐�
		bool CheckOpenList(noDel_ptr<Square>);
		bool CheckCloseList(noDel_ptr<Square>);
	};
}