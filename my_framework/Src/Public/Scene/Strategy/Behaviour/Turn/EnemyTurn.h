/*-----------------------------------------------------------

	EnemyTurn�N���X(Turn�h��)
		�G�̃^�[���Ǘ�

-------------------------------------------------------------*/
#pragma once


namespace nsStrategy {

	class GameManager;

	class EnemyTurn : public Turn {
	private:
		//�Q�[���}�l�[�W���[
		noDel_ptr<GameManager> pGM;

		//�o�g���p�l��
		noDel_ptr<BattlePanel> pBtPanel;

		//�v���C���[�̍�
		noDel_ptr<Country> pPlayerCountry;

		//���ݏ������Ă���X
		int curCityID = 0;
		noDel_ptr<City> pTargetCity = NULL;

		//�X�P�ʂ̏����ɂ����鎞��
		Timer timer;

		//�X�̏������I��������m�F����g���K�[
		bool isSimulated = false;

		//��������������~�߂�
		bool stopSimulate = false;

	private:
		void Start() override;
		void Update() override;

		//�V�~�����[�g
		void Simulate();

		//�e�V�~�����[�g����
		bool Battle();
		bool CharaEnhance();
		bool CharaMove();
		bool MoneyMove();
		bool EconomicUp();
		bool PoliticalUp();
		bool DisasterUp();
		bool SupportUp();

	public:
		//�^�[���J�n���̏�����
		void TurnInit();
		//�r������V�~�����[�g�ĊJ
		void TurnInit(int id);
	};
}