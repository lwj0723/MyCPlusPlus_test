#include <iostream>
#include <vector>
#include "MySmart_ptr.h"
#include "MyCommon.h"
#include "MyQueue.h"
#include <thread>

int main()
{
#if Smart_ptr
	{
		MySmart_ptr<Circle> cir(new Circle(4));
		std::cout << " Circle Area: " << cir->Area() << std::endl;

		//	MySmart_ptr<Circle> other(cir); // not ok
		MySmart_ptr<Circle> other(std::move(cir));
		std::cout << " After move operation, Circle Area: " << other->Area() << std::endl;
	}
#endif

#if 1
	bool bStop = false;
	MyQueue<int> queue; 
	std::thread read_worker([&queue, bStop]() {
		while (!bStop)
		{
			auto dat = queue.Get();
			std::cout << " Read Thread | Get val : " << dat << std::endl;
		}

	}); 

	//write worker
	std::cout << " Please input value: " << std::endl;

	int val = 0;
	while (std::cin >> val)//ctrl z -> quit
	{
		std::cout << " Write Thread | Set val : " << val << std::endl;
		queue.Push(val);
	}

	bStop = true;
	read_worker.joinable();
	std::cout << " Write Thread stop : " << std::endl;
#endif
	//
	system("pause");
	return 0;
}
