/*-----------------------------------------------------------

	���[�U�[�ݒ�N���X

-------------------------------------------------------------*/
#pragma once

class UserSetting {
public:
	static bool sound;
	static int device;

	static void SetUpUserSetting();
	static void Save();
};