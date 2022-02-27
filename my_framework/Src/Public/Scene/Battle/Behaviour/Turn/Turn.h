#pragma once
/*-----------------------------------------------------------

	Turn�N���X(Behaviour�̔h��) : �e�^�[���̊��N���X
	�^�[���̊Ǘ�

-------------------------------------------------------------*/
namespace nsBattle {
	//�^�[�����
	enum class eTurnState {
		None,
		Back,
		Field,
		BeforeCommand,
		AfterCommand,
		Move,
		Skill,
		SelectSkill,
		Battle,
		EnemyAttack,
		EnemyAfterAttack,
		EnemyMove,
	};
	//���݂̏�Ԃƈ�O�̏�Ԃ��������\����
	struct stTurnState {
		eTurnState state = eTurnState::None;
		std::weak_ptr<stTurnState> bef;

		stTurnState() {};
		stTurnState(eTurnState st) {
			state = st;
		}
	};

	class Turn : public Behaviour {
	protected:
		//���݂̐i�s���
		std::shared_ptr<stTurnState> curState;

		//�^�[����Ԉꗗ
		std::unordered_map<eTurnState, std::shared_ptr<stTurnState>> umStates;


	public:
		//��ԃZ�b�g
		void SetTurnState(eTurnState state, int backNum = 1);


	protected:
		//�I�𒆂̃L�����N�^
		noDel_ptr<BattleChara> pSelectChara;

		//�G�ꗗ
		std::vector <noDel_ptr<PlayerChara>> vPlayer;
		//�v���C���[�ꗗ
		std::vector <noDel_ptr<EnemyChara>> vEnemy;
		
		//�Q�[���I���t���O
		bool isGameEnd = false;

	public:
		void Start() override;

		//�L�����N�^�[��BattleChara�N���X�擾
		std::vector<noDel_ptr<BattleChara>> vBattleChara;

	protected:
		//�J�����ړ�
		void MoveCamera();

		//�S�Ŕ���
		void CheckEndGame();

		//�V�[���J��
		void TransScene();


	public:
		virtual void TurnInit();

		//�Q�[���I������
		void CheckGameEnd();
	};
}
