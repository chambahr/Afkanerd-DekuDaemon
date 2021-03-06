#include "modem_listener.cxx"

using namespace std;

int main() {
	std::mutex blocking_mutex;
	vector<string> list_of_test_modems_indexes {"4"};
	vector<string> list_of_test_modems_isp {"MTN"};
	vector<string> list_of_test_modems_imei {"861050001177396"};
	map<string,string> configs;

	Modems modems( Modems::TEST );

	modems.__INIT__( configs );
	// logger::logger(__FUNCTION__, "Checking against size.." );
	if(modems.getAllIndexes().size() == list_of_test_modems_indexes.size()) {
	}
	else {
		logger::logger(__FUNCTION__, "Not all modems accounted", "stderr" );
	}

	if(modems.getAllIndexes() == list_of_test_modems_indexes) {
	
	}
	else {
		logger::logger(__FUNCTION__, "Modems indexes do not match", "stderr");
		for(auto indexes : modems.getAllIndexes() ) 
			logger::logger(__FUNCTION__, "index: " + indexes + " - " + to_string(indexes.size()), "stderr" );
		//logger::logger(__FUNCTION__, "size = " + to_string(modems.getAllIndexes().size()), "stderr");
	}

	if(modems.getAllISP() == list_of_test_modems_isp) {}
	else {
		logger::logger(__FUNCTION__, "Modems ISP do not match", "stderr");
	}

	if(modems.getAllIMEI() == list_of_test_modems_imei) {}
	else {
		logger::logger(__FUNCTION__, "Modems IMEI do not match", "stderr");
		for(auto imei : modems.getAllIMEI()) {
			logger::logger(__FUNCTION__, "imei: " + imei, "stderr");
		}
	}

	if(modems.getAllModems().size() == list_of_test_modems_indexes.size()) {}
	else {
		logger::logger(__FUNCTION__, "Modems do not match", "stderr");
		logger::logger(__FUNCTION__, "# of modems: " + to_string(modems.getAllModems().size()), "stderr");
	}

	
	vector<std::thread> threads;

	vector<Modem> all_modems = modems.getAllModems();

	for(auto& modem : all_modems) {
		if(std::find( list_of_test_modems_indexes.begin(), list_of_test_modems_indexes.end(), modem.getIndex()) != list_of_test_modems_indexes.end()) {}
		else {
			logger::logger(__FUNCTION__, "Modem Index not found in list..", "stderr");
			logger::logger(__FUNCTION__, modem.getIndex(), "stderr");
		}

		if(std::find(list_of_test_modems_isp.begin(), list_of_test_modems_isp.end(), modem.getISP()) != list_of_test_modems_isp.end()){}
		else {
			logger::logger(__FUNCTION__, "Modem ISP not found in list...", "stderr");
			logger::logger(__FUNCTION__, modem.getISP(), "stderr");
		}

		if(std::find(list_of_test_modems_imei.begin(), list_of_test_modems_imei.end(), modem.getIMEI()) != list_of_test_modems_imei.end()){}
		else {
			logger::logger(__FUNCTION__, "Modem IMEI not found in list...", "stderr");
			logger::logger(__FUNCTION__, modem.getIMEI(), "stderr");
		}

		/*
		if(std::find(list_of_test_modems_imei.begin(), list_of_test_modems_imei.end(), modem.start()) != list_of_test_modems_imei.end() ){}
		else {
			logger::logger(__FUNCTION__, "Modem failed to start...", "stderr");
			logger::logger(__FUNCTION__, modem.getErrorLogs(), "stderr");
		}
		*/

		threads.push_back( std::thread(&Modem::start, std::ref(modem)));
		//tr_modem_start.detach();
		helpers::sleep_thread(2);
		cout << boolalpha << modem.getKeepAlive() << endl;
		cout << &modem << endl;

		cout << boolalpha << modem.getKeepAlive() << endl;
		if(modem.getKeepAlive()){}
		else {
			logger::logger(__FUNCTION__, "Failed to start modem thread", "stderr");
			logger::logger(__FUNCTION__, modem.getErrorLogs(), "stderr");
			cout << __FUNCTION__ << ": " << &modem << endl;
		}
	}

	for(int i=0;i<threads.size();++i) {
		threads[i].join();
	}

	for(auto& modem : all_modems ) {
		modem.end();
		while( modem.getThreadSafety()) {
			logger::logger(__FUNCTION__, "Thread safety on");
			helpers::sleep_thread(2);
		}
		if( !modem.getKeepAlive()) {
			logger::logger(__FUNCTION__, "modem set for termination...");
		}
		else {
			logger::logger(__FUNCTION__, "Modem failed to end...", "stderr");
			logger::logger(__FUNCTION__, modem.getErrorLogs(), "stderr");
		}
	}


	logger::logger(__FUNCTION__, "finishing the program...");
	return 0;
}
