#include <map>
#include <algorithm>
#include <unordered_set>
#include <set>
#include <vector>
#include <iostream>

using namespace std;

namespace L2{

	vector<string> SortedGPRegisters{"r10","r11","r8","r9","rax","rcx","rdi","rdx","rsi","r12","r13","r14","r15","rbp","rbx"};


	void printgraph(map<string, unordered_set<string>> &graph){
		map<string, unordered_set<string>>::iterator itr;
		unordered_set<string>::iterator itr2;
		for(itr = graph.begin();itr!=graph.end();++itr){
			cout<<itr->first<<" ";
			for(itr2 = (itr->second).begin();itr2 != (itr->second).end(); ++itr2){
				cout<<*itr2<<" ";
			}
			cout<<endl;
		}

	}

	pair<vector<string>,map<string,int>> graphcolor(map<string,unordered_set<string>> graph){
		map<string,int> colorMap;
		map<string,int>::iterator colorMapItr;
		vector<string> stack;
		vector<string> spillVar;
  		string node;
		set<int> temp;
		set<int>::iterator setitr;
		unordered_set<string> tempset;
		unordered_set<string>::iterator unsetItr;
		map<string,unordered_set<string>>::iterator graphitr;
  		vector<pair<string,unordered_set<string>>> numOfNeighbours;
		int i,j,k,l;



		for(i = 0; i < SortedGPRegisters.size(); i++){
			colorMap.insert(pair<string,int>(SortedGPRegisters[i],i));
		}


  		auto compFunction = [](pair<string,unordered_set<string>> &i, pair<string,unordered_set<string>> &j){	
			bool val = (i.second.size() > j.second.size());
			return val;
  		};

  		for(graphitr = graph.begin();graphitr != graph.end(); ++graphitr){
  			numOfNeighbours.push_back(*graphitr);
  		}


  		sort(numOfNeighbours.begin(),numOfNeighbours.end(),compFunction);

 
  		i = 0;
  		while (numOfNeighbours.size() > 0 && i < numOfNeighbours.size()){
  			if(numOfNeighbours[i].second.size() >= 15){
  				i++;
  				continue;
  			}
  			stack.push_back(numOfNeighbours[i].first);
  			for(j = 0;j<numOfNeighbours.size();j++){
  				numOfNeighbours[j].second.erase(numOfNeighbours[i].first);
  			}
  			numOfNeighbours.erase(numOfNeighbours.begin() + i);
  			sort(numOfNeighbours.begin(),numOfNeighbours.end(),compFunction);
  		}
  		
  		// k = -1;
  		// for(i = 0; i < numOfNeighbours.size(); i++){
  		// 	if(numOfNeighbours[i].second.size() >= 15){
  		// 		continue;
  		// 	}
  		// 	if(k == -1)
  		// 		k = i;
  		// 	stack.push_back(numOfNeighbours[i].first);
  		// 	for(j = 0;j<numOfNeighbours.size();j++){
  		// 		numOfNeighbours[j].second.erase(numOfNeighbours[i].first);
  		// 	}
  		// 	num
  		// }
  		// for(i = 0;i<numOfNeighbours.size();i++){
  		// 	cout<<numOfNeighbours[i].second.size()<<" ";
  		// }
  		// cout<<endl;

  		i = 0;
  		while(numOfNeighbours.size() > 0 && i < numOfNeighbours.size()){
  			stack.push_back(numOfNeighbours[i].first);
  			for(j = 0;j<numOfNeighbours.size();j++){
  				numOfNeighbours[j].second.erase(numOfNeighbours[i].first);
  			}
  			numOfNeighbours.erase(numOfNeighbours.begin() + i);
  			sort(numOfNeighbours.begin(),numOfNeighbours.end(),compFunction);
  			i = 0;
  		}


  		// if(k == -1)
  		// 	k = numOfNeighbours.size();
  		// for(i = k-1; i >= 0; i--){
  		// 	stack.push_back(numOfNeighbours[i].first);
  		// 	cout<<"stack push size = "<<numOfNeighbours[i].second.size()<<endl;
  		// }


  		while(stack.size() != 0){
  			node = stack.back();
  			// cout<<node<<endl;
  			stack.pop_back();
  			if(colorMap.find(node) != colorMap.end()){
  				continue;
  			}
  			tempset = graph[node];
  			for(unsetItr = tempset.begin(); unsetItr != tempset.end(); ++unsetItr){
  				colorMapItr = colorMap.find(*unsetItr);
  				if(colorMapItr != colorMap.end())
  					temp.insert(colorMapItr->second);
  			}
  			i=0;
  			for(setitr = temp.begin();setitr != temp.end();++setitr){
  				if(*setitr != i)
  					break;
  				i++;
  			}
  			if(i<15)
  				colorMap.insert(pair<string,int>(node,i));
  			else
  				spillVar.push_back(node);
  			temp.clear();
  		}

  		// printgraph(graph);

  		// cout<<"spillVars"<<endl;

  		// for(auto p : spillVar)
  		// 	cout<<p<<endl;

  		// cout<<"colorMap"<<endl;

  		// for(auto p : colorMap)
  		// 	cout<<p.first<<" "<<p.second<<endl;

  		pair<vector<string>,map<string,int>> res(spillVar,colorMap);

  		return res;

	}
}
