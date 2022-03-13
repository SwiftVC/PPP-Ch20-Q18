/*
*		Author:	CS
*		13/03/2022
*		
*		Add a range-checked iterator support for vector.
*		Edited copy of Ch20 Q05.
*		The iterator keeps track of the vector object which is was made to return the most updated vector<double>::size()
*		as well as the correct elem address.
*/

#include <iostream>
#include <exception>
#include <memory>
#include "cs_allocator.h"
#include "cs_vector.h"
#include <concepts>
#include <string>

int main() {
	try {
		{
			cs_vector<double> dbl_vect(3);
			dbl_vect[0] = 6.5;
			dbl_vect[1] = 80;
			dbl_vect[2] = 27;
			std::cout << "Attempting to deincrement by \"vect.begin() - 3\"\n";
			try {
				dbl_vect.begin() - 3;
			}
			catch (std::exception& e) {
				std::cerr << "exception: " << e.what() << std::endl;
			}

			std::cout << "\nPrinting [6.5, 80, 27] in STL style, using a C++11 range-based-for-loop:\n";
			for (auto i : dbl_vect)
				std::cout << i << std::endl;

			std::cout << "\nAttempting to increment past the end by \"++vect.end()\":\n";
			try {
				++dbl_vect.end();
			}
			catch (std::exception& e) {
				std::cerr << "exception: " << e.what() << std::endl;
			}

			std::cout << "\nFinally, checking the effects of repeated vector<double>::push_back()s against an existing iterator:\n";
			auto it = dbl_vect.begin();
			for (int i = 0; i < 10; i++)
				dbl_vect.push_back(i*10);
			while (it != dbl_vect.end())
			{
				std::cout << *it << ' ';
				++it;
			}
			std::cout << "\nWe conclude the iterator keeps track of its own index and the vector itself as opposed to the element address,\n"
				<< "it stays valid after the reallocation of the vector object's |T* elem|.\n";
		}
	return 0;
	}
	catch (std::exception& e) {
		std::cerr << "exception: " << e.what() << std::endl;
		return 1;
	}
	catch (...) {
		std::cerr << "exception\n";
		return 2;
	}
}