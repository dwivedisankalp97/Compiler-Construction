#include <string>
#include <iostream>
#include <fstream>

#include <code_generator.h>

using namespace std;

namespace L1{

	string enumToString[17] = {"rdi", "rax", "rbx", "rbp", "r10", "r11", "r12", "r13", "r14", "r15", "rsi", "r8", "r9", "rcx", "rdx", "rsp"};
	string enumToString8Bit[17] = {"dil","al","bl","bpl","r10b","r11b","r12b","r13b","r14b","r15b","sil","r8b","r9b","cl","dl","spl"};
	

  string removeColon(string a){
	a[0] = '_';
	return a;
  }

  string GenerateRegister(int r){
  	return "%" + enumToString[r];
  }

  string GenerateRegister8Bit(int r){
  	return "%" + enumToString8Bit[r];
  }


  string GenerateMem(int r, string labelName){
  	return labelName + "(" + GenerateRegister(r) + ")";

  }

  string GenerateNumber(string num){
  	return "$" + num;
  }

  string GenerateOperation(string op){
  	if(op == "&="){
		return "andq";
		}
	if(op == "+="){
		return "addq";
	}
	if(op == "-="){
		return "subq";
	}
	if(op == "*="){
		return "imulq";
	}
	if(op == "<="){
		return "setle";
	}
	if (op == "<"){
		return "setl";
	}
	if (op == "="){
		return "sete";
	}
	return "";
  }


  string GenerateOperationJmp(string op){
	if(op == "<="){
		return "jle";
	}
	if (op == "<"){
		return "jl";
	}
	if (op == "="){
		return "je";
	}
	return "";
  }

  string GenerateOperationReverse(string op){
  	if(op == "<="){
		return "setge";
  	}
	else if (op == "<"){
		return "setg";
	}
	else{
		return "sete";
	}
  }

    string GenerateOperationReverseJmp(string op){
  	if(op == "<="){
		return "jge";
  	}
	else if (op == "<"){
		return "jg";
	}
	else{
		return "je";
	}
  }

  string GenerateFunctionEntry(int64_t locals){
  	int tempint = locals;
  	tempint *= 8;
  	if (tempint != 0)
  		return "subq $" + to_string(tempint) + ", %rsp\n";
  	else 
  		return "";

  }

  string GenerateFunctionExit(int64_t argn, int64_t locals){
  	int tempint = locals;
  	int tempint2 = argn;
  	if(tempint2 > 6){
  		tempint2 = (tempint2 - 6) * 8; 
  	}
  	else tempint2 = 0;
  	tempint = tempint * 8 + tempint2;
  	if (tempint != 0)
  		return "addq $" + to_string(tempint) + ", %rsp\n";
  	else 
  		return "";

  }

/*  string GenerateCmp(Item lc,Item rc,Item operation,Item left,ofstream& outputFile){
  	int tempint,tempint1,tempint2;
  	string tempstr,tempstr1,tempstr2;
  	tempstr = GenerateOperation(operation.labelName);
	tempstr1 = GenerateRegister8Bit(left.r);
	cout<<"Herer1"<<endl;
  	if(lc.isANumber){
		if(rc.isANumber){
			tempint = stoll(lc.labelName);
			tempint1 = stoll(rc.labelName);
			if(tempstr == "setle"){
				if(tempint <= tempint1){
					tempstr1 = "$1";
				}
				else{
					tempstr1 = "$0";
				}
			}
			else if(tempstr == "sete"){
				if(tempint == tempint1){
					tempstr1 = "$1";
				}
				else{
					tempstr1 = "$0";
				}
			}
			else if(tempstr == "setl"){
				if(tempint < tempint1){
					tempstr1 = "$1";
				}
				else{
					tempstr1 = "$0";
				}
			}
			outputFile << "movq " << tempstr1 << ", ";
			outputFile << GenerateRegister(left.r)<<endl;
		}
		else{
			outputFile << "cmpq $" << lc.labelName << ", ";
			outputFile << GenerateRegister(rc.r) << endl;
			outputFile << GenerateOperationReverse(operation.labelName);
			outputFile << " " << tempstr1 <<endl;
			outputFile << "movzbq " << tempstr1 << ", ";
			outputFile << GenerateRegister(left.r) <<endl;
		}	
	}
	else{
		outputFile << "cmpq ";
		if(rc.isARegister)
			outputFile << GenerateRegister(rc.r);
		else
			outputFile << GenerateNumber(rc.labelName);
		outputFile << ", ";
		outputFile << GenerateRegister(lc.r) << endl;
		outputFile << tempstr << " " << tempstr1 << endl;
		outputFile << "movzbq " << tempstr1 << ", " << GenerateRegister(left.r)<<endl;
	}
	cout<<"here2"<<endl;
}*/ // Function gives random errors

  void GenerateJmp(Item lc,Item rc,Item operation, Item label,ofstream& outputFile){
  	int tempint,tempint1,tempint2;
  	string tempstr,tempstr1,tempstr2;
  	tempstr = GenerateOperationJmp(operation.labelName);
	tempstr2 = removeColon(label.labelName);
  	if(lc.isANumber){
		if(rc.isANumber){
			tempint = stoll(lc.labelName);
			tempint1 = stoll(rc.labelName);
			if(tempstr == "jle"){
				if(tempint <= tempint1){
					tempstr1 = "$1";
				}
				else{
					tempstr1 = "$0";
				}
			}
			else if(tempstr == "je"){
				if(tempint == tempint1){
					tempstr1 = "$1";
				}
				else{
					tempstr1 = "$0";
				}
			}
			else if(tempstr == "jl"){
				if(tempint < tempint1){
					tempstr1 = "$1";
				}
				else{
					tempstr1 = "$0";
				}
			}
			if(tempstr1 == "$0")
				outputFile << endl;
			else
				outputFile << "jmp " << tempstr2 <<endl;
		}
		else{
			outputFile << "cmpq $" << lc.labelName << ", ";
			outputFile << GenerateRegister(rc.r) << endl;
			outputFile << GenerateOperationReverseJmp(operation.labelName);
			outputFile << " " << tempstr2 <<endl;
		}	
	}
	else{
		outputFile << "cmpq ";
		if(rc.isARegister)
			outputFile << GenerateRegister(rc.r);
		else
			outputFile << GenerateNumber(rc.labelName);
		outputFile << ", ";
		outputFile << GenerateRegister(lc.r) << endl;
		outputFile << tempstr << " " << tempstr2 << endl;
	}
}



  void GenerateInstructions(Function* entryFun,ofstream& outputFile){
  	string tempstr,tempstr1,tempstr2;
	int tempint,tempint1;
	outputFile << GenerateFunctionEntry(entryFun->locals);
	for(vector<Instruction *>::iterator i = entryFun->instructions.begin(); i != entryFun->instructions.end(); i++){
		auto g = *i;
		//cout<<g->iType()<<endl;
		IType ins_type = g->iType();
		switch (ins_type)
		{
			case ret:
				outputFile << GenerateFunctionExit(entryFun->arguments,entryFun->locals);
				outputFile << "retq\n";
				break;


			case assign_cmp:{
				Instruction_assignment_cmp* g1 = static_cast<Instruction_assignment_cmp*>(g);
				//GenerateCmp(g1->lc,g1->rc,g1->operation,g1->left,outputFile);
				tempstr = GenerateOperation(g1->operation.labelName);
				tempstr1 = GenerateRegister8Bit(g1->left.r);
				if(g1->lc.isANumber){
					if(g1->rc.isANumber){
						tempint = stoll(g1->lc.labelName);
						tempint1 = stoll(g1->rc.labelName);
						if(tempstr == "setle"){
							if(tempint <= tempint1){
								tempstr1 = "$1";
							}
							else{
								tempstr1 = "$0";
							}
						}
						else if(tempstr == "sete"){
							if(tempint == tempint1){
								tempstr1 = "$1";
							}
							else{
								tempstr1 = "$0";
							}
						}
						else if(tempstr == "setl"){
							if(tempint < tempint1){
								tempstr1 = "$1";
							}
							else{
								tempstr1 = "$0";
							}
						}
						outputFile << "movq " << tempstr1 << ", ";
						outputFile << GenerateRegister(g1->left.r)<<endl;

					}
					else{
						outputFile << "cmpq $" << g1->lc.labelName << ", ";
						outputFile << GenerateRegister(g1->rc.r) << endl;
						outputFile << GenerateOperationReverse(g1->operation.labelName);
						outputFile << " " << tempstr1 <<endl;
						outputFile << "movzbq " << tempstr1 << ", ";
						outputFile << GenerateRegister(g1->left.r) <<endl;
					}
				}
				else{
					outputFile << "cmpq ";
					if(g1->rc.isARegister)
						outputFile << GenerateRegister(g1->rc.r);
					else
						outputFile << GenerateNumber(g1->rc.labelName);
					outputFile << ", ";
					outputFile << GenerateRegister(g1->lc.r) << endl;
					outputFile << tempstr << " " << tempstr1 << endl;
					outputFile << "movzbq " << tempstr1 << ", " << GenerateRegister(g1->left.r)<<endl;
				}
			}
				break;


			case assign:{
				outputFile << "movq ";
				Instruction_assignment* g1 = static_cast<Instruction_assignment*>(g);
				if(g1->src.isANumber){
					outputFile<< GenerateNumber(g1->src.labelName);
				}
				else if(g1->src.isARegister){
					outputFile<<GenerateRegister(g1->src.r);
				}
				else {
					g1->src.labelName = removeColon(g1->src.labelName);
					outputFile<< "$" + (g1->src.labelName);
				}
				outputFile<<", " + GenerateRegister(g1->dst.r)<<endl;
			}
				break;


			case assign_mem:{
				outputFile << "movq ";
				instruction_assignment_mem_x_M* g1 = static_cast<instruction_assignment_mem_x_M*>(g);
				tempstr = GenerateMem(g1->x.r,g1->M.labelName);
				if(g1->isSrc){
					outputFile<<" " + tempstr + ", " + GenerateRegister(g1->other.r)<<endl;
				}
				else{
					if(g1->other.isANumber){
						tempstr1 = GenerateNumber(g1->other.labelName);
					}
					else if(g1->other.isARegister){
						tempstr1 = GenerateRegister(g1->other.r);
					}
					else {
						g1->other.labelName = removeColon(g1->other.labelName);
						tempstr1 = "$" + g1->other.labelName;
					}
					outputFile<<tempstr1 + ", " + tempstr<<endl;
				}
			}
				break;


			case aop:{
				instruction_aop* g1 = static_cast<instruction_aop*>(g);
				if(g1->operation.labelName == "&="){
					tempstr = "andq ";
				}
				else if(g1->operation.labelName == "+="){
					tempstr = "addq ";
				}
				else if(g1->operation.labelName == "-="){
					tempstr = "subq ";
				}
				else if(g1->operation.labelName == "*="){
					tempstr = "imulq ";
				}
				if(g1->src.isANumber){
					tempstr1 = GenerateNumber(g1->src.labelName);
				}
				else if(g1->src.isARegister){
					tempstr1 = GenerateRegister(g1->src.r);
				}
				else {
					g1->src.labelName = removeColon(g1->src.labelName);
					tempstr1 = "$" + g1->src.labelName;
				}
				outputFile << tempstr + " " + tempstr1 + ", " + GenerateRegister(g1->dest.r)<<endl;
			}
				break;


			case sop:{
				instruction_sop* g1 = static_cast<instruction_sop*>(g);	
				if(g1->operation.labelName == "<<=")
					tempstr = "salq";
				else
					tempstr = "sarq";
				if(g1->rightop.isARegister)
					tempstr1 = "%cl";
				else
					tempstr1 = GenerateNumber(g1->rightop.labelName);
				outputFile << tempstr + " " + tempstr1 + ", " + GenerateRegister(g1->leftop.r)<<endl;
			}
				break;


			case aop_mem:{
				instruction_aop_mem_x_M* g1 = static_cast<instruction_aop_mem_x_M*>(g);
				tempstr1 = GenerateOperation(g1->operation.labelName);
				tempstr2 = GenerateMem(g1->x.r,g1->M.labelName);
				if(g1->isLeft){
				  if(g1->other.isANumber)
				  	tempstr = GenerateNumber(g1->other.labelName);
				  else
				  	tempstr = GenerateRegister(g1->other.r);
				  outputFile << tempstr1 + " " + tempstr + ", " + tempstr2 << endl;
				}
				else{
					tempstr = GenerateRegister(g1->other.r);
					outputFile << tempstr1 + " " + tempstr2 + ", " + tempstr << endl;
				}
				
			}
				break;


			case cjump:{
				Instruction_cjump* g1 = static_cast<Instruction_cjump*>(g);
				GenerateJmp(g1->lc,g1->rc,g1->operation,g1->label,outputFile);
			}
				break;


			case labelType:{
				Instruction_label* g1 = static_cast<Instruction_label*>(g);
				g1->label.labelName = removeColon(g1->label.labelName);
				outputFile << g1->label.labelName + ":" << "\n";
			}
				break;


			case goto1:{
				Instruction_goto* g1 = static_cast<Instruction_goto*>(g);
				g1->label.labelName = removeColon(g1->label.labelName);
				outputFile << "jmp "<< g1->label.labelName << endl;
			}
				break;


			case call:{
				Instruction_call* g1 = static_cast<Instruction_call*>(g);
				tempstr = "subq ";
				tempint = stoll(g1->argn.labelName);
				if(tempint >= 6){
					tempint = tempint - 6;
					tempint *= 8;
					tempint += 8;
				}
				else tempint = 8;
				tempstr1 = to_string(tempint);
				outputFile << tempstr + GenerateNumber(tempstr1) + ", %rsp" << endl;
				if(g1->fname.isARegister)
					outputFile << "jmp *" + GenerateRegister(g1->fname.r)<<endl;
				else
					outputFile << "jmp " + removeColon(g1->fname.labelName)<<endl;
			}
				break;


			case print:
				outputFile << "call print" << endl;
				break;


			case allocate:
				outputFile << "call allocate" <<endl;
				break;


			case array:
				outputFile << "call array_error"<<endl;
				break;


			case inc_dec:{
				Instruction_inc_dec* g1 = static_cast<Instruction_inc_dec*>(g);
				if(g1->isInc)
					outputFile << "inc ";
				else
					outputFile << "dec ";
				outputFile << GenerateRegister(g1->reg.r)<<endl;
			}
				break;

			case lea:{
				Instruction_lea* g1 = static_cast<Instruction_lea*>(g);
				outputFile << "lea (";
				outputFile << GenerateRegister(g1->second.r) + ", ";
				outputFile << GenerateRegister(g1->third.r) + ", ";
				outputFile << g1->fourth.labelName + "), ";
				outputFile << GenerateRegister(g1->first.r)<<endl;
			}

		}
	}// For loop end
}
    

  void generate_code(Program p){


    /* 
     * Open the output file.
     */ 
	std::ofstream outputFile;
	outputFile.open("prog.S");
	outputFile<< ".text\n.globl go\ngo:\npushq %rbx\npushq %rbp\npushq %r12\npushq %r13\npushq %r14\npushq %r15\n"; 
	string tempstr = p.entryPointLabel;
	tempstr =removeColon(tempstr);
	outputFile<<"call "<<tempstr<<'\n';
	outputFile << "popq %r15\npopq %r14\npopq %r13\npopq %r12\npopq %rbp\npopq %rbx\nretq\n";
	outputFile << tempstr << ":\n";

	Function* entryFun;
	for (vector<Function *>::iterator i = p.functions.begin();i!=p.functions.end();i++){
		auto g = *i;
		if(g->name == p.entryPointLabel){
			entryFun = g;
			break;
		}
	}

	//outputFile << entryFun->name<<"\n";

	GenerateInstructions(entryFun,outputFile);

	for (vector<Function *>::iterator i = p.functions.begin();i!=p.functions.end();i++){
		auto g = *i;
		if(g->name == p.entryPointLabel){
			continue;
		}
		tempstr = removeColon(g->name) + ":";
		outputFile << tempstr << endl;
		GenerateInstructions(g,outputFile);
	}



	
		 

  
    /* 
     * Generate target code
     */ 
    //TODO

    /* 
     * Close the output file.
     */ 

    outputFile.close();
   
    return ;
  }
}
