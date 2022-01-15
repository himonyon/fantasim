#include "../../../../../framework.h"
#include "../../../../../environment.h"
#include "SelectSquare.h"

using namespace nsBattle;

void SelectSquare::OnTrigger2D(noDel_ptr<Collider2D> hitCollider) {
	//カーソルと接触しているか
	isHover = true;
}
void SelectSquare::OnTriggerExit2D(noDel_ptr<Collider2D> hitCollider) {
	//カーソルと接触しているか
	isHover = false;
}

void SelectSquare::Reset() {
	isHover = false;
	enable = false;
}


void SelectSquare::SetSelectEnable(bool flag){
	enable = flag;
	if (flag) {
		gameObject->GetComponent<SpriteRenderer>()->SetColor(1, 1, 1, 0.8f);
	}
	else {
		gameObject->GetComponent<SpriteRenderer>()->SetColor(1, 0.3f, 0.3f, 0.8f);
	}
}