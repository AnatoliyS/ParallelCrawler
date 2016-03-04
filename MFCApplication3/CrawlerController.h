#pragma once


#include <functional>
#include "Crawler.h"
#include "CrawlerView.h"

namespace crawler {

	class CrawlerController {
	private:
		Crawler &crawler;
		CrawlerView &view;

	public:
		void s() {
			//CrawlerView* p = nullptr;
		}
		CrawlerController(Crawler& _model, CrawlerView& _view);
		~CrawlerController();
	};

}