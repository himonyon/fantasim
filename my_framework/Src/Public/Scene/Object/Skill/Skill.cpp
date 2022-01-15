#include "../../../../../framework.h"
#include "../../../../../environment.h"

#include "Skill.h"

std::unordered_map<int, Skill*> Skill::umSkill = {};

void Skill::DestroySkill() {
	for (auto& skill : umSkill) {
		delete skill.second;
	}
	umSkill.clear();
}