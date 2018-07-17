#include "bridge.h"
#include <algorithm>

using namespace std;

struct compare{ // for the forwarding table values to be sorted
	bool operator()(string a, string b) const{
		int x = str_to_int(a.substr(1));
		int y = str_to_int(b.substr(1));
		return x < y;
	}
};

int main(){

	int trace_flag, n;
	cin>>trace_flag>>n;

	bridge bdg[n];

	// Make LAN mapping
	map<string, vector<int> > lan_map; // A-> 0,2 i.e. B1 and B3

	int time = 0;

	for(int i=0;i<n;i++){
		string s1 = "B" + int_to_str(i+1);
		bdg[i].init(s1,"0",s1,"");

		if(trace_flag == 1){
			cout<<time<<" "<<s1<<" s ("<<s1<<", "<<"0"<<", "<<s1<<")\n";
		}

	}

	string s1;
	getline(cin,s1);
	for(int i=0;i<n;i++){ // making lan map
		string temp="";
		int bdgnumber;
		getline(cin,s1);
		int flag = 0, l=s1.length();

		for(int j=0;j<l;j++){
			if(s1[j]==' ' && flag==0){
				bdgnumber = str_to_int(temp.substr(1))-1; // 0 indexed
				flag = 1;
				temp = "";
			}
			else if(s1[j]==' '){
				bdg[bdgnumber].bdg_map[temp] = "DP";
				lan_map[temp].push_back(bdgnumber);
				temp= "";
			}
			else if(j==(l-1)){
				temp = temp + s1[j];
				bdg[bdgnumber].bdg_map[temp] = "DP";
				lan_map[temp].push_back(bdgnumber);
			}
			else
				temp = temp+s1[j];
		}
	}

	time++;
	while(true){
		// Lan as vector of messages (each message is a 3x1 vector of strings)
		map<string, vector<vector<string> > >lan_vec;
		// populate lan_vectors

		map<string,vector<int> >::iterator it; //iterate on lan_map

		for(it=lan_map.begin();it!=lan_map.end();it++){
			string lanname = it->first;
			int l1 = it->second.size();
			// cout<<"\n"<<lanname<<" "<<l1<<"\n";
			for(int i=0;i<l1;i++){
				int reqbdg = it->second[i];
				if(bdg[reqbdg].bdg_map[lanname] == "DP"){ // forward messages only on Designated Ports
					// make config message
					string x = bdg[reqbdg].rp, d = bdg[reqbdg].dist, y = bdg[reqbdg].self;
					vector<string> v1;//cout<<x<<" "<<d<<" "<<y;
					v1.push_back(x);v1.push_back(d);v1.push_back(y);
					lan_vec[lanname].push_back(v1);

					// Tracing when trace_flag=1
					// if(trace_flag == 1){
					// 	vector<int>trv = it->second;
					// 	for(int j=0;j<l1;j++){
					// 		if(trv[j]==reqbdg){
					// 			cout<<time<<" B"<<trv[j]+1<<" s ("<<x<<", "<<d<<", "<<y<<")\n";
					// 		}
					// 		else if(bdg[trv[j]].bdg_map[lanname] != "NP"){
					// 			cout<<time<<" B"<<trv[j]+1<<" r ("<<x<<", "<<d<<", "<<y<<")\n";
					// 		}
					// 	}
					// }
					////////////////////////////
				}
			}
		}
		///////////////////Printing messages(lan vectors) (Debugging)
		// map<string, vector<vector<string> > >::iterator it5;
		// for(it5=lan_vec.begin();it5!=lan_vec.end();it5++){
		// 	cout<<it5->first<<" ";
		// 	vector<vector<string> >vct = it5->second;
		// 	for(int i=0;i<vct.size();i++){
		// 		for(int j=0;j<vct[i].size();j++){
		// 			cout<<vct[i][j]<<",";
		// 		}
		// 		cout<<" ";
		// 	}
		// 	cout<<"\n";
		// }
		// cout<<"Baba\n";
		////////////////////////////////

		int ctr=0;
		map<string, vector<vector<string> > >::iterator lit;
		for(lit=lan_vec.begin();lit!=lan_vec.end();lit++){
			if(lit->second.size()==1)
				ctr++;
		}
		int no_of_lan = lan_map.size();

		if(ctr == no_of_lan){ // Ending condition
			break;
		}

		//iterate on bridges(may start from B2 also) and update
		for(int i=0;i<n;i++){
			map<string,string>::iterator it;
			for(it = bdg[i].bdg_map.begin();it!=bdg[i].bdg_map.end();it++){
				string lanp = it->first;
				int l2 = lan_vec[lanp].size();
				/////////////////////////////////////////// Tracing
				if(trace_flag == 1){
					if(it->second == "DP"){ // One message sent
						cout<<time<<" B"<<i+1<<" s ("<<bdg[i].rp<<", "<<bdg[i].dist<<", "<<bdg[i].self<<")\n";
					}
					
					for(int j=0;j<l2;j++){
						string u = lan_vec[lanp][j][0], v = lan_vec[lanp][j][1], w = lan_vec[lanp][j][2];
						
						if(w.substr(1) != int_to_str(i+1))
							cout<<time<<" B"<<i+1<<" r ("<<u<<", "<<v<<", "<<w<<")\n";
					}					
				}
				/////////////////////////////////////////
				for(int j=0;j<l2;j++){

					bdg[i].comp_and_updt(lan_vec[lanp][j],lanp);
				}
			}
		}
		time++;
		///////////////////
		// for(int i=0;i<n;i++){
		// 	cout<<bdg[i].self<<" dist="<<bdg[i].dist<<" rp="<<bdg[i].rp;
		// 	cout<<"\n";

		// 	map<string,string>::iterator it;

		// 	for(it=bdg[i].bdg_map.begin();it!=bdg[i].bdg_map.end();it++){
		// 		cout<<it->first<<":"<<it->second<<"\n";
		// 	}
		// 	cout<<"\n";
		// }
		///////////////////
	}

	// bridge details
	for(int i=0;i<n;i++){
		// cout<<bdg[i].self<<" dist="<<bdg[i].dist<<" rp="<<bdg[i].rp;
		cout<<bdg[i].self<<":";

		map<string,string>::iterator it;

		for(it=bdg[i].bdg_map.begin();it!=bdg[i].bdg_map.end();it++){
			cout<<" "<<it->first<<"-"<<it->second;
		}
		cout<<"\n";
	}
	// lan details
	// map<string,vector<int> >::iterator it;

	// for(it=lan_map.begin();it!=lan_map.end();it++){
	// 	cout<<it->first<<":";
	// 	for(int i=0;i<it->second.size();i++)
	// 		cout<<it->second[i]<<" ";
	// 	cout<<"\n";
	// }
	// cout<<"\n";
	/////////////////////////////

	map<string,string> host_lanmap; // H1:A, H2:C, ...

	while(getline(cin,s1)){
		if(s1.length()==1)
			break;
		string lan_l = s1.substr(0,1),temp;
		int l = s1.length(), flag=0;
		for(int j=0;j<l;j++){
			if(s1[j]==' ' && flag==0){
				flag = 1;
				temp = "";
			}
			else if(s1[j]==' ' && flag == 1){
				host_lanmap[temp] = lan_l;
				temp = "";
			}
			else if(j==(l-1)){
				temp = temp + s1[j];
				host_lanmap[temp] = lan_l;
			}
			else
				temp = temp + s1[j];
		}
	}
	///////////////////////////
	// map<string,string>::iterator it;

	// for(it=host_lanmap.begin();it!=host_lanmap.end();it++){
	// 	cout<<"\n"<<it->first<<"-"<<it->second;
	// }
	///////////////////////////

	int no_trans = str_to_int(s1); // no. of transmissions

	// Forwarding Table
	map<int, map<string, string, compare> >fwd_tbl;
	// cout<<fwd_tbl.count(1);

	for(int i=0;i<no_trans;i++){
		getline(cin,s1);
		int pos = s1.find(" ");
		string sender = s1.substr(0,pos);
		string receiver = s1.substr(pos+1);
		string senderlan = host_lanmap[sender];
		string receiverlan = host_lanmap[receiver];
		// cout<<"\n"<<sender<<" "<<receiver<<"\n";
		map<int,string>lan_to_bdg;
		map<string, int>bdg_to_lan;

		vector<int>v = lan_map[senderlan];
		int l3 = v.size();
		for(int i=0;i<l3;i++){
			if(bdg[v[i]].bdg_map[senderlan] != "NP"){
				lan_to_bdg[v[i]] = senderlan;
				fwd_tbl[v[i]].insert(make_pair(sender,senderlan));
			}

		}
		
		int check=0; // 0:lan to bdg, 1:bdg to lan

		while(true){
			if(check==0){
				// cout<<check<<"\t";
				map<int, string>::iterator it;

				for(it=lan_to_bdg.begin();it!=lan_to_bdg.end();it++){

					// cout<<check<<"\n"<<it->first<<" "<<it->second<<"\n";

					if(fwd_tbl.count(it->first)==1){
						if(fwd_tbl[it->first].count(receiver)==1){
							string t2 = fwd_tbl[it->first][receiver];
							if(it->second != t2){ // Don't send on the same port from where it came from
								bdg_to_lan[t2] = it->first;
								if(trace_flag == 1)
									cout<<time<<" B"<<it->first+1<<" s "<<senderlan<<"-->"<<receiverlan<<"\n";

								continue;
							}
							else
								continue;
						}
					}

					map<string,string>::iterator mit;

					for(mit=bdg[it->first].bdg_map.begin();mit!=bdg[it->first].bdg_map.end();mit++){
						if(mit->first != it->second && mit->second != "NP"){
							bdg_to_lan[mit->first] = it->first;
							if(trace_flag == 1)
								cout<<time<<" B"<<it->first+1<<" s "<<senderlan<<"-->"<<receiverlan<<"\n";
						}
					}
				}
				lan_to_bdg.clear();
				check = 1;
			}
			else if(check==1){
				map<string, int>::iterator it;
				for(it=bdg_to_lan.begin();it!=bdg_to_lan.end();it++){

					// cout<<check<<"\n"<<it->first<<" "<<it->second<<"\n";

					vector<int>v = lan_map[it->first];
					int l4 = v.size();
					for(int i=0;i<l4;i++){
						if(v[i] != it->second && bdg[v[i]].bdg_map[it->first]!="NP"){
							lan_to_bdg[v[i]] = it->first;

							if(trace_flag == 1)
								cout<<time<<" B"<<v[i]+1<<" r "<<senderlan<<"-->"<<receiverlan<<"\n";

							fwd_tbl[v[i]].insert(make_pair(sender,it->first));
						}
					}
				}
				bdg_to_lan.clear();
				check = 0;

				if(lan_to_bdg.size()==0) //No forward destination available
					break;
			}
			time++;
		}
		// Print Forwarding Table
		map<int, map<string, string, compare> >::iterator fit;
		for(fit=fwd_tbl.begin();fit!=fwd_tbl.end();fit++){
			cout<<"B"<<fit->first+1<<":\nHOST ID | FORWARDING PORT\n";

			map<string, string, compare>::iterator nit;
			for(nit = fit->second.begin();nit!=fit->second.end();nit++){
				cout<<nit->first<<" | "<<nit->second<<"\n";
			}
		}
		cout<<"\n";
	}
	return 0;
}