/*-----------------------------------------------------------

	Square�N���X
	�X�e�[�W�̊e�}�X�̏��

-------------------------------------------------------------*/
#pragma once
namespace nsBattle {
	enum class eNeighborDir {
		Left,
		Top,
		Right,
		Bottom,
		Max,
	};

	class Square {
	private:
		static int ID_Count;

	private:
		//�}�X��ID
		int id = 0;

		//�}�X�ɉ����ړ��s�I�u�W�F�N�g������Ă��邩
		bool OnUnMoveObj = false;
		 
		
		//�}�X�̃I�u�W�F�N�g
		noDel_ptr<GameObject> pSquare;
		//�I�����̃}�X�I�u�W�F�N�g
		noDel_ptr<GameObject> pSelectSquare;
		//�ׂ̃}�X
		noDel_ptr<Square> pNeighbor[(int)eNeighborDir::Max];

	public:
		noDel_ptr<Transform> transform;

		struct stSearchInfo {
			//�e�̃}�X
			noDel_ptr<Square> pParent = NULL;
			//�I�[�v���N���[�Y���
			bool isOpen = false;
			bool isClose = false;
			//�ړ��ɂ�����R�X�g
			int move_cost = 0;
			//���R�X�g
			int curCost = 0;
			//����R�X�g
			int heuristicCost = 0;
			//���̃}�X�̍��v�R�X�g
			int totalCost = 0;

			void Reset();
		};

		stSearchInfo searchInfo;

	public:
		Square(noDel_ptr<GameObject> square, noDel_ptr<GameObject> selectSquare);

		void SetUnMove(bool flag) { OnUnMoveObj = flag; }
		bool GetIsUnMove() { return OnUnMoveObj; }

		void SetMoveCost(int val) { searchInfo.move_cost = val; }
		int GetMoveCost() const { return searchInfo.move_cost; }

		noDel_ptr<Square> GetNeighbor(eNeighborDir dir) { return pNeighbor[(int)dir]; }

		void SetNeighbor(noDel_ptr<Square> next, bool isHorizon);

		//�I���}�X�ɃJ�[�\��������Ă��邩
		bool GetSelectSquareHover() { return pSelectSquare->GetComponent<SelectSquare>()->GetHoverState(); }
		//�I���}�X���I���\��Ԃ�
		bool GetSelectEnable() { return pSelectSquare->GetComponent<SelectSquare>()->GetSelectEnable(); }
		void ShowSelectSquare(bool flag, bool isEnable = true);
	};

	//SquareRow�N���X----------------------------------------------------------------------
	class SquareRow {
	public:
		std::unordered_map<int, Square*> umRow;

	public:
		~SquareRow();
	};
}