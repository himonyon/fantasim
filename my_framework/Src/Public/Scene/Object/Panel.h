#pragma once
/*-----------------------------------------------------------

	Panel�N���X(Behaviour�h���N���X)

-------------------------------------------------------------*/
#pragma once

class Panel : public Behaviour{
protected:
	bool openTrg = false;

public:
	virtual void Open();
	virtual void Close();

	bool IsOpen() { return openTrg; }
};