#pragma once
/*-----------------------------------------------------------

	FieldManagerクラス(Behaviourの派生)
	フィールド全体の管理

-------------------------------------------------------------*/
namespace nsBattle {
	class SelectSquare : public Behaviour {
	private:
		bool isHover = false;

		bool enable = false;

	private:
		//初期化
		void Awake() override {};

		void Start() override {};

		//処理
		void Update() override {};

		//当たり判定
		void OnTrigger2D(noDel_ptr<Collider2D> hitCollider) override;
		void OnTriggerExit2D(noDel_ptr<Collider2D> hitCollider) override;

	public:
		void Reset();

		void SetSelectEnable(bool flag);

		bool GetHoverState() { return isHover; }
		bool GetSelectEnable() { return enable; }
	};
}
