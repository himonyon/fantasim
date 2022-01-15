#pragma once
/*-----------------------------------------------------------

	BattleChara�N���X(Behaviour�̔h��)
	�o�g���Ŏg�p����L�����̋���

-------------------------------------------------------------*/
namespace nsBattle {
	enum class eCharaState {
		None,
		Move,
		Attack,
		Heal,
		Buff,
	};

	class BattleChara : public Behaviour {
	protected:
		//�o�H�T���C���X�^���X
		static Astar pSearcher;
	
		//�L�����̏��
		eCharaState charaState = eCharaState::None;

		//����ł���}�X
		noDel_ptr<Square> pCurSquare;

		//�ړ����̃J�E���g
		int moveCount = 0;

	public:
		//�L�������I�����ꂽ��
		static noDel_ptr<BattleChara> pSelectChara;

		noDel_ptr<Charactor> pCharaInfo;

		//�J�[�\�����������Ă��邩
		bool isCursorHovered = false;

	protected:
		//�����蔻��
		void OnTriggerEnter2D(noDel_ptr<Collider2D> hitCollider) override;
		void OnTrigger2D(noDel_ptr<Collider2D> hitCollider) override;
		void OnTriggerExit2D(noDel_ptr<Collider2D> hitCollider) override;

	protected:
		//�L�����̈ړ�
		bool Move(noDel_ptr<Square> start);

	public:
		//Setter,Getter
		void SetCurrentSquare(noDel_ptr<Square> square) {
			if (pCurSquare != NULL) pCurSquare->SetUnMove(false);
			pCurSquare = square;
			pCurSquare->SetUnMove(true);
		}
		noDel_ptr<Square> GetCurrentSquare() { return pCurSquare; }

	};
}
