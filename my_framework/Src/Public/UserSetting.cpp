#include "../../framework.h"
#include "../../environment.h"

bool UserSetting::sound = true;
int UserSetting::device = 0;

void UserSetting::SetUpUserSetting() {
	char _key[256] = { 0 };
	FILE* fp = NULL;
	fopen_s(&fp, "Data/UserSetting.txt", "r");
	if (fp == NULL) return;
	while (!feof(fp)) {
		fscanf_s(fp, "%s", _key, (int)sizeof(_key));
		if (strcmp(_key, "sound") == 0) {
			fscanf_s(fp,"%s", _key, (int)sizeof(_key));
			sound = atoi(_key);
		}
		if (strcmp(_key, "device") == 0) {
			fscanf_s(fp, "%s", _key, (int)sizeof(_key));
			device = atoi(_key);
		}
	}
	fclose(fp);
}
void UserSetting::Save() {
	FILE* fp;
	fopen_s(&fp, "Data/UserSetting.txt", "w");
	if (fp == NULL) return;
	fprintf_s(fp, "sound %d\n", sound);
	fprintf_s(fp, "device %d ", device);
	fclose(fp);
}