/*-----------------------------------------------------------

	Cursor�N���X(Behaviour�̔h��)
	�J�[�\���̈ړ����Ǘ�

-------------------------------------------------------------*/
#pragma once
namespace nsStrategy {
	class Cursor : public Behaviour {
	public:
		float speed = 8;

	public:
		void Start(void) override;
		void Update(void) override;
	};
}