#pragma once
/*-----------------------------------------------------------

	FieldManager�N���X(Behaviour�̔h��)
	�t�B�[���h�S�̂̊Ǘ�

-------------------------------------------------------------*/
namespace nsBattle {
	class SelectSquare : public Behaviour {
	private:
		bool isHover = false;

		bool enable = false;

	private:
		//������
		void Awake() override {};

		void Start() override {};

		//����
		void Update() override {};

		//�����蔻��
		void OnTrigger2D(noDel_ptr<Collider2D> hitCollider) override;
		void OnTriggerExit2D(noDel_ptr<Collider2D> hitCollider) override;

	public:
		void Reset();

		void SetSelectEnable(bool flag);

		bool GetHoverState() { return isHover; }
		bool GetSelectEnable() { return enable; }
	};
}
