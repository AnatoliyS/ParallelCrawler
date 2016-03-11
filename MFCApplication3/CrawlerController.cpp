#include "stdafx.h"
#include "CrawlerController.h"

namespace crawler {

	CrawlerController::CrawlerController(shared_ptr<Crawler> _crawler, shared_ptr<CrawlerView> _view) 
		: crawler(_crawler), view(_view) 
	{
		// Called when task is finished
		crawler->setOnTaskFinishedListener([&](const Task task) {
			// In MFC controls are not thread-safe, so we should not modify UI from thread other than UI.
			// Instead, we post message to view to Update itself (to add new task).
			// We create copy on heap, cause it should live until will be processed in GUI thread.
			// We could not use unique_ptr here, cause in PostMessage we should pass pointer.
			if (view) {
				Task* new_task = new Task(task);
				// Send msg to GUI view, which will be proccessed in main thread
				::PostMessage(view->getHandle(), WM_UPDATE_CONTROL, 0, reinterpret_cast<LPARAM>(new_task));
			}
		});

		view->setOnStartButtonListener([&] {	
			view->UpdateData(TRUE);
			std::string url(CT2CA(view->mURL));
			std::string pattern(CT2CA(view->mPattern));
			view->UpdateData(FALSE);
			crawler->start(url, pattern, view->mMaxPages, view->mMaxThreads);
		});

		view->setOnStopButtonListener([&] {
			crawler->stop();
		});
	}

	CrawlerController::~CrawlerController() {
	}

}