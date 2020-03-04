#pragma once

#include <vector>
#include <string>

namespace L2 {

  enum Register {rdi, rax, rbx, rbp, r10, r11, r12, r13, r14, r15, rsi, r8, r9, rcx, rdx, rsp};
  enum IType {none, ret,assign,assign_mem,aop,sop,aop_mem,assign_cmp,\
    cjump,labelType,call,print,array,allocate,inc_dec,\
    goto1,lea,stack_arg};


  struct Item {
    std::string labelName;
    Register r;
    bool isANumber;
    bool isARegister;
    bool isAVar;
    bool isAOperator;
    Item() : isANumber(false),isAVar(false),isARegister(false),isAOperator(false){};
  };

  /*
   * Instruction interface.
   */
  struct Instruction{
	virtual IType iType(){
		return none;
	}
  };

  /*
   * Instructions.
   */
  struct Instruction_ret : Instruction{
	IType iType(){
		return ret;
	}
  };

  struct Instruction_assignment : Instruction{
	IType iType(){
		return assign;
	}
    Item src,dst;
  };

  struct instruction_assignment_mem_x_M : Instruction{
	IType iType(){
		return assign_mem;
	}
    Item other,x,M;
    bool isSrc;
  };

  struct instruction_aop : Instruction{
	IType iType(){
		return aop;
	}
    Item src,dest,operation;
  };

  struct instruction_sop : Instruction{
	IType iType(){
		return sop;
	}
    Item leftop,rightop,operation;
  };

  struct instruction_aop_mem_x_M : Instruction{
	IType iType(){
		return aop_mem;
	}
    Item other,operation,x,M;
    bool isLeft;
  };

  struct Instruction_assignment_cmp : Instruction{
	IType iType(){
		return assign_cmp;
	}
    Item left,lc,operation,rc;
  };

  struct Instruction_cjump : Instruction{
	IType iType(){
		return cjump;
	}
    Item lc,operation,rc,label;
  };

  struct Instruction_label : Instruction{
	IType iType(){
		return labelType;
	}
    Item label;
  };

  struct Instruction_goto : Instruction{
	IType iType(){
		return goto1;
	}
    Item label;
  };

  struct Instruction_call :Instruction{
	IType iType(){
		return call;
	}
    Item fname,argn;
  };

  struct Instruction_print :Instruction{
	IType iType(){
		return print;
	}
  };

  struct Instruction_array :Instruction{
	IType iType(){
		return array;
	}
  };

  struct Instruction_allocate :Instruction{
	IType iType(){
		return allocate;
	}
  };

  struct Instruction_inc_dec :Instruction{
	IType iType(){
		return inc_dec;
	}
    Item reg;
    bool isInc;
  };

  struct Instruction_lea :Instruction{
  IType iType(){
    return lea;
  }
    Item first,second,third,fourth;
  };

  struct Instruction_stack_arg :Instruction{
  IType iType(){
    return stack_arg;
  }
    Item left,M;
  };

  /*
   * Function.
   */
  struct Function{
    std::string name;
    int64_t arguments;
    std::vector<Instruction *> instructions;
    Function() : arguments(-1){};
  };

  /*
   * Program.
   */
  struct Program{
    std::string entryPointLabel;
    std::vector<Function *> functions;
  };

  struct Program_spill
  {
    Function *p;
    Item spill_var, new_var;
    Program_spill() : p(),spill_var(),new_var(){};
    Program_spill(Function* i, Item s, Item n) : p(i),spill_var(s),new_var(n) {}; 
  };

}