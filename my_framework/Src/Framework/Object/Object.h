#pragma once
/*-----------------------------------------------------------

	オブジェクトクラス
		オブジェクト、コンポーネント共通の情報

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