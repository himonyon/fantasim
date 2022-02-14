#pragma once
/*-----------------------------------------------------------

	SelectSkillPanel�N���X(Panel�̔h��)
	�X�L���I����ʂ̊Ǘ�

-------------------------------------------------------------*/
namespace nsBattle {
	class SelectCounterPanel : public Panel {
	private:
		//�I�����ꂽ�L�����N�^
		noDel_ptr<PlayerChara> pSelectChara;

		//�����ł��鋗��
		int range;

		//�I���J�[�\��
		noDel_ptr<GameObject> pSelecter;
		int selectNum = 0;

		//�U���X�L���ꗗ
		std::vector <noDel_ptr<Skill>> vAtSkills;

		//�X�L�����e�L�X�g
		noDel_ptr<Font> pSkillTexts[MAX_OWN_SKILL + 1];

		//�����p�e�L�X�g
		noDel_ptr<GameObject> pTitleText;
		noDel_ptr<GameObject> pBorder;
		static const int DescNum = 5;
		noDel_ptr<Font> pDescText[DescNum];

	private:
		void Awake() override;

		void Update() override;

		//�J�[�\���ړ�
		void MoveCursor();

		//���������Z�b�g����
		void SetDescText(noDel_ptr<Skill> skill);
		void SetAttackDescText(noDel_ptr<Skill> skill);

		//�X�L���I��
		bool DecideSkill();

	public:
		void Open(noDel_ptr<PlayerChara> chara, int range);
	};
}
