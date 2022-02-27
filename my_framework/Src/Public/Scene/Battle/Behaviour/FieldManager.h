#pragma once
/*-----------------------------------------------------------

	FieldManager�N���X(Behaviour�̔h��)
	�t�B�[���h�S�̂̊Ǘ�

-------------------------------------------------------------*/
namespace nsBattle {
	class FieldManager : public Behaviour {
	private:
		//�T�E���h�}�l�[�W���[
		noDel_ptr<SoundManager> pSoundManager;
		//�������
		noDel_ptr<Operation> pOperation;

		//�X�e�[�W
		int stageType = 0;
		std::unordered_map<int, SquareRow*> umStageRows;

		//�L�����N�^�[��BattleChara�N���X�擾
		std::vector<noDel_ptr<BattleChara>> vBattleChara;

		//�L�����N�^�[
		std::vector<noDel_ptr<GameObject>> vPlayerChara;
		std::vector<noDel_ptr<GameObject>> vEnemyChara;

		//�^�[���}�l�[�W���[
		noDel_ptr<PlayerTurn> pPlayerTurn;
		noDel_ptr<EnemyTurn> pEnemyTurn;

		//�J�n�g���K�[
		bool startTrg = false;
		

	public:
		static const float StageSize; //�}�X�̑傫��
		static const float StageLeft; //���[�̃}�X�̍��W
		static const float StageTop; //�}�X�̈�ԏ�̍��W
		static int SquareNum_X; //�}�X�̐�X
		static int SquareNum_Y; //�}�X�̐�Y

		//�L�����N�^�[�X�e�[�^�X�i�O�̃V�[����������p���j
		std::vector<noDel_ptr<Charactor>> vPlayerCharaBace;
		std::vector<noDel_ptr<Charactor>> vEnemyCharaBace;

	public:
		~FieldManager();

		void ChangeTurn();
	private:
		//������
		void Awake() override;
		void Start() override;

	private:
		//�X�e�[�W�ƃL�����N�^�[�̔z�u
		void InitStage();
		std::string ChooseStage(); //�X�e�[�W��I��
		noDel_ptr<Sprite> GetRoad(); //���摜�擾
		noDel_ptr<Sprite> GetObstacle(); //�X�e�[�W��Q���擾
		void InitCharactor();

		//�T�E���h�쐬
		void CreateSound();

	public:
		void SetTurnState(eTurnState, int backNum = 1);
	};
}
