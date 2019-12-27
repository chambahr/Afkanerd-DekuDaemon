#ifndef HELPERS_H_INCLUDED_
#define HELPERS_H_INCLUDED_
#include <algorithm>
#include <random>
#include <fstream>
#include <thread>
#include <map>
#include "logger.hpp"
using namespace std;

namespace helpers {
	string unescape_string( string input ) {
		for(size_t i=0;i<input.size();++i) {
			if( input[i] == '\n' ) {
				input.erase(i, 1);
				input.insert(i, "\\n");
			}
		}
		return input;
	}


	void make_dir( string path_dirname ) {
		string func_name = "make_dir";
		if( mkdir( path_dirname.c_str(), 0777 ) == -1) {
			if( errno != 17 ) logger::logger_errno( errno );
		}
		
		return;
	}
	void write_file( string path_filename, auto input, bool b_unescape_string, ios_base::openmode mode = ios::app ) { //TODO: what about auto
		if( b_unescape_string ) input = unescape_string( input );
		ofstream writefile( path_filename.c_str(), mode );
		writefile << input;
		writefile.close();
	}

	vector<string> read_file( string filename ) {
		// TODO: add variable to read into string not vector, change return type to auto when this happens
		ifstream readfile( filename.c_str() );
		vector<string> file_contents;
		if( !readfile.good() ) return file_contents;

		string tmp;
		while(getline(readfile, tmp)) file_contents.push_back( tmp );
		readfile.close();
		
		return file_contents;
	}

	bool file_exist( string path_filename ) {
		struct stat buffer; 
		return stat( path_filename.c_str(), &buffer) == 0;
	}

	bool delete_file( string path_filename) {}

	bool rename_file( string path_filename) {}
	
	bool unhide_file( string path_filename) {}

	bool hide_file( string path_filename) {}

	void sleep_thread( int duration ) {
		std::this_thread::sleep_for(std::chrono::seconds( duration ));
	}

	string terminal_stdout(string command) {
		string data;
		FILE * stream;
		const int max_buffer = 1024;
		char buffer[max_buffer];
		command.append(" 2>&1");

		stream = popen(command.c_str(), "r");
		if (stream) {
			while (!feof(stream)) if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
			pclose(stream);
		}
		return data;
	}

	vector<string> split(string _string, char del = ' ', bool strict = false) {
		vector<string> return_value;
		string temp_string = "";
		for(auto _char : _string) {
			if(_char==del) {
				if(strict and temp_string.empty()) continue;
				return_value.push_back(temp_string);
				temp_string="";
			}
			else {
				temp_string+=_char;
			}
		}
		if(strict and !temp_string.empty()) return_value.push_back(temp_string);

		return return_value;
	}


	string ISPFinder(string number) {
		if(number[0] == '6') {
			switch(number[1]) {
				case '5':
					switch(number[2]) {
						case '0':
						case '1':
						case '2':
						case '3':
						case '4':
							return "MTN";
						break;

						case '5':
						case '6':
						case '7':
						case '8':
						case '9':
							return "ORANGE";
						break;

					}
				break;

				case '6': return "NEXTTEL";

				case '7': return "MTN";
				break;

				case '8':
					  switch(number[2]) {
						case '0':
						case '1':
						case '2':
						case '3':
						case '4':
							return "MTN";
						break;
					  }
				break;

				case '9': return "ORANGE";
				break;
			}
		}
		return "";
							
	}

	string random_string(){
	     string str("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz");

	     random_device rd;
	     mt19937 generator(rd());

	     shuffle(str.begin(), str.end(), generator);

	     return str.substr(0, 32);    // assumes 32 < number of characters in str         
	}

	string to_upper(string input) {
		string str = input;
		transform(str.begin(), str.end(),str.begin(), ::toupper);
		return str;
	}

	string remove_char( string input, char value = '\n', char location = 'B' ) {
		//TODO: Make location determine where it takes the input file from
		size_t check_pos = 0;
		if( location == 'E' ) check_pos = input.size() -1;
		size_t space_location = input.find( value );
		while( space_location != string::npos and space_location == check_pos) {
			input.erase( check_pos, 1);
			space_location = input.find( value );
		}

		return input;
	}

	void curl_server( string TCP_HOST, string TCP_PORT, string URL, string message) {
		string func_name = "curl_server";
		string command = "curl -X POST -H \"Content-Type: text/plain\" " + TCP_HOST + ":" + TCP_PORT + "/" + URL + " -d \"" + message + "\"";
		cout << func_name << "=> command [" << command << "]" << endl;
		string terminal_output = helpers::terminal_stdout( command );
	}

	auto one_line_parser( string string_to_parse ) {
		string tmp_string_buffer = "";
		string tmp_key = "";
		map<string, string> request_tuple;
		bool ignore = false;
		bool safe = false;
		for(auto i : string_to_parse) {
			if(i == 'n' and safe and ignore) {
				tmp_string_buffer += '\n';
				safe = false;
				continue;
			}
			if(i == '=' and !ignore) {
				tmp_key = tmp_string_buffer;
				tmp_string_buffer = "";
				continue;
			}
			if(i == ',' and !ignore) {
				request_tuple.insert(make_pair(tmp_key, tmp_string_buffer));
				tmp_key = "";
				tmp_string_buffer = "";
				continue;
			}
			if(i == '"') {
				ignore = !ignore;
				continue;
			}
			if(i == '\\' and ignore) {
				safe = true;
				continue;
			}
			tmp_string_buffer += i;
		}

		if(!tmp_key.empty() and !tmp_string_buffer.empty()) {
			request_tuple.insert(make_pair(tmp_key, tmp_string_buffer));
		}

		return request_tuple;
	}
}

#endif
