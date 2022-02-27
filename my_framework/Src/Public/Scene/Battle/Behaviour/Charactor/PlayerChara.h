#pragma once
/*-----------------------------------------------------------

	PlyaerChara�N���X(BattleChara�̔h��
	�o�g���Ŏg�p����v���C���[�L�����̋���

-------------------------------------------------------------*/
namespace nsBattle {
	class PlayerChara : public BattleChara {
	private:
		//�ړ������̊֐��|�C���^
		std::function<bool(PlayerChara&)> fMoveFunc;
		//�U�������̊֐��|�C���^
		std::function<bool(PlayerChara&)> fSkillFunc;

		//�ړ��O�̈ʒu
		noDel_ptr<Square> pBeforeSquare = {};

	private:
		//�ړ�����------------------------------------------------
		bool ShowMoveArea();
		bool SelectMoveSquare();
		bool MoveToTarget();

		//�X�L������--------------------------------------------
		bool ShowSkillRange();
		//�U�����Ώۂ̑I��
		bool SelectEnemy();
		//�U���ȊO�Ώۂ̑I��
		bool SelectFriend();

	public:
		void Start() override;

		//�ړ��n����
		bool Move();
		
		//�X�L���n����
		bool SkillFunc();

		//�ȑO�̈ʒu�ɖ߂�
		void BackBeforePos();

		//�ړ��O�̃}�X��o�^
		void SetBeforeSquare() { pBeforeSquare = pCurSquare; }

	};
}
