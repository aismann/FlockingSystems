#pragma once
#include <string>
#include <vector>
#include <map>
#include <iostream>
#include <sstream>
#include <fstream>
#include <initializer_list>

// Shortcuts
//#define DEBUG

#define C_CONSOLE	ConsoleLogger::Console::instance()

#ifdef DEBUG
	#define C_INFO(...)		ConsoleLogger::Console::instance().log("INFO", __VA_ARGS__);
	#define C_WARNING(...)	ConsoleLogger::Console::instance().log("WARNING", __VA_ARGS__);
	#define C_ERROR(...)	ConsoleLogger::Console::instance().log("ERROR", __VA_ARGS__);
#else
	#define C_INFO(...)		
	#define C_WARNING(...)	
	#define C_ERROR(...)	
#endif

namespace ConsoleLogger {
	/************************************************************************/
	/** Config **************************************************************/
	/************************************************************************/
	struct Cfg { 
		std::string nm; 
		bool sh; 
		bool sv; 
		
		Cfg(std::string _nm, bool _sh, bool _sv) :nm(_nm), sh(_sh), sv(_sv){} 
	};

	const std::vector<Cfg> CONSOLE_CONFIG = {
		Cfg("INFO", true, false),
		Cfg("WARNING", true, false),
		Cfg("ERROR", true, false),
	};

	/************************************************************************/
	/** Log *****************************************************************/
	/************************************************************************/
	class Log {
	public:
		const int LOG_MAX = 200;

	public:
		// Contructor / Destructor
		Log(std::string _name, bool _show, bool _save) :
			name(_name), show(_show), save(_save), start(0), end(0)
		{
			list.resize(LOG_MAX);
		}

		~Log() {
			if (this->save && (this->start != this->end)) {
				save_file();
			}
		}

		void set_show(bool _show) {
			this->show = _show;
		}

		void set_save_file(bool _save) {
			this->save = _save;
		}

		// Save to file
		void save_file() {
			std::ofstream file;
			file.open(name + ".log", std::ios::trunc);

			int l_curr = start;
			int l_end = end;
			while (l_curr != l_end) {
				file << list[l_curr];
				l_curr = (++l_curr) % list.size();
			}

			file.close();
		}

	public:
		// Prepare string
		template <typename T>
		std::string prepare_string(T t) {
			std::ostringstream tmpStr;
			tmpStr << "[" << name << "]" << t << std::endl;

			return tmpStr.str();
		}

		// Push message to log
		template <typename... Args>
		void log(Args... args) {
			auto str = prepare_string(args...);

			if (this->show) {
				std::cout << str;
			}

			if (this->save) {
				list[end] = str;

				end = (++end) % this->list.size();
				start = (start < end ? start : ((++end) % list.size()));
			}
		}

		// Push msg to log
		//template <typename T>
		//Log& operator<< (const T& x) {
		//	if (this->show) {
		//		std::cout << x;
		//	}

		//	std::ostringstream tmpStr;
		//	tmpStr << x;

		//	if (this->save) {
		//		list[end] = tmpStr.str();

		//		// Delete old messages
		//		end = (++end) % this->list.size();
		//		start = (start < end ? start : ((++end) % list.size()));
		//	}

		//	return *this;
		//}

		//Log& operator<< (std::ostream& (*f)(std::ostream&)) {
		//	if (this->show) {
		//		f(std::cout);
		//	}

		//	std::ostringstream tmpStr;
		//	f(tmpStr);

		//	if (this->save) {
		//		list[end] = tmpStr.str();

		//		// Delete old messages
		//		end = (++end) % this->list.size();
		//		start = (start < end ? start : ((++end) % list.size()));
		//	}

		//	return *this;
		//}

		//Log& operator<< (std::ostream& (*f)(std::ios&)) {
		//	if (this->show) {
		//		f(std::cout);
		//	}

		//	std::ostringstream tmpStr;
		//	f(tmpStr);

		//	if (this->save) {
		//		list[end] = tmpStr.str();

		//		// Delete old messages
		//		end = (++end) % this->list.size();
		//		start = (start < end ? start : ((++end) % list.size()));
		//	}

		//	return *this;
		//}

		//Log& operator<< (std::ostream& (*f)(std::ios_base&)) {
		//	if (this->show) {
		//		f(std::cout);
		//	}

		//	std::ostringstream tmpStr;
		//	f(tmpStr);

		//	if (this->save) {
		//		list[end] = tmpStr.str();

		//		// Delete old messages
		//		end = (++end) % this->list.size();
		//		start = (start < end ? start : ((++end) % list.size()));
		//	}

		//	return *this;
		//}


	private:
		// Logger name
		std::string name;

		// Messages list
		std::vector<std::string>	list;
		int							start, end;

		// Others
		bool show;	// show in stdout
		bool save;	// save to file
	};

	/************************************************************************/
	/** Console *************************************************************/
	/************************************************************************/
	class Console
	{
	public:
		// Manage
		void setup(std::string _name, bool _show, bool _save) {
			if (this->logs.find(_name) != logs.end()) {
				return;
			}

			this->logs.insert(std::pair<std::string, Log>(_name, Log(_name, _show, _save)));
		}


		void set_show(std::string _name, bool _show) {
			auto it = this->logs.find(_name);
			if (it == this->logs.end()) {
				return;
			}

			it->second.set_show(_show);
		}

		void set_save_file(std::string _name, bool _save) {
			auto it = this->logs.find(_name);
			if (it == this->logs.end()) {
				return;
			}

			it->second.set_save_file(_save);
		}

		// Use logger
		template <typename... Args>
		void log(std::string _name, Args... args) {
			auto it = this->logs.find(_name);
			if (it == this->logs.end()) {
				throw std::exception("No logger with given name.");
			}

			it->second.log(args...);
		}

		// Singleton
		static Console& instance() {
			static Console console;
			return console;
		}

	private:
		// Constructor / Destructor
		Console() {
			for (auto& cfg : CONSOLE_CONFIG) {
				this->setup(cfg.nm, cfg.sh, cfg.sv);
			}
		}

		~Console() {}

	private:
		// List of loggers
		std::map<std::string, Log> logs;
	};

// namespace
}