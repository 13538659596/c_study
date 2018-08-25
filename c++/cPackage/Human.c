#include <stdio.h>
#include <stdlib.h>
#include "comm.h"
#include "Human.h"


static void talk() 
{	
	printf("human can speek\n");
}
static void walk()
{
	printf("human can walk\n");
}

static void setName(char * c_name)
{
	name = c_name;
}
static void setAge(int c_age){
	age = c_age;
}
static void setSex(char * c_sex)
{
	sex = c_sex;
}
static void printAllInfo()
{
	printf("name: %s\nage: %d\nsex: %s\n", name, age, sex);
}


void init_human(struct Human * person)
{
	person->p_setAge = setAge;
	person->p_setName = setName;
	person->p_setSex = setSex;
	person->p_printAllInfo = printAllInfo;
}

void delet_human(struct Human *person)
{
	free(person);
	person = NULL;
}



