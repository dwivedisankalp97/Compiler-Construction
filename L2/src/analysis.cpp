#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <unordered_set>

#include <analysis.h>

using namespace std;




namespace L2{

	std::string enumToStringA[17] = {"rdi", "rax", "rbx", "rbp", "r10", "r11", "r12", "r13", "r14", "r15", "rsi", "r8", "r9", "rcx", "rdx", "rsp"};
  	std::string enumToString8BitA[17] = {"dil","al","bl","bpl","r10b","r11b","r12b","r13b","r14b","r15b","sil","r8b","r9b","cl","dl","spl"};

 
	string remove_percent(string name){
		return name.substr(1);
	}

	/*void remove_percents(vector<vector<string>> &a){
		// cout<<"here2";
		for(int i =0;i<a.size();i++){
			// cout<<"here1";
			for(int j = 0; j < a[i].size();j++){
				if(a[i][j][0] == '%'){
					// cout<<"here";
					a[i][j] = a[i][j].substr(1);
				}

			}
		}

	}*/

	void Generate_label_index(Function* entryFun, map<string,int> &labels){

		for(vector<Instruction *>::iterator i = entryFun->instructions.begin(); i != entryFun->instructions.end(); i++){
			auto g = *i;
			//cout<<g->iType()<<endl;
			IType ins_type = g->iType();
			switch (ins_type)
			{
				case labelType:{
					Instruction_label* g1 = static_cast<Instruction_label*>(g);
					labels.insert(pair<string,int>(g1->label.labelName,(i - (entryFun->instructions.begin()))));
				}
					break;

				default:
					continue;
			}

		}
	}

	void push_several_back(vector<string> &a, vector<string> &b, int n){
		for(int i = 0;i<n;i++){
			a.push_back(b[i]);
		}
	}

	void Generate_gen_kill(vector<vector<string>> &gen,vector<vector<string>> &kill,\
		Function* entryFun, map<string,int> &labels, vector<vector<int>> &successor){

		int pos;
		auto begin = entryFun->instructions.begin();
  		string tempstr = "";
  		string tempstr1 = "";
  		string tempstr2 = "";
  		vector<string> tempgen,tempkill;
  		vector<int> tempsuc;
  		vector<string> caller_save;
  		caller_save.push_back("r8");
		caller_save.push_back("r9");
		caller_save.push_back("r10");
		caller_save.push_back("r11");
		caller_save.push_back("rax");
		caller_save.push_back("rcx");
		caller_save.push_back("rdi");
		caller_save.push_back("rdx");
		caller_save.push_back("rsi");

		vector<string> arguments;
		arguments.push_back("rdi");
		arguments.push_back("rsi");
		arguments.push_back("rdx");
		arguments.push_back("rcx");
		arguments.push_back("r8");
		arguments.push_back("r9");


  		auto reset_values = [&](){
  			tempstr = "";
			tempstr1 = "";
  			tempstr2 = "";
  			tempgen.clear();
  			tempkill.clear();
  			tempsuc.clear();
  		};
		for(vector<Instruction *>::iterator i = entryFun->instructions.begin(); i != entryFun->instructions.end(); i++){
			auto g = *i;
			int pos = i - begin;
			//cout<<g->iType()<<endl;
			IType ins_type = g->iType();
			switch (ins_type)
			{
				case ret:
					tempgen.push_back("rax");
					tempgen.push_back("rbx");
					tempgen.push_back("rbp");
					tempgen.push_back("r15");
					tempgen.push_back("r14");
					tempgen.push_back("r13");
					tempgen.push_back("r12");
					kill.push_back(tempkill);
					gen.push_back(tempgen);
					successor.push_back(tempsuc);
					reset_values();
					break;


				case assign_cmp:{
					Instruction_assignment_cmp* g1 = static_cast<Instruction_assignment_cmp*>(g);
					if(g1->left.isAVar){
						tempstr = remove_percent(g1->left.labelName);
					}
					else{
						tempstr = enumToStringA[g1->left.r];
					}
					if(tempstr != "" )
						tempkill.push_back(tempstr);

					if(g1->lc.isAVar)
						tempstr1 = remove_percent(g1->lc.labelName);
					else if(g1->lc.isARegister){
						tempstr1 = enumToStringA[g1->lc.r];
					}
					if(tempstr1 != "")
						tempgen.push_back(tempstr1);

					if(g1->rc.isAVar)
						tempstr2 = remove_percent(g1->rc.labelName);
					else if(g1->rc.isARegister){
						tempstr2 = enumToStringA[g1->rc.r];
					}
					if(tempstr2 != "")
						tempgen.push_back(tempstr2);
					kill.push_back(tempkill);
					gen.push_back(tempgen);
					tempsuc.push_back(pos+1);
					successor.push_back(tempsuc);
					reset_values();
				}
					break;


				case assign:{
					Instruction_assignment* g1 = static_cast<Instruction_assignment*>(g);
					if(g1->dst.isAVar){
						tempstr = remove_percent(g1->dst.labelName);
					}
					else{
						tempstr = enumToStringA[g1->dst.r];
					}
					tempkill.push_back(tempstr);


					if(g1->src.isAVar)
						tempstr2 = remove_percent(g1->src.labelName);
					else if(g1->src.isARegister){
						tempstr2 = enumToStringA[g1->src.r];
					}
					if(tempstr2 != "")
						tempgen.push_back(tempstr2);
					kill.push_back(tempkill);
					gen.push_back(tempgen);
					tempsuc.push_back(pos+1);
					successor.push_back(tempsuc);
					reset_values();

				}
					break;


				case assign_mem:{
					instruction_assignment_mem_x_M* g1 = static_cast<instruction_assignment_mem_x_M*>(g);

					if(g1->x.isAVar){
						tempstr = remove_percent(g1->x.labelName);
					}
					else{
						tempstr = enumToStringA[g1->x.r];
					}

					if(g1->other.isAVar){
						tempstr1 = remove_percent(g1->other.labelName);
					}
					else if(g1->other.isARegister){
						tempstr1 = enumToStringA[g1->other.r];
						
					}
					else
						tempstr1 = "";

					if(g1->isSrc){
						tempkill.push_back(tempstr1);
						tempgen.push_back(tempstr);
					}
					else{
						tempgen.push_back(tempstr1);
						tempgen.push_back(tempstr);
					}

					kill.push_back(tempkill);
					gen.push_back(tempgen);
					tempsuc.push_back(pos+1);
					successor.push_back(tempsuc);
					reset_values();


				}
					break;


				case aop:{
					instruction_aop* g1 = static_cast<instruction_aop*>(g);

					if(g1->dest.isAVar){
						tempstr = remove_percent(g1->dest.labelName);
					}
					else{
						tempstr = enumToStringA[g1->dest.r];
					}
					tempkill.push_back(tempstr);
					tempgen.push_back(tempstr);

					if(g1->src.isAVar){
						tempstr1 = remove_percent(g1->src.labelName);
					}
					else if(g1->src.isARegister){
						tempstr1 = enumToStringA[g1->src.r];
					}
					tempgen.push_back(tempstr1);
					gen.push_back(tempgen);
					kill.push_back(tempkill);
					tempsuc.push_back(pos+1);
					successor.push_back(tempsuc);
					reset_values();
				}
					break;


				case sop:{
					instruction_sop* g1 = static_cast<instruction_sop*>(g);	

					if(g1->leftop.isAVar){
						tempstr = remove_percent(g1->leftop.labelName);
					}
					else{
						tempstr = enumToStringA[g1->leftop.r];
					}
					tempkill.push_back(tempstr);

					if(g1->rightop.isAVar){
						tempstr1 = remove_percent(g1->rightop.labelName);
					}
					else if(g1->rightop.isARegister){
						tempstr1 = enumToStringA[g1->rightop.r];
					}
					tempgen.push_back(tempstr1);
					tempgen.push_back(tempstr);

					kill.push_back(tempkill);
					gen.push_back(tempgen);
					tempsuc.push_back(pos+1);
					successor.push_back(tempsuc);
					reset_values();
				}
					break;


				case aop_mem:{
					instruction_aop_mem_x_M* g1 = static_cast<instruction_aop_mem_x_M*>(g);

					if(g1->x.isAVar){
						tempstr = remove_percent(g1->x.labelName);
					}
					else{
						tempstr = enumToStringA[g1->x.r];
					}

					if(g1->other.isAVar)
						tempstr1 = remove_percent(g1->other.labelName);
					else if(g1->other.isARegister){
						tempstr1 = enumToStringA[g1->other.r];
						
					}
					else
						tempstr1 = "";

					tempgen.push_back(tempstr1);
					tempgen.push_back(tempstr);

					if(g1->isLeft){
						tempkill.push_back(tempstr1);
					}

					kill.push_back(tempkill);
					gen.push_back(tempgen);
					tempsuc.push_back(pos+1);
					successor.push_back(tempsuc);
					reset_values();
					
				}
					break;


				case cjump:{
					Instruction_cjump* g1 = static_cast<Instruction_cjump*>(g);

					if(g1->lc.isAVar)
						tempstr = remove_percent(g1->lc.labelName);
					else if(g1->lc.isARegister){
						tempstr = enumToStringA[g1->lc.r];
					}

					if(g1->rc.isAVar)
						tempstr1 = remove_percent(g1->rc.labelName);
					else if(g1->rc.isARegister){
						tempstr1 = enumToStringA[g1->rc.r];
					}
					tempgen.push_back(tempstr);
					tempgen.push_back(tempstr1);

					kill.push_back(tempkill);
					gen.push_back(tempgen);
					tempsuc.push_back(pos+1);
					tempsuc.push_back(labels[g1->label.labelName]);
					successor.push_back(tempsuc);
					reset_values();
				}
					break;


				case labelType:{
					Instruction_label* g1 = static_cast<Instruction_label*>(g);
					kill.push_back(tempkill);
					gen.push_back(tempgen);
					tempsuc.push_back(pos+1);
					successor.push_back(tempsuc);
					reset_values();

				}
					break;


				case goto1:{
					Instruction_goto* g1 = static_cast<Instruction_goto*>(g);
					kill.push_back(tempkill);
					gen.push_back(tempgen);
					tempsuc.push_back(labels[g1->label.labelName]);
					successor.push_back(tempsuc);
					reset_values();
				}
					break;


				case call:{
					Instruction_call* g1 = static_cast<Instruction_call*>(g);
					if(g1->fname.isARegister){
						tempstr = enumToStringA[g1->fname.r];
						tempgen.push_back(tempstr);
					}
					else if(g1->fname.isAVar){
						tempstr = remove_percent(g1->fname.labelName);
						tempgen.push_back(tempstr);
					}
					if(stoi(g1->argn.labelName) <= 5)
						push_several_back(tempgen,arguments,stoi(g1->argn.labelName));
					else
						push_several_back(tempgen,arguments,5);
					kill.push_back(caller_save);
					gen.push_back(tempgen);
					tempsuc.push_back(pos+1);
					successor.push_back(tempsuc);
					reset_values();

				}
					break;


				case print:
					push_several_back(tempgen,arguments,1);
					kill.push_back(caller_save);
					gen.push_back(tempgen);
					tempsuc.push_back(pos+1);
					successor.push_back(tempsuc);
					reset_values();
					break;


				case allocate:
					push_several_back(tempgen,arguments,2);
					kill.push_back(caller_save);
					gen.push_back(tempgen);
					tempsuc.push_back(pos+1);
					successor.push_back(tempsuc);
					reset_values();
					break;


				case array:
					push_several_back(tempgen,arguments,2);
					kill.push_back(caller_save);
					gen.push_back(tempgen);
					tempsuc.push_back(pos+1);
					successor.push_back(tempsuc);
					reset_values();
					break;


				case inc_dec:{
					Instruction_inc_dec* g1 = static_cast<Instruction_inc_dec*>(g);

					if(g1->reg.isAVar)
						tempstr = remove_percent(g1->reg.labelName);
					else if(g1->reg.isARegister){
						tempstr = enumToStringA[g1->reg.r];
					}

					tempkill.push_back(tempstr);
					tempgen.push_back(tempstr);

					kill.push_back(tempkill);
					gen.push_back(tempgen);
					//cout<<tempstr<<endl;
					tempsuc.push_back(pos+1);
					successor.push_back(tempsuc);
					reset_values();
				}
					break;

				case lea:{
					Instruction_lea* g1 = static_cast<Instruction_lea*>(g);

					if(g1->first.isAVar)
						tempstr = remove_percent(g1->first.labelName);
					else if(g1->first.isARegister){
						tempstr = enumToStringA[g1->first.r];
					}

					if(g1->second.isAVar)
						tempstr1 = remove_percent(g1->second.labelName);
					else if(g1->second.isARegister){
						tempstr1 = enumToStringA[g1->second.r];
					}

					if(g1->third.isAVar)
						tempstr2 = remove_percent(g1->third.labelName);
					else if(g1->third.isARegister){
						tempstr2 = enumToStringA[g1->third.r];
					}

					tempgen.push_back(tempstr2);
					tempgen.push_back(tempstr1);
					tempkill.push_back(tempstr);

					kill.push_back(tempkill);
					gen.push_back(tempgen);
					tempsuc.push_back(pos+1);
					successor.push_back(tempsuc);
					reset_values();
				}
				break;

				case stack_arg:{
					Instruction_stack_arg* g1 = static_cast<Instruction_stack_arg*>(g);

					if(g1->left.isAVar)
						tempstr = remove_percent(g1->left.labelName);
					else if(g1->left.isARegister){
						tempstr = enumToStringA[g1->left.r];
					}

					tempkill.push_back(tempstr);

					kill.push_back(tempkill);
					gen.push_back(tempgen);
					//cout<<tempstr<<endl;
					tempsuc.push_back(pos+1);
					successor.push_back(tempsuc);
					reset_values();


				}

			}
		}// For loop end

		return;
	}


	vector<string> subtract_set(vector<string> a, vector<string> b) // a - b
	{
		vector<string> res;
		auto begin = b.begin();
		auto end = b.end();
		for(int i=0;i<a.size();i++){
			if(find(begin,end,a[i]) != end)
				continue;
			else
				res.push_back(a[i]);
		}

		return res;

	}

	vector<string> union_set(vector<string> a, vector<string> b){
		for(int i=0;i<b.size();i++){
			a.push_back(b[i]);
		}

		unordered_set<string> tempset (a.begin(),a.end());
		vector<string> res (tempset.begin(),tempset.end());
		return res;
	}

	vector<string> union_set_successors(vector<int> successor, vector<vector<string>> &in)
	{
		vector<string> res;
		for(int i = 0;i< successor.size();i++){
			res = union_set(res,in[successor[i]]);
		}

		return res;


	}

	void remove_rsp(vector<vector<string>> &a){
		int temp = a.size();
		vector<vector<int>::iterator> posi;
		int i = 0;
		vector<string>::iterator j;
		for(i=0;i<temp;i++){
			j = find(a[i].begin(),a[i].end(),"rsp");
			if(j!=a[i].end())
				a[i].erase(j);
		}
	}




	vector<vector<vector<string>>> liveness_analysis(Function *f){

		Function *entryFun = f;
		//cout << entryFun -> name<<endl;

		vector<vector<string>> gen,kill;
		vector<string> tempvec;
		map<string,int> labels;
		vector<vector<int>> successor;
		int i,j;

		Generate_label_index(entryFun, labels);

		Generate_gen_kill(gen, kill, entryFun,labels,successor);
		
		remove_rsp(gen);
		remove_rsp(kill);


		/*cout<<"Gen"<<endl;

		for(i=0;i<gen.size();i++){
			cout<<i<<endl;
			cout<<"gen -> ";
			//cout<<gen[i].size();
			for(j=0;j<gen[i].size();j++){
			//	cout<<"j = "<<j;
				cout<<gen[i][j]<<" ";
			}
			cout<<endl<<"kill -> ";
			for(j=0;j<kill[i].size();j++){

				cout<<kill[i][j]<<" ";
			}
			cout<<endl;
		}
		cout<<"here"<<endl;

		

		cout << successor.size()<<endl;;

		for(i=0;i<successor.size();i++){
			cout << "i = "<<i<<endl;
			for(j=0;j<successor[i].size();j++){
				cout<<successor[i][j]<< " ";
			}
			cout<<endl;
		}*/

		vector<vector<string>> in (gen.size(),vector<string> (0,""));
		vector<vector<string>> out (gen.size(),vector<string> (0,""));

		
		
		
		bool change;
		do{
			change = false;
			for(i=gen.size()-1;i>-1;i--){
				tempvec = union_set(gen[i],subtract_set(out[i],kill[i]));
				sort(tempvec.begin(),tempvec.end());
				if(tempvec != in[i]){
					in[i] = tempvec;
					change = true;
				}
				tempvec = union_set_successors(successor[i],in);
				if(tempvec != out[i]){
					out[i] = tempvec;
					change = true;
				}
			}
		}while(change == true);

		// cout<<"("<<endl<<"(in"<<endl;
		// for(i=0;i<in.size();i++){
		// 	cout<<"(";
		// 	for(j=0;j<in[i].size();j++){
		// 		cout<<in[i][j]<<" ";
		// 	}
		// 	cout<<")"<<endl;
		// }

		// cout<<")"<<endl<<endl<<"(out"<<endl;
		// for(i=0;i<out.size();i++){
		// 	cout<<"(";
		// 	for(j=0;j<out[i].size();j++){
		// 		cout<<out[i][j]<<" ";
		// 	}
		// 	cout<<")"<<endl;
		// }
		// cout<<")"<<endl<<endl<<")"; //STDOUT RESULT

		vector<vector<vector<string>>> res;
		res.push_back(in);
		res.push_back(out);
		res.push_back(kill);
	   
	    return res;
	  }
}
