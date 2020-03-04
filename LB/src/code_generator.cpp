#include <string>
#include <iostream>
#include <fstream>

#include <code_generator.h>
#include <parser.h>
#include <map>
#include <vector>
#include <time.h>
#include <set>
#include <unordered_set>

using namespace std;



namespace LB{

	int newVarNum = 0;
	int newLabelNum = 0;
	string newVarString = "NewVarLB";
	string newLabelString = ":NewLabelLB";
	unordered_set<string> codeMap;
	int currentScope = -1;
	vector<map<string,string>> scopes;
	map<Instruction*,string> condLabels;
	map<Instruction*,string> endWhile;
	map<Instruction*,string> beginWhile;
	set<Instruction*> whileSeen;
	vector<Instruction*> loopStack;
	map<Instruction*,Instruction*> loop;

	void AddPercent(Item &c){
		if(c.isAVar){
			c.labelName = "%" + c.labelName;
			return;
		}
	}

  void GenerateFunctionEntry(Function* entryFun,ofstream& outputFile){
  	outputFile<<entryFun->type.labelName<<" "<<entryFun->name<<" (";
  	int j;
  	string s = "";
  	if(entryFun->arguments.size()>0){
	  	for(j=0 ;j<entryFun->arguments.size();j++){
	  		s = s + entryFun->argumentsType[j].labelName + " "+entryFun->arguments[j].labelName + ",";
	  	}
	  	s.pop_back();
	  	outputFile<<s;
  	}
  	outputFile<<"){"<<endl;
  	return;

  }

  string GetNewVarNum(){
  	newVarNum++;
  	return to_string(newVarNum);
  }

  string GetNewLabelNum(){
  	newLabelNum++;
  	return to_string(newLabelNum);
  }

  void checkScope(Item &var){
  	map<string,string>::iterator s;
  	string tempstr;
  	for(int i = currentScope; i>=0 ; i--){
  		s = scopes[i].find(var.labelName);
  		if(s != scopes[i].end()){
  			var.labelName = s->second;
  		}
  	}
  	return;
  }

  void GenerateWhileMaps(Function* entryFun){
  	string tempstr;
  	for(vector<Instruction *>::iterator i = entryFun->instructions.begin(); i != entryFun->instructions.end(); i++){
		auto g = *i;
		IType ins_type = g->iType();
		switch (ins_type)
		{
			case whileins:{
				Instruction_while* g1 = static_cast<Instruction_while*>(g);
				tempstr = newLabelString+GetNewLabelNum();
				// cout<<"map"<<endl;
				// cout<<g1<<endl;
				// cout<<tempstr<<endl;
				beginWhile.insert(pair<Instruction*,string>(g1,g1->tlabel.labelName));
				endWhile.insert(pair<Instruction*,string>(g1,g1->flabel.labelName));
				condLabels.insert(pair<Instruction*,string>(g1,tempstr));
				break;
			}
		}
	}
}

void MapInstructionToLoops(Function* entryFun){
  	string tempstr;
  	set<Instruction*>::iterator si;
  	map<Instruction*,string>::iterator mis;
  	for(vector<Instruction *>::iterator i = entryFun->instructions.begin(); i != entryFun->instructions.end(); i++){
		auto g = *i;
		IType ins_type = g->iType();
		switch (ins_type)
		{
			case whileins:{
				Instruction_while* g1 = static_cast<Instruction_while*>(g);
				si = whileSeen.find(g1);
				if(si==whileSeen.end()){
					loopStack.push_back(g1);
					whileSeen.insert(g1);
				}
				break;
			}

			case contins:{
				Instruction_continue* g1 = static_cast<Instruction_continue*>(g);
				// cout<<"loop ins"<<loopStack.back()<<endl;
				loop.insert(pair<Instruction*,Instruction*>(g1,loopStack.back()));
				break;
			}

			case breakins:{
				Instruction_break* g1 = static_cast<Instruction_break*>(g);
				loop.insert(pair<Instruction*,Instruction*>(g1,loopStack.back()));
				break;
			}

			case labelType:{
				Instruction_label* g1 = static_cast<Instruction_label*>(g);
				for(mis = beginWhile.begin(); mis != beginWhile.end(); ++mis){
					if(mis->second == g1->label.labelName){
						si = whileSeen.find(mis->first);
						if(si == whileSeen.end()){
							loopStack.push_back(mis->first);
						}
					}
				}

				for(mis = endWhile.begin(); mis != endWhile.end(); ++mis){
					if(mis->second == g1->label.labelName){
						loopStack.pop_back();
					}
				}
				break;
			}

		}
	}
}	

  void GenerateInstructions(Function* entryFun,ofstream& outputFile){
  	string tempstr,tempstr1,tempstr2;
	int tempint,tempint1;
	// vector<TreeNode*> trees;
	set<Instruction*> whileSeen;
	vector<Instruction*> loopStack;
	Instruction* tempins;

	int numOfCalls = 0;
	GenerateFunctionEntry(entryFun,outputFile);
	string function_name = entryFun->name;
	for(vector<Instruction *>::iterator i = entryFun->instructions.begin(); i != entryFun->instructions.end(); i++){
		auto g = *i;
		IType ins_type = g->iType();
		// cout<<"INs "<<ins_type<<endl;
		switch (ins_type)
		{
			case scope_start:{
				currentScope++;
				// cout<<currentScope<<endl;
				map<string,string> vlb;
				scopes.push_back(vlb);
				break;
			}

			case scope_end:{
				currentScope--;
				scopes.pop_back();
				break;
			}

			case ifins:{
				Instruction_if* g1 = static_cast<Instruction_if*>(g);
				checkScope(g1->lc);
				checkScope(g1->rc);
				outputFile<<"\t";
				outputFile<<"int64 "<<newVarString<<GetNewVarNum()<<endl;
				outputFile<<"\t";
				outputFile<<newVarString<<newVarNum<<" <- "<<g1->lc.labelName<<" "<<g1->operation.labelName<<" "<<g1->rc.labelName<<endl;
				outputFile<<"\t";
				outputFile<<"br "<<newVarString<<newVarNum<<" "<<g1->tlabel.labelName<<" "<<g1->flabel.labelName<<endl;
				break;
			}

			case whileins:{
				Instruction_while* g1 = static_cast<Instruction_while*>(g);
				outputFile<<condLabels[g1]<<endl;
				checkScope(g1->lc);
				checkScope(g1->rc);
				outputFile<<"\t";
				outputFile<<"int64 "<<newVarString<<GetNewVarNum()<<endl;
				outputFile<<"\t";
				outputFile<<newVarString<<newVarNum<<" <- "<<g1->lc.labelName<<" "<<g1->operation.labelName<<" "<<g1->rc.labelName<<endl;
				outputFile<<"\t";
				outputFile<<"br "<<newVarString<<newVarNum<<" "<<g1->tlabel.labelName<<" "<<g1->flabel.labelName<<endl;
				break;
			}

			case contins:{
				Instruction_continue* g1 = static_cast<Instruction_continue*>(g);
				tempins = loop[g1];
				outputFile<<"\t";
				// cout<<"continue"<<endl;
				// cout<<loop.size()<<endl;
				// cout<<tempins<<endl;
				tempstr = condLabels[tempins];
				// cout<<tempstr<<endl;
				outputFile<<"br "<<tempstr<<endl;
				break;
			}

			case breakins:{
				Instruction_break* g1 = static_cast<Instruction_break*>(g);
				tempins = loop[g1];
				outputFile<<"\t";
				tempstr = endWhile[tempins];
				outputFile<<"br "<<tempstr<<endl;
				break;
			}

			case ret:
				outputFile<<"\t";
				outputFile << "return\n";
				break;

			case ret_t:{
				outputFile<<"\t";
				Instruction_ret_t* g1 = static_cast<Instruction_ret_t*>(g);
				checkScope(g1->returnVar);
				outputFile<<g1->tostring();
				break;
			}

			case br:{
				outputFile<<"\t";
				Instruction_br* g1 = static_cast<Instruction_br*>(g);
				outputFile<<g1->tostring();
				break;
			}

			case assign:{
				outputFile<<"\t";
				Instruction_assignment* g1 = static_cast<Instruction_assignment*>(g);
				checkScope(g1->src);
				checkScope(g1->dst);
				outputFile<<g1->tostring();
				break;
			}

			case define:{
				Instruction_define* g1 = static_cast<Instruction_define*>(g);
				for(auto vlb : g1->vars){
					outputFile<<"\t";
					// cout<<currentScope<<endl;
					tempstr = vlb.labelName + newVarString+ "scope" + to_string(currentScope);
					outputFile<<g1->type.labelName<<" "<<tempstr<<endl;
					scopes[currentScope].insert(pair<string,string>(vlb.labelName,tempstr));
				}
				break;
			}

			case assign_cmp_aop:{
				Instruction_assignment_cmp_aop* g1 = static_cast<Instruction_assignment_cmp_aop*>(g);
				outputFile<<"\t";
				checkScope(g1->left);
				checkScope(g1->lc);
				checkScope(g1->rc);
				outputFile<<g1->tostring();
				break;

			}


			case labelType:{
				outputFile<<"\t";
				Instruction_label* g1 = static_cast<Instruction_label*>(g);
				outputFile<<g1->tostring();
				break;

			}


			case call:{
				Instruction_call* g1 = static_cast<Instruction_call*>(g);
				outputFile<<"\t";
				for(vector<Item>::iterator v1 = g1->arguments.begin();v1 != g1->arguments.end(); ++v1){
					checkScope(*v1);
				}
				outputFile<<g1->tostring();
				break;

			}

			case assign_call:{
				Instruction_assign_call* g1 = static_cast<Instruction_assign_call*>(g);
				outputFile<<"\t";
				checkScope(g1->var);
				for(vector<Item>::iterator v1 = g1->arguments.begin();v1 != g1->arguments.end(); ++v1){
					checkScope(*v1);
				}
				outputFile<<g1->tostring();
				break;
			}

			case new_array:{
				outputFile<<"\t";
				Instruction_new_array* g1 = static_cast<Instruction_new_array*>(g);
				checkScope(g1->var);
				for(vector<Item>::iterator v1 = g1->arguments.begin();v1 != g1->arguments.end(); v1++){
					checkScope(*v1);
				}
				outputFile<<g1->tostring();
				break;
			}

			case array_dest:{
				Instruction_array_dest* g1 = static_cast<Instruction_array_dest*>(g);
				outputFile<<"\t";
				checkScope(g1->var);
				checkScope(g1->src);
				for(vector<Item>::iterator v1 = g1->dimensions.begin();v1 != g1->dimensions.end(); v1++){
						checkScope(*v1);
				}
				outputFile<<g1->tostring();
				break;
			}

			case array_src:{
				Instruction_array_src* g1 = static_cast<Instruction_array_src*>(g);
				outputFile<<"\t";
				checkScope(g1->var);
				checkScope(g1->dest);
				for(vector<Item>::iterator v1 = g1->dimensions.begin();v1 != g1->dimensions.end(); v1++){
						checkScope(*v1);
				}
				outputFile<<g1->tostring();
				break;
			}

			case new_tuple:{
				Instruction_new_tuple* g1 = static_cast<Instruction_new_tuple*>(g);
				outputFile<<"\t";
				checkScope(g1->var);
				checkScope(g1->arg);
				outputFile<<g1->tostring();
				break;
			}

			case length:{
				Instruction_length* g1 = static_cast<Instruction_length*>(g);
				outputFile<<"\t";
				checkScope(g1->src);
				checkScope(g1->dest);
				checkScope(g1->arg);
				outputFile<<g1->tostring();
				break;
			}
		}
	}// For loop end*/
}


   void PrintInstructions(Function* entryFun){
	for(vector<Instruction *>::iterator i = entryFun->instructions.begin(); i != entryFun->instructions.end(); i++){
		auto g = *i;
		IType ins_type = g->iType();
		cout<<g->tostring();
	}
}

  void generate_code(Program p){

	std::ofstream outputFile;
	outputFile.open("prog.a");
	string entry = p.entryPointLabel;
	vector<Instruction*> newInstList;

	for (vector<Function *>::iterator i = p.functions.begin();i!=p.functions.end();i++){
		auto g = *i;
		GenerateWhileMaps(g);
		MapInstructionToLoops(g);
		GenerateInstructions(g,outputFile);
		outputFile<<"}\n\n";
		loopStack.clear();
		whileSeen.clear();
		loop.clear();
		condLabels.clear();
		beginWhile.clear();
		endWhile.clear();
	}
	outputFile.close();


   return ;
}

}
