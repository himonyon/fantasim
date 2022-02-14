#pragma once
/*-----------------------------------------------------------

	SelectSkillPanel�N���X(Panel�̔h��)
	�X�L���I����ʂ̊Ǘ�

-------------------------------------------------------------*/
namespace nsBattle {
	class BattlePanel : public Panel {
	private:
		std::function<bool(BattlePanel&)> fBattleFunc;

		//�z�񎯕ʔԍ�
		const int playerNum = 0;
		const int enemyNum = 1;

		//�������Ă��邩
		bool counterTurn = false;

		//�^����_���[�W
		int damage = 0;
		//�����{���e�L�X�g
		std::wstring attrText;
		//����g���K�[
		bool evadeTrg = false;

		//��������L�����N�^�[
		noDel_ptr<BattleChara> pAttacker;
		noDel_ptr<BattleChara> pDefender;

		//��ʏ�ɉf���o���L�����N�^�[
		noDel_ptr<GameObject> pPlayerView;
		noDel_ptr<GameObject> pEnemyView;

		//�e���\���p�̃p�l��
		noDel_ptr<GameObject> pCharaInfoPanel[2];
		noDel_ptr<GameObject> pBattleTextPanel;

		//�����Ɩ��O
		noDel_ptr<GameObject> pAttrObj[2];
		noDel_ptr<Font> pNameText[2];

		//HP,MP�o�[�ƃe�L�X�g
		noDel_ptr<GameObject> pHPBar[2];
		noDel_ptr<GameObject> pMPBar[2];
		noDel_ptr<Font> pHPText[2];
		noDel_ptr<Font> pMPText[2];
		noDel_ptr<GameObject> pHPFrame[2];
		noDel_ptr<GameObject> pMPFrame[2];

		noDel_ptr<Font> pBattleText;

		//�A�j���[�V����
		noDel_ptr<Animator> pAttack_anim;

		//�o�g����ʂ̃^�C�}�[�Ǘ�
		Timer timer;

		//��x�������s�����������̂��߂̃g���K�[
		bool onceTrg = false;

	private:
		void Awake() override;

		void Update() override;

		void SetInfoPanel(int index);

		//HP,MP�̃o�[�̊Ǘ�
		void SetBarPosition(noDel_ptr<ImageRenderer> pTarget, noDel_ptr<ImageRenderer> pFrame, int curVal, int maxVal);

		//�J�n����
		bool InitFunc();

		//�퓬�̏���
		bool Attack();
		bool Damage();

		//���ʏ���
		bool ResultFunc();

		//�o�g�����Z
		void Battle(noDel_ptr<BattleChara> attacker, noDel_ptr<BattleChara> defender);

		//HP,MP�o�[�̐ݒ�
		void SetStatusBar();

		//�������̃A�^�b�J�[�ύX
		void ChangeAttacker();

	public:
		void Open(noDel_ptr<PlayerChara> player, noDel_ptr<EnemyChara> enemy, bool isPlayerAttack);
	};
}
