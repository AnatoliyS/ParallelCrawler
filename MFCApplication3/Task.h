#pragma once

#include<vector>
#include<iostream>
#include<string>

using namespace std;

namespace crawler {
	

	class Task {
	public:
		enum TASK_STATUS { SCHEDULED, PROCESSING, WRONG_URL, NOTFOUND, REQUEST_FAIL, SUCCESS_FOUND, SUCCESS };
		
		Task(string _url) : url(_url) {
			status = SCHEDULED;
		}

		Task(const Task& other) {
			url = other.url;
			status = other.status;
		}

		string getURL() const {
			return url;
		}

		void setStatus(TASK_STATUS s) {
			status = s;
		}

		string getStatus() const {
			switch (status) {
				case SCHEDULED:
					return "Scheduled.";
				case PROCESSING:
					return "In process.";
				case NOTFOUND:
					return "Page was not found.";
				case SUCCESS_FOUND:
					return "Processed, found match";
				case SUCCESS:
					return "Processed.";
				case WRONG_URL:
					return "Wrong url.";
				case REQUEST_FAIL: // timeout and etc
					return "Failed to process request.";
			}
		}
		
	private:
		string url;
		TASK_STATUS status;
	};
}