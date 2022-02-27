/*-----------------------------------------------------------

	OptionPanelクラス
		ゲーム設定画面

-------------------------------------------------------------*/
#pragma once

namespace nsStrategy {
	class OptionPanel : public Panel {
	private:
		//Findobject
		noDel_ptr<SoundManager> pSoundManager;

		//テキスト
		noDel_ptr<Font> pSoundText[2];
		noDel_ptr<Font> pDeviceText[3];

		//オプションの項目
		enum class eOptionSeed {
			Sound,
			Device,
			Max,
		};

		//選択された設定
		bool sound = true;
		eDeviceType deviceType = eDeviceType::Both;

		//カーソル
		int colSelecter = 0; //縦列のカーソル
		int rowSelecter = 0; //横列のカーソル

	private:
		void Awake() override;
		void Update() override;

		//オプション項目を選択
		void MoveColSelecter();
		void MoveRowSelecter();

		//サウンド項目の選択
		void SoundSetting(bool sound);
		//デバイス項目の選択
		void DeviceSetting(int type);

	public:
		void Open();
		void Close();
	};
}
