#pragma once
/*-----------------------------------------------------------

	BattleChara�N���X(Behaviour�̔h��)�F�v���C���[�A�G�L�����N���X�̊��N���X
	�o�g���Ŏg�p����L�����̋���

-------------------------------------------------------------*/
namespace nsBattle {
	class BattleChara : public Behaviour {
	private:
		static bool OnHitFunc;

	protected:
		//�񕜁E�o�t�A�j���[�V����
		noDel_ptr<Animator> pHealAnimator;

		//�o�H�T���C���X�^���X
		static Astar pSearcher;

		//����ł���}�X
		noDel_ptr<Square> pCurSquare;

		//�ړ����̃J�E���g
		int moveCount = 0;

		//�s���I��������
		bool actionEnable = true;

	public:
		//�����蔻��::�L�������I�����ꂽ��
		static noDel_ptr<BattleChara> pCollisionChara;

		//�L�����̃X�e�[�^�X
		noDel_ptr<Charactor> pCharaInfo;

		//�^�[�Q�b�g�ɂ���L�����N�^
		noDel_ptr<BattleChara> pTargetChara;
		//�W�I�܂ł̋���
		int rangeToTarget = 0;

		//�J�[�\�����������Ă��邩
		bool isCursorHovered = false;

		//�I�������X�L��
		noDel_ptr<Skill> pSelectSkill;
	protected:
		//�����蔻��
		void OnTriggerEnter2D(noDel_ptr<Collider2D> hitCollider) override;
		void OnTrigger2D(noDel_ptr<Collider2D> hitCollider) override;
		void OnTriggerExit2D(noDel_ptr<Collider2D> hitCollider) override;

	protected:
		void Awake() override;

		//�L�����̈ړ�
		bool Move(noDel_ptr<Square> start);

		//��
		void Heal();

		//�o�t�t�^
		void Buff();

	public:
		//���S����
		void Death();
		bool IsDeath() { return !gameObject->IsObjEnable(); }

		//�o�t�̃J�E���g�����炷
		void DecreaseBuffCount();

		//Setter,Getter
		void SetCurrentSquare(noDel_ptr<Square> square);
		noDel_ptr<Square> GetCurrentSquare() { return pCurSquare; }

		void SetActionEnable(bool flag);
		bool IsActionEnable() { return actionEnable; }

		//�����蔻�菈�����J�n�A���~����
		static void SetOnHitFunc(bool flag) { OnHitFunc = flag; if (!flag) pCollisionChara == NULL; }
	};
}
