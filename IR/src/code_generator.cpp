#include <string>
#include <iostream>
#include <fstream>

#include <code_generator.h>
#include <parser.h>
#include <map>
#include <vector>
#include <time.h>
#include <set>

using namespace std;



namespace IR{

	struct BasicBlock{
		vector<Instruction*> instructions;
		string entryLabel;
		vector<int> successors;
		BasicBlock():instructions(){};
	};

	set<BasicBlock*> markedBasicBlocks;
	vector<string> arguments{"rdi","rsi","rdx","rcx","r8","r9"};
	set<string> tupleMap;


  void GenerateBasicBlocks(vector<BasicBlock*> &BasicBlocks,Function* entryFun,map<string,int> &labels){
  	BasicBlock* currentBlock = new BasicBlock();
  	for(vector<Instruction *>::iterator i = entryFun->instructions.begin(); i != entryFun->instructions.end(); i++){
  		auto g = *i;
  		IType ins_type = g->iType();
  		switch (ins_type)
  		{
  			case labelType:{
  				if(currentBlock->instructions.size()>0)
  					BasicBlocks.push_back(currentBlock);
  				currentBlock = new BasicBlock();
  				currentBlock->instructions.push_back(g);
  				Instruction_label* g1 = static_cast<Instruction_label*>(g);
  				currentBlock->entryLabel = g1->label.labelName;
  			}

  			break;

  			default:{
  				currentBlock->instructions.push_back(g);
  			}

  			break;

  		}
  	}

  	if(currentBlock->instructions.size()>0){
  		BasicBlocks.push_back(currentBlock);
  	}

  }

	void Generate_label_index(vector<BasicBlock*> &BasicBlocks, map<string,int> &labels){

	  	for(int i = 0;i<BasicBlocks.size();i++){
	  		labels.insert(pair<string,int>(BasicBlocks[i]->entryLabel,i));
	  		
	  	}
	}


	void GenerateBasicBlocksSuccessors(vector<BasicBlock*> &BasicBlocks, map<string,int> &labels){
		for(int i = 0; i<BasicBlocks.size();i++){
			int last = (BasicBlocks[i]->instructions.size())-1;
			Instruction* lastIns = BasicBlocks[i]->instructions[last];
			IType ins_type = lastIns->iType();
  			switch (ins_type)
  			{
  				case br:{
  					Instruction_br* g1 = static_cast<Instruction_br*>(lastIns);
  					BasicBlocks[i]->successors.push_back(labels[g1->label.labelName]);
  					break;
  				}

  				case br_var:{
  					Instruction_br_var* g1 = static_cast<Instruction_br_var*>(lastIns);
  					BasicBlocks[i]->successors.push_back(labels[g1->labelTrue.labelName]);
  					BasicBlocks[i]->successors.push_back(labels[g1->labelFalse.labelName]);
  					break;
  				}
  			}
		}
	}


  void GenerateFunctionEntry(Function* entryFun,ofstream& outputFile){
  	outputFile<<"define "<<entryFun->name<<" (";
  	int j;
  	string s = "";
  	if(entryFun->arguments.size()>0){
	  	for(j=0 ;j<entryFun->arguments.size();j++){
	  		s = s + entryFun->arguments[j].labelName + ",";
	  		if(entryFun->argumentsType[j].labelName == "tuple"){
					tupleMap.insert(entryFun->arguments[j].labelName);
			}
	  	}
	  	s.pop_back();
	  	outputFile<<s;
  	}
  	outputFile<<"){"<<endl;
  	return;

  }

  BasicBlock* FetchAndRemove(vector<BasicBlock*> &BasicBlocks){
  	BasicBlock* returnBlock = BasicBlocks.front();
  	BasicBlocks.erase(BasicBlocks.begin());
  	return returnBlock;
  }

  void Mark(BasicBlock* bb){
  	markedBasicBlocks.insert(bb);
  }

  bool CheckNotMarked(BasicBlock* bb){
  	set<BasicBlock*>::iterator it = markedBasicBlocks.find(bb);
  	if(it != markedBasicBlocks.end())
  		return false;
  	else
  		return true;
  }

  bool Profitable(BasicBlock* bb, BasicBlock* c){
  	return true;
  }

  void GenerateInstructions(Function* entryFun,ofstream& outputFile){
  	string tempstr,tempstr1,tempstr2;
	int tempint,tempint1;
	vector<BasicBlock*> BasicBlocks;
	std::map<string, int> labels;
	GenerateBasicBlocks(BasicBlocks,entryFun,labels);
	Generate_label_index(BasicBlocks,labels);
	GenerateBasicBlocksSuccessors(BasicBlocks,labels);
	tempint = 0;
	vector<BasicBlock*> trace_list;
	BasicBlock* tempBasicBlock;
	vector<int> tempSucc;
	vector<BasicBlock*> originalBasicBlocks;
	originalBasicBlocks.insert(originalBasicBlocks.begin(),BasicBlocks.begin(),BasicBlocks.end());

	do{
		BasicBlock* trace = new BasicBlock();
		tempBasicBlock = FetchAndRemove(BasicBlocks);
		while(CheckNotMarked(tempBasicBlock)){
			Mark(tempBasicBlock);
			trace->instructions.insert(trace->instructions.end(),tempBasicBlock->instructions.begin(),tempBasicBlock->instructions.end());
			tempSucc = tempBasicBlock->successors;
			for(auto k : tempSucc){
				if(CheckNotMarked(originalBasicBlocks[k]) && Profitable(tempBasicBlock,originalBasicBlocks[k])){
					tempBasicBlock = originalBasicBlocks[k];
					break;
				}
			}
		}
		if(trace->instructions.size() > 0){
			trace_list.push_back(trace);
		}
	}while(BasicBlocks.size()>0);

	tempint = 0;
	vector<Instruction*> translate;
	for(vector<BasicBlock*>::iterator i = trace_list.begin(); i != trace_list.end(); i++){
		for(vector<Instruction *>::iterator j = (*i)->instructions.begin(); j != (*i)->instructions.end(); j++){
			auto g = *j;
			translate.push_back(g);
		}
		tempint++;
	}

	int numOfCalls = 0;
	int newVarNum = 0;
	GenerateFunctionEntry(entryFun,outputFile);
	string function_name = entryFun->name;
	for(vector<Instruction *>::iterator i = translate.begin(); i != translate.end(); i++){
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
				outputFile<<"br "<<g1->var.labelName<<" "<<g1->labelTrue.labelName<<endl;
				outputFile<<"\t";
				outputFile<<"br "<<g1->labelFalse.labelName<<endl;
				break;
			}

			case assign:{
				outputFile<<"\t";
				Instruction_assignment* g1 = static_cast<Instruction_assignment*>(g);
				outputFile<<g1->tostring();
				break;
			}

			case define:{
				Instruction_define* g1 = static_cast<Instruction_define*>(g);
				if(g1->type.labelName == "tuple"){
					tupleMap.insert(g1->var.labelName);
				}
				break;
			}

			case assign_cmp_aop:{
				outputFile<<"\t";
				Instruction_assignment_cmp_aop* g1 = static_cast<Instruction_assignment_cmp_aop*>(g);
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
				outputFile<<g1->tostring();
				break;

			}

			case assign_call:{
				Instruction_assign_call* g1 = static_cast<Instruction_assign_call*>(g);
				outputFile<<"\t";
				outputFile<<g1->tostring();
				break;
			}

			case new_array:{
				Instruction_new_array* g1 = static_cast<Instruction_new_array*>(g);
				int mainVar = newVarNum;
				newVarNum++;
				// Multiplying all arguments
				for(tempint1=0;tempint1<g1->arguments.size();tempint1++){
					outputFile<<"\t";
					outputFile<< "%newVar" << newVarNum <<" <- "<< g1->arguments[tempint1].labelName << " >> 1"<< endl;
					outputFile<<"\t";
					if(tempint1 == 0)
						outputFile<< "%newVar" << mainVar <<" <- %newVar" << newVarNum<< endl;
					else
						outputFile<< "%newVar" << mainVar <<" <- %newVar" << mainVar << " * %newVar" << newVarNum <<endl;
					newVarNum++;
				}
				outputFile<<"\t";
				// Encode multiplication result
				outputFile<<"%newVar"<<mainVar<<" <- %newVar"<<mainVar<<" << 1"<<endl;
				outputFile<<"\t";
				outputFile<<"%newVar"<<mainVar<<" <- %newVar"<<mainVar<<" + 1"<<endl;
				outputFile<<"\t";
				outputFile<<"%newVar"<<mainVar<<" <- %newVar"<<mainVar<<" + "<<to_string(2+g1->arguments.size()*2)<<endl;
				outputFile<<"\t";
				outputFile<<g1->var.labelName<<" <- call allocate (%newVar"<<mainVar<<", 1)"<<endl;
				tempint1 = g1->arguments.size() << 1;
				tempint1 += 1;
				outputFile<<"\t";
				outputFile<<"%newVar"<<newVarNum<<" <- "<<g1->var.labelName<<" + 8"<<endl;
				outputFile<<"\t";
				outputFile<<"store %newVar"<<newVarNum<<" <- "<<tempint1<<endl;
				newVarNum++;
				tempint = 16;
				for(tempint1=0;tempint1<g1->arguments.size();tempint1++){
					outputFile<<"\t";
					outputFile<<"%newVar"<<newVarNum<<" <- "<<g1->var.labelName<<" + "<<tempint<<endl;	
					outputFile<<"\t";
					outputFile<<"store %newVar"<<newVarNum<<" <- "<<g1->arguments[tempint1].labelName<<endl;
					newVarNum++;
					tempint+=8;
				}			
				break;
			}

			case array_dest:{
				Instruction_array_dest* g1 = static_cast<Instruction_array_dest*>(g);
				vector<int> tempIndexNums;
				set<string>::iterator tempiter;
				tempiter = tupleMap.find(g1->var.labelName);
				if(tempiter != tupleMap.end()){
					outputFile<<"\t";
					outputFile<<"%newVar"<<newVarNum<<" <- "<<g1->dimensions[0].labelName<<" * 8"<<endl;
					outputFile<<"\t";
					outputFile<<"%newVar"<<newVarNum<<" <- %newVar"<<newVarNum<<" + 8"<<endl;
					outputFile<<"\t";
					outputFile<<"%newVar"<<newVarNum<<" <- %newVar"<<newVarNum<<" + "<<g1->var.labelName<<endl;
					outputFile<<"\t";
					outputFile<<"store %newVar"<<newVarNum<<" <- "<<g1->src.labelName<<endl;
					newVarNum++;
					break;
				}
				tempint = 24;
				// Loading lengths in variables
				for(tempint1 = 1; tempint1 < g1->dimensions.size(); tempint1++){
					outputFile<<"\t";
					outputFile<<"%newVar"<<newVarNum<<" <- "<<g1->var.labelName<<" + "<<tempint<<endl;
					outputFile<<"\t";
					newVarNum++;
					outputFile<<"%newVar"<<newVarNum<<"<- load %newVar"<<newVarNum-1<<endl;
					outputFile<<"\t";
					newVarNum++;
					outputFile<<"%newVar"<<newVarNum<<"<- %newVar"<<newVarNum-1<<" >> 1"<<endl;
					tempIndexNums.push_back(newVarNum);
					tempint += 8;
					newVarNum++;
				}
				tempint = 0;
				int currentDimension = g1->dimensions.size()-1;
				vector<int> offsetIndexes;
				// Calculate offset for every index
				for(tempint1 = 0; tempint1 < g1->dimensions.size(); tempint1++){
					outputFile<<"\t";
					outputFile<<"%newVar"<<newVarNum<<" <- 1"<<endl;
					for(int k=tempint;k<tempIndexNums.size();k++){
						outputFile<<"\t";
						outputFile<<"%newVar"<<newVarNum<<" <- %newVar"<<newVarNum << " * %newVar"<<tempIndexNums[k]<<endl;
					}
					outputFile<<"\t";
					outputFile<<"%newVar"<<newVarNum<<" <- %newVar"<<newVarNum<<" * "<<g1->dimensions[tempint].labelName<<endl;
					tempint++;
					offsetIndexes.push_back(newVarNum);
					newVarNum++;
				}

				outputFile<<"\t";
				outputFile<<"%newVar"<<newVarNum<<" <- 0"<<endl;

				// Calculating total offset
				for(auto k : offsetIndexes){
					outputFile<<"\t";
					outputFile<<"%newVar"<<newVarNum<<" <- %newVar"<<newVarNum<< " + %newVar"<<k<<endl;
				}

				outputFile<<"\t";
				outputFile<<"%newVar"<<newVarNum<<" <- %newVar"<<newVarNum<< " * 8"<<endl;
				outputFile<<"\t";
				outputFile<<"%newVar"<<newVarNum<<" <- %newVar"<<newVarNum<< " + "<<(16 + g1->dimensions.size() * 8)<<endl;
				newVarNum++;
				outputFile<<"\t";
				outputFile<<"%newVar"<<newVarNum<<" <- "<<g1->var.labelName<<" + %newVar"<<newVarNum-1<<endl;
				outputFile<<"\t";
				outputFile<<"store %newVar"<<newVarNum<<" <- "<<g1->src.labelName<<endl;
				newVarNum++;
				break;
			}

			case array_src:{
				Instruction_array_src* g1 = static_cast<Instruction_array_src*>(g);
				vector<int> tempIndexNums;
				set<string>::iterator tempiter;
				tempiter = tupleMap.find(g1->var.labelName);
				if(tempiter != tupleMap.end()){
					outputFile<<"\t";
					outputFile<<"%newVar"<<newVarNum<<" <- "<<g1->dimensions[0].labelName<<" * 8"<<endl;
					outputFile<<"\t";
					outputFile<<"%newVar"<<newVarNum<<" <- %newVar"<<newVarNum<<" + 8"<<endl;
					outputFile<<"\t";
					outputFile<<"%newVar"<<newVarNum<<" <- %newVar"<<newVarNum<<" + "<<g1->var.labelName<<endl;
					outputFile<<"\t";
					outputFile<<g1->dest.labelName<<" <- load %newVar"<<newVarNum<<endl;
					newVarNum++;
					break;
				}
				tempint = 24;
				for(tempint1 = 1; tempint1 < g1->dimensions.size(); tempint1++){
					outputFile<<"\t";
					outputFile<<"%newVar"<<newVarNum<<" <- "<<g1->var.labelName<<" + "<<tempint<<endl;
					outputFile<<"\t";
					newVarNum++;
					outputFile<<"%newVar"<<newVarNum<<"<- load %newVar"<<newVarNum-1<<endl;
					outputFile<<"\t";
					newVarNum++;
					outputFile<<"%newVar"<<newVarNum<<"<- %newVar"<<newVarNum-1<<" >> 1"<<endl;
					tempIndexNums.push_back(newVarNum);
					tempint += 8;
					newVarNum++;
				}
				tempint = 0;
				int currentDimension = g1->dimensions.size()-1;
				vector<int> offsetIndexes;

				for(tempint1 = 0; tempint1 < g1->dimensions.size(); tempint1++){
					outputFile<<"\t";
					outputFile<<"%newVar"<<newVarNum<<" <- 1"<<endl;
					for(int k=tempint;k<tempIndexNums.size();k++){
						outputFile<<"\t";
						outputFile<<"%newVar"<<newVarNum<<" <- %newVar"<<newVarNum << " * %newVar"<<tempIndexNums[k]<<endl;
					}
					outputFile<<"\t";
					outputFile<<"%newVar"<<newVarNum<<" <- %newVar"<<newVarNum<<" * "<<g1->dimensions[tempint].labelName<<endl;
					tempint++;
					offsetIndexes.push_back(newVarNum);
					newVarNum++;
				}

				outputFile<<"\t";
				outputFile<<"%newVar"<<newVarNum<<" <- 0"<<endl;

				for(auto k : offsetIndexes){
					outputFile<<"\t";
					outputFile<<"%newVar"<<newVarNum<<" <- %newVar"<<newVarNum<< " + %newVar"<<k<<endl;
				}

				outputFile<<"\t";
				outputFile<<"%newVar"<<newVarNum<<" <- %newVar"<<newVarNum<< " * 8"<<endl;
				outputFile<<"\t";
				outputFile<<"%newVar"<<newVarNum<<" <- %newVar"<<newVarNum<< " + "<<(16 + g1->dimensions.size() * 8)<<endl;
				newVarNum++;
				outputFile<<"\t";
				outputFile<<"%newVar"<<newVarNum<<" <- "<<g1->var.labelName<<" + %newVar"<<newVarNum-1<<endl;
				outputFile<<"\t";
				outputFile<<g1->dest.labelName<<" <- load %newVar"<<newVarNum<<endl;
				newVarNum++;
				break;
			}

			case new_tuple:{
				Instruction_new_tuple* g1 = static_cast<Instruction_new_tuple*>(g);
				outputFile<<"\t";
				outputFile<<g1->var.labelName<<" <- call allocate("<<g1->arg.labelName<<",1)"<<endl;
				break;
			}

			case length:{
				Instruction_length* g1 = static_cast<Instruction_length*>(g);
				outputFile<<"\t";
				outputFile<<"%newVar"<<newVarNum<<" <- "<<g1->arg.labelName<<" * 8"<<endl;
				newVarNum++;
				outputFile<<"\t";
				outputFile<<"%newVar"<<newVarNum<<" <- %newVar"<<newVarNum-1<<" + 16"<<endl;
				newVarNum++;
				outputFile<<"\t";
				outputFile<<"%newVar"<<newVarNum<<" <- "<<g1->src.labelName<<" + %newVar"<<newVarNum-1<<endl;
				outputFile<<"\t";
				outputFile<<g1->dest.labelName<<" <- load %newVar"<<newVarNum<<endl;
				newVarNum++;
				break;
			}
		}
	}// For loop end*/
	markedBasicBlocks.clear();
}
    

  void generate_code(Program p){

	std::ofstream outputFile;
	outputFile.open("prog.L3");

	for (vector<Function *>::iterator i = p.functions.begin();i!=p.functions.end();i++){
		auto g = *i;
		GenerateInstructions(g,outputFile);
		tupleMap.clear();
		outputFile<<"}\n\n";
	}
    outputFile.close();
   return ;
}

}
