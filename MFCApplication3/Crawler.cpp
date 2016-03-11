#include "stdafx.h"
#include "Crawler.h"

namespace crawler {
	bool Crawler::isAcceptableURLChar(char c) {
		return chars.find(c) != string::npos;
	}

	bool Crawler::checkAndSetVisited(string url) {
		lock_guard<mutex> lk(set_mutex);
		bool is_visited = visited.find(url) != visited.end();
		if (!is_visited)
			visited.insert(url);
		return is_visited;
	}
	
	bool Crawler::isVisited(string url) {
		lock_guard<mutex> lk(set_mutex);
		return visited.find(url) != visited.end();
	}
	
	int Crawler::getVisitedCount() {
		lock_guard<mutex> lk(set_mutex);
		return visited.size();
	}
	
	int Crawler::process(Task& task) {
		CInternetSession session(_T("Crawler"), PRE_CONFIG_INTERNET_ACCESS);
		session.SetOption(INTERNET_OPTION_RECEIVE_TIMEOUT, REQUEST_TIMEOUT);

		unique_ptr<CHttpConnection> pServer;
		unique_ptr<CHttpFile> pFile;

		CString url = CString(task.getURL().c_str());

		try	{
			CString strServerName; // server name
			CString strObject;     // object name
			INTERNET_PORT nPort;   // port
			DWORD dwServiceType;   // service type

			// Get data from URL
			if (AfxParseURL(url, dwServiceType, strServerName, strObject, nPort) == 0) {
				task.setStatus(Task::TASK_STATUS::WRONG_URL);
				return 1;
			}

			// Establish HTTP connection
			pServer = unique_ptr<CHttpConnection> (session.GetHttpConnection(strServerName, nPort));

			// Send GET request for object ( strObject )
			pFile = unique_ptr<CHttpFile> (pServer->OpenRequest(
				CHttpConnection::HTTP_VERB_GET, strObject, NULL, 1, NULL, NULL,
				INTERNET_FLAG_EXISTING_CONNECT
				));
			pFile->SendRequest();

			DWORD dwRet; // Status code of HTTP response
			pFile->QueryInfoStatusCode(dwRet);

			CString strHeader; // header
			pFile->QueryInfo(HTTP_QUERY_RAW_HEADERS_CRLF, strHeader);

			 // if code is not OK
			if (dwRet != HTTP_STATUS_OK) {
				// Handle 404 not found
				if (dwRet == HTTP_STATUS_NOT_FOUND) {
					task.setStatus(Task::TASK_STATUS::NOTFOUND);
				} else { // Can be extended to add new types of codes
					task.setStatus(Task::TASK_STATUS::REQUEST_FAIL);
				}
				return 1;
			}

			// Read response body into string
			string body;
			int allRead = 0; // total read bytes
			int nRead = 0;   // currently read bytes

			while (nRead = pFile->Read(strBody, BUFFER_SIZE)) {
				body += string(strBody);  // TODO: abort if we have too big web page 
				allRead += nRead;
			}

			// turn to lowercase
			transform(body.begin(), body.end(), body.begin(), ::tolower);
			transform(pattern.begin(), pattern.end(), pattern.begin(), ::tolower);

			// TODO: replace following URL extraction with REGEXP
			int index = 0;
			vector<Task> exctracted_urls;
			while ((index = body.find(pref, index)) != -1) {
				int tail = index + pref.size(); // tail is index after last index of url
				while (tail < body.size() && iswalnum(body[tail]) || isAcceptableURLChar(body[tail])) {
					tail++;
				}

				//addTask(Task(body.substr(index, tail - index)));
				exctracted_urls.push_back(Task(body.substr(index, tail - index)));
				index = tail;
			}

			// add many tasks with one lock
			addTasks(exctracted_urls);

			// Search
			if (body.find(pattern) != string::npos) {
				task.setStatus(Task::TASK_STATUS::SUCCESS_FOUND);
			}
			else {
				task.setStatus(Task::TASK_STATUS::SUCCESS);
			}


		} catch (CInternetException* pEx) {
			session.Close();
			task.setStatus(Task::TASK_STATUS::REQUEST_FAIL);
			return 1;
		}

		session.Close();
		return 0;

	}
	
	void Crawler::workerThread() {
		while (true) {

			// If we visited all pages, notify all threds that they should exit
			if (getVisitedCount() == maxPages) {
				need_to_exit = true;
				data_rdy.notify_all();
			}

			unique_lock<mutex> lk(queue_mutex); // can be unlocked later

			// wait until queue has something or we have to exit from thread
			data_rdy.wait(lk, [&] {
				return need_to_exit || !tasks.empty();
			});

			if (need_to_exit) {
				return;
			}

			// Get task from queue
			Task task = tasks.front();
			tasks.pop();
			lk.unlock();

			string url = task.getURL();

			// Istead of doing two functions with 2 locks, do one
			// if (!isVisited(url))
			//	 setVisited(url);
			// else
			//    continue;
			if (checkAndSetVisited(url))
				continue;

			task.setStatus(Task::TASK_STATUS::PROCESSING);
			process(task);

			listener(task); // notify that task is finished
		}
	}
	
	Crawler::Crawler() {
	}
	
	Crawler::~Crawler() {
		stop();
	}
	
	// Stops crawling threads and join them
	void Crawler::stop() {
		need_to_exit = true;
		data_rdy.notify_all();

		for (int i = 0; i < threads.size(); ++i)
			threads[i].join();

		threads.clear();
	}
	
	void Crawler::setOnTaskFinishedListener(OnTaskFinishedListener _listener) {
		listener = _listener;
	}
	
	// Add one task to queue
	void Crawler::addTask(const Task & task) {
		if (isVisited(task.getURL()))
			return;

		lock_guard<mutex> lk(queue_mutex); // will lock queue until end of scope
		tasks.push(task);
		data_rdy.notify_all(); // notify
	}
	
	// Add many tasks to queue
	void Crawler::addTasks(const vector<Task>& task_vector) {
		lock_guard<mutex> lk(queue_mutex); // will lock queue until end of scope

		bool added = false;
		for (Task task : task_vector) {
			if (!isVisited(task.getURL())) {
				added = true;
				tasks.push(task);
			}
		}

		if (added)
			data_rdy.notify_all(); // notify
	}
	
	// Start crawler with params
	void Crawler::start(string url, string _pattern, int max_pages, int _threads_number) {
		stop(); // stop if it was started

		threads_number = _threads_number;
		maxPages = max_pages;
		need_to_exit = false;
		pattern = _pattern;

		// clear all data structures
		visited.clear();
		while (!tasks.empty())
			tasks.pop();

		// initial task
		const Task task = Task(url);
		addTask(task);

		threads.clear();
		// spawn 10 threads:
		for (int i = 0; i < threads_number; ++i)
			threads.push_back(thread(&Crawler::workerThread, this));

	}
}
