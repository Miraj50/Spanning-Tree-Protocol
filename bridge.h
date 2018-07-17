#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <sstream>

using namespace std;

struct bridge{

	string rp,dist,self,parent,root_lan_seg; // No need of root_lan_seg actually
	map<string, string> bdg_map;

	void init(string root_port, string d, string self_n, string rootlanseg){
		rp = root_port;
		dist = d;
		self = self_n;
		root_lan_seg = rootlanseg;
	}

	void comp_and_updt(vector<string>&v, string lan);
};

string int_to_str(int num);
int str_to_int(string str);