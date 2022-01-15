/*-----------------------------------------------------------

	Astar�N���X
	�o�H�T���N���X

-------------------------------------------------------------*/
#pragma once
namespace nsBattle {
	enum class eAstarFunc {
		ShowSelectSquare,
		SearchShortPass,
	};

	class Astar {
	public:
		std::vector<noDel_ptr<Square>> vCulcList = {};
		std::vector<noDel_ptr<Square>> vOpenList = {};
		std::vector<noDel_ptr<Square>> vCloseList = {};

	public:
		//�ړ��͈͂̕\��
		Astar();

		//�ړ��͈͂̕\��
		void ShowMoveRange(noDel_ptr<Square> startSquare, int range);

		//�X�^�[�g�n�_����ړI�n�܂ł̍ŒZ���������߁A�X�^�[�g�n�_��Ԃ�
		void GetCulcedStartSquare(noDel_ptr<Square> start, noDel_ptr<Square> goal);

		//������
		void Reset();

	private:
		//�ړ��͈͂̕\��
		void SearchEnableSquare(noDel_ptr<Square> startSquare, int range);
		void HideMoveRange();

		//�X�^�[�g�n�_����ړI�n�܂ł̍ŒZ���������߁A�X�^�[�g�n�_��Ԃ�
		void SearchShortPath(noDel_ptr<Square> start, noDel_ptr<Square> goal);
		int CulcHeuristicCost(float x, float y, float x2, float y2);
		noDel_ptr<Square> DecideSearchSquare();
	};
}