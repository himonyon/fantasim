/*-----------------------------------------------------------

	GameManager�N���X(Behaviour�̔h��)
	�X�g���e�W�[�V�[���S�̂̊Ǘ�

-------------------------------------------------------------*/
#pragma once
namespace nsStrategy {
	struct stBattleData {
		bool data = false;
		bool isPlayerTurn = true;
		bool isWin = true;
		noDel_ptr<City> pP_City;;
		noDel_ptr<City> pE_City;;
	};

	class GameManager : public Behaviour {
	private:
		//�Z�[�u�Ǘ�
		std::unique_ptr<Save> saveManager;

		//�o�g����̃f�[�^
		stBattleData battleData;

		//�X�e�[�W
		noDel_ptr<GameObject> pBg;
		noDel_ptr<City> pCities[TERRITORY_NUM];
		std::vector<noDel_ptr<GameObject>> vpRoad;

		//�J�[�\��
		noDel_ptr<GameObject> pCursor;

		//�p�l��
		noDel_ptr<SelectCountryPanel> pSelectCountryPanel;

		//�L�����N�^�[
		std::unordered_map<int, Charactor*> umCharactors;

		//�^�[��
		noDel_ptr<PlayerTurn> pPlayerTurn;
		noDel_ptr<EnemyTurn> pEnemyTurn;

		//�T�E���h
		noDel_ptr<SoundManager> pSoundManager;

		//�������
		noDel_ptr<Operation> pOperation;

		//�Q�[���I���t���O
		bool isGameEnd = false;

		//�Q�[�����̔N��
		noDel_ptr<Font> pDateText;
		int year = 1000;
		int month = 1;

	public:
		//�f�X�g���N�^
		~GameManager();

		//������
		void Awake(void) override;

		//�^�[���؂�ւ�
		void ChangeTurn();

		//�I�����ꂽ�X���^�[���ɓn��
		void SetFocusCity(noDel_ptr<City> city);

		//�^�[�Q�b�g�ɂ���X�̃Z�b�g
		void SetTargetCity(noDel_ptr<City> city);

		//�Q�[���J�n�t���O
		void GameStart();

		//�Q�[���I���i�N���A�E�Q�[���I�[�o�[�j
		void EndGame(bool isClear); //�I���t���O�𗧂Ă�
		bool IsEndGame();//�I���m�F
		void CheckClear(); //�N���A�m�F

		//���̎擾
		noDel_ptr<City> GetCity(int id);

		//�^�[����ԃZ�b�g
		void SetTurnState(eTurnState state, int backNum = 1);

		//�Z�[�u
		void SaveGame();
		//�퓬����X�̕ۑ�(p_id = �v���C���[�̊XID e_id = �G�̊XID)
		void SaveBattleInfo(int p_id, int e_id);

		//�N���X�V
		void UpdateMonth();
		//�N���擾
		int GetYear() { return year; }
		int GetMonth() { return month; }

	private:
		//�̒n�̏�����
		void CreateCharactor(bool saveData);
		void CreateTerritory();

		//�T�E���h�쐬
		void CreateSound();

		//���[�h(��{���)
		bool LoadBaceDate();

		//���[�h(�o�g�����)
		bool LoadBattleInfo();
	};
}