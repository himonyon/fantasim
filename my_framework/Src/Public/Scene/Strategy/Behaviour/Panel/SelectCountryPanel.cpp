#include "../../../../../../framework.h"
#include "../../../../../../environment.h"

using namespace nsStrategy;

void SelectCountryPanel::Awake() {
	//サウンドマネージャー
	pSoundManager = gameObject->FindGameObject("soundManager")->GetComponent<SoundManager>();

	//カーソルと国の取得
	pCursor = gameObject->FindGameObject("cursor");
	for (int i = 0; i < COUNTRY_NUM; i++) {
		std::string _name = "city" + std::to_string(i);
		countries[i] = gameObject->FindGameObject(_name)->GetComponent<Country>();
	}

	//テキストの設定
	for (int i = 0; i < 4; i++) {
		pText[i] = gameObject->CreateObject(0, 0, 0);
		pText[i]->AddComponent<Font>();
		pText_font[i] = pText[i]->GetComponent<Font>();
		pText_font[i]->gameObject->SetRenderOrder((int)eRenderOrder::UI + 1);
		pText_font[i]->SetRectWH(
			transform->position.x,
			transform->position.y,
			gameObject->GetComponent<ImageRenderer>()->size.x,
			gameObject->GetComponent<ImageRenderer>()->size.y
		);
		pText_font[i]->SetTextAlignment(eTextAlignment::Left);
	}

	pText_font[0]->Print(30.0f, 30.0f, L"国を選択してください");
	pText_font[1]->Print(30.0f, 60.0f, L"←　           　       →");

	Open();
	SetTextState(countries[cursor]);
}

void SelectCountryPanel::Update() {

	if (Input::Trg(InputConfig::input["right"])) {
		countries[cursor]->FocusCity(false);
		cursor = CountUpInRange(cursor, COUNTRY_NUM - 1);
		countries[cursor]->FocusCity(true);
		SetTextState(countries[cursor]);
	}
	if (Input::Trg(InputConfig::input["left"])) {
		countries[cursor]->FocusCity(false);
		cursor = CountDownInRange(cursor, COUNTRY_NUM - 1);
		countries[cursor]->FocusCity(true);
		SetTextState(countries[cursor]);
	}

	if (Input::Trg(InputConfig::input["decide"])) {
		pSoundManager->Play("decide"); //決定音
		//操作する国を選択
		countries[cursor]->SetIsPlayer();
		//パネル閉じる
		Close();
		//ゲーム開始
		gameObject->FindGameObject("gameManager")->GetComponent<GameManager>()->GameStart();
		//カーソル表示
		pCursor->SetObjEnable(true);
		pCursor->transform->position.x = countries[cursor]->transform->position.x;
		pCursor->transform->position.y = countries[cursor]->transform->position.y;
		//このパネルを破棄
		gameObject->Destroy();
		for (int i = 0; i < 4; i++) {
			gameObject->Destroy(pText[i]);
		}
	}

	
	WCHAR description[COUNTRY_NUM][256] = {
		L"領地数が少なく、内政も\n整っていない発展途上国。\n強力な兵士がそろっている。\n領地数 : 3\n難易度 : HARD",
		L"いたって平均的な国。\n内政が整っているため\n内乱が起きにくい。\n領地数 : 5\n難易度 : NORMAL",
		L"災害に強く比較的安定\nしている国。\n金銭面にも余裕あり\n領地数 : 4\n難易度 : NORMAL",
		L"最近力をつけてきた国で、\n領地数が圧倒的に多い。\n国の位置が大陸の中央のため\n他国から狙われやすい\n領地数 : 7\n難易度 : EASY",
		L"内政、軍事ともにバランスの\n取れた国。\n隣国が強力な国に囲まれている。\n領地数 : 4\n難易度 : NORMAL",
		L"好戦的ではないがかなりの\n兵士を有している。\n金銭面には少し不安がある\n領地数 : 4\n難易度 : EASY",
	};

	unsigned int col = ColorConvert16(textCol);

	pText_font[2]->SetColor(col);
	pText_font[2]->Print(70.0f, 60.0f, L"%s", countries[cursor]->GetName().c_str());
	pText_font[3]->SetColor(0xffffffff);
	pText_font[3]->Print(30.0f, 80.0f, 
		L"----------------------------\n%s", description[cursor]);
}

void SelectCountryPanel::SetTextState(noDel_ptr<Country> country) {
	textCol = country->gameObject->GetComponent<ImageRenderer>()->GetColor();
}