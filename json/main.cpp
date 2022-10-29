#include"json.h"
#include<iostream>

using json = at::json;
int main()
{
	json j;
	j["name"] = "JSON中国";
	j["url"] = "http://www.json.org.cn";
	j["page"] = 88;
	j["isNonProfit"] = true;
	j["address"]["street"] = "浙大路38号.";
	j["address"]["city"] = "浙江杭州";
	j["address"]["country"] = "中国";
	j["links"][0]["name"] = "Google";
	j["links"][0]["url"] = "http://www.google.com";
	j["links"][1]["name"] = "Baidu";
	j["links"][1]["url"] = "http://www.baidu.com";
	j["links"][2]["name"] = "SoSo";
	j["links"][2]["url"] = "http://www.SoSo.com";
	std::string s =  j.dump();
	std::cout << s << std::endl;
	std::cout << std::endl;

	int i = j["page"];

	json j2;
	j2.parse(s);

	std::string s2 = j2.dump();
	std::cout << s2 << std::endl;
}