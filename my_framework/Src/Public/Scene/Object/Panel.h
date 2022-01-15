#pragma once
/*-----------------------------------------------------------

	Panelクラス(Behaviour派生クラス)

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