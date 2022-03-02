#include "../../framework.h"
#include "../../environment.h"
#include "Strategy/Behaviour/City.h"

using namespace nsStrategy;

noDel_ptr<City> Debug::pCities[TERRITORY_NUM] = {};

//キャラクターステータス（前のシーンから引き継ぐ）
std::vector<noDel_ptr<Charactor>> Debug::vChara = {};

bool Debug::open = false;
int Debug::scene = 0;

int Debug::rowCursor = 0;
int Debug::colCursor = 0;

void Debug::Execute() {
	DebugFont::SetFontSize(22.0f);
	DebugFont::SetColor(0xffffffff);
	if (scene == (int)eSceneTable::Battle) Battle();
	else Strategy();

	if (Keyboard::Trg(DIK_F10)) open = false;
}

void Debug::Strategy() {
	if (Keyboard::Trg(DIK_RIGHT)) rowCursor = CountUpInRange(rowCursor, TERRITORY_NUM - 1);
	if (Keyboard::Trg(DIK_LEFTARROW)) rowCursor = CountDownInRange(rowCursor, TERRITORY_NUM - 1);
	if (Keyboard::Trg(DIK_UPARROW)) colCursor =  CountUpInRange(colCursor, COUNTRY_NUM - 1);

	DebugFont::Print(50, 20, L"← 街名 : %s →", pCities[rowCursor]->GetName().c_str());
	DebugFont::Print(50, 80, L"↑ 変更先 : %s", pCities[colCursor]->GetName().c_str());
	DebugFont::Print(50, 130, L"SPACE：所属国変更");

	if (Keyboard::Trg(DIK_SPACE)) {
		noDel_ptr<Country> _cr = dynamic_noDel_cast<Country>(pCities[colCursor]);
		pCities[rowCursor]->ChangeBelongCountry(_cr);
	}
}
void Debug::SetStrategy(noDel_ptr<City>* cities) {
	for (int i = 0; i < TERRITORY_NUM; i++) {
		pCities[i] = cities[i];
	}
}

void Debug::Battle() {
	int _size = (int)vChara.size();
	if (Keyboard::Trg(DIK_UPARROW)) colCursor = CountDownInRange(colCursor, 2);
	if (Keyboard::Trg(DIK_DOWNARROW)) colCursor = CountUpInRange(colCursor, 2);

	if (colCursor == 0) {
		if (Keyboard::Trg(DIK_RIGHTARROW)) rowCursor = CountUpInRange(rowCursor, _size - 1);
	}
	else if (colCursor == 1) {
		if (Keyboard::Trg(DIK_RIGHTARROW)) vChara[rowCursor]->hp += 10;
		if (Keyboard::Trg(DIK_LEFTARROW)) vChara[rowCursor]->hp -= 10;
		if (vChara[rowCursor]->hp < 1) vChara[rowCursor]->hp = 0;
		if (vChara[rowCursor]->hp >= vChara[rowCursor]->maxHp) vChara[rowCursor]->hp = vChara[rowCursor]->maxHp;
	}
	else {
		if (Keyboard::Trg(DIK_RIGHTARROW)) vChara[rowCursor]->mp += 10;
		if (Keyboard::Trg(DIK_LEFTARROW)) vChara[rowCursor]->mp -= 10;
		if (vChara[rowCursor]->mp < 1) vChara[rowCursor]->mp = 0;
		if (vChara[rowCursor]->mp >= vChara[rowCursor]->maxMp) vChara[rowCursor]->mp = vChara[rowCursor]->maxMp;
	}

	DebugFont::SetColor(0xffffffff);
	if (colCursor == 0) DebugFont::SetColor(0xffffff22);
	DebugFont::Print(50, 20, L"キャラ : %s →", vChara[rowCursor]->GetName().c_str());
	DebugFont::SetColor(0xffffffff);
	if (colCursor == 1) DebugFont::SetColor(0xffffff22);
	DebugFont::Print(50, 80, L"HP ← %d →", vChara[rowCursor]->hp);
	DebugFont::SetColor(0xffffffff);
	if (colCursor == 2) DebugFont::SetColor(0xffffff22);
	DebugFont::Print(50, 130, L"MP ← %d →", vChara[rowCursor]->mp);
}
void Debug::SetBattle(std::vector<noDel_ptr<Charactor>> p, std::vector<noDel_ptr<Charactor>> e) {
	vChara.clear();
	for (auto& player : p) {
		vChara.emplace_back(player);
	}
	for (auto& enemy : e) {
		vChara.emplace_back(enemy);
	}
}


