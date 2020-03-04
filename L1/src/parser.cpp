#include <sched.h>
#include <string>
#include <vector>
#include <utility>
#include <algorithm>
#include <set>
#include <iterator>
#include <cstring>
#include <cctype>
#include <cstdlib>
#include <stdint.h>
#include <assert.h>

#include <tao/pegtl.hpp>
#include <tao/pegtl/analyze.hpp>
#include <tao/pegtl/contrib/raw_string.hpp>

#include <L1.h>
#include <parser.h>

namespace pegtl = tao::TAO_PEGTL_NAMESPACE;

using namespace pegtl;
using namespace std;

namespace L1 {

  /* 
   * Data required to parse
   */ 
  std::vector<Item> parsed_items;

  /* 
   * Grammar rules from now on.
   */
  struct name:
    pegtl::seq<
      pegtl::plus< 
        pegtl::sor<
          pegtl::alpha,
          pegtl::one< '_' >
        >
      >,
      pegtl::star<
        pegtl::sor<
          pegtl::alpha,
          pegtl::one< '_' >,
          pegtl::digit
        >
      >
    > {};

  /* 
   * Keywords.
   */
  struct str_arrow : TAOCPP_PEGTL_STRING( "<-" ) {};
  struct str_return : TAOCPP_PEGTL_STRING( "return" ) {};

/*
Register Strings w
*/

  struct str_rax : TAOCPP_PEGTL_STRING( "rax" ) {};
  struct str_rbx : TAOCPP_PEGTL_STRING( "rbx" ) {};
  struct str_rbp : TAOCPP_PEGTL_STRING( "rbp" ) {};
  struct str_r10 : TAOCPP_PEGTL_STRING( "r10" ) {};
  struct str_r11 : TAOCPP_PEGTL_STRING( "r11" ) {};
  struct str_r12 : TAOCPP_PEGTL_STRING( "r12" ) {};
  struct str_r13 : TAOCPP_PEGTL_STRING( "r13" ) {};
  struct str_r14 : TAOCPP_PEGTL_STRING( "r14" ) {};
  struct str_r15 : TAOCPP_PEGTL_STRING( "r15" ) {};

/*
Register Strings a + sx
*/
  struct str_rdi : TAOCPP_PEGTL_STRING( "rdi" ) {};
  struct str_rsi : TAOCPP_PEGTL_STRING( "rsi" ) {};
  struct str_r8 : TAOCPP_PEGTL_STRING( "r8" ) {};
  struct str_r9 : TAOCPP_PEGTL_STRING( "r9" ) {};
  struct str_rcx : TAOCPP_PEGTL_STRING( "rcx" ) {};
  struct str_rdx : TAOCPP_PEGTL_STRING( "rdx" ) {};

/*
Register String rsp
*/
  struct str_rsp : TAOCPP_PEGTL_STRING( "rsp" ) {};

/*
Operation strings
*/
  struct str_mem : TAOCPP_PEGTL_STRING( "mem" ) {};
  struct str_mult : TAOCPP_PEGTL_STRING( "*=" ) {};
  struct str_sub : TAOCPP_PEGTL_STRING( "-=" ) {};
  struct str_add : TAOCPP_PEGTL_STRING( "+=" ) {};
  struct str_and : TAOCPP_PEGTL_STRING( "&=" ) {};
  struct str_sright : TAOCPP_PEGTL_STRING( ">>=" ) {};
  struct str_sleft : TAOCPP_PEGTL_STRING( "<<=" ) {};
  struct str_less : TAOCPP_PEGTL_STRING( "<" ) {};
  struct str_lesseq : TAOCPP_PEGTL_STRING( "<=" ) {};
  struct str_eq : TAOCPP_PEGTL_STRING( "=" ) {};
  struct str_inc : TAOCPP_PEGTL_STRING( "++" ) {};
  struct str_dec : TAOCPP_PEGTL_STRING( "--" ) {};
  struct str_at : TAOCPP_PEGTL_STRING( "@" ) {};

/*
Command strings
*/

  struct str_cjump : TAOCPP_PEGTL_STRING( "cjump" ) {};
  struct str_goto : TAOCPP_PEGTL_STRING( "goto" ) {};
  struct str_call : TAOCPP_PEGTL_STRING( "call" ) {};
  struct str_print : TAOCPP_PEGTL_STRING( "print 1" ) {};
  struct str_allocate : TAOCPP_PEGTL_STRING( "allocate 2" ) {};
  struct str_array : TAOCPP_PEGTL_STRING( "array-error 2" ) {};


  struct register_rdi_rule:
      str_rdi {};

  struct register_rax_rule:
      str_rax {};

  struct register_rbx_rule:
      str_rbx {};

  struct register_rbp_rule:
      str_rbp {};

  struct register_r10_rule:
      str_r10 {};

  struct register_r11_rule:
      str_r11 {};

  struct register_r12_rule:
      str_r12 {};

  struct register_r13_rule:
      str_r13 {};

  struct register_r14_rule:
      str_r14 {};

  struct register_r15_rule:
      str_r15 {};

  struct register_rsi_rule:
      str_rsi {};

  struct register_r8_rule:
      str_r8 {};

  struct register_r9_rule:
      str_r9 {};

  struct register_rcx_rule:
      str_rcx {};

  struct register_rdx_rule:
      str_rdx {};

  struct register_rsp_rule:
      str_rsp {};
   
  struct register_rule: // w
    pegtl::sor<
      register_rdi_rule,
      register_rax_rule,
      register_rbx_rule,
      register_rbp_rule,
      register_r10_rule,
      register_r11_rule,
      register_r12_rule,
      register_r13_rule,
      register_r14_rule,
      register_r15_rule,
      register_rsi_rule,
      register_r8_rule,
      register_r9_rule,
      register_rcx_rule,
      register_rdx_rule
    > {};

  struct label:
    pegtl::seq<
      pegtl::one<':'>,
      name
    > {};

  struct number:
    pegtl::seq<
      pegtl::opt<
        pegtl::sor<
          pegtl::one< '-' >,
          pegtl::one< '+' >
        >
      >,
      pegtl::plus< 
        pegtl::digit
      >
    >{};

  struct function_name:
    label {};

  struct argument_number:
    number {};

  struct local_number:
    number {} ;

  struct comment: 
    pegtl::disable< 
      TAOCPP_PEGTL_STRING( "//" ), 
      pegtl::until< pegtl::eolf > 
    > {};

  struct seps: 
    pegtl::star< 
      pegtl::sor< 
        pegtl::ascii::space, 
        comment 
      > 
    > {};

  struct Label_rule:
    label {};

  struct Number_rule:
    number {};

  struct mem_x_M:
    pegtl::seq<
      seps,
      str_mem,
      seps,
      pegtl::sor<
        register_rule,
        register_rsp_rule
      >,
      seps,
      Number_rule
    > {};

  struct arithmatic_operators:
    pegtl::sor<
      str_mult,
      str_add,
      str_sub,
      str_and
    > {};

  struct Arithmatic_operators_rule:
    arithmatic_operators {};

  struct shift_operators:
    pegtl::sor<
      str_sleft,
      str_sright
    > {};

  struct Shift_operators_rule:
    shift_operators {};

  struct cmp_operators:
    pegtl::sor<
      str_lesseq,
      str_less,
      str_eq
    > {};

  struct Cmp_operators_rule:
    cmp_operators {};

/*
INSTRUCTION RULES
*/

  struct Instruction_label_rule:
    label {};

  struct Instruction_return_rule:
    pegtl::seq<
      str_return
    > { };

  struct Instruction_assignment_dest_rule: // s {labels,numbers,registers,rsp}
    pegtl::sor<
      register_rule,
      register_rsp_rule,
      Number_rule,
      Label_rule
    > {};

  struct Instruction_cmp_rule:
    pegtl::seq<
      pegtl::sor<
        register_rule,
        Number_rule
      >,
      seps,
      Cmp_operators_rule,
      seps,
      pegtl::sor<
        register_rule,
        Number_rule
      >
    > {};


  struct Instruction_assignment_cmp_rule:
    pegtl::seq<
      register_rule,
      seps,
      str_arrow,
      seps,
      Instruction_cmp_rule
    > {};

  struct Instruction_assignment_only_rule:
      pegtl::seq<
        register_rule,
        seps,
        str_arrow,
        seps,
        Instruction_assignment_dest_rule
      > {};


  struct Instruction_assignment_rule:
    pegtl::seq<
      register_rule,
      seps,
      str_arrow,
      seps,
      pegtl::sor<
        pegtl::seq<pegtl::at<Instruction_cmp_rule>, Instruction_cmp_rule>,
        pegtl::seq<pegtl::at<Instruction_assignment_dest_rule>, Instruction_assignment_dest_rule>
      >  
    > {};

  struct Instruction_assignment_rule_src_mem_x_M:
    pegtl::seq<
      register_rule,
      seps,
      str_arrow,
      seps,
      mem_x_M
    > {};


  struct Instruction_assignment_rule_dest_mem_x_M:
    pegtl::seq<
      mem_x_M,
      seps,
      str_arrow,
      seps,
      Instruction_assignment_dest_rule
    > {};

  struct Instruction_aop_rule:
    pegtl::seq<
      register_rule,
      seps,
      Arithmatic_operators_rule,
      seps,
      pegtl::sor<
        register_rule,
        register_rsp_rule,
        Number_rule
      >
    > {};

  struct Instruction_sop_rule:
    pegtl::seq<
      register_rule,
      seps,
      Shift_operators_rule,
      seps,
      pegtl::sor<
        register_rcx_rule,
        Number_rule
      >
    > {};

  struct Instruction_aop_rule_left_mem_x_M:
    pegtl::seq<
      mem_x_M,
      seps,
      Arithmatic_operators_rule,
      seps,
      pegtl::sor<
        register_rule,
        register_rsp_rule,
        Number_rule
      >
    > {};

  struct Instruction_aop_rule_right_mem_x_M:
    pegtl::seq<
      register_rule,
      seps,
      Arithmatic_operators_rule,
      seps,
      mem_x_M
    > {};

  struct Instruction_lea_rule:
    pegtl::seq<
      register_rule,
      seps,
      str_at,
      seps,
      register_rule,
      seps,
      register_rule,
      seps,
      Number_rule
    > {};  


/*  struct Instruction_assignment_cmp_rule:
    pegtl::seq<
      register_rule,
      seps,
      str_arrow,
      seps,
      Instruction_cmp_rule
    > {};
*/
  struct Instruction_cjump_rule:
    pegtl::seq<
      str_cjump,
      seps,
      Instruction_cmp_rule,
      seps,
      Label_rule
    > {};


  struct Instruction_goto_rule:
    pegtl::seq<
      str_goto,
      seps,
      Label_rule
    > {};
      
  struct Instruction_call_rule:
    pegtl::seq<
      str_call,
      seps,
      pegtl::sor<
        register_rule,
        Label_rule
      >,
      seps,
      Number_rule
    > {};

  struct Instruction_print_rule:
    pegtl::seq<
      str_call,
      seps,
      str_print
    > {};

  struct Instruction_allocate_rule:
    pegtl::seq<
      str_call,
      seps,
      str_allocate
    > {};

  struct Instruction_array_rule:
    pegtl::seq<
      str_call,
      seps,
      str_array
    > {};

  struct Instruction_inc_rule:
    pegtl::seq<
      register_rule,
      seps,
      str_inc
    > {};

  struct Instruction_dec_rule:
    pegtl::seq<
      register_rule,
      seps,
      str_dec
    > {};

  struct Instruction_rule:
    pegtl::sor<
      pegtl::seq< pegtl::at<Instruction_return_rule>, Instruction_return_rule>,
      pegtl::seq< pegtl::at<Instruction_assignment_rule>, Instruction_assignment_rule>,
      pegtl::seq< pegtl::at<Instruction_assignment_rule_src_mem_x_M>, Instruction_assignment_rule_src_mem_x_M>,
      pegtl::seq< pegtl::at<Instruction_assignment_rule_dest_mem_x_M>, Instruction_assignment_rule_dest_mem_x_M>,
      pegtl::seq< pegtl::at<Instruction_aop_rule>, Instruction_aop_rule>,
      pegtl::seq< pegtl::at<Instruction_sop_rule>, Instruction_sop_rule>,
      pegtl::seq< pegtl::at<Instruction_aop_rule_left_mem_x_M>, Instruction_aop_rule_left_mem_x_M>,
      pegtl::seq< pegtl::at<Instruction_aop_rule_right_mem_x_M>, Instruction_aop_rule_right_mem_x_M>,
      pegtl::seq< pegtl::at<Instruction_cjump_rule>, Instruction_cjump_rule>,
      pegtl::seq< pegtl::at<Instruction_goto_rule>, Instruction_goto_rule>,
      pegtl::seq< pegtl::at<Instruction_label_rule>, Instruction_label_rule>,
      pegtl::seq< pegtl::at<Instruction_call_rule>, Instruction_call_rule>,
      pegtl::seq< pegtl::at<Instruction_print_rule>, Instruction_print_rule>,
      pegtl::seq< pegtl::at<Instruction_allocate_rule>, Instruction_allocate_rule>,
      pegtl::seq< pegtl::at<Instruction_array_rule>, Instruction_array_rule>,
      pegtl::seq< pegtl::at<Instruction_inc_rule>, Instruction_inc_rule>,
      pegtl::seq< pegtl::at<Instruction_dec_rule>, Instruction_dec_rule>,
      pegtl::seq< pegtl::at<Instruction_lea_rule>, Instruction_lea_rule>
    > { };

  struct Instructions_rule:
    pegtl::plus<
      pegtl::seq<
        seps,
        Instruction_rule,
        seps
      >
    > { };

  struct Function_rule:
    pegtl::seq<
      pegtl::one< '(' >,
      seps,
      function_name,
      seps,
      argument_number,
      seps,
      local_number,
      seps,
      Instructions_rule,
      seps,
      pegtl::one< ')' >
    > {};

  struct Functions_rule:
    pegtl::plus<
      seps,
      Function_rule,
      seps
    > {};

  struct entry_point_rule:
    pegtl::seq<
      seps,
      pegtl::one< '(' >,
      seps,
      label,
      seps,
      Functions_rule,
      seps,
      pegtl::one< ')' >,
      seps
    > { };

  struct grammar : 
    pegtl::must< 
      entry_point_rule
    > {};

  /* 
   * Actions attached to grammar rules.
   */
  template< typename Rule >
  struct action : pegtl::nothing< Rule > {};

  template<> struct action < label > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      if (p.entryPointLabel.empty()){
        p.entryPointLabel = in.string();
      } else {
        abort();
      }
    }
  };

  template<> struct action < function_name > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      auto newF = new Function();
      newF->name = in.string();
      p.functions.push_back(newF);
    }
  };

  template<> struct action < argument_number > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      if (currentF->arguments == -1){
        currentF->arguments = std::stoll(in.string());
      }
    }
  };

  template<> struct action < local_number > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      if (currentF->locals == -1){
        currentF->locals = std::stoll(in.string());
      }
    }
  };

  template<> struct action < str_return > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      auto i = new Instruction_ret();
      currentF->instructions.push_back(i);
    }
  };


  template<> struct action < Label_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      Item i;
      i.isARegister = false;
      i.isANumber = false;
      i.labelName = in.string();
      parsed_items.push_back(i);
    }
  };

  template<> struct action < Number_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      Item i;
      i.isARegister = false;
      i.isANumber = true;
      i.labelName = in.string();
      parsed_items.push_back(i);
    }
  };

  template<> struct action < Arithmatic_operators_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      Item i;
      i.isAOperator = true;
      i.labelName = in.string();
      parsed_items.push_back(i);
    }
  };

  template<> struct action < Shift_operators_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      Item i;
      i.isAOperator = true;
      i.labelName = in.string();
      parsed_items.push_back(i);
    }
  };

  template<> struct action < Cmp_operators_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      Item i;
      i.isAOperator = true;
      i.labelName = in.string();
      parsed_items.push_back(i);
    }
  };

/*
REGISTER Actions
*/
  template<> struct action < register_rdi_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      Item i;
      i.isARegister = true;
      i.r = rdi;
      parsed_items.push_back(i);
    }
  };

  template<> struct action < register_rax_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      Item i;
      i.isARegister = true;
      i.isANumber = false;
      i.r = rax;
      parsed_items.push_back(i);
    }
  };

  template<> struct action < register_rbx_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      Item i;
      i.isARegister = true;
      i.isANumber = false;
      i.r = rbx;
      parsed_items.push_back(i);
    }
  };

  template<> struct action < register_rbp_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      Item i;
      i.isARegister = true;
      i.isANumber = false;
      i.r = rbp;
      parsed_items.push_back(i);
    }
  };

  template<> struct action < register_r10_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      Item i;
      i.isARegister = true;
      i.isANumber = false;
      i.r = r10;
      parsed_items.push_back(i);
    }
  };

  template<> struct action < register_r11_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      Item i;
      i.isARegister = true;
      i.isANumber = false;
      i.r = r11;
      parsed_items.push_back(i);
    }
  };

  template<> struct action < register_r12_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      Item i;
      i.isARegister = true;
      i.isANumber = false;
      i.r = r12;
      parsed_items.push_back(i);
    }
  };

  template<> struct action < register_r13_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      Item i;
      i.isARegister = true;
      i.isANumber = false;
      i.r = r13;
      parsed_items.push_back(i);
    }
  };

  template<> struct action < register_r14_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      Item i;
      i.isARegister = true;
      i.isANumber = false;
      i.r = r14;
      parsed_items.push_back(i);
    }
  };

  template<> struct action < register_r15_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      Item i;
      i.isARegister = true;
      i.isANumber = false;
      i.r = r15;
      parsed_items.push_back(i);
    }
  };

  template<> struct action < register_rsi_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      Item i;
      i.isARegister = true;
      i.isANumber = false;
      i.r = rsi;
      parsed_items.push_back(i);
    }
  };

  template<> struct action < register_r8_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      Item i;
      i.isARegister = true;
      i.isANumber = false;
      i.r = r8;
      parsed_items.push_back(i);
    }
  };

  template<> struct action < register_r9_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      Item i;
      i.isARegister = true;
      i.isANumber = false;
      i.r = r9;
      parsed_items.push_back(i);
    }
  };

  template<> struct action < register_rcx_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      Item i;
      i.isARegister = true;
      i.isANumber = false;
      i.r = rcx;
      parsed_items.push_back(i);
    }
  };

  template<> struct action < register_rdx_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      Item i;
      i.isARegister = true;
      i.isANumber = false;
      i.r = rdx;
      parsed_items.push_back(i);
    }
  };

  template<> struct action < register_rsp_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      Item i;
      i.isARegister = true;
      i.isANumber = false;
      i.r = rsp;
      parsed_items.push_back(i);
    }
  };

/*
REGISTER Actions FINISH
*/
/*
Instruction Rules
*/
/*  template<> struct action < Instruction_assignment_cmp_rule > {
    template< typename Input >
	  static void apply( const Input & in, Program & p){

 
      auto currentF = p.functions.back();


      auto i = new Instruction_assignment();
      i->src = parsed_items.back();
      parsed_items.pop_back();
      i->dst = parsed_items.back();
      parsed_items.pop_back();


      currentF->instructions.push_back(i);
    }
  };
*/
/* 
mem_x_M Rules
*/

  template<> struct action < Instruction_assignment_rule_src_mem_x_M > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /* 
       * Create the instruction.
       */ 
      auto i = new instruction_assignment_mem_x_M();
      i->M = parsed_items.back();
      parsed_items.pop_back();
      i->x = parsed_items.back();
      parsed_items.pop_back();
      i->other = parsed_items.back();
      parsed_items.pop_back();
      i->isSrc = true;
      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
    }
  }; 

  template<> struct action < Instruction_assignment_rule_dest_mem_x_M > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /* 
       * Create the instruction.
       */ 
      auto i = new instruction_assignment_mem_x_M;
      i->other = parsed_items.back();
      parsed_items.pop_back();
      i->M = parsed_items.back();
      parsed_items.pop_back();
      i->x = parsed_items.back();
      parsed_items.pop_back();
      i->isSrc = false;
      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
    }
  }; 


/*
instruction_aop rule
*/

  template<> struct action < Instruction_aop_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /* 
       * Create the instruction.
       */ 
      auto i = new instruction_aop;
      i->src = parsed_items.back();
      parsed_items.pop_back();
      i->operation = parsed_items.back();
      parsed_items.pop_back();
      i->dest = parsed_items.back();
      parsed_items.pop_back();
      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
    }
  }; 


/*
instruction_sop_rule
*/

  template<> struct action < Instruction_sop_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /* 
       * Create the instruction.
       */ 
      auto i = new instruction_sop;
      i->rightop = parsed_items.back();
      parsed_items.pop_back();
      i->operation = parsed_items.back();
      parsed_items.pop_back();
      i->leftop = parsed_items.back();
      parsed_items.pop_back();
      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
    }
  }; 

/*
mem x M aop rules
*/

  template<> struct action < Instruction_aop_rule_left_mem_x_M > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /* 
       * Create the instruction.
       */ 
      auto i = new instruction_aop_mem_x_M;
      i->other = parsed_items.back();
      parsed_items.pop_back();
      i->operation = parsed_items.back();
      parsed_items.pop_back();
      i->M = parsed_items.back();
      parsed_items.pop_back();
      i->x = parsed_items.back();
      parsed_items.pop_back();
      i->isLeft = true;
      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
    }
  }; 

  template<> struct action < Instruction_aop_rule_right_mem_x_M > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /* 
       * Create the instruction.
       */ 
      auto i = new instruction_aop_mem_x_M();
      i->M = parsed_items.back();
      parsed_items.pop_back();
      i->x = parsed_items.back();
      parsed_items.pop_back();
      i->operation = parsed_items.back();
      parsed_items.pop_back();
      i->other = parsed_items.back();
      parsed_items.pop_back();
      i->isLeft = false;
      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
    }
  }; 

/*
cmp rules
*/

  template<> struct action < Instruction_assignment_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /* 
       * Create the instruction.
       */ 
      auto i = new Instruction_assignment_cmp();
      i->rc = parsed_items.back();
      parsed_items.pop_back();
      if (parsed_items.back().isAOperator != true){
	auto i1 = new Instruction_assignment();
	i1->src = i->rc;
	i1->dst = parsed_items.back();
	parsed_items.pop_back();
	currentF->instructions.push_back(i1);
      }
      else {
        i->operation = parsed_items.back();
        parsed_items.pop_back();
        i->lc = parsed_items.back();
        parsed_items.pop_back();
        i->left = parsed_items.back();
        parsed_items.pop_back();
        currentF->instructions.push_back(i);
      }
      /* 
       * Add the just-created instruction to the current function.
       */ 
      
    }
  }; 

  template<> struct action < Instruction_cjump_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /* 
       * Create the instruction.
       */ 
      auto i = new Instruction_cjump();
      i->label = parsed_items.back();
      parsed_items.pop_back();
      i->rc = parsed_items.back();
      parsed_items.pop_back();
      i->operation = parsed_items.back();
      parsed_items.pop_back();
      i->lc = parsed_items.back();
      parsed_items.pop_back();
      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
    }
  }; 

/*
Label instruction
*/

  template<> struct action < Instruction_label_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      /* 
       * Fetch the current function.
       */ 
      auto currentF = p.functions.back();

      /* 
       * Create the instruction.
       */ 
      auto i = new Instruction_label();
      i->label.labelName = in.string();
      /* 
       * Add the just-created instruction to the current function.
       */ 
      currentF->instructions.push_back(i);
    }
  }; 

/*
Goto instruction
*/

  template<> struct action < Instruction_goto_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      auto i = new Instruction_goto();
      i->label = parsed_items.back();
      parsed_items.pop_back();
      currentF->instructions.push_back(i);
    }
  };

/*
Call Instruction
*/

  template<> struct action < Instruction_call_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      auto i = new Instruction_call();
      i->argn = parsed_items.back();
      parsed_items.pop_back();
      i->fname = parsed_items.back();
      parsed_items.pop_back();
      currentF->instructions.push_back(i);
    }
  };

  template<> struct action < Instruction_print_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      auto i = new Instruction_print();
      currentF->instructions.push_back(i);
    }
  };

  template<> struct action < Instruction_array_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      auto i = new Instruction_array();
      currentF->instructions.push_back(i);
    }
  };

  template<> struct action < Instruction_allocate_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      auto i = new Instruction_allocate();
      currentF->instructions.push_back(i);
    }
  };

/*

inc and dec instructions
*/


  template<> struct action < Instruction_inc_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      auto i = new Instruction_inc_dec();
      i->reg = parsed_items.back();
      i->isInc = true;
      parsed_items.pop_back();
      currentF->instructions.push_back(i);
    }
  };

  template<> struct action < Instruction_dec_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      auto i = new Instruction_inc_dec();
      i->reg = parsed_items.back();
      i->isInc = false;
      parsed_items.pop_back();
      currentF->instructions.push_back(i);
    }
  };

  template<> struct action < Instruction_lea_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      auto i = new Instruction_lea();
      i->fourth = parsed_items.back();
      parsed_items.pop_back();
      i->third = parsed_items.back();
      parsed_items.pop_back();
      i->second = parsed_items.back();
      parsed_items.pop_back();
      i->first = parsed_items.back();
      parsed_items.pop_back();
      currentF->instructions.push_back(i);
    }
  };


//-----------------------------------------------------------------------------------
Program parse_file (char *fileName){

    /* 
     * Check the grammar for some possible issues.
     */
    pegtl::analyze< grammar >();

    /*
     * Parse.
     */   
    file_input< > fileInput(fileName);
    Program p;
    parse< grammar, action >(fileInput, p);

    return p;
  }

}


