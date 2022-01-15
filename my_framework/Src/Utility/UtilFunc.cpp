#include "../../framework.h"
#include "../../environment.h"
#include "UtilFunc.h"

unsigned int ColorConvert16(stColor4 color) {
	unsigned int r = unsigned int((int(color.r * 255)) * (256 * 256));
	unsigned int g = unsigned int((int(color.g * 255)) * (256));
	unsigned int b = unsigned int(int(color.b * 255));
	unsigned int s = r + g + b + unsigned int(0xff000000);
	return s;
 }

stVector3 RoundDec3forVector(stVector3 vec) {
	vec.x = round(vec.x * 1000) / 1000;
	vec.y = round(vec.y * 1000) / 1000;
	vec.z = round(vec.z * 1000) / 1000;
	return vec;
}
float RoundDec3forFloat(float val) {
	val = round(val * 100.0f) / 100.0f;
	return val;
}


//”ÍˆÍ“à‚Å”š‚ğ‘Œ¸‚³‚¹‚é
//intŒ^‚Ì”š‚Ì{‚P‚ğw’è‚Ì•(0~maxNum)‚Ås‚¤
int CountUpInRange(int target, int maxRange) {
	if (target + 1 > maxRange) return 0;
	return ++target;
}
//intŒ^‚Ì”š‚Ì{‚P‚ğw’è‚Ì•(0~maxNum)‚Ås‚¤
int CountDownInRange(int target, int maxRange) {
	if (target - 1 < 0) return maxRange;
	return --target;
}

