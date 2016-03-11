#pragma once

#include<vector>
#include<iostream>
#include<queue>
#include<unordered_set>
#include<thread>
#include<atomic>
#include<mutex>

#include<afxinet.h>
#include "Task.h"

using namespace std;

namespace crawler {

	using OnTaskFinishedListener = function<void(const Task&)>;

	class Crawler {
	public:
		Crawler();
		~Crawler();
		// Stops crawling threads and join them
		void stop();

		void setOnTaskFinishedListener(OnTaskFinishedListener _listener);
		// Add one task to queue
		void addTask(const Task& task);
		// Add many tasks to queue
		void addTasks(const vector<Task>& task_vector);
		// Start crawler with params
		void start(string url, string _pattern, int max_pages = 50, int _threads_number = 5);
	
	protected:
		vector<thread> threads;
		int maxPages;
		int threads_number;
		OnTaskFinishedListener listener;
		
		static const int BUFFER_SIZE = 1024;      // buffer for webpage download
		static const int REQUEST_TIMEOUT = 1000;  // timeout for webpage download

		mutex queue_mutex;
		mutex set_mutex;
		queue<Task> tasks; // queue of tasks (URLs)
		condition_variable data_rdy;   // conditional var. that tells when threads need to sleep and wake
		unordered_set<string> visited; // hash table of visited pages
		atomic<bool> need_to_exit;     // bool used to stop and exit all threads

		char strBody[BUFFER_SIZE];  // buffer for webpage dowload
		const string chars = "@:%._\\+~#=?&//"; // chars allowed in URL
		const string pref = "http://"; // prefix of URL
		string pattern; // word we search for

		// Check if char allowed in URL
		bool isAcceptableURLChar(char c);

		// Check if url is visited and return bool.
		// If not visited, then mark it as visited.
		bool checkAndSetVisited(string url);

		bool isVisited(string url);

		int getVisitedCount();

		int process(Task &task);

		// Function for worker thread
		void workerThread();

	
	};
}