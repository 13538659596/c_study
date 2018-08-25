#include "human.h"
#include <stdio.h>

void Human::setName(char *c_name)
{
	name = c_name;
}

void Human::setAge(unsigned int c_age)
{
	age = c_age;
}
void Human::setSex(char *c_sex)
{
	sex = c_sex;

}


void Human::printInfo()
{
	printf("name: %s\nage: %d\nsex: %s\n", name, age, sex);
}


