#pragma once
/*-----------------------------------------------------------

	Operatorクラス(Behaviour派生クラス)
	操作説明の表示

-------------------------------------------------------------*/
#pragma once

class Operation : public Behaviour{
private:
	struct stOprText {
		bool trigger; //使用済みか
		noDel_ptr<Font> pText;
	};

	//操作説明の最大数
	static const int OprNum = 5;

	//操作ボタン一覧（コントローラー）
	std::unordered_map<std::string, std::wstring> umJoyButton;
	//操作ボタン一覧（キーボード＆マウス）
	std::unordered_map<std::string, std::wstring> umKeyMouButton;

	//操作説明テキスト
	stOprText oprText[OprNum];

private:
	//初期化
	void Awake() override;

	//説明テキストの作成
	void CreateOperation();

	//操作ボタン一覧作成（コントローラー）
	void CreateJoyButton();
	//操作ボタン一覧作成（キーマウス)
	void CreateKeyMouButton();

public:
	//説明オブジェクトをセット
	void AddOperation(std::string type, std::wstring text);
	//操作説明オブジェクトのリセット
	void ResetOperation();
	//表示非表示
	void Open();
	void Hide();

};