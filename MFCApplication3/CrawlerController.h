#pragma once

#include <functional>
#include "Crawler.h"
#include "CrawlerView.h"

namespace crawler {

	class CrawlerController {
	public:
		CrawlerController(shared_ptr<Crawler> _model, shared_ptr<CrawlerView> _view);
		~CrawlerController();

	private:
		// Pointers to view and model
		// These classes do not know about each other, so there are no cycled pointers
		shared_ptr<Crawler> crawler;
		shared_ptr<CrawlerView> view;
	};

}