#include "comm.h"
#include <stdio.h>
#include <stdlib.h>

int main() 
{
	struct Human *person = (struct Human *)malloc(sizeof(struct Human));
	if(person == NULL) {
		perror("malloc");
	}
	init_human(person);
	person->p_setName("xiaowang");
	person->p_setAge(20);
	person->p_setSex("mail");
	person->p_printAllInfo();
	delet_human(person);
	return 0;
}
