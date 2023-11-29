#include <iostream>

int main()
{
    std::cout << "Hello World" << std::endl;
    float *ptr1, val=3.14;
	char *ptr2;
	ptr1 = &val;
	ptr2 = &val;
    return 0;
}