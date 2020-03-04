#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <algorithm>
#include <unordered_set>

#include <analysis.h>
#include <sstream>

using namespace std;




namespace L2{

	int spiller(Program_spill f,int local){

		std::ofstream tempSpillFile;
		tempSpillFile.open("tempSpillFile.L2");
		Function* entryFun = f.p;
		int spillCount = 0;
		string var_to_spill = f.spill_var.labelName;
		string new_var = f.new_var.labelName;
		bool write_flag = false;
		bool read_flag = false;
		bool op1,op2,op3;
		stringstream res,tempstream;


		auto set_values = [&](){
  			write_flag = false;
  			read_flag = false;
  			op1 = false;
  			op2 = false;
  			op3 = false;
  		};

  		auto check_var_preset = [&](Item i){
  			if(i.isAVar && i.labelName == var_to_spill)
  				return true;
  			else
  				return false;
  		};

  		auto write_write_op = [&](bool flag){
  			if(flag == false)
  				return;
  			res<<"mem rsp "<<local*8<<" <- "<< new_var << spillCount <<endl;
  			res<<"\t";
  		};

  		auto write_read_op = [&](bool flag){
  			if(flag == false)
  				return;
  			res<< new_var << spillCount << " <- mem rsp "<<local*8<<endl;
  			res<<"\t";
  		};

  		auto check_change = [&](){
  			if(read_flag || write_flag)
  				spillCount++;
  		};

  		set_values();


		for(vector<Instruction *>::iterator i = entryFun->instructions.begin(); i != entryFun->instructions.end(); i++){
			auto g = *i;
			//res<<g->iType()<<endl;
			IType ins_type = g->iType();
			switch (ins_type)
			{
				case ret:
					res << "return" << endl;
					check_change();
					set_values();
					break;


				case assign_cmp:{
					Instruction_assignment_cmp* g1 = static_cast<Instruction_assignment_cmp*>(g);
					if(check_var_preset(g1->left)){
						op1 = true;
						write_flag = true;
					}

					if(check_var_preset(g1->lc)){
						op2 = true;
						read_flag = true;
					}

					if(check_var_preset(g1->rc)){
						op3 = true;
						read_flag = true;
					}

					write_read_op(read_flag);

					if(op1)
						res << new_var <<spillCount;
					else
						res<<g1->left.labelName;
					res<<" <- ";
					if(op2)
						res<<new_var<<spillCount;
					else
						res<<g1->lc.labelName;
					res<<" "<<g1->operation.labelName<<" ";
					if(op3)
						res<<new_var<<spillCount;
					else
						res<<g1->rc.labelName;

					res<<endl<<"\t";

					write_write_op(write_flag);

				}
					check_change();
					set_values();
					break;


				case assign:{
					Instruction_assignment* g1 = static_cast<Instruction_assignment*>(g);
					if(check_var_preset(g1->dst)){
						write_flag = true;
					}

					if(check_var_preset(g1->src)){
						read_flag = true;
					}

					write_read_op(read_flag);

					if (write_flag){
						res<< new_var << spillCount;
					}
					else
						res<< g1->dst.labelName;

					res << " <- ";

					if (read_flag)
						res<<new_var<<spillCount<<endl;
					else
						res<<g1->src.labelName<<endl;
					res<<"\t";

					write_write_op(write_flag);

				}
					check_change();
					set_values();
					break;


				case assign_mem:{
					instruction_assignment_mem_x_M* g1 = static_cast<instruction_assignment_mem_x_M*>(g);

					if(check_var_preset(g1->x)){
						op1 = true;
					}

					if(check_var_preset(g1->other)){
						if(g1->isSrc)
							write_flag=true;
						op2 = true;
					}


					if(g1->isSrc){
						if(op2)
							tempstream<<new_var<<spillCount;
						else
							tempstream<<g1->other.labelName;
						tempstream <<" <- "<< "mem ";
						if(op1){
							read_flag = true;
							tempstream<<new_var<<spillCount;
						}
						else
							tempstream<<g1->x.labelName;
						tempstream << " " << g1->M.labelName;
					}
					else{
						tempstream<<"mem ";
						if(op1){
							read_flag = true;
							tempstream<<new_var<<spillCount;
						}
						else
							tempstream<<g1->x.labelName;
						tempstream << " " << g1->M.labelName << " <- ";
						if(op2){
							read_flag = true;
							tempstream<<new_var<<spillCount;
						}
						else
							tempstream<<g1->other.labelName;
					}
					tempstream<<endl;
					tempstream<<"\t";
					write_read_op(read_flag);
					res<<tempstream.str();
					write_write_op(write_flag);


				}
					tempstream.str("");
					check_change();
					set_values();
					break;


				case aop:{
					instruction_aop* g1 = static_cast<instruction_aop*>(g);

					if(check_var_preset(g1->dest)){
						op1 = true;
						write_flag = true;
						read_flag = true;
					}

					if(check_var_preset(g1->src)){
						op2 = true;
						read_flag = true;;
					}

					write_read_op(read_flag);

					if(op1)
						res<<new_var<<spillCount;
					else
						res<<g1->dest.labelName;

					res << " " << g1->operation.labelName << " ";

					if(op2)
						res<<new_var<<spillCount;
					else
						res<<g1->src.labelName;
					res<<endl;
					res<<"\t";

					write_write_op(write_flag);

				}
					check_change();
					set_values();
					break;


				case sop:{
					instruction_sop* g1 = static_cast<instruction_sop*>(g);	

					if(check_var_preset(g1->leftop)){
						read_flag=true;
						write_flag = true;
						op1 = true;
					}

					if(check_var_preset(g1->rightop)){
						read_flag = true;
						op2 = true;
					}

					write_read_op(read_flag);

					if(op1)
						res<<new_var<<spillCount;
					else
						res<<g1->leftop.labelName;

					res << " " << g1->operation.labelName << " ";

					if(op2)
						res<<new_var<<spillCount;
					else
						res<<g1->rightop.labelName;

					res<<endl;
					res<<"\t";

					write_write_op(write_flag);


				}
					check_change();
					set_values();
					break;


				case aop_mem:{
					instruction_aop_mem_x_M* g1 = static_cast<instruction_aop_mem_x_M*>(g);


					if(check_var_preset(g1->x)){
						op1 = true;
					}

					if(check_var_preset(g1->other)){
						op2 = true;
					}

					if(g1->isLeft){
						tempstream<<"mem ";
						if(op1){
							read_flag = true;
							write_flag = true;
							tempstream<<new_var<<spillCount;
						}
						else
							tempstream<<g1->x.labelName;
						tempstream<<" "<<g1->M.labelName;
						tempstream<<" "<<g1->operation.labelName<<" ";
						if(op2){
							read_flag = true;
							tempstream<<new_var<<spillCount;
						}
						else
							tempstream<<g1->other.labelName;
						tempstream<<endl;
						tempstream<<"\t";

						
					}
					else{
						if(op2){
							write_flag = true;
							read_flag = true;
							tempstream<<new_var<<spillCount;
						}
						else
							tempstream<<g1->other.labelName;
						tempstream<<" "<<g1->operation.labelName<< " mem ";
						if(op1){
							read_flag = true;
							tempstream<<new_var<<spillCount;
						}
						else
							tempstream<<g1->x.labelName;
						tempstream<<" "<<g1->M.labelName;
						tempstream<<endl;
						tempstream<<"\t";
					}

					write_read_op(read_flag);
					res<<tempstream.str();
					write_write_op(write_flag);


					
				}
					check_change();
					tempstream.str("");
					set_values();
					break;


				case cjump:{
					Instruction_cjump* g1 = static_cast<Instruction_cjump*>(g);

					if(check_var_preset(g1->lc)){
						op1 = true;
						read_flag = true;
					}


					if(check_var_preset(g1->rc)){
						op2 = true;
						read_flag = true;
					}

					write_read_op(read_flag);

					res<<"cjump ";

					if(op1)
						res<<new_var<<spillCount;
					else
						res<<g1->lc.labelName;

					res << " " << g1->operation.labelName << " ";

					if(op2)
						res<<new_var<<spillCount;
					else
						res<<g1->rc.labelName;

					res<<" "<<g1->label.labelName;

					res<<endl;
					res<<"\t";

					write_write_op(write_flag);

				}
					check_change();
					set_values();
					break;


				case labelType:{
					Instruction_label* g1 = static_cast<Instruction_label*>(g);
					res<<g1->label.labelName<<endl;
					res<<"\t";

				}
					check_change();
					set_values();
					break;


				case goto1:{
					Instruction_goto* g1 = static_cast<Instruction_goto*>(g);
					res<<"goto "<<g1->label.labelName<<endl;
					res<<"\t";

				}
					check_change();
					set_values();
					break;


				case call:{
					Instruction_call* g1 = static_cast<Instruction_call*>(g);
					if(check_var_preset(g1->fname)){
						read_flag = true;
					}

					write_read_op(read_flag);
					res<<"call ";
					if(read_flag)
						res<<new_var<<spillCount;
					else
						res<<g1->fname.labelName;
					res<<" "<<g1->argn.labelName<<endl;
					res<<"\t";

				}
					check_change();
					set_values();
					break;


				case print:
					res<<"call print 1"<<endl;
					res<<"\t";
					break;


				case allocate:
					res<<"call allocate 2"<<endl;
					res<<"\t";
					break;


				case array:
					res<<"call array-error 2"<<endl;
					res<<"\t";
					break;


				case inc_dec:{
					Instruction_inc_dec* g1 = static_cast<Instruction_inc_dec*>(g);

					if(check_var_preset(g1->reg)){
						read_flag = true;
						write_flag = true;
						op1 = true;
					}

					if(op1){
						write_read_op(read_flag);
						res<<new_var<<spillCount;
						if(g1->isInc)
							res<<"++"<<endl;
						else
							res<<"--"<<endl;
						res<<"\t";
						write_write_op(write_flag);
					}
					else{
						res<<g1->reg.labelName;
						if(g1->isInc)
							res<<"++"<<endl;
						else
							res<<"--"<<endl;
						res<<"\t";
					}

				}
					check_change();
					set_values();
					break;

				case lea:{
					Instruction_lea* g1 = static_cast<Instruction_lea*>(g);

					if(check_var_preset(g1->first)){
						op1 = true;
						write_flag = true;
					}
						

					if(check_var_preset(g1->second)){
						op2 = true;
						read_flag = true;
					}

					if(check_var_preset(g1->third)){
						op3 = true;
						read_flag = true;
					}

					write_read_op(read_flag);
					if(op1)
						res<<new_var<<spillCount;
					else
						res<<g1->first.labelName;

					res<<" @ ";

					if(op2)
						res<<new_var<<spillCount;
					else
						res<<g1->second.labelName;

					res<<" ";

					if(op3)
						res<<new_var<<spillCount;
					else
						res<<g1->third.labelName;

					res<<" "<<g1->fourth.labelName<<endl<<"\t";
					write_write_op(write_flag);

				}
					check_change();
					set_values();
					break;

				case stack_arg:{
					Instruction_stack_arg* g1 = static_cast<Instruction_stack_arg*>(g);

					if(check_var_preset(g1->left))
						write_flag = true;

					if(write_flag)
						res<<new_var<<spillCount;
					else
						res<<g1->left.labelName;

					res <<" <- stack-arg "<<g1->M.labelName<<endl;;
					res<<"\t";
					write_write_op(write_flag);
				}
					check_change();
					set_values();
					break;

			}
		}// For loop end*/


		tempSpillFile<<"("<<entryFun->name<<endl;
  		tempSpillFile<<"\t";
  		tempSpillFile<<entryFun->arguments;
  		tempSpillFile<<endl;
  		tempSpillFile<<"\t";
  		tempSpillFile<<res.str();
  		tempSpillFile << ")"<<endl;

  		if(spillCount>0){
  			// cout<<"Spilled"<<endl;
  			return 1;
  		}
  		else{
  			// cout<<"No Spill"<<endl;
  			return 0;
  		}



	}
}