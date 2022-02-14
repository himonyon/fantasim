/*-----------------------------------------------------------

	Cursor�N���X(Behaviour�̔h��)
		���̏����Ǘ�

-------------------------------------------------------------*/
#pragma once

namespace nsStrategy {
	class Country : public City {
	private:
		noDel_ptr<GameObject> pPlayerText = 0;
		noDel_ptr<Sprite> pCountry_sp = 0;
		//�����Ă���X
		bool isPlayer = false;
		//�ח�������
		bool isFalled = false;

	public:
		std::vector<noDel_ptr<City>> vOwnCities;

	public:
		void Start() override;
		void Update() override;

		void SetSprite(noDel_ptr<Sprite> country, noDel_ptr<Sprite> city, noDel_ptr<Sprite> frame);

		//�����ύX
		void ChangeBelongCountry(noDel_ptr<Country> country) override;

		void SetIsPlayer();
		bool IsPlayer() { return isPlayer; }

		//�X�̔z�񂩂�X�𔲂�
		void PullOutCity(noDel_ptr<City> city);
		//�X��z��ɉ�����
		void SetOwnCity(noDel_ptr<City> city);

		//�ח����Ă��邩
		bool IsFallen() { return isFalled; }
		void SetFallen() { isFalled = true; }
	};
}
