/*-----------------------------------------------------------

	CharaEnhancePanel
		キャラクター強化パネル

-------------------------------------------------------------*/
#pragma once

namespace nsStrategy {
	class CharaEnhancePanel : public Panel {
	private:
		//Findobject
		noDel_ptr<SoundManager> pSoundManager;

		//ステータスの上昇値
		struct stUpValue {
			const int level = 1;
			const int power = 5;
			const int hp = 10;
			const int mp = 8;
			const int def = 2;
			const int evd = 2;
			const int move = 1;
		};
		//上昇値
		stUpValue upValue;

		//対象の街
		noDel_ptr<City> pSelectCity;

		//画像
		noDel_ptr<ImageRenderer> pBorder;
		noDel_ptr<ImageRenderer> pCharaImage;

		//確認画面パネル
		noDel_ptr<GameObject> pConfirmPanel;
		noDel_ptr<Font> pConfirmText;
		noDel_ptr<Font> pYesNoText;

		//セレクトカーソル
		noDel_ptr<ImageRenderer> pCursor;
		int cursorNum;

		//テキスト---------------------------------
		//題目テキスト
		noDel_ptr<Font> pTopText;
		//強化金額テキスト
		noDel_ptr<Font> pNeedMoneyText;
		//キャラ名
		noDel_ptr<Font> pNameText[OWN_CHARACTOR_NUM];
		//属性
		noDel_ptr<Font> pAttrText;
		//レベル
		noDel_ptr<Font> pLevelText;
		//個体値
		noDel_ptr<Font> pPowerText;
		//HP
		noDel_ptr<Font> pHpText;
		//MP
		noDel_ptr<Font> pMpText;
		//防御力
		noDel_ptr<Font> pDefText;
		//回避率
		noDel_ptr<Font> pEvdText;
		//移動量
		noDel_ptr<Font> pMoveText;

		
	public:
		void Awake() override;
		void Update() override;

		void Open(noDel_ptr<City> city);

	private:

		//ステータステキストセット
		void SetStatusText(noDel_ptr<Charactor> chara);

		//カーソル移動
		void MoveCursor();

		//強化
		void Enhance();

		//強化可能か
		bool IsEnhance(noDel_ptr<Charactor> chara);

		//必要金額計算
		int GetNeedMoney(int level);
	};
}