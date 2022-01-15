#pragma once
/*-----------------------------------------------------------

	BattleChara�N���X(Behaviour�̔h��)
	�o�g���Ŏg�p����L�����̋���

-------------------------------------------------------------*/
namespace nsBattle {
	enum class eFieldState {
		None,
		Field,
		Command,
	};

	class Turn : public Behaviour {
	public:
		//�Q�[���̐i�s���
		static eFieldState curState;


	protected:
		//�I�𒆂̃L�����N�^
		noDel_ptr<BattleChara> pSelectChara;

	public:
		//�L�����N�^�[��BattleChara�N���X�擾
		std::vector<noDel_ptr<BattleChara>> vBattleChara;

	protected:

		//�J�����ړ�
		void MoveCamera();
	};
}
