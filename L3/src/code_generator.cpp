#include <string>
#include <iostream>
#include <fstream>

#include <code_generator.h>
#include <parser.h>
#include <map>
#include <vector>
#include <time.h>

using namespace std;



namespace L3{

	// vector<string> SortedGPRegistersA{"r10","r11","r8","r9","rax","rcx","rdi","rdx","rsi","r12","r13","r14","r15","rbp","rbx"};
	// string enumToString[17] = {"rdi", "rax", "rbx", "rbp", "r10", "r11", "r12", "r13", "r14", "r15", "rsi", "r8", "r9", "rcx", "rdx", "rsp"};

 //  string removeColon(string a){
	// a[0] = '_';
	// return a;
 //  }

 //  string removeFirstChar(string a){
	// return a.substr(1);
 //  }

 //  string GenerateRegister(int r){
 //  	return enumToString[r];
 //  }


	struct TreeNode{
		Item item;
		TreeNode* parent;
		TreeNode* c1;
		TreeNode* c2;
		TreeNode(Item v, TreeNode* p, TreeNode* cc1, TreeNode* cc2):item(v),parent(p),c1(cc1),c2(cc2){}
	};


	vector<string> arguments{"rdi","rsi","rdx","rcx","r8","r9"};


 //  void GenerateContexts(vector<vector<Instruction*>> &Contexts,Function* entryFun){
 //  	vector<Instruction*> currentContext;
 //  	for(vector<Instruction *>::iterator i = entryFun->instructions.begin(); i != entryFun->instructions.end(); i++){
 //  		auto g = *i;
 //  		IType ins_type = g->iType();
 //  		switch (ins_type)
 //  		{
 //  			case br:
 //  			case br_var:
 //  			case labelType:{
 //  				currentContext.push_back(g);
 //  				Contexts.push_back(currentContext);
 //  				currentContext.clear();
 //  			}

 //  			break;

 //  			default:{
 //  				currentContext.push_back(g);
 //  			}

 //  			break;

 //  		}
 //  	}

 //  	if(currentContext.size()>0){
 //  		Contexts.push_back(currentContext);
 //  	}

 //  }

 //  void GenerateTrees(vector<vector<Instruction*>> &Contexts, vector<TreeNode*> &trees){
 //  	TreeNode* currentNode;
 //  	TreeNode* root;
 //  	TreeNode* temp1;
 //  	TreeNode* temp2;
 //  	TreeNode* temp3;
 //  	Item ret_item = Item("return");
 //  	Item br_item = Item("br");
 //  	Item br_var_item = Item("br_var");
 //  	Item arrow_item = Item("<-");
 //  	Item store_item = Item("store");
 //  	Item load_item = Item("load");
 //  	Item label_item = Item("label");
 //  	Item call_item = Item("call");
 //  	for(vector<vector<Instruction *>>::iterator i = Contexts.begin(); i != Contexts.end(); i++){
	// 	for(vector<Instruction *>::iterator j = (*i).begin(); j != (*i).end(); j++){
	// 		auto g = *j;
	// 		string s = g->tostring();
	// 		IType ins_type = g->iType();
	// 		switch (ins_type)
	// 		{
	// 			case ret:{
	// 				root = new TreeNode(ret_item,NULL,NULL,NULL);
	// 				trees.push_back(root);
	// 				break;
	// 			}

	// 			case ret_t:{
	// 				Instruction_ret_t* g1 = static_cast<Instruction_ret_t*>(g);
	// 				temp1 = new TreeNode(g1->returnVar,NULL,NULL,NULL);
	// 				root = new TreeNode(ret_item,NULL,temp1,NULL);
	// 				temp1->parent = root;
	// 				trees.push_back(root);
	// 				break;
	// 			}

	// 			case br:{
	// 				Instruction_br* g1 = static_cast<Instruction_br*>(g);
	// 				temp1 = new TreeNode(g1->label,NULL,NULL,NULL);
	// 				root = new TreeNode(br_item,NULL,temp1,NULL);
	// 				temp1->parent = root;
	// 				trees.push_back(root);
	// 				break;
	// 			}

	// 			case br_var:{
	// 				Instruction_br_var* g1 = static_cast<Instruction_br_var*>(g);
	// 				temp1 = new TreeNode(g1->label,NULL,NULL,NULL);
	// 				temp2 = new TreeNode(g1->var,NULL,NULL,NULL);
	// 				root = new TreeNode(br_var_item,NULL,temp1,temp2);
	// 				temp1->parent = root;
	// 				temp2->parent = root;
	// 				trees.push_back(root);
	// 				break;
	// 			}

	// 			case assign:{
	// 				Instruction_assignment* g1 = static_cast<Instruction_assignment*>(g);
	// 				temp1 = new TreeNode(g1->src,NULL,NULL,NULL);
	// 				temp2 = new TreeNode(arrow_item,NULL,temp1,NULL);
	// 				root = new TreeNode(g1->dst,NULL,temp2,NULL);
	// 				temp2->parent = root;
	// 				temp1->parent = temp2;
	// 				trees.push_back(root);
	// 				break;
	// 			}

	// 			case load:{
	// 				Instruction_load* g1 = static_cast<Instruction_load*>(g);
	// 				temp1 = new TreeNode(g1->rvar,NULL,NULL,NULL);
	// 				temp3 = new TreeNode(load_item,NULL,temp1,NULL);
	// 				temp1->parent = temp3;
	// 				temp2 = new TreeNode(arrow_item,NULL,temp3,NULL);
	// 				temp3->parent = temp2;
	// 				root = new TreeNode(g1->lvar,NULL,temp2,NULL);
	// 				temp2->parent = root;
	// 				trees.push_back(root);
	// 				break;
	// 			}

	// 			case store:{
	// 				root = new TreeNode(store_item,NULL,NULL,NULL);
	// 				trees.push_back(root);
	// 				break;
	// 			}

	// 			case assign_cmp_aop:{
	// 				Instruction_assignment_cmp_aop* g1 = static_cast<Instruction_assignment_cmp_aop*>(g);
	// 				temp1 = new TreeNode(g1->rc,NULL,NULL,NULL);
	// 				temp2 = new TreeNode(g1->lc,NULL,NULL,NULL);
	// 				temp3 = new TreeNode(g1->operation,NULL,temp2,temp1);
	// 				temp2->parent = temp3;
	// 				temp1->parent = temp3;
	// 				root = new TreeNode(g1->left,NULL,temp3,NULL);
	// 				temp3->parent = root;
	// 				trees.push_back(root);
	// 				break; 
	// 			}

	// 			case labelType:{
	// 				root = new TreeNode(label_item,NULL,NULL,NULL);
	// 				trees.push_back(root);
	// 				break;
	// 			}

	// 			case assign_call:
	// 			case call:{
	// 				root = new TreeNode(call_item,NULL,NULL,NULL);
	// 				trees.push_back(root);
	// 				break;
	// 			}

	// 		}

	// 	}
	// }

 //  }

  void GenerateFunctionEntry(Function* entryFun,ofstream& outputFile){
  	outputFile<<"("<<entryFun->name<<endl;
  	outputFile<<entryFun->arguments.size()<<endl;
  	int tempint = 0;
  	for(auto j : entryFun->arguments){
		outputFile<<"\t";
  		outputFile<<j.labelName<<" <- "<<arguments[tempint]<<endl;
  		tempint++;
  		if (tempint == 6)
  			break;
  	}
  	if(entryFun->arguments.size()>6){
  		int start_stack = 0;
  		for(int i = 6;i<entryFun->arguments.size();i++){
  			outputFile<<"\t";
  			outputFile<<entryFun->arguments[i].labelName<<" <- "<<"stack-arg "<<start_stack<<endl;
  			start_stack += 8;
  		}
  	}
  	return;

  }

  void GenerateInstructions(Function* entryFun,ofstream& outputFile){
  	string tempstr,tempstr1,tempstr2;
	int tempint,tempint1;
	// vector<vector<Instruction*>> Contexts;
	// vector<TreeNode*> trees;
	// GenerateContexts(Contexts,entryFun);
	// for(vector<vector<Instruction *>>::iterator i = Contexts.begin(); i != Contexts.end(); i++){
	// 	cout<<"Context change"<<endl;
	// 	cout<<Contexts.size()<<endl;
	// 	for(vector<Instruction *>::iterator j = (*i).begin(); j != (*i).end(); j++){
	// 		auto g = *j;
	// 		string s = g->tostring();
	// 		cout<<s<<endl;

	// 	}
	// }

	// GenerateTrees(Contexts,trees);

	// cout<<"HLKDJSFLKJDFLK"<<endl;
	// cout<<trees.size()<<endl;
	// cout<<entryFun->instructions.size()<<endl;
	// for(auto i : trees){
	// 	cout<<i->item.labelName<<endl;
	// }

	int numOfCalls = 0;
	GenerateFunctionEntry(entryFun,outputFile);
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
				outputFile<<"rax <- "<< g1->returnVar.labelName<<endl;
				outputFile<<"\t";
				outputFile<<"return"<<endl;
				break;
			}

			case br:{
				outputFile<<"\t";
				Instruction_br* g1 = static_cast<Instruction_br*>(g);
				outputFile<<"goto "<<function_name<<"_"<<g1->label.labelName.substr(1)<<endl;
				break;
			}

			case br_var:{
				outputFile<<"\t";
				Instruction_br_var* g1 = static_cast<Instruction_br_var*>(g);
				outputFile<<"cjump "<<g1->var.labelName<<" = 1 "<<function_name<<"_"<<g1->label.labelName.substr(1)<<endl;
				break;
			}

			case assign:{
				outputFile<<"\t";
				Instruction_assignment* g1 = static_cast<Instruction_assignment*>(g);
				outputFile<<g1->dst.labelName<<" <- "<<g1->src.labelName;
				outputFile<<endl;
				break;
			}

			case load:{
				outputFile<<"\t";
				Instruction_load* g1 = static_cast<Instruction_load*>(g);
				outputFile<<g1->lvar.labelName<<" <- mem "<<g1->rvar.labelName<<" 0"<<endl;
				break;
			}

			case store:{
				outputFile<<"\t";
				Instruction_store* g1 = static_cast<Instruction_store*>(g);
				outputFile<<"mem "<<g1->lvar.labelName<<" 0 <- "<<g1->rvar.labelName<<endl;
				break;
			}


			case assign_cmp_aop:{
				outputFile<<"\t";
				Instruction_assignment_cmp_aop* g1 = static_cast<Instruction_assignment_cmp_aop*>(g);
				if(g1->operation.labelName == "<" || g1->operation.labelName == "<=" || g1->operation.labelName == "="){
					outputFile<<g1->left.labelName<<" <- "<<g1->lc.labelName<<" "<<g1->operation.labelName<<" "<<g1->rc.labelName;
				}
				else if(g1->operation.labelName == ">"){
					outputFile<<g1->left.labelName<<" <- "<<g1->rc.labelName<<" < "<<g1->lc.labelName;
				}
				else if(g1->operation.labelName == ">="){
					outputFile<<g1->left.labelName<<" <- "<<g1->rc.labelName<<" <= "<<g1->lc.labelName;
				}
				else{
					if(g1->lc.labelName == g1->left.labelName){
						outputFile<<g1->left.labelName<<" "<<g1->operation.labelName<<"= "<<g1->rc.labelName;
					}
					else if(g1->rc.labelName == g1->left.labelName){
						outputFile<<g1->left.labelName<<" "<<g1->operation.labelName<<"= "<<g1->lc.labelName;
					}
					else{
						outputFile<<g1->left.labelName<<" <- "<<g1->lc.labelName<<endl;
						outputFile<<"\t";
						outputFile<<g1->left.labelName<<" "<<g1->operation.labelName<<"= "<<g1->rc.labelName;
					}
				}
				outputFile<<endl;
				break;

			}


			case labelType:{
				outputFile<<"\t";
				Instruction_label* g1 = static_cast<Instruction_label*>(g);
				outputFile <<function_name<<"_"<<g1->label.labelName.substr(1) << "\n";
				break;

			}


			case call:{
				Instruction_call* g1 = static_cast<Instruction_call*>(g);
				if(g1->fname.labelName != "print" && g1->fname.labelName != "allocate" && g1->fname.labelName != "array-error"){
					outputFile<<"\t";
					outputFile<<"mem rsp -8 <- "<<function_name<<"_ret"<<numOfCalls<<endl;
				}
				tempint1 = 0;
				for(auto j : g1->arguments){
					outputFile<<"\t";
					outputFile<<arguments[tempint1]<<" <- "<<j.labelName<<endl;
					tempint1++;
					if(tempint1==6)
						break;
				}
				if(g1->arguments.size()>6){
			  		int start_stack = g1->arguments.size()-6;
			  		start_stack *= 8;
			  		start_stack += 8;
			  		for(int tt = 6;tt<g1->arguments.size();tt++){
			  			outputFile<<"\t";
			  			outputFile<<"mem rsp -"<<start_stack<<" <- "<<g1->arguments[tt].labelName<<endl;
			  			start_stack -= 8;
			  		}
			  	}
				outputFile<<"\t";
				outputFile<<"call "<<g1->fname.labelName<<" "<<g1->arguments.size()<<endl;
				if(g1->fname.labelName != "print" && g1->fname.labelName != "allocate" && g1->fname.labelName != "array-error"){
					outputFile<<"\t";
					outputFile<<function_name<<"_ret"<<numOfCalls<<endl;
				}
				numOfCalls++;
				break;

			}

			case assign_call:{
				Instruction_assign_call* g1 = static_cast<Instruction_assign_call*>(g);
				if(g1->fname.labelName != "print" && g1->fname.labelName != "allocate" && g1->fname.labelName != "array-error"){
					outputFile<<"\t";
					outputFile<<"mem rsp -8 <- "<<function_name<<"_ret"<<numOfCalls<<endl;
				}
				tempint1 = 0;
				for(auto j : g1->arguments){
					outputFile<<"\t";
					outputFile<<arguments[tempint1]<<" <- "<<j.labelName<<endl;
					tempint1++; 
					if(tempint1==6)
						break;
				}
				if(g1->arguments.size()>6){
			  		int start_stack = g1->arguments.size()-6;
			  		start_stack *= 8;
			  		start_stack += 8;
			  		for(int tt = 6;tt<g1->arguments.size();tt++){
			  			outputFile<<"\t";
			  			outputFile<<"mem rsp -"<<start_stack<<" <- "<<g1->arguments[tt].labelName<<endl;
			  			start_stack -= 8;
			  		}
			  	}
				outputFile<<"\t";
				outputFile<<"call "<<g1->fname.labelName<<" "<<g1->arguments.size()<<endl;
				if(g1->fname.labelName != "print" && g1->fname.labelName != "allocate" && g1->fname.labelName != "array-error"){
					outputFile<<"\t";
					outputFile<<function_name<<"_ret"<<numOfCalls<<endl;
				}
				numOfCalls++;
				outputFile<<"\t";
				outputFile<<g1->var.labelName<<" <- rax"<<endl;
				break;
			}
		}
	}// For loop end*/
}
    

  void generate_code(Program p){

	std::ofstream outputFile;
	outputFile.open("prog.L2");
	string entry = p.entryPointLabel;
	outputFile<<"(:main"<<endl;

	for (vector<Function *>::iterator i = p.functions.begin();i!=p.functions.end();i++){
		auto g = *i;
		GenerateInstructions(g,outputFile);
		outputFile<<")\n\n";
	}
	outputFile<<")"<<endl;
    outputFile.close();
    // cout<<"DONE"<<endl;

						// for (vector<Function *>::iterator i = p.functions.begin();i!=p.functions.end();i++){
						// 	cout<<(*i)->name<<endl;
						// 	for(auto j : (*i)->arguments){
						// 		cout<<j.labelName<<endl;
						// 	}
						// 	auto g = *i;
						// 	for(vector<Instruction *>::iterator k = g->instructions.begin(); k != g->instructions.end(); k++){
						// 		auto g2 = *k;
						// 		IType ins_type = g2->iType();
						// 		switch (ins_type)
						// 		{
						// 			case call:{
						// 			// outputFile<<"\t";
						// 			Instruction_call* g1 = static_cast<Instruction_call*>(g2);
						// 			for(auto j : g1->arguments){
						// 				cout<<j.labelName<<endl;
						// 			}
						// 			// outputFile<<"call ";
						// 			// if(g1->fname.isAVar)
						// 			// 	outputFile<<SortedGPRegistersA[colorMap[removeFirstChar(g1->fname.labelName)]];
						// 			// else
						// 			// 	outputFile<<g1->fname.labelName;

						// 			// outputFile<<" "<<g1->argn.labelName;
						// 			// outputFile<<endl;
						// 			}
						// 				break;
						// 		}
						// }
					   
					   
					 //  }
   return ;
}

}
