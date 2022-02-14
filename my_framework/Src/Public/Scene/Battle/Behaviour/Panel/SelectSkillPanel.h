#pragma once
/*-----------------------------------------------------------

	SelectSkillPanel�N���X(Panel�̔h��)
	�X�L���I����ʂ̊Ǘ�

-------------------------------------------------------------*/
namespace nsBattle {
	class SelectSkillPanel : public Panel {
	private:
		//�I�����ꂽ�L�����N�^
		noDel_ptr<PlayerChara> pSelectChara;

		//�I���J�[�\��
		noDel_ptr<GameObject> pSelecter;
		int selectNum = 0;

		//�X�L�����e�L�X�g
		noDel_ptr<Font> pSkillTexts[MAX_OWN_SKILL];

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
		void SetHealDescText(noDel_ptr<Skill> skill);
		void SetBuffDescText(noDel_ptr<Skill> skill);

		//�X�L���I��
		bool DecideSkill();

	public:
		void Open(noDel_ptr<PlayerChara> chara);
	};
}
