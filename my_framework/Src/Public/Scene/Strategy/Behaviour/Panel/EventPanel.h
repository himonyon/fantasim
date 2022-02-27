/*-----------------------------------------------------------

	Event�N���X
		�C�x���g����\������

-------------------------------------------------------------*/
#pragma once

namespace nsStrategy {
	class GameManager;
	class EventPanel : public Panel {
	private:
		//Findobject
		noDel_ptr<GameManager> pGM;
		noDel_ptr<SoundManager> pSoundManager;

	private:
		//�e�L�X�g�̍ő吔
		static const int EventMax = 10;

		//�e�L�X�g
		struct stEventText {
			bool flag = false;
			noDel_ptr<Font> pText;
		};
		noDel_ptr<Font> pTitleText;
		stEventText pTexts[EventMax];

	public:
		void Awake() override;
		void Update() override;

		void Open() override;
		void Close() override;

	private:
		//�󂢂Ă���e�L�X�g�ɏ�������
		noDel_ptr<Font> GetEventText();

		//�C�x���g����
		void Event();

		//�e�X�ł̃C�x���g�v�Z
		void EventCulc(int month);
		//����
		void Income(noDel_ptr<City> city);
		//�x��������
		void DecSupport(noDel_ptr<City> city);
		//�ЊQ�ϐ�����
		void DecDisaster(noDel_ptr<City> city);
		//�䕗
		void Typhoon(noDel_ptr<City> city);
		//�\��
		void Riot(noDel_ptr<City> city);
		
	};
}
