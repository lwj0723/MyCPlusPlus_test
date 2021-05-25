#include <iostream>
#include <vector>
#include "MySmart_ptr.h"

int main()
{
	{
		MySmart_ptr<Circle> cir(new Circle(4));
		std::cout << " Circle Area: " << cir->Area() << std::endl;

		//	MySmart_ptr<Circle> other(cir); // not ok
		MySmart_ptr<Circle> other(std::move(cir));
		std::cout << " After move operation, Circle Area: " << other->Area() << std::endl;
	}

	system("pause");
	return 0;
}
