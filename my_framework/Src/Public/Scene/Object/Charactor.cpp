#include "../../../../framework.h"
#include "../../../../environment.h"

#include "Charactor.h"

Charactor::Charactor(int id) {
	FILE* fp;
	fopen_s(&fp, "Data/Status/Common/chara.txt", "r");
	if (fp == NULL) return;
	char _key[256] = "";
	bool _flag = false;

	this->id = id;

	//キャラクター情報を読み込む
	while (!feof(fp))
	{
		if (!_flag) {
			fscanf_s(fp, "%s", _key, (int)sizeof(_key)); //読み込み
			if (strcmp(_key, "id") == 0) {
				int _id = 0;
				fscanf_s(fp, "%d", &_id);
				if (this->id == _id) _flag = true;
			}
		}
		else {
			fscanf_s(fp, "%s", _key, (int)sizeof(_key)); //読み込み

			if (strcmp(_key, "name") == 0) {
				fscanf_s(fp, "%s", _key, (int)sizeof(_key));
				WCHAR _temp[256] = L"";
				MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCCH)_key, _countof(_key), _temp, _countof(_temp));
				name = _temp;
			}
			if (strcmp(_key, "sp_name") == 0) {
				fscanf_s(fp, "%s", _key, (int)sizeof(_key));
				WCHAR _temp[256] = L"";
				MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCCH)_key, _countof(_key), _temp, _countof(_temp));
				sp_name = _temp;
				pSprite = new Sprite(L"Data/Image/Common/chara.spr", sp_name.c_str());
			}
			else if (strcmp(_key, "attr") == 0) {
				fscanf_s(fp, "%s", _key, (int)sizeof(_key));
				WCHAR _attrName[8] = L"";
				MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCCH)_key, _countof(_key), _attrName, _countof(_attrName));
				pAttr = Attribute::GetAttr(_attrName);
			}
			else if (strcmp(_key, "power") == 0) {
				fscanf_s(fp, "%d", &power);
			}
			else if (strcmp(_key, "hp") == 0) {
				fscanf_s(fp, "%d", &maxHp);
				hp = maxHp;
				
			}
			else if (strcmp(_key, "mp") == 0) {
				fscanf_s(fp, "%d", &maxMp);
				mp = maxMp;
			}
			else if (strcmp(_key, "def") == 0) {
				fscanf_s(fp, "%d", &def);
			}
			else if (strcmp(_key, "evd") == 0) {
				fscanf_s(fp, "%d", &evd);
			}
			else if (strcmp(_key, "move") == 0) {
				fscanf_s(fp, "%d", &move);
			}
			else if (strcmp(_key, "skill") == 0) {
				while (!feof(fp)) {
					fscanf_s(fp, "%s", _key, (int)sizeof(_key));
					if (strcmp(_key, "id") == 0) break;
					int _skillID = atoi(_key);
					vpSkill.emplace_back(noDel_ptr<Skill>(Skill::GetSkill(_skillID)));
				}
				break;
			}
		}
	}

	fclose(fp);
}

Charactor::Charactor(const Charactor& c) {
	id = c.id;
	name = c.name; //名前
	sp_name = c.sp_name; //スプライトの名前
	pSprite = new Sprite(L"Data/Image/Common/chara.spr", c.sp_name.c_str()); //スプライト
	pAttr = c.pAttr; //属性
	vpSkill = c.vpSkill; //スキル
	vBuff = c.vBuff;

	level = c.level;
	power = c.power; //個体値
	maxHp = c.maxHp; //HP
	hp = c.hp; //HP
	maxMp = c.maxMp; //MP
	mp = c.mp; //MP
	def = c.def; //防御力
	evd = c.evd; //回避力
	move = c.move; //移動量
}

Charactor::~Charactor() {
	delete pSprite;
	pSprite = NULL;
}
