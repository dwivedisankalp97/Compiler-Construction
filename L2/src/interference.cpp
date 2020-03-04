#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <unordered_set>

#include <analysis.h>

using namespace std;




namespace L2{

	vector<string> GPregisters{"rdi", "rax", "rbx", "rbp", "r10", "r11", "r12", "r13", "r14", "r15", "rsi", "r8", "r9", "rcx", "rdx"};

	void create_graph(map<string, unordered_set<string>> &graph, Function *entryFun){
		vector<vector<vector<string>>> data = liveness_analysis(entryFun);
		vector<vector<string>> in = data[0];
		vector<vector<string>> out = data[1];
		vector<vector<string>> kill = data[2];
		int i,j,k,tempint;
		map<string, unordered_set<string>>::iterator itr,itr3;
		unordered_set<string> temp;
		vector<string>::iterator vecsitr;

		// IN set
		for(i=0;i<in.size();i++){
			for(j=0;j<in[i].size();j++){
				if(in[i][j]=="")
					continue;
				itr = graph.find(in[i][j]);
				if(itr == graph.end()){
					graph.insert(pair<string,unordered_set<string>>(in[i][j],temp));
					itr = graph.find(in[i][j]);
				}
				if(itr!=graph.end()){
					for(k=0;k<in[i].size();k++){
						if(j==k){
							continue;
						}
						(itr->second).insert(in[i][k]);
					}
				}
			}
		}

		//OUT set
		for(i=0;i<out.size();i++){
			for(j=0;j<out[i].size();j++){
				if(out[i][j]=="")
					continue;
				itr = graph.find(out[i][j]);
				if(itr == graph.end()){
					graph.insert(pair<string,unordered_set<string>>(out[i][j],temp));
					itr = graph.find(out[i][j]);
				}
				if(itr!=graph.end()){
					for(k=0;k<out[i].size();k++){
						if(j==k){
							continue;
						}
						(itr->second).insert(out[i][k]);
					}
				}
			}

		}

		//KILL set
		for(i=0;i<kill.size();i++){
			for(j=0;j<kill[i].size();j++){
				if(kill[i][j]=="")
					continue;
				itr = graph.find(kill[i][j]);
				if(itr == graph.end()){
					graph.insert(pair<string,unordered_set<string>>(kill[i][j],temp));
					itr = graph.find(kill[i][j]);
				}
				if(itr!=graph.end()){
					for(k=0;k<out[i].size();k++){
						if(kill[i][j] == out[i][k])
							continue;
						(itr->second).insert(out[i][k]);
						itr3 = graph.find(out[i][k]);
						if(itr3 == graph.end()){
							graph.insert(pair<string,unordered_set<string>>(out[i][k],temp));
							itr3 = graph.find(out[i][k]);
						}
						(itr3->second).insert(kill[i][j]);
					}
				}
			}

		}

		// SOP Case
		string tempstr;
		for(i = 0; i < entryFun->instructions.size(); i++){
			auto g = entryFun->instructions[i];
			IType ins_type = g->iType();
			switch (ins_type)
			{
				case sop:{
					instruction_sop* g1 = static_cast<instruction_sop*>(g);	
					if(g1->rightop.isAVar){
						tempstr = (g1->rightop.labelName).substr(1);
					}
					else
						break;

					itr = graph.find(tempstr);
					if(itr == graph.end()){
						graph.insert(pair<string,unordered_set<string>>(tempstr,temp));
						itr = graph.find(tempstr);
					}
					for(j=0;j<GPregisters.size();j++){
						if(GPregisters[j]=="rcx")
							continue;
						(itr->second).insert(GPregisters[j]);
						itr3 = graph.find(GPregisters[j]);
						if(itr3 == graph.end()){
							graph.insert(pair<string,unordered_set<string>>(GPregisters[j],temp));
							itr3 = graph.find(GPregisters[j]);
						}
						(itr3->second).insert(tempstr);
					}


			}
					break;

				default:
					continue;
			}

		}

		//Connect every GP to other GP
		for(i=0;i<GPregisters.size();i++){
			itr = graph.find(GPregisters[i]);
			if(itr == graph.end()){
				graph.insert(pair<string,unordered_set<string>>(GPregisters[i],temp));
				itr = graph.find(GPregisters[i]);
			}
			for(j=0;j<GPregisters.size();j++){
				if(i==j)
					continue;
				(itr->second).insert(GPregisters[j]);
			}

		}
		return;
	}


	void printGraph(map<string, unordered_set<string>> &graph){
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


	map<string,unordered_set<string>> interference_graph (Function *f){
		map<string, unordered_set<string>> graph;
		create_graph(graph, f);
		graph.erase("");
		// printGraph(graph);
		return graph;
	}
}