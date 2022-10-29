#pragma once
#include<string>
#include<sstream>
#include<vector>
#include<map>
#include<memory>
namespace at
{
	
	class json
	{
	public:
		json();
		~json();
		json(const json& j);
		json(const std::string& v);
		json(const char* v);
		json(bool v);
		json(double v);
		json(int v);
		json(long long v);

		operator int();
		operator long long();
		operator bool();
		operator double();
		operator std::string();

		json& operator=(const json& j);
		json& operator[](const std::string key);
		json& operator[](const char* key);
		json& operator[](int index);
		void clear();
		void parse(const std::string &s);
		std::string dump();
	private:
		enum node_type
		{
			json_null,
			json_object,
			json_array,
			json_string,
			json_bool,
			json_double,
			json_int,
		}_node_type;
		union
		{
			std::map<std::string, json*>* _object;
			std::vector<json*>* _array;
			std::string* _string;
			bool _bool;
			double _double;
			unsigned long long _uint64;
		}_data;
		unsigned int *_use;
	};
	class analysis
	{
	public:
		analysis(const std::string &str);
		char get_next_token();
		json parse();
		json parse_null();
		json parse_bool();
		json parse_number();
		json parse_array();
		json parse_object();
		std::string parse_string();
		inline bool in_range(int x, int lower, int upper)
		{
			return (x >= lower && x <= upper);
		}
	private:
		std::string _buf;
		const char* _bufp;
		unsigned long long _len;
		unsigned long long _pos;
	};
}

