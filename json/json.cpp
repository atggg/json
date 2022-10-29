#include "json.h"

using namespace at;

json::json()
{
	_node_type = node_type::json_null;
	_data._uint64 = 0;
	_use = new unsigned int;
	*_use = 1;
}

at::json::~json()
{
	clear();
}

at::json::json(const json& j)
{
	_node_type = j._node_type;
	_data = j._data;
	_use = new unsigned int;
	*_use = 1;
	if (_node_type == node_type::json_array)
	{
		_data._array = new std::vector<json*>;
		for (auto it : *j._data._array)
		{
			_data._array->push_back(new json(*it));
		}
	}
	if (_node_type == node_type::json_string)
	{
		_data._string = new std::string(*j._data._string);
	}
	if (_node_type == node_type::json_object)
	{
		_data._object = new std::map<std::string, json*>;
		for (auto it = j._data._object->begin();it != j._data._object->end();it++)
		{
			_data._object->insert(std::make_pair(it->first,new json(*(it->second))));
		}
	}
	
}

at::json::json(const std::string& v)
{
	_node_type = node_type::json_string;
	_data._string = new std::string(v);
	_use = new unsigned int;
	*_use = 1;
}

at::json::json(const char* v)
{
	_node_type = node_type::json_string;
	_data._string = new std::string(v);
	_use = new unsigned int;
	*_use = 1;
}

at::json::json(bool v)
{
	_node_type = node_type::json_bool;
	_data._bool = v;
	_use = new unsigned int;
	*_use = 1;
}

at::json::json(double v)
{
	_node_type = node_type::json_double;
	_data._double = v;
	_use = new unsigned int;
	*_use = 1;
}

at::json::json(int v)
{
	_node_type = node_type::json_int;
	_data._uint64 = v;
	_use = new unsigned int;
	*_use = 1;
}

at::json::json(long long v)
{
	_node_type = node_type::json_int;
	_data._uint64 = v;
	_use = new unsigned int;
	*_use = 1;
}

at::json::operator int()
{
	if (_node_type != node_type::json_int)
	{
		//Å×Òì³£
	}
	return static_cast<int>(_data._uint64);
}

at::json::operator long long()
{
	if (_node_type != node_type::json_int)
	{
		//Å×Òì³£
	}
	return static_cast<long long>(_data._uint64);
}

at::json::operator bool()
{
	if (_node_type != node_type::json_bool)
	{
		//Å×Òì³£
	}
	return _data._bool;
}

at::json::operator double()
{
	if (_node_type != node_type::json_double)
	{
		//Å×Òì³£
	}
	return _data._double;
}

at::json::operator std::string()
{
	if (_node_type != node_type::json_string)
	{
		//Å×Òì³£
	}
	return *_data._string;
}

json& at::json::operator=(const json& j)
{
	clear();
	_node_type = j._node_type;
	_data = j._data;
	_use = j._use;
	(*_use)++;
	return *this;
}

json& at::json::operator[](std::string key)
{
	if (_node_type != node_type::json_object)
	{
		clear();
		_data._object = new std::map<std::string, json*>;
		_node_type = node_type::json_object;
		_use = new unsigned int;
		*_use = 1;
	}
	json* node = nullptr;
	auto it = _data._object->find(key);
	if (it != _data._object->end())
	{
		node = it->second;
	}
	else
	{
		node = new json;
		_data._object->insert(std::make_pair(key, node));
	}
	
	return *node;
}

json& at::json::operator[](const char* key)
{
	return (*this)[std::string(key)];
}

json& at::json::operator[](int index)
{
	if (index < 0)
	{
		//Å×Òì³£
	}
	if (_node_type != node_type::json_array)
	{
		clear();
		_data._array = new std::vector<json*>;
		_node_type = node_type::json_array;
		_use = new unsigned int;
		*_use = 1;
	}
	json* node = nullptr;
	while (_data._array->size() <= index)
	{
		node = new json;
		_data._array->push_back(node);
	}
	node = (*_data._array)[index];
	return *node;
}

void at::json::clear()
{
	if (*_use > 1)
	{
		(* _use)--;
		return;
	}
	if (_node_type == node_type::json_array && _data._array != nullptr)
	{
		for (auto it :* _data._array)
		{
			delete it;
		}
		_data._array->clear();
		delete _data._array;
		_data._array = nullptr;
	}
	if (_node_type == node_type::json_string && _data._string != nullptr)
	{
		delete _data._string;
		_data._string = nullptr;
	}
	if (_node_type == node_type::json_array && _data._object != nullptr)
	{
		for (auto it = _data._object->begin();it != _data._object->end();it++)
		{
			delete it->second;
		}
		_data._object->clear();
		delete _data._object;
		_data._object = nullptr;
	}
	if (_use != nullptr)
	{
		delete _use;
		_use = nullptr;
	}
}

void at::json::parse(const std::string& s)
{
	analysis p(s);
	*this = p.parse();
}

std::string at::json::dump()
{
	std::stringstream strs;
	switch (_node_type)
	{
		case json_null:
			strs << "null";
			break;
		case json_bool:
			if (_data._bool)
			{
				strs << "true";
			}
			else
			{
				strs << "false";
			}
			break;
		case json_int:
			strs << _data._uint64;
			break;
		case json_double:
			strs << _data._double;
			break;
		case json_string:
			strs << "\"" << *_data._string << "\"";
			break;
		case json_array:
		{
			strs << "[";
			for (auto it = _data._array->begin(); it != _data._array->end(); it++)
			{
				if (it != _data._array->begin())
				{
					strs << ",";
				}
				strs << (*it)->dump();
			}
			strs << "]";
			break;
		}
		case json_object:
		{
			strs << "{";
			for (auto it = _data._object->begin(); it != _data._object->end(); it++)
			{
				if (it != _data._object->begin())
				{
					strs << ",";
				}
				strs << "\"" << it->first << "\":" << it->second->dump();
			}
			strs << "}";
		}
	}
	return strs.str();
}

at::analysis::analysis(const std::string& str)
{
	_buf = str;
	_bufp = &_buf[0];
	_len = _buf.size();
	_pos = 0;
}

char at::analysis::get_next_token()
{
	while (_bufp[_pos] == '\n' || _bufp[_pos] == '\r' || _bufp[_pos] == '\t' || _bufp[_pos] == ' ')
	{
		if (_pos >= _len)
		{
			//Å×Òì³£
		}
		_pos++;
	}
	return _bufp[_pos++];
}

json at::analysis::parse()
{
	char ch = get_next_token();
	switch (ch)
	{
	case 'n':
		_pos--;
		return parse_null();
	case 't':
	case 'f':
		_pos--;
		return parse_bool();
	case '-':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
		_pos--;
		return parse_number();
	case '"':
		return json(parse_string());
	case '[':
		return parse_array();
	case '{':
		return parse_object();
	}
	return json();
}

json at::analysis::parse_null()
{
	if (_buf.compare(static_cast<size_t>(_pos), 4, "null") == 0)
	{
		_pos += 4;
		return json();
	}
	//Å×Òì³£
}

json at::analysis::parse_bool()
{
	if (_buf.compare(static_cast<size_t>(_pos), 4, "true") == 0)
	{
		_pos += 4;
		return json(true);
	}
	if (_buf.compare(static_cast<size_t>(_pos), 5, "false") == 0)
	{
		_pos += 5;
		return json(false);
	}
	//Å×Òì³£
}

json at::analysis::parse_number()
{
	unsigned long long pos = _pos;

	if (_bufp[_pos] == '-')
		_pos++;

	// integer part
	if (_bufp[_pos] == '0')
	{
		_pos++;
	}
	else if (in_range(_bufp[_pos], '1', '9'))
	{
		_pos++;
		while (in_range(_bufp[_pos], '0', '9'))
		{
			_pos++;
		}
	}
	else
	{
		throw std::logic_error("invalid character in number");
	}

	if (_bufp[_pos] != '.')
	{
		return json(std::atoi(_buf.c_str() + pos));
	}

	// decimal part
	_pos++;
	if (!in_range(_bufp[_pos], '0', '9'))
	{
		throw std::logic_error("at least one digit required in fractional part");
	}
	while (in_range(_bufp[_pos], '0', '9'))
	{
		_pos++;
	}
	return json(std::atof(_buf.c_str() + pos));
}

json at::analysis::parse_array()
{
	json arr;
	char ch = get_next_token();
	int i = 0;
	if (ch == ']')
	{
		return arr;
	}
	_pos--;
	while (true)
	{
		arr[i] = parse();
		i++;
		ch = get_next_token();
		if (ch == ']')
		{
			break;
		}
		if (ch != ',')
		{
			//Å×Òì³£
		}
	}
	return arr;
}

json at::analysis::parse_object()
{
	json object;
	char ch = get_next_token();
	if (ch == '}')
	{
		return object;
	}
	_pos--;
	while (true)
	{
		ch = get_next_token();
		if (ch != '"')
		{
			//Å×Òì³£
		}
		std::string key = parse_string();
		ch = get_next_token();
		if (ch != ':')
		{
			//Å×Òì³£
		}
		object[key] = parse();
		ch = get_next_token();
		if (ch == '}')
		{
			break;
		}
		if (ch != ',')
		{
			//Å×Òì³£
		}
	}
	return object;
}

std::string at::analysis::parse_string()
{
	unsigned long long pos = _pos;
	while (true)
	{
		if (_pos == _buf.size())
		{
			//Å×Òì³£
		}

		char ch = _bufp[_pos++];
		if (ch == '"')
		{
			break;
		}

		if (ch == '\\')
		{
			ch = _bufp[_pos++];
			switch (ch)
			{
			case 'b':
			case 't':
			case 'n':
			case 'f':
			case 'r':
			case '"':
			case '\\':
				break;
			case 'u':
				_pos += 4;
				break;
			default:
				break;
			}
		}
	}
	return _buf.substr(static_cast<size_t>(pos), static_cast<size_t>( _pos - pos - 1));
}
