/*-----------------------------------------------------------

	Cursorクラス(Behaviourの派生)
	カーソルの移動を管理

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