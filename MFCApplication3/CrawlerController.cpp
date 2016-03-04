#include "stdafx.h"
#include "CrawlerController.h"

namespace crawler {

	CrawlerController::CrawlerController(Crawler& _model, CrawlerView& _view) : crawler(_model), view(_view)
	{
		
	}


	CrawlerController::~CrawlerController()
	{
	}

}