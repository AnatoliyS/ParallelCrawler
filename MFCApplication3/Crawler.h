#pragma once

#include<vector>
#include<iostream>

namespace crawler {

	class Crawler {
	public:
		std::vector<int> v;
		Crawler();
		void start() {
			std::cout << "I'm started";
		}
	private:
		int num;
	};
}