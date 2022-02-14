#include "../../../../../framework.h"
#include "../../../../../environment.h"

#include "HealSkill.h"

void HealSkill::InitHealSkill() {
	FILE* fp;
	fopen_s(&fp, "Data/Status/Common/heal_skill.txt", "r");
	if (fp == NULL) return;

	char _key[256] = "";
	HealSkill* _pInstance = 0;

	//ŠeƒXƒLƒ‹î•ñ‚ð“Ç‚Ýž‚Þ
	while (!feof(fp))
	{
		fscanf_s(fp, "%s", _key, (int)sizeof(_key));

		if (strcmp(_key, "id") == 0) {
			_pInstance = new HealSkill();
			fscanf_s(fp, "%d", &_pInstance->id);
			_pInstance->skillType = eSkillType::Heal;
			_pInstance->pAttr = Attribute::GetAttr(L"–³");
		}
		if (_pInstance != NULL) {
			if (strcmp(_key, "name") == 0) {
				fscanf_s(fp, "%s", _key, (int)sizeof(_key));
				WCHAR _temp[256] = L"";
				MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, (LPCCH)_key, _countof(_key), _temp, _countof(_temp));
				_pInstance->name = _temp;
			}
			else if (strcmp(_key, "isFixed") == 0) {
				int _temp = 0;
				fscanf_s(fp, "%d", &_temp);
				if (_temp == 1) _pInstance->isFixed = true;
			}
			else if (strcmp(_key, "type") == 0) {
				fscanf_s(fp, "%s", _key, (int)sizeof(_key));
				if (strcmp(_key, "hp") == 0) _pInstance->healType = eHealType::HP;
				if (strcmp(_key, "all_hp") == 0) {
					_pInstance->isAllRange = true;
					_pInstance->healType = eHealType::HP;
				}
				if (strcmp(_key, "mp") == 0) _pInstance->healType = eHealType::MP;
				if (strcmp(_key, "all_mp") == 0) {
					_pInstance->isAllRange = true;
					_pInstance->healType = eHealType::MP;
				}
			}
			else if (strcmp(_key, "consume") == 0) {
				fscanf_s(fp, "%d", &_pInstance->consumeMP);
			}
			else if (strcmp(_key, "value") == 0) {
				fscanf_s(fp, "%d", &_pInstance->heal);
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