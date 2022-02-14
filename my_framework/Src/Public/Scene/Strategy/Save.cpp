#include "../../../../framework.h"
#include "../../../../environment.h"

using namespace nsStrategy;


void Save::SaveBaceData() {
	FILE* fp;
	fopen_s(&fp, "Data/SaveData/base.txt", "w");
	if (fp == NULL) return;
	fprintf_s(fp, "save");

	fclose(fp);
}
void Save::SaveCities() {
	FILE* fp;
	_wfopen_s(&fp, L"Data/SaveData/city.txt", L"w");
	if (fp == NULL) return;
	for (int i = 0; i < TERRITORY_NUM; i++) {
		//生成
		fprintf_s(fp, "new ");
		//ID
		fprintf_s(fp, "id %d ", pCities[i]->GetID());
		//名前
		char _name[256];
		WideCharToMultiByte(CP_ACP, 0, pCities[i]->GetName().c_str(), pCities[i]->GetName().length() *4, _name, _countof(_name), NULL, NULL);
		fprintf_s(fp, "name %s ", _name);
		//国の場合
		if (i < COUNTRY_NUM) {
			//陥落しているか
			if (dynamic_noDel_cast<Country>(pCities[i])->IsFallen()) {
				fprintf_s(fp, "fallen ");
			}
		}
		//所属国ID
		fprintf_s(fp, "country %d ", pCities[i]->pCountry->GetID());
		//位置
		fprintf_s(fp, "position %f %f ", pCities[i]->transform->position.x, pCities[i]->transform->position.y);
		//国の場合
		if (i < COUNTRY_NUM) {
			//プレイヤーの国か
			if (dynamic_noDel_cast<Country>(pCities[i])->IsPlayer()) {
				fprintf_s(fp, "player ");
			}
		}
		//隣国ID
		fprintf_s(fp, "neighbor ");
		for (auto& neigh : pCities[i]->GetNeighbor()) {
			fprintf_s(fp, "%d ", neigh->GetID());
		}
		//資金
		fprintf_s(fp, "money %d ", pCities[i]->money);
		//経済力
		fprintf_s(fp, "economic %d ", pCities[i]->economic);
		//政策力
		fprintf_s(fp, "political %d ", pCities[i]->political);
		//災害耐性
		fprintf_s(fp, "disaster %d ", pCities[i]->disaster);
		//支持率
		fprintf_s(fp, "support %d ", pCities[i]->support);
		//景気
		fprintf_s(fp, "bussiness %f ", pCities[i]->bussiness);
		//所持キャラID
		fprintf_s(fp, "chara ");
		for (auto& chara : pCities[i]->vOwnChara) {
			fprintf_s(fp, "%d ", chara->GetID());
		}

		//改行
		fprintf_s(fp, "\n");
	}

	fclose(fp);
}
void Save::SaveCharactor() {
	FILE* fp;
	_wfopen_s(&fp, L"Data/SaveData/chara.txt", L"w");
	if (fp == NULL) return;
	for (auto& ch: umCharactors) {
		Charactor* chara = ch.second;

		//生成
		fprintf_s(fp, "new ");
		//ID
		fprintf_s(fp, "id %d ", chara->GetID());
		//名前
		char _name[256];
		WideCharToMultiByte(CP_ACP, 0, chara->GetName().c_str(), _countof(_name), _name, 256, NULL, NULL);
		fprintf_s(fp, "name %s ", _name);
		//画像の名前
		WideCharToMultiByte(CP_ACP, 0, chara->GetSpriteName().c_str(), _countof(_name), _name, 256, NULL, NULL);
		fprintf_s(fp, "sp_name %s ", _name);
		//属性名
		WideCharToMultiByte(CP_ACP, 0, chara->GetAttr()->GetName().c_str(), _countof(_name), _name, 256, NULL, NULL);
		fprintf_s(fp, "attr %s ", _name);
		//レベル
		fprintf_s(fp, "level %d ", chara->level);
		//戦闘力
		fprintf_s(fp, "power %d ", chara->power);
		//HP
		fprintf_s(fp, "hp %d ", chara->maxHp);
		//MP
		fprintf_s(fp, "mp %d ", chara->maxMp);
		//DEF
		fprintf_s(fp, "def %d ", chara->def);
		//EVD
		fprintf_s(fp, "evd %d ", chara->evd);
		//MOVE
		fprintf_s(fp, "move %d ", chara->move);
		//SKILL
		fprintf_s(fp, "skill ");
		for (auto& skill : chara->GetSkills()) {
			fprintf_s(fp, "%d ", skill->GetID());
		}

		//改行
		fprintf_s(fp, "\n");
	}

	fclose(fp);
}
void Save::SaveBattleCities(int p_id, int e_id, bool playerTurn) {
	//全体保存
	SaveBaceData();
	SaveCities();
	SaveCharactor();

	//バトル情報の保存
	FILE* fp;
	fopen_s(&fp, "Data/SaveData/battleCity.txt", "w");
	if (fp == NULL) return;

	//どちらのターンか
	fprintf_s(fp, "turn ");
	if(playerTurn) fprintf_s(fp, "player\n");
	else fprintf_s(fp, "enemy\n");

	//戦闘する街ID保存
	fprintf_s(fp, "p_city %d\n", p_id);
	fprintf_s(fp, "e_city %d\n", e_id);

	fclose(fp);
}