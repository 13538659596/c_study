#include "human.h"
#include <stdlib.h>

int main() 
{
	struct Human *person = (struct Human *)malloc(sizeof(struct Human));
	if(person == NULL) {
		perror("malloc");
		return -1;
	}
	person->setName((char *)"xiaowang");
	person->setAge(20);
	person->setSex((char *)"mail");
	person->printInfo();
	free(person);
	return 0;
}
