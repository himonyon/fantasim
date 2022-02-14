#include "../../../../framework.h"
#include "../../../../environment.h"

#include "Charactor.h"

Charactor::Charactor(int id, std::string file) {
	FILE* fp;
	fopen_s(&fp, file.c_str(), "r");
	if (fp == NULL) return;
	char _key[256] = "";
	bool _flag = false;

	this->id = id;

	//�L�����N�^�[����ǂݍ���
	while (!feof(fp))
	{
		if (!_flag) {
			fscanf_s(fp, "%s", _key, (int)sizeof(_key)); //�ǂݍ���
			if (strcmp(_key, "id") == 0) {
				int _id = 0;
				fscanf_s(fp, "%d", &_id);
				if (this->id == _id) _flag = true;
			}
		}
		else {
			fscanf_s(fp, "%s", _key, (int)sizeof(_key)); //�ǂݍ���

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
			else if (strcmp(_key, "level") == 0) {
				fscanf_s(fp, "%d", &level);
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
	name = c.name; //���O
	sp_name = c.sp_name; //�X�v���C�g�̖��O
	pSprite = new Sprite(L"Data/Image/Common/chara.spr", c.sp_name.c_str()); //�X�v���C�g
	pAttr = c.pAttr; //����
	vpSkill = c.vpSkill; //�X�L��
	umBuff = c.umBuff;

	level = c.level;
	power = c.power; //�̒l
	maxHp = c.maxHp; //HP
	hp = c.hp; //HP
	maxMp = c.maxMp; //MP
	mp = c.mp; //MP
	def = c.def; //�h���
	evd = c.evd; //����
	move = c.move; //�ړ���
}

Charactor::~Charactor() {
	delete pSprite;
	pSprite = NULL;
}

void Charactor::SetBuff(const stBuff buff) {
	std::shared_ptr<stBuff> _buff = std::make_shared<stBuff>(buff);
	umBuff[_buff->type] = _buff;
}

std::shared_ptr<stBuff> Charactor::GetBuff(eBuffType type) {
	if (umBuff.find(type) == umBuff.end()) return NULL;
	return umBuff[type];
}

void Charactor::DeleteBuff(eBuffType type) {
	if (umBuff.find(type) == umBuff.end()) return;
	 umBuff.erase(type);
}
