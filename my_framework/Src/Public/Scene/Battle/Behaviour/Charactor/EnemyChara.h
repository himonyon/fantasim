#pragma once
/*-----------------------------------------------------------

	BattleChara�N���X(BattleChara�̔h��
	�o�g���Ŏg�p����G�L�����̋���

-------------------------------------------------------------*/
namespace nsBattle {
	class EnemyChara : public BattleChara {
	private:
		std::function<bool(EnemyChara&)> fMoveFunc;

	private:
		//�ړ���̐ݒ�
		bool StartMove();
		//�ڕW�n�_�ֈړ�
		bool MoveToTarget();

	public:
		void Start() override;

		//�͈͓��Ŏd�l�ł���X�L����Ԃ�
		void ChooseSkill(eSkillType);

		//�ݒ肳�ꂽ�X�L���̏���
		bool Skill();

		//�ړ�����
		bool Move();
		
	};
}
