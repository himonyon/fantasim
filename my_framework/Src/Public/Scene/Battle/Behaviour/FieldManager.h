#pragma once
/*-----------------------------------------------------------

	FieldManager�N���X(Behaviour�̔h��)
	�t�B�[���h�S�̂̊Ǘ�

-------------------------------------------------------------*/
namespace nsBattle {
	class FieldManager : public Behaviour {
	private:
		//�X�e�[�W
		std::unordered_map<int, SquareRow*> umStageRows;

		//�L�����N�^�[��BattleChara�N���X�擾
		std::vector<noDel_ptr<BattleChara>> vBattleChara;

		//�L�����N�^�[
		std::vector<noDel_ptr<GameObject>> vPlayerChara;
		std::vector<noDel_ptr<GameObject>> vEnemyChara;

		//�^�[���}�l�[�W���[
		noDel_ptr<GameObject> pPlayerTurn;
		noDel_ptr<GameObject> pEnemyTurn;
		

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
	private:
		//������
		void Start() override;

		//����
		void Update() override;

	private:
		//�X�e�[�W�ƃL�����N�^�[�̔z�u
		void InitStage();
		void InitCharactor();

		
	};
}
