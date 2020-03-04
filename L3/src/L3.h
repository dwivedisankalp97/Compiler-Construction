#pragma once

#include <vector>
#include <string>

using namespace std;

namespace L3 {

  enum Register {rdi, rax, rbx, rbp, r10, r11, r12, r13, r14, r15, rsi, r8, r9, rcx, rdx, rsp};
  enum IType {none,ret,assign,load,assign_cmp_aop,\
    br_var,labelType,call,assign_call,array,allocate,inc_dec,\
    store,br,ret_t};


  struct Item {
    std::string labelName;
    Register r;
    bool isANumber;
    bool isARegister;
    bool isAVar;
    bool isAOperator;
    Item() : isANumber(false),isAVar(false),isARegister(false),isAOperator(false){};
    Item(string l) : labelName(l){};
  };

  /*
   * Instruction interface.
   */
  struct Instruction{
	virtual IType iType(){
		return none;
	}
  virtual string tostring(){
    return "";
  }
  };

  /*
   * Instructions.
   */
  struct Instruction_ret : Instruction{
	IType iType(){
		return ret;
	}

  string tostring(){
    return "return";
  }
  };

  struct Instruction_ret_t : Instruction{
  IType iType(){
    return ret_t;
  }

    Item returnVar;

  string tostring(){
    return "return " + returnVar.labelName;
  }
  };

  struct Instruction_br : Instruction{
  IType iType(){
    return br;
  }
    Item label;

  string tostring(){
    return "br " + label.labelName;
  }
  };

  struct Instruction_br_var : Instruction{
  IType iType(){
    return br_var;
  }
    Item label,var;

  string tostring(){
    return "br " + var.labelName + " " + label.labelName;
  }
  };

  struct Instruction_assignment : Instruction{
	IType iType(){
		return assign;
	}
    Item src,dst;

  string tostring(){
    return src.labelName + "<-" + dst.labelName;
  }
  };

  struct Instruction_load : Instruction{
  IType iType(){
    return load;
  }
    Item lvar,rvar;

  string tostring(){
    return lvar.labelName + " <- load " + rvar.labelName;
  }
  };

  struct Instruction_store : Instruction{
  IType iType(){
    return store;
  }
    Item lvar,rvar;

  string tostring(){
    return "store " + lvar.labelName + " <- " + rvar.labelName;
  }
  };


  struct Instruction_assignment_cmp_aop : Instruction{
	IType iType(){
		return assign_cmp_aop;
	}
    Item left,lc,operation,rc;

  string tostring(){
    return left.labelName + " <- " + lc.labelName + " " + operation.labelName + " " +rc.labelName;
  }
  };

  struct Instruction_label : Instruction{
	IType iType(){
		return labelType;
	}
    Item label;
  string tostring(){
    return label.labelName;
  }
  };

  struct Instruction_call :Instruction{
	IType iType(){
		return call;
	}
    Item fname;
    std::vector<Item> arguments;

  string tostring(){
    string s = "call ";
    s += fname.labelName;
    s+=" (";
    for(auto i : arguments){
      s+=i.labelName;
      s+=",";
    }
    s+=")";
    return s;
  }
  };

  struct Instruction_assign_call :Instruction{
  IType iType(){
    return assign_call;
  }
    Item var;
    Item fname;
    std::vector<Item> arguments;

  string tostring(){
    string s = "call " + var.labelName + " <- ";
    s += fname.labelName;
    s+=" (";
    for(auto i : arguments){
      s+=i.labelName;
      s+=",";
    }
    s+=")";
    return s;
  }
  };


  struct Instruction_inc_dec :Instruction{
	IType iType(){
		return inc_dec;
	}
    Item reg;
    bool isInc;
  };

  /*
   * Function.
   */
  struct Function{
    std::string name;
    std::vector<Item> arguments;
    std::vector<Instruction *> instructions;
  };

  /*
   * Program.
   */
  struct Program{
    std::string entryPointLabel;
    std::vector<Function *> functions;
  };


}