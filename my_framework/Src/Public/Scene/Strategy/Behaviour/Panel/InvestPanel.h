/*-----------------------------------------------------------

	InvestPanel�N���X
		�����Ŋe�X�e�[�^�X���㏸������p�l��

-------------------------------------------------------------*/
#pragma once

namespace nsStrategy {
	class InvestPanel : public Panel {
	private:
		//Findobject
		noDel_ptr<GameObject> pCityPanel;

	private:
		//�Ώۂ̍��A�X
		noDel_ptr<City> pCity;
		noDel_ptr<City> pCity2;

		//�����ォ�ǂ���
		bool isResult = false;

		//����������
		int pressCount = 0;

		//���s��
		std::function<void(InvestPanel&)> InvestFunc;

		//�������z
		int investMoney = 0;
		const int minInvestMoney = 100;
		int maxInvestMoney = 0;

		//�����O�̎����Ɗe�X�e�[�^�X�l
		int beforeMoney = 0;
		int beforeMoney2 = 0;
		int beforeValue = 0;

		//�e�L�X�g
		noDel_ptr<GameObject> pDescText;
		noDel_ptr<GameObject> pPriceText;
		noDel_ptr<GameObject> pResultText;

	private:
		void Awake() override;
		void Update() override;

		void SetInvestEconomic();
		void SetInvestPolitical();
		void SetInvestDisaster();
		void SetInvestSupport();
		void SetMoveMoney();
		void InvestEconomic();
		void InvestPolitical();
		void InvestDisaster();
		void InvestSupport();
		void MoveMoney();

	public:
		void Open(noDel_ptr<City> city, int cmd);
		void Open(noDel_ptr<City> city, noDel_ptr<City> city2);
	};
}