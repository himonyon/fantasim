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
		//�͈͂̕\��
		Astar();

		//�͈͂̕\��
		void ShowRange(noDel_ptr<Square> startSquare, int range, eAstarFunc type);

		//�X�^�[�g�n�_����ړI�n�܂ł̍ŒZ���������߁A�X�^�[�g�n�_��Ԃ�
		void GetCulcedStartSquare(noDel_ptr<Square> start, noDel_ptr<Square> goal);

		//�ړI�n�܂ł̋���(�}�X�̐�)��Ԃ�
		int ReturnDistance(noDel_ptr<Square> start, noDel_ptr<Square> goal);

		//������
		void Reset();

	private:
		//�͈͂̕\��
		void SearchEnableSquare(noDel_ptr<Square> startSquare, int range, eAstarFunc type);
		void HideMoveRange();

		//�X�^�[�g�n�_����ړI�n�܂ł̍ŒZ���������߁A�X�^�[�g�n�_��Ԃ�
		void SearchShortPath(noDel_ptr<Square> start, noDel_ptr<Square> goal);
		int CulcHeuristicCost(float x, float y, float x2, float y2);
		noDel_ptr<Square> DecideSearchSquare();

		//�ړI�n�܂ł̋���(�}�X�̐�)��Ԃ�
		void GetDistanceToGoal(noDel_ptr<Square> start, noDel_ptr<Square> goal);

		//���X�g�ɂ��邩�m�F����֐�
		bool CheckOpenList(noDel_ptr<Square>);
		bool CheckCloseList(noDel_ptr<Square>);
	};
}