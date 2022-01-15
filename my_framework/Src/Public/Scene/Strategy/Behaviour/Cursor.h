/*-----------------------------------------------------------

	Cursorクラス(Behaviourの派生)
	カーソルの移動を管理

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