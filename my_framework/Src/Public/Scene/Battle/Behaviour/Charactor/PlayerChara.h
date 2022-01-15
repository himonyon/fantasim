#pragma once
/*-----------------------------------------------------------

	BattleChara�N���X(BattleChara�̔h��
	�o�g���Ŏg�p����v���C���[�L�����̋���

-------------------------------------------------------------*/
namespace nsBattle {
	class PlayerChara : public BattleChara {
	private:
		std::function<bool(PlayerChara&)> fCharaFunc;

	public:
		void Update() override;

		//�ړ��n����
		bool SelectMoveSquare();
		bool Move();

		//�ړ���ԂɑJ��
		void StartMoveFunc();

		//�U�������J��
		void StartAttackFunc(noDel_ptr<AttackSkill> skill);
		bool SelectEnemy();

		//�񕜃X�L�������J��
		void StartHealFunc(noDel_ptr<AttackSkill> skill);
		bool SelectFriend();

		//�o�t�X�L�������J��
		void StartBuffFunc(noDel_ptr<AttackSkill> skill);
	};
}
