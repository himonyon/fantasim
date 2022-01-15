#include "../../../../../framework.h"
#include "../../../../../environment.h"

#include "BuffSkill.h"

void BuffSkill::InitBuffSkill() {
	FILE* fp;
	fopen_s(&fp, "Data/Status/Common/buff_skill.txt", "r");
	if (fp == NULL) return;

	char _key[256] = "";
	BuffSkill* _pInstance = 0;

	//ŠeƒXƒLƒ‹î•ñ‚ð“Ç‚Ýž‚Þ
	while (!feof(fp))
	{
		fscanf_s(fp, "%s", _key, (int)sizeof(_key));

		if (strcmp(_key, "id") == 0) {
			_pInstance = new BuffSkill();
			fscanf_s(fp, "%d", &_pInstance->id);
			_pInstance->skillType = eSkillType::Buff;
			_pInstance->pAttr = Attribute::GetAttr(L"–³");
		}
		if (_pInstance != NULL) {
			if (strcmp(_key, "name") == 0) {
				fscanf_s(fp, "%s", _key, (int)sizeof(_key));
				WCHAR _temp[256] = L"";
				MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCCH)_key, _countof(_key), _temp, _countof(_temp));
				_pInstance->name = _temp;
			}
			else if (strcmp(_key, "type") == 0) {
				fscanf_s(fp, "%s", _key, (int)sizeof(_key));
				if (strcmp(_key, "at") == 0) _pInstance->buffInfo.type = eBuffType::AT;
				if (strcmp(_key, "evd") == 0) _pInstance->buffInfo.type = eBuffType::EVD;
				if (strcmp(_key, "def") == 0) _pInstance->buffInfo.type = eBuffType::DEF;
			}
			else if (strcmp(_key, "consume") == 0) {
				fscanf_s(fp, "%d", &_pInstance->consumeMP);
			}
			else if (strcmp(_key, "value") == 0) {
				fscanf_s(fp, "%f", &_pInstance->buffInfo.value);
			}
			else if (strcmp(_key, "count") == 0) {
				fscanf_s(fp, "%d", &_pInstance->buffInfo.count);
			}
			else if (strcmp(_key, "min_dis") == 0) {
				fscanf_s(fp, "%d", &_pInstance->min_dis);
			}
			else if (strcmp(_key, "max_dis") == 0) {
				fscanf_s(fp, "%d", &_pInstance->max_dis);
				umSkill[_pInstance->id] = _pInstance;
				_pInstance = NULL;
			}
		}
	}

	fclose(fp);
}