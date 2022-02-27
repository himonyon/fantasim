/*-----------------------------------------------------------

	Eventクラス
		イベント情報を表示する

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
		//テキストの最大数
		static const int EventMax = 10;

		//テキスト
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
		//空いているテキストに書き込む
		noDel_ptr<Font> GetEventText();

		//イベント処理
		void Event();

		//各街でのイベント計算
		void EventCulc(int month);
		//収入
		void Income(noDel_ptr<City> city);
		//支持率減少
		void DecSupport(noDel_ptr<City> city);
		//災害耐性減少
		void DecDisaster(noDel_ptr<City> city);
		//台風
		void Typhoon(noDel_ptr<City> city);
		//暴動
		void Riot(noDel_ptr<City> city);
		
	};
}
