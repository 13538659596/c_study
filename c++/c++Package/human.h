struct Human {
	private:
		unsigned int age;
		char *name;
		char *sex;
	public:
		void setName(char *);
		void setAge(unsigned int );
		void setSex(char *);
		void printInfo();
};
