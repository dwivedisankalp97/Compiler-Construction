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



namespace LA{

	int newVarNum = 0;
	int newLabelNum = 0;
	string newVarString = "NewVarLA";
	unordered_set<string> codeMap;

	void AddPercent(Item &c){
		if(c.isAVar){
			c.labelName = "%" + c.labelName;
			return;
		}
	}


  void GenerateFunctionEntry(Function* entryFun,ofstream& outputFile){
  	outputFile<<"define "<<entryFun->type.labelName<<" :"<<entryFun->name<<" (";
  	int j;
  	string s = "";
  	if(entryFun->arguments.size()>0){
	  	for(j=0 ;j<entryFun->arguments.size();j++){
	  		s = s + entryFun->argumentsType[j].labelName + " %"+entryFun->arguments[j].labelName + ",";
	  	}
	  	s.pop_back();
	  	outputFile<<s;
  	}
  	outputFile<<"){"<<endl;
  	return;

  }

  void GenerateFunctionEntryTemp(Function* entryFun,ofstream& outputFile){
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

  void EncodeConstant(Item &c){
  	if(c.isANumber){
  		int t = stoi(c.labelName);
  		t = t << 1;
  		t += 1;
  		c.labelName = to_string(t);
  		return;
  	}
  }

  bool Decode(Item c, ofstream& outputFile){
  	if(!c.isAVar)
  		return false;
  	outputFile<<"\t";
  	outputFile<<"int64 "<<newVarString<<newVarNum<<endl;
  	outputFile<<"\t";
  	outputFile<<newVarString<<newVarNum<<" <- "<<c.labelName<<" >> 1"<<endl;
  	newVarNum++;
  	return true;
  }

  void EncodeVarAssign(Item &c,ofstream& outputFile){
  	outputFile<<"\t";
  	outputFile<<c.labelName<<" <- "<<c.labelName<<" << 1"<<endl;
  	outputFile<<"\t";
  	outputFile<<c.labelName<<" <- "<<c.labelName<<" + 1"<<endl;
  }


  void DeclarNewVar(ofstream& outputFile){
  	outputFile<<"\t";
  	outputFile<<"int64 "<<newVarString<<newVarNum<<endl;
  }

  void GenerateInstructions(Function* entryFun,ofstream& outputFile){
  	string tempstr,tempstr1,tempstr2;
	int tempint,tempint1;
	// vector<TreeNode*> trees;

	int numOfCalls = 0;
	GenerateFunctionEntryTemp(entryFun,outputFile);
	string function_name = entryFun->name;
	for(vector<Instruction *>::iterator i = entryFun->instructions.begin(); i != entryFun->instructions.end(); i++){
		auto g = *i;
		IType ins_type = g->iType();
		switch (ins_type)
		{
			case ret:
				outputFile<<"\t";
				outputFile << "return\n";
				break;

			case ret_t:{
				outputFile<<"\t";
				Instruction_ret_t* g1 = static_cast<Instruction_ret_t*>(g);
				EncodeConstant(g1->returnVar);
				outputFile<<g1->tostring();
				break;
			}

			case br:{
				outputFile<<"\t";
				Instruction_br* g1 = static_cast<Instruction_br*>(g);
				outputFile<<g1->tostring();
				break;
			}

			case br_var:{
				outputFile<<"\t";
				Instruction_br_var* g1 = static_cast<Instruction_br_var*>(g);
				if(Decode(g1->var,outputFile)){
					tempint = newVarNum-1;
					g1->var.labelName = newVarString+to_string(tempint);
				}
				else
					outputFile<<g1->var.labelName;
				outputFile<<"br ";
				outputFile<<g1->var.labelName<<" "<<g1->labelTrue.labelName<<" "<<g1->labelFalse.labelName<<endl;
				break;
			}

			case assign:{
				outputFile<<"\t";
				Instruction_assignment* g1 = static_cast<Instruction_assignment*>(g);
				EncodeConstant(g1->src);
				outputFile<<g1->tostring();
				break;
			}

			case define:{
				outputFile<<"\t";
				Instruction_define* g1 = static_cast<Instruction_define*>(g);
				if(g1->type.labelName == "code"){
					codeMap.insert(g1->var.labelName);
				}
				outputFile<<g1->tostring();
				if(g1->type.labelName.size() > 5 || g1->type.labelName == "tuple"){
					outputFile<<"\t";
					outputFile<<g1->var.labelName<<" <- 0"<<endl;
				}
				break;
			}

			case assign_cmp_aop:{
				Instruction_assignment_cmp_aop* g1 = static_cast<Instruction_assignment_cmp_aop*>(g);
				if(Decode(g1->lc,outputFile)){
					tempint = newVarNum - 1;
					g1->lc.labelName = newVarString+to_string(tempint);
				}
				if(Decode(g1->rc,outputFile)){
					tempint = newVarNum - 1;
					g1->rc.labelName = newVarString+to_string(tempint);
				}
				outputFile<<"\t";
				outputFile<<g1->tostring();
				EncodeVarAssign(g1->left,outputFile);
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
					EncodeConstant(*v1);
				}
				outputFile<<g1->tostring();
				break;

			}

			case assign_call:{
				Instruction_assign_call* g1 = static_cast<Instruction_assign_call*>(g);
				outputFile<<"\t";
				for(vector<Item>::iterator v1 = g1->arguments.begin();v1 != g1->arguments.end(); ++v1){
					EncodeConstant(*v1);
				}
				outputFile<<g1->tostring();
				break;
			}

			case new_array:{
				outputFile<<"\t";
				Instruction_new_array* g1 = static_cast<Instruction_new_array*>(g);
				for(vector<Item>::iterator v1 = g1->arguments.begin();v1 != g1->arguments.end(); v1++){
					EncodeConstant(*v1);
				}
				outputFile<<g1->tostring();
				break;
			}

			case array_dest:{
				Instruction_array_dest* g1 = static_cast<Instruction_array_dest*>(g);
				outputFile<<"\t";
				DeclarNewVar(outputFile);
				outputFile<<"\t";
				outputFile<<newVarString<<newVarNum<<" <- "<<g1->var.labelName<<" = 0"<<endl;
				outputFile<<"\t";
				outputFile<<"br "<<newVarString<<newVarNum<<" :newLabel"<<newLabelNum<<"F :newLabel"<<newLabelNum<<"T"<<endl;
				outputFile<<"\t";
				outputFile<<":newLabel"<<newLabelNum<<"F"<<endl;
				outputFile<<"\t";
				outputFile<<"array-error(0,0)"<<endl;
				outputFile<<"\t";
				outputFile<<":newLabel"<<newLabelNum<<"T"<<endl;
				newLabelNum++;
				newVarNum++;
				if(g1->dimensions.size()>1){
					int numOfDimension = 0;
					for(vector<Item>::iterator v1 = g1->dimensions.begin();v1 != g1->dimensions.end(); v1++){
						DeclarNewVar(outputFile);
						outputFile<<"\t";
						outputFile<<newVarString<<newVarNum<<" <- length "<<g1->var.labelName<<" "<<numOfDimension<<endl;
						outputFile<<"\t";
						outputFile<<newVarString<<newVarNum<<" <- "<<newVarString<<newVarNum<<" >> 1"<<endl;
						newVarNum++;
						DeclarNewVar(outputFile);
						if(v1->isANumber){
							outputFile<<"\t";
							outputFile<<newVarString<<newVarNum<<" <- "<<v1->labelName<<endl;
						}
						else{
							outputFile<<"\t";
							outputFile<<newVarString<<newVarNum<<" <- "<<v1->labelName<<" >> 1"<<endl;
						}
						newVarNum++;
						DeclarNewVar(outputFile);
						outputFile<<"\t";
						outputFile<<newVarString<<newVarNum<<" <- "<<newVarString<<newVarNum-1<<" < "<<newVarString<<newVarNum-2<<endl;
						outputFile<<"\t";
						outputFile<<"br "<<newVarString<<newVarNum<<" :newLabel"<<newLabelNum<<"T :newLabel"<<newLabelNum<<"F"<<endl;
						outputFile<<"\t";
						outputFile<<":newLabel"<<newLabelNum<<"F"<<endl;
						outputFile<<"\t";
						outputFile<<newVarString<<newVarNum-1<<" <- "<<newVarString<<newVarNum-1<<" << 1"<<endl;
						outputFile<<"\t";
						outputFile<<newVarString<<newVarNum-1<<" <- "<<newVarString<<newVarNum-1<<" + 1"<<endl;
						outputFile<<"\t";
						outputFile<<"array-error("<<g1->var.labelName<<","<<newVarString<<newVarNum-1<<")"<<endl;
						outputFile<<"\t";
						outputFile<<":newLabel"<<newLabelNum<<"T"<<endl;
						newVarNum++;
						newLabelNum++;
						numOfDimension++;
					}
				}
				for(vector<Item>::iterator v1 = g1->dimensions.begin();v1 != g1->dimensions.end(); v1++){
					if(Decode(*v1,outputFile)){
						tempint = newVarNum - 1;
						v1->labelName = newVarString+to_string(tempint);
					}
				}
				EncodeConstant(g1->src);
				outputFile<<"\t";
				outputFile<<g1->tostring();
				break;
			}

			case array_src:{
				Instruction_array_src* g1 = static_cast<Instruction_array_src*>(g);
				DeclarNewVar(outputFile);
				outputFile<<"\t";
				outputFile<<newVarString<<newVarNum<<" <- "<<g1->var.labelName<<" = 0"<<endl;
				outputFile<<"\t";
				outputFile<<"br "<<newVarString<<newVarNum<<" :newLabel"<<newLabelNum<<"F :newLabel"<<newLabelNum<<"T"<<endl;
				outputFile<<"\t";
				outputFile<<":newLabel"<<newLabelNum<<"F"<<endl;
				outputFile<<"\t";
				outputFile<<"array-error(0,0)"<<endl;
				outputFile<<"\t";
				outputFile<<":newLabel"<<newLabelNum<<"T"<<endl;
				newLabelNum++;
				newVarNum++;
				if(g1->dimensions.size()>1){
					int numOfDimension = 0;
					for(vector<Item>::iterator v1 = g1->dimensions.begin();v1 != g1->dimensions.end(); v1++){
						DeclarNewVar(outputFile);
						outputFile<<"\t";
						outputFile<<newVarString<<newVarNum<<" <- length "<<g1->var.labelName<<" "<<numOfDimension<<endl;
						outputFile<<"\t";
						outputFile<<newVarString<<newVarNum<<" <- "<<newVarString<<newVarNum<<" >> 1"<<endl;
						newVarNum++;
						DeclarNewVar(outputFile);
						if(v1->isANumber){
							outputFile<<"\t";
							outputFile<<newVarString<<newVarNum<<" <- "<<v1->labelName<<endl;
						}
						else{
							outputFile<<"\t";
							outputFile<<newVarString<<newVarNum<<" <- "<<v1->labelName<<" >> 1"<<endl;
						}
						newVarNum++;
						DeclarNewVar(outputFile);
						outputFile<<"\t";
						outputFile<<newVarString<<newVarNum<<" <- "<<newVarString<<newVarNum-1<<" < "<<newVarString<<newVarNum-2<<endl;
						outputFile<<"\t";
						outputFile<<"br "<<newVarString<<newVarNum<<" :newLabel"<<newLabelNum<<"T :newLabel"<<newLabelNum<<"F"<<endl;
						outputFile<<"\t";
						outputFile<<":newLabel"<<newLabelNum<<"F"<<endl;
						outputFile<<"\t";
						outputFile<<newVarString<<newVarNum-1<<" <- "<<newVarString<<newVarNum-1<<" << 1"<<endl;
						outputFile<<"\t";
						outputFile<<newVarString<<newVarNum-1<<" <- "<<newVarString<<newVarNum-1<<" + 1"<<endl;
						outputFile<<"\t";
						outputFile<<"array-error("<<g1->var.labelName<<","<<newVarString<<newVarNum-1<<")"<<endl;
						outputFile<<"\t";
						outputFile<<":newLabel"<<newLabelNum<<"T"<<endl;
						newVarNum++;
						newLabelNum++;
						numOfDimension++;
					}
				}
				for(vector<Item>::iterator v1 = g1->dimensions.begin();v1 != g1->dimensions.end(); v1++){
					if(Decode(*v1,outputFile)){
						tempint = newVarNum - 1;
						v1->labelName = newVarString+to_string(tempint);
					}
				}
				outputFile<<g1->tostring();
				break;
			}

			case new_tuple:{
				Instruction_new_tuple* g1 = static_cast<Instruction_new_tuple*>(g);
				outputFile<<"\t";
				EncodeConstant(g1->arg);
				outputFile<<g1->tostring();
				break;
			}

			case length:{
				Instruction_length* g1 = static_cast<Instruction_length*>(g);
				outputFile<<"\t";
				if(Decode(g1->arg,outputFile)){
					tempint = newVarNum-1;
					g1->arg.labelName = newVarString+to_string(tempint);
				}
				outputFile<<g1->tostring();
				break;
			}
		}
	}// For loop end*/
}


	void GenerateString(Instruction* g,ofstream& outputFile){
		IType ins_type = g->iType();
		// cout<<ins_type<<endl;
		switch (ins_type)
		{
			case ret:
				outputFile<<"\t";
				outputFile << "return\n";
				break;

			case ret_t:{
				outputFile<<"\t";
				Instruction_ret_t* g1 = static_cast<Instruction_ret_t*>(g);
				AddPercent(g1->returnVar);
				outputFile<<g1->tostring();
				break;
			}

			case br:{
				outputFile<<"\t";
				Instruction_br* g1 = static_cast<Instruction_br*>(g);
				outputFile<<g1->tostring();
				break;
			}

			case br_var:{
				outputFile<<"\t";
				Instruction_br_var* g1 = static_cast<Instruction_br_var*>(g);
				AddPercent(g1->var);
				outputFile<<"br "<<g1->var.labelName<<" "<<g1->labelTrue.labelName<<" "<<g1->labelFalse.labelName<<endl;
				break;
			}

			case assign:{
				outputFile<<"\t";
				Instruction_assignment* g1 = static_cast<Instruction_assignment*>(g);
				AddPercent(g1->src);
				AddPercent(g1->dst);
				outputFile<<g1->tostring();
				break;
			}

			case define:{
				outputFile<<"\t";
				Instruction_define* g1 = static_cast<Instruction_define*>(g);
				AddPercent(g1->var);
				outputFile<<g1->tostring();
				break;
			}

			case assign_cmp_aop:{
				outputFile<<"\t";
				Instruction_assignment_cmp_aop* g1 = static_cast<Instruction_assignment_cmp_aop*>(g);
				AddPercent(g1->lc);
				AddPercent(g1->rc);
				AddPercent(g1->left);
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
				for(vector<Item>::iterator v1 = g1->arguments.begin(); v1 != g1->arguments.end(); ++v1){
					AddPercent(*v1);
				}
				if(codeMap.find(g1->fname.labelName) != codeMap.end()){
					string s = "call %";
				    s += g1->fname.labelName;
				    s+=" (";
				    for(auto i : g1->arguments){
				      s+=i.labelName;
				      s+=",";
				    }
				    if(g1->arguments.size()>0)
				      s.pop_back();
				    s+=")";
				    outputFile<<s<<endl;
				}
				else
					outputFile<<g1->tostringIR();
				break;

			}

			case assign_call:{
				Instruction_assign_call* g1 = static_cast<Instruction_assign_call*>(g);
				outputFile<<"\t";
				AddPercent(g1->var);
				for(vector<Item>::iterator v1 = g1->arguments.begin(); v1 != g1->arguments.end(); ++v1){
					AddPercent(*v1);
				}

				if(codeMap.find(g1->fname.labelName) != codeMap.end()){
					string s = g1->var.labelName + " <- ";
				    s += "call ";
				    s += "%";
				    s += g1->fname.labelName + " (";;
				    for(auto i : g1->arguments){
				      s+=i.labelName;
				      s+=",";
				    }
				    if(g1->arguments.size()>0)
				      s.pop_back();
				    s+=")";
				    outputFile<<s<<endl;
				}
				else
					outputFile<<g1->tostringIR();
				break;
			}

			case new_array:{
				outputFile<<"\t";
				Instruction_new_array* g1 = static_cast<Instruction_new_array*>(g);
				AddPercent(g1->var);
				for(vector<Item>::iterator v1 = g1->arguments.begin();v1 != g1->arguments.end(); v1++){
					AddPercent(*v1);
				}
				outputFile<<g1->tostring();
				break;
			}

			case array_dest:{
				Instruction_array_dest* g1 = static_cast<Instruction_array_dest*>(g);
				AddPercent(g1->var);
				AddPercent(g1->src);
				for(vector<Item>::iterator v1 = g1->dimensions.begin();v1 != g1->dimensions.end(); v1++){
					AddPercent(*v1);
				}
				outputFile<<"\t";
				outputFile<<g1->tostring();
				break;
			}

			case array_src:{
				Instruction_array_src* g1 = static_cast<Instruction_array_src*>(g);
				AddPercent(g1->var);
				AddPercent(g1->dest);
				for(vector<Item>::iterator v1 = g1->dimensions.begin();v1 != g1->dimensions.end(); v1++){
					AddPercent(*v1);
				}
				outputFile<<"\t";
				outputFile<<g1->tostring();
				break;
			}

			case new_tuple:{
				Instruction_new_tuple* g1 = static_cast<Instruction_new_tuple*>(g);
				outputFile<<"\t";
				AddPercent(g1->var);
				AddPercent(g1->arg);
				outputFile<<"\t";
				outputFile<<g1->tostring();
				break;
			}

			case length:{
				Instruction_length* g1 = static_cast<Instruction_length*>(g);
				outputFile<<"\t";
				AddPercent(g1->arg);
				AddPercent(g1->dest);
				AddPercent(g1->src);
				outputFile<<g1->tostring();
				break;
			}
		}

	}

	void GenerateBasicBlocks(Function* entryFun, ofstream& outputFile){
		bool startBB = true;
		for(auto i : entryFun->instructions){
			IType ins_type = i->iType();
			if(startBB){
				if(ins_type != labelType){
					outputFile<<"\t";
					outputFile<<":newLabel"<<newLabelNum<<endl;
					newLabelNum++;
				}
				startBB = false;
			}
			else if(ins_type == labelType){
				Instruction_label* g1 = static_cast<Instruction_label*>(i);
				outputFile<<"\t";
				outputFile<<"br "<<g1->label.labelName<<endl;
			}
			GenerateString(i,outputFile);
			if(ins_type == ret || ins_type == ret_t || ins_type == br || ins_type == br_var)
				startBB = true;
		}
	}
    

  void generate_code(Program p){

	std::ofstream outputFile;
	outputFile.open("temp.IR");
	string entry = p.entryPointLabel;
	vector<Instruction*> newInstList;

	for (vector<Function *>::iterator i = p.functions.begin();i!=p.functions.end();i++){
		auto g = *i;
		GenerateInstructions(g,outputFile);
		outputFile<<"}\n\n";
	}
	outputFile.close();

	Program P1 = parse_file((char*)"temp.IR");
    
    outputFile.open("prog.IR");
    for (vector<Function *>::iterator i = P1.functions.begin();i!=P1.functions.end();i++){
		auto g = *i;
		// cout<<"here"<<endl;
		GenerateFunctionEntry(g,outputFile);
		GenerateBasicBlocks(g,outputFile);
		outputFile<<"}\n\n";
	}


   return ;
}

}
