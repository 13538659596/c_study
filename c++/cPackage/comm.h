#include "Human.h"

struct Human {
	void(*p_setName) (char * c_name);
	void(*p_setSex) (char * c_sex);
	void(*p_setAge)(int c_age);
	void(*p_printAllInfo) ();

};
void init_human(struct Human *);
void delet_human(struct Human *);