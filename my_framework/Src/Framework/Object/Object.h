#pragma once
/*-----------------------------------------------------------

	�I�u�W�F�N�g�N���X
		�I�u�W�F�N�g�̋��ʂ̊Ǘ�
		�R���|�[�l���g�̏��L

-------------------------------------------------------------*/

class Object {
private:
	static int max_instance;
	int instanceID = 0;

public:
	Object();
	virtual ~Object() {};

	int GetInstanceID() const { return instanceID; }

};