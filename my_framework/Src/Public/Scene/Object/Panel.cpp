#include "../../../../framework.h"
#include "../../../../environment.h"

#include "Panel.h"

void Panel::Open() {
	openTrg = true;
	gameObject->SetObjEnable(true);
}

void Panel::Close() {
	openTrg = false;
	gameObject->SetObjEnable(false);
}