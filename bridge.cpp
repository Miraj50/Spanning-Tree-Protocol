#include "bridge.h"

using namespace std;

string int_to_str(int num){
    stringstream ss;
    ss << num;
    return ss.str();
}

int str_to_int(string str){
    stringstream ss(str);
    int num;
    ss >> num;
    return num;
}

void bridge::comp_and_updt(vector<string>&v, string lan){
	string lan_rp = v[0];
	string dis = v[1];
	string itself = v[2];

	string bridge_rp = this->rp;
	string di = this->dist;
	string bridge_itself = this->self;

	if(bridge_itself != itself){
		int a = str_to_int(lan_rp.substr(1));
		int b = str_to_int(bridge_rp.substr(1));
		if(a<b){
			this->rp = lan_rp;
			this->parent = itself;
			// this->root_lan_seg = lan;
			this->dist = int_to_str(str_to_int(dis)+1);
			map<string,string>::iterator it;
			for(it=this->bdg_map.begin();it!=this->bdg_map.end();it++){
				if(it->first == lan)
					it->second = "RP";
				else
					it->second = "DP";
			}		
		}
		else if(a==b && dis!=di){ //some port may be deactivated now
			if(str_to_int(dis) == str_to_int(di)-1){
				
				int x = str_to_int(this->parent.substr(1));
				int y = str_to_int(itself.substr(1));
				if(x > y){ // parent is bad, change root
					this->parent = itself;

					map<string,string>::iterator it;
					for(it=this->bdg_map.begin();it!=this->bdg_map.end();it++){
						if(it->first == lan)
							it->second = "RP";
						else
							it->second = "DP";
					}
					
				}
				else if(x==y && bdg_map[lan]!="RP"){
					this->bdg_map[lan]="NP";
				}
				else if(x<y && bdg_map[lan]!="RP"){ // deactivate
					this->bdg_map[lan]="NP";
				}
			}
			else if(str_to_int(dis) < str_to_int(di)-1){
				// make root
				this->parent = itself;
				map<string,string>::iterator it;
				for(it=this->bdg_map.begin();it!=this->bdg_map.end();it++){
					if(it->first == lan)
						it->second = "RP";
					else
						it->second = "DP";
				}
			}
		}
		else if(a==b && dis==di && itself != bridge_itself){
			int a = str_to_int(itself.substr(1));
			int b = str_to_int(bridge_itself.substr(1));
			if(b>a && bdg_map[lan]!="RP")
				this->bdg_map[lan]="NP";
		}
	}
}