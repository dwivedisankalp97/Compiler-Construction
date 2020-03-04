#pragma once

#include <vector>
#include <string>

using namespace std;

namespace IR {

  enum Register {rdi, rax, rbx, rbp, r10, r11, r12, r13, r14, r15, rsi, r8, r9, rcx, rdx, rsp};
  enum IType {none,ret,assign,load,assign_cmp_aop,\
    br_var,labelType,call,assign_call,array,allocate,inc_dec,\
    store,br,ret_t,define,array_dest,array_src,new_array,new_tuple,length};


  struct Item {
    std::string labelName;
    Register r;
    bool isANumber;
    bool isARegister;
    bool isAVar;
    bool isAOperator;
    bool isAType;
    bool isAArray;
    Item() : isANumber(false),isAVar(false),isARegister(false),isAOperator(false),isAType(false),isAArray(false){};
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
    return "return\n";
  }
  };

  struct Instruction_define : Instruction{
  IType iType(){
    return define;
  }

    Item type, var;

  string tostring(){
    return type.labelName + " " + var.labelName + "\n";
  }
  };

  struct Instruction_ret_t : Instruction{
  IType iType(){
    return ret_t;
  }

    Item returnVar;

  string tostring(){
    return "return " + returnVar.labelName + "\n";
  }
  };

  struct Instruction_br : Instruction{
  IType iType(){
    return br;
  }
    Item label;

  string tostring(){
    return "br " + label.labelName + "\n";
  }
  };

  struct Instruction_br_var : Instruction{
  IType iType(){
    return br_var;
  }
    Item labelTrue,labelFalse,var;

  string tostring(){
    return "br " + var.labelName + " " + labelTrue.labelName + " " + labelFalse.labelName + "\n";
  }
  };

  struct Instruction_assignment : Instruction{
	IType iType(){
		return assign;
	}
    Item src,dst;

  string tostring(){
    return dst.labelName + " <- " + src.labelName + "\n";
  }
  };


  struct Instruction_assignment_cmp_aop : Instruction{
	IType iType(){
		return assign_cmp_aop;
	}
    Item left,lc,operation,rc;

  string tostring(){
    return left.labelName + " <- " + lc.labelName + " " + operation.labelName + " " +rc.labelName + "\n";
  }
  };

  struct Instruction_label : Instruction{
	IType iType(){
		return labelType;
	}
    Item label;
  string tostring(){
    return label.labelName + "\n";
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
    if(arguments.size()>0)
      s.pop_back();
    s+=")";
    return s + "\n";
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
    string s = var.labelName + " <- ";
    s += "call " + fname.labelName + " (";;
    for(auto i : arguments){
      s+=i.labelName;
      s+=",";
    }
    if(arguments.size()>0)
      s.pop_back();
    s+=")";
    return s + "\n";
  }
  };

  struct Instruction_array_dest : Instruction{
  IType iType(){
    return array_dest;
  }
    Item var,src;
    std::vector<Item> dimensions;

  string tostring(){
    return var.labelName + " " + to_string(dimensions.size()) + " <- " + src.labelName + "\n";
  }
  };

  struct Instruction_array_src : Instruction{
  IType iType(){
    return array_src;
  }
    Item var,dest;
    std::vector<Item> dimensions;

  string tostring(){
    return  dest.labelName + " <- " + var.labelName + " " + to_string(dimensions.size()) + "\n";
  }
  };

  struct Instruction_new_array :Instruction{
  IType iType(){
    return new_array;
  }
    Item var;
    std::vector<Item> arguments;

  string tostring(){
    string s = var.labelName + " <- ";
    s += "new array ";
    s+=" (";
    for(auto i : arguments){
      s+=i.labelName;
      s+=",";
    }
    s+=")";
    return s + "\n";
  }
  };

  struct Instruction_new_tuple : Instruction{
  IType iType(){
    return new_tuple;
  }
    Item var,arg;

  string tostring(){
    return  var.labelName + " <- new Tuple(" + arg.labelName  + ")\n";
  }
  };

  struct Instruction_length : Instruction{
  IType iType(){
    return length;
  }
    Item dest,src,arg;

  string tostring(){
    return  dest.labelName + " <- length " + " " + src.labelName + " " + arg.labelName  + "\n";
  }
  };



  /*
   * Function.
   */
  struct Function{
    std::string name;
    Item type;
    std::vector<Item> argumentsType;
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