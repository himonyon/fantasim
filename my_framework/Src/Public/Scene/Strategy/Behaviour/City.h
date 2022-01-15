/*-----------------------------------------------------------

	City�N���X(Behaviour�̔h��)
		�X�̏����Ǘ�

-------------------------------------------------------------*/
#pragma once

namespace nsStrategy {

	class Country;

	class City : public Behaviour {
	protected:
		//Find����I�u�W�F�N�g
		noDel_ptr<GameObject> pCityPanel;

	protected:
		//�X�摜�̘g
		noDel_ptr<GameObject> pCityFrame;

		//�X�v���C�g
		noDel_ptr<Sprite> pCity_sp;
		noDel_ptr<Sprite> pCityFrame_sp;

		//ID
		int id = 0;
		//���O
		std::wstring name = L"";
		//�ׂ荇���̒n
		std::vector<noDel_ptr<City>> vNeighbor; 

	public:
		//�X�e�[�^�X
		noDel_ptr<Country> pCountry = 0;
		int money = 0;
		int economic = 0;
		int political = 0;
		int disaster = 0;
		int support = 0;
		float bussiness = 0;

		//���L�L����
		std::vector<noDel_ptr<Charactor>> vOwnChara;

	public:
		virtual ~City() {};

		void Start() override;
		void Update() override;
		void OnTriggerEnter2D(noDel_ptr<Collider2D> hitCol) override;
		void OnTrigger2D(noDel_ptr<Collider2D> hitCol) override;
		void OnTriggerExit2D(noDel_ptr<Collider2D> hitCol) override;

		void ChangeBelongCountry(noDel_ptr<Country> country);

		//�摜����p
		void SetSprite(noDel_ptr<Sprite> city, noDel_ptr<Sprite> frame);

		//�摜�̃t�H�[�J�X
		void FocusCity(bool trg);

		//Setter/getter
		void SetID(int id) { this->id = id; }
		int GetID() { return id; }

		void SetName(std::wstring name) { this->name = name; }
		std::wstring& GetName(){ return name; }

		void SetNeighbor(noDel_ptr<City> city) { vNeighbor.emplace_back(city); }
		std::vector<noDel_ptr<City>> GetNeighbor() { return vNeighbor; }

		std::wstring GetBussinessText();
	};
}
