#include <string>
#include <iostream>
#include <fstream>

#include <code_generator.h>
#include <interference.h>
#include <graphcolor.h>
#include <parser.h>
#include <spiller.h>
#include <map>
#include <vector>
#include <time.h>

using namespace std;



namespace L2{

	vector<string> SortedGPRegistersA{"r10","r11","r8","r9","rax","rcx","rdi","rdx","rsi","r12","r13","r14","r15","rbp","rbx"};
	string enumToString[17] = {"rdi", "rax", "rbx", "rbp", "r10", "r11", "r12", "r13", "r14", "r15", "rsi", "r8", "r9", "rcx", "rdx", "rsp"};

  string removeColon(string a){
	a[0] = '_';
	return a;
  }

  string removeFirstChar(string a){
	return a.substr(1);
  }

  string GenerateRegister(int r){
  	return enumToString[r];
  }



  void GenerateFunctionEntry(Function* entryFun,ofstream& outputFile,int local){
  	outputFile<<"("<<entryFun->name<<endl;
  	outputFile<<"\t";
  	outputFile<<entryFun->arguments<<" "<<local<<endl;
  	return;

  }

  void GenerateInstructions(Function* entryFun,ofstream& outputFile){
  	string tempstr,tempstr1,tempstr2;
	int tempint,tempint1;
	bool spill = false;
	map<string,int> colorMap;
	int local = 0;
	Item nvar,svar;
	Program p;
	nvar.labelName = "";
	svar.labelName = "";
	spiller(Program_spill(entryFun,svar,nvar),local);

	do{
		p = parse_function_file((char*)"tempSpillFile.L2");
		entryFun = p.functions.back();
		map<string,unordered_set<string>> graph;
		graph = interference_graph(entryFun);
		pair<vector<string>,map<string,int>> GraphColorResult(graphcolor(graph));
		if(GraphColorResult.first.size() == 0){
			colorMap = GraphColorResult.second;
			spill = false;
		}
		else{
			spill = true;
			for(string k : GraphColorResult.first){
				p = parse_function_file((char*)"tempSpillFile.L2");
				entryFun = p.functions.back();
				nvar.labelName = "%"+k+k+"0000";
				svar.labelName = "%"+k;
				nvar.isAVar = true;
				svar.isAVar = true;
				tempint = spiller(Program_spill(entryFun,svar,nvar),local);
				if(tempint != 0)
					local += 1;
			}
		}
	}while(spill);
	

	GenerateFunctionEntry(entryFun,outputFile,local);
	for(vector<Instruction *>::iterator i = entryFun->instructions.begin(); i != entryFun->instructions.end(); i++){
		auto g = *i;
		IType ins_type = g->iType();
		switch (ins_type)
		{
			case ret:
				outputFile<<"\t";
				outputFile << "return\n";
				break;


			case assign_cmp:{
				outputFile<<"\t";
				Instruction_assignment_cmp* g1 = static_cast<Instruction_assignment_cmp*>(g);
				if(g1->left.isAVar)
					outputFile<<SortedGPRegistersA[colorMap[removeFirstChar(g1->left.labelName)]];
				else
					outputFile<<g1->left.labelName;

				outputFile<<" <- ";
				
				if(g1->lc.isAVar)
					outputFile<<SortedGPRegistersA[colorMap[removeFirstChar(g1->lc.labelName)]];
				else
					outputFile<<g1->lc.labelName;

				outputFile<<" "<< g1->operation.labelName <<" ";

				if(g1->rc.isAVar)
					outputFile<<SortedGPRegistersA[colorMap[removeFirstChar(g1->rc.labelName)]];
				else
					outputFile<<g1->rc.labelName;

				outputFile<<endl;

			}
				break;


			case assign:{
				outputFile<<"\t";
				Instruction_assignment* g1 = static_cast<Instruction_assignment*>(g);

				if(g1->dst.isAVar)
					outputFile<<SortedGPRegistersA[colorMap[removeFirstChar(g1->dst.labelName)]];
				else
					outputFile<<g1->dst.labelName;

				outputFile<<" <- ";

				if(g1->src.isAVar)
					outputFile<<SortedGPRegistersA[colorMap[removeFirstChar(g1->src.labelName)]];
				else
					outputFile<<g1->src.labelName;

				outputFile<<endl;
			}
				break;


			case assign_mem:{
				outputFile<<"\t";
				instruction_assignment_mem_x_M* g1 = static_cast<instruction_assignment_mem_x_M*>(g);
				if(g1->isSrc){

					if(g1->other.isAVar)
						outputFile<<SortedGPRegistersA[colorMap[removeFirstChar(g1->other.labelName)]];
					else
						outputFile<<g1->other.labelName;

					outputFile<<" <- ";

					outputFile<<"mem ";
					if(g1->x.isAVar)
						outputFile<<SortedGPRegistersA[colorMap[removeFirstChar(g1->x.labelName)]];
					else
						outputFile<<g1->x.labelName;

					outputFile<<" "<<g1->M.labelName;

				}
				else{
					outputFile<<"mem ";
					if(g1->x.isAVar)
						outputFile<<SortedGPRegistersA[colorMap[removeFirstChar(g1->x.labelName)]];
					else
						outputFile<<g1->x.labelName;

					outputFile<<" "<<g1->M.labelName;

					outputFile<<" <- ";

					if(g1->other.isAVar)
						outputFile<<SortedGPRegistersA[colorMap[removeFirstChar(g1->other.labelName)]];
					else
						outputFile<<g1->other.labelName;
				}
				
				outputFile<<endl;
			}
				break;


			case aop:{
				outputFile<<"\t";
				instruction_aop* g1 = static_cast<instruction_aop*>(g);

				if(g1->dest.isAVar)
					outputFile<<SortedGPRegistersA[colorMap[removeFirstChar(g1->dest.labelName)]];
				else
					outputFile<<g1->dest.labelName;

				outputFile<<" "<<g1->operation.labelName<<" ";
				if(g1->src.isAVar)
					outputFile<<SortedGPRegistersA[colorMap[removeFirstChar(g1->src.labelName)]];
				else
					outputFile<<g1->src.labelName;

				outputFile<<endl;
			}
				break;


			case sop:{
				outputFile<<"\t";
				instruction_sop* g1 = static_cast<instruction_sop*>(g);	
				if(g1->leftop.isAVar)
					outputFile<<SortedGPRegistersA[colorMap[removeFirstChar(g1->leftop.labelName)]];
				else
					outputFile<<g1->leftop.labelName;

				outputFile<<" "<<g1->operation.labelName<<" ";

				if(g1->rightop.isAVar)
					outputFile<<SortedGPRegistersA[colorMap[removeFirstChar(g1->rightop.labelName)]];
				else
					outputFile<<g1->rightop.labelName;
				
				outputFile<<endl;
			}
				break;


			case aop_mem:{
				outputFile<<"\t";
				instruction_aop_mem_x_M* g1 = static_cast<instruction_aop_mem_x_M*>(g);
				if(g1->isLeft){
					outputFile<<"mem ";
					if(g1->x.isAVar)
						outputFile<<SortedGPRegistersA[colorMap[removeFirstChar(g1->x.labelName)]];
					else
						outputFile<<g1->x.labelName;

					outputFile<<" "<<g1->M.labelName<<" ";
					outputFile<<g1->operation.labelName<<" ";

					if(g1->other.isAVar)
						outputFile<<SortedGPRegistersA[colorMap[removeFirstChar(g1->other.labelName)]];
					else
						outputFile<<g1->other.labelName;
				}
				else{

					if(g1->other.isAVar)
						outputFile<<SortedGPRegistersA[colorMap[removeFirstChar(g1->other.labelName)]];
					else
						outputFile<<g1->other.labelName;

					outputFile<<" "<<g1->operation.labelName<<" ";
					outputFile<<"mem ";

					if(g1->x.isAVar)
						outputFile<<SortedGPRegistersA[colorMap[removeFirstChar(g1->x.labelName)]];
					else
						outputFile<<g1->x.labelName;

					outputFile<<" "<<g1->M.labelName;

					outputFile<<endl;


				}
			}
				break;


			case cjump:{
				outputFile<<"\t";
				Instruction_cjump* g1 = static_cast<Instruction_cjump*>(g);
				outputFile<<"cjump ";
				if(g1->lc.isAVar)
					outputFile<<SortedGPRegistersA[colorMap[removeFirstChar(g1->lc.labelName)]];
				else
					outputFile<<g1->lc.labelName;

				outputFile<<" "<<g1->operation.labelName<<" ";

				if(g1->rc.isAVar)
					outputFile<<SortedGPRegistersA[colorMap[removeFirstChar(g1->rc.labelName)]];
				else
					outputFile<<g1->rc.labelName;

				outputFile<<" "<<g1->label.labelName<<endl;

				// GenerateJmp(g1->lc,g1->rc,g1->operation,g1->label,outputFile);
			}
				break;


			case labelType:{
				outputFile<<"\t";
				Instruction_label* g1 = static_cast<Instruction_label*>(g);
				outputFile << g1->label.labelName << "\n";
			}
				break;


			case goto1:{
				outputFile<<"\t";
				Instruction_goto* g1 = static_cast<Instruction_goto*>(g);
				outputFile<<"goto "<<g1->label.labelName<<endl;
			}
				break;


			case call:{
				outputFile<<"\t";
				Instruction_call* g1 = static_cast<Instruction_call*>(g);
				outputFile<<"call ";
				if(g1->fname.isAVar)
					outputFile<<SortedGPRegistersA[colorMap[removeFirstChar(g1->fname.labelName)]];
				else
					outputFile<<g1->fname.labelName;

				outputFile<<" "<<g1->argn.labelName;
				outputFile<<endl;
			}
				break;


			case print:
				outputFile<<"\t";
				outputFile << "call print 1" << endl;
				break;


			case allocate:
				outputFile<<"\t";
				outputFile << "call allocate 2" <<endl;
				break;


			case array:
				outputFile<<"\t";
				outputFile << "call array-error 2"<<endl;
				break;


			case inc_dec:{
				outputFile<<"\t";
				Instruction_inc_dec* g1 = static_cast<Instruction_inc_dec*>(g);

				if(g1->reg.isAVar)
					outputFile<<SortedGPRegistersA[colorMap[removeFirstChar(g1->reg.labelName)]];
				else
					outputFile<<g1->reg.labelName;

				if(g1->isInc)
					outputFile<<"++";
				else
					outputFile<<"--";

				outputFile<<endl;
			}
				break;

			case lea:{
				outputFile<<"\t";
				Instruction_lea* g1 = static_cast<Instruction_lea*>(g);

				if(g1->first.isAVar)
					outputFile<<SortedGPRegistersA[colorMap[removeFirstChar(g1->first.labelName)]];
				else
					outputFile<<g1->first.labelName;

				outputFile<<" @ ";

				if(g1->second.isAVar)
					outputFile<<SortedGPRegistersA[colorMap[removeFirstChar(g1->second.labelName)]];
				else
					outputFile<<g1->second.labelName;

				outputFile<<" ";

				if(g1->third.isAVar)
					outputFile<<SortedGPRegistersA[colorMap[removeFirstChar(g1->third.labelName)]];
				else
					outputFile<<g1->third.labelName;

				outputFile<<g1->fourth.labelName;

				outputFile<<endl;
			}
				break;

			case stack_arg:{
				outputFile<<"\t";
				Instruction_stack_arg* g1 = static_cast<Instruction_stack_arg*>(g);

				if(g1->left.isAVar)
					outputFile<<SortedGPRegistersA[colorMap[removeFirstChar(g1->left.labelName)]];
				else
					outputFile<<g1->left.labelName;

				outputFile<<" <- mem rsp ";

				outputFile<<local*8+stoll(g1->M.labelName)<<endl;

			}
				break;
		}
	}// For loop end
}
    

  void generate_code(Program p){

	std::ofstream outputFile;
	outputFile.open("prog.L1");
	string entry = p.entryPointLabel;
	outputFile<<"("<<entry<<endl;

	for (vector<Function *>::iterator i = p.functions.begin();i!=p.functions.end();i++){
		auto g = *i;
		GenerateInstructions(g,outputFile);
		outputFile<<")\n\n";
	}
	outputFile<<")"<<endl;
    outputFile.close();
    // cout<<"DONE"<<endl;
   
    return ;
  }
}
