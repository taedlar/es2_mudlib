// skill.h
#ifndef __SKILL__
#define __SKILL__

#define SKILL_FLAG_ABANDONED	0x8000

varargs int query_skill(string, int);
string skill_mapped(string);
varargs void improve_skill(string skill, int amount);

#endif
