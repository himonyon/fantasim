/*-----------------------------------------------------------

	Cursor�N���X(Behaviour�̔h��)
	�J�[�\���̈ړ����Ǘ�

-------------------------------------------------------------*/
#pragma once
namespace nsBattle {
	class Cursor : public Behaviour {
	public:
		float speed = 0.05f;

	public:
		void Start(void) override;
		void Update(void) override;
	};
}