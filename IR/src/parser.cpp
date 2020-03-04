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

#include <IR.h>
#include <parser.h>

namespace pegtl = tao::TAO_PEGTL_NAMESPACE;

using namespace pegtl;
using namespace std;

namespace IR {

  /* 
   * Data required to parse
   */ 
  std::vector<Item> parsed_items;
  int numOfArgs = 0;
  int numOfDim = 0;

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


  /* 
   * Keywords.
   */
  struct str_arrow : TAOCPP_PEGTL_STRING( "<-" ) {};
  struct str_br : TAOCPP_PEGTL_STRING( "br" ) {};




/*
Operation strings
*/
  struct str_mult : TAOCPP_PEGTL_STRING( "*" ) {};
  struct str_sub : TAOCPP_PEGTL_STRING( "-" ) {};
  struct str_add : TAOCPP_PEGTL_STRING( "+" ) {};
  struct str_and : TAOCPP_PEGTL_STRING( "&" ) {};
  struct str_sright : TAOCPP_PEGTL_STRING( ">>" ) {};
  struct str_sleft : TAOCPP_PEGTL_STRING( "<<" ) {};
  struct str_less : TAOCPP_PEGTL_STRING( "<" ) {};
  struct str_lesseq : TAOCPP_PEGTL_STRING( "<=" ) {};
  struct str_eq : TAOCPP_PEGTL_STRING( "=" ) {};
  struct str_greater : TAOCPP_PEGTL_STRING( ">" ) {};
  struct str_greatereq : TAOCPP_PEGTL_STRING( ">=" ) {};
  struct str_inc : TAOCPP_PEGTL_STRING( "++" ) {};
  struct str_dec : TAOCPP_PEGTL_STRING( "--" ) {};

/*
Command strings
*/

  struct str_return : TAOCPP_PEGTL_STRING( "return" ) {};
  struct str_define : TAOCPP_PEGTL_STRING( "define" ) {};
  struct str_load : TAOCPP_PEGTL_STRING( "load" ) {};
  struct str_store : TAOCPP_PEGTL_STRING( "store" ) {};
  struct str_call : TAOCPP_PEGTL_STRING( "call" ) {};
  struct str_print : TAOCPP_PEGTL_STRING( "print" ) {};
  struct str_allocate : TAOCPP_PEGTL_STRING( "allocate" ) {};
  struct str_array : TAOCPP_PEGTL_STRING( "array-error" ) {};

  struct str_int64 : TAOCPP_PEGTL_STRING( "int64" ) {};
  struct str_brackets : TAOCPP_PEGTL_STRING( "[]" ) {};
  struct str_void : TAOCPP_PEGTL_STRING( "void" ) {};
  struct str_tuple : TAOCPP_PEGTL_STRING( "tuple" ) {};
  struct str_code : TAOCPP_PEGTL_STRING( "code" ) {};
  struct str_new_array : TAOCPP_PEGTL_STRING( "new Array" ) {};
  struct str_new_tuple : TAOCPP_PEGTL_STRING( "new Tuple" ) {};
  struct str_length : TAOCPP_PEGTL_STRING( "length" ) {};
   
  struct var:
    pegtl::seq<
      pegtl::one< '%' >,
      name
    > {};

  struct var_rule:
    var {};


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


  struct Label_rule:
    label {};

  struct Number_rule:
    number {};

  struct type_int64:
    pegtl::seq<
      str_int64,
      pegtl::star<
        str_brackets
      >
    >{};

  struct type_void:
    str_void {};

  struct type_tuple:
    str_tuple {};

  struct type_code:
    str_code {};

  struct type:
    pegtl::sor<
      type_int64,
      type_void,
      type_tuple,
      type_code
    >{};

  struct type_rule:
    type {};

  struct function_type_rule:
    type {};



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
      str_eq,
      str_greatereq,
      str_greater
    > {};

  struct Cmp_operators_rule:
    cmp_operators {};

  struct Instruction_cmp_rule:
    pegtl::seq<
      pegtl::sor<
        var_rule,
        Number_rule
      >,
      seps,
      Cmp_operators_rule,
      seps,
      pegtl::sor<
        var_rule,
        Number_rule
      >
    > {};

    struct Instruction_aop_rule:
    pegtl::seq<
      pegtl::sor<
        var_rule,
        Number_rule
      >,
      seps,
      pegtl::sor<
        Arithmatic_operators_rule,
        Shift_operators_rule
      >,
      seps,
      pegtl::sor<
        var_rule,
        Number_rule
      >
    > {};

    struct Instruction_assignment_dest_rule: // s {labels,numbers,registers,rsp}
    pegtl::sor<
      var_rule,
      Number_rule,
      Label_rule
    > {};



  struct Instruction_assignment_rule:
    pegtl::seq<
      var_rule,
      seps,
      str_arrow,
      seps,
      pegtl::sor<
        pegtl::seq<pegtl::at<Instruction_cmp_rule>, Instruction_cmp_rule>,
        pegtl::seq<pegtl::at<Instruction_aop_rule>, Instruction_aop_rule>,
        pegtl::seq<pegtl::at<Instruction_assignment_dest_rule>, Instruction_assignment_dest_rule>
      >  
    > {};


  struct Instruction_load_rule:
    pegtl::seq<
      var_rule,
      seps,
      str_arrow,
      seps,
      str_load,
      seps,
      var_rule
    > {};


  struct Instruction_store_rule:
    pegtl::seq<
      str_store,
      seps,
      var_rule,
      seps,
      str_arrow,
      seps,
      pegtl::sor<
        var_rule,
        Number_rule,
        Label_rule
      >
    > {};

  struct Instruction_return_rule:
    pegtl::seq<
      str_return
    > { };

  struct Instruction_return_t_rule:
    pegtl::seq<
      str_return,
      seps,
      pegtl::sor<
        var_rule,
        Number_rule
      >
    > { };

  struct Instruction_label_rule:
    label {};

  struct Instruction_br_rule:
    pegtl::seq<
      str_br,
      seps,
      Label_rule
    > { };

  struct Instruction_br_var_rule:
    pegtl::seq<
      str_br,
      seps,
      pegtl::sor<
        var_rule,
        Number_rule
      >,
      seps,
      Label_rule,
      seps,
      Label_rule
    > { };  

  struct Args_rule:
    pegtl::seq<
      seps,
      pegtl::sor<
        var_rule,
        Number_rule
      >,
      seps,
      pegtl::opt<
        pegtl::one< ',' >
      >,
      seps
    > { };

  struct Instruction_call_rule:
    pegtl::seq<
      str_call,
      seps,
      pegtl::sor<
        var_rule,
        Label_rule,
        str_print,
        str_allocate,
        str_array
      >,
      seps,
      pegtl::one< '(' >,
      seps,
      pegtl::star<
        Args_rule
      >,
      seps,
      pegtl::one< ')' >
    > { };  


  struct Instruction_assign_call_rule:
    pegtl::seq<
      var_rule,
      seps,
      str_arrow,
      seps,
      pegtl::seq<
        str_call,
        seps,
        pegtl::sor<
          var_rule,
          Label_rule,
          str_print,
          str_allocate,
          str_array
        >,
        seps,
        pegtl::one< '(' >,
        seps,
        pegtl::star<
          Args_rule
        >,
        seps,
        pegtl::one< ')' >
      >
    > { };

  struct Instruction_define_rule:
    pegtl::seq<
      type_rule,
      seps,
      var_rule
    >{};

  struct dimensions_rule:
    pegtl::seq<
      seps,
      pegtl::one< '[' >,
      seps,
      pegtl::sor<
        Number_rule,
        var_rule
      >,
      seps,
      pegtl::one< ']' >,
      seps
    >{};

  struct Instruction_array_dest_rule:
    pegtl::seq<
      var_rule,
      seps,
      pegtl::plus<
        dimensions_rule
      >,
      seps,
      str_arrow,
      seps,
      pegtl::sor<
        var_rule,
        Number_rule,
        Label_rule
      >,
      seps
    >{};


  struct Instruction_array_src_rule:
    pegtl::seq<
      var_rule,
      seps,
      str_arrow,
      seps,
      var_rule,
      seps,
      pegtl::plus<
        dimensions_rule
      >,
      seps
    >{};

  struct Instruction_new_array_rule:
    pegtl::seq<
      var_rule,
      seps,
      str_arrow,
      seps,
      str_new_array,
      seps,
      pegtl::one< '(' >,
      seps,
      pegtl::star<
        Args_rule
      >,
      seps,
      pegtl::one< ')' >,
      seps
    >{};

  struct Instruction_new_tuple_rule:
    pegtl::seq<
      var_rule,
      seps,
      str_arrow,
      seps,
      str_new_tuple,
      seps,
      pegtl::one< '(' >,
      seps,
      pegtl::sor<
        var_rule,
        Number_rule
      >,
      seps,
      pegtl::one< ')' >,
      seps
    >{};

    struct Instruction_length_rule:
    pegtl::seq<
      var_rule,
      seps,
      str_arrow,
      seps,
      str_length,
      seps,
      var_rule,
      seps,
      pegtl::sor<
        var_rule,
        Number_rule
      >,
      seps
    >{};

  struct Instruction_rule:
    pegtl::sor<
      pegtl::seq< pegtl::at<Instruction_new_tuple_rule>, Instruction_new_tuple_rule>,
      pegtl::seq< pegtl::at<Instruction_length_rule>, Instruction_length_rule>,
      pegtl::seq< pegtl::at<Instruction_new_array_rule>, Instruction_new_array_rule>,
      pegtl::seq< pegtl::at<Instruction_array_dest_rule>, Instruction_array_dest_rule>,
      pegtl::seq< pegtl::at<Instruction_array_src_rule>, Instruction_array_src_rule>,
      pegtl::seq< pegtl::at<Instruction_define_rule>, Instruction_define_rule>,
      pegtl::seq< pegtl::at<Instruction_assign_call_rule>, Instruction_assign_call_rule>,
      pegtl::seq< pegtl::at<Instruction_call_rule>, Instruction_call_rule>,
      pegtl::seq< pegtl::at<Instruction_assignment_rule>, Instruction_assignment_rule>,
      pegtl::seq< pegtl::at<Instruction_load_rule>, Instruction_load_rule>,
      pegtl::seq< pegtl::at<Instruction_store_rule>, Instruction_store_rule>
    > { };

  struct Instructions_rule:
    pegtl::star<
      pegtl::seq<
        seps,
        Instruction_label_rule,
        seps,
        pegtl::star<
          pegtl::seq<
            seps,
            Instruction_rule,
            seps
          >
        >,
        seps,
        pegtl::sor<
          pegtl::seq< pegtl::at<Instruction_br_rule>, Instruction_br_rule>,
          pegtl::seq< pegtl::at<Instruction_br_var_rule>, Instruction_br_var_rule>,
          pegtl::seq< pegtl::at<Instruction_return_t_rule>, Instruction_return_t_rule>,
          pegtl::seq< pegtl::at<Instruction_return_rule>, Instruction_return_rule>
        >,
        seps
      >
    > { };

  struct Vars_rule:
    pegtl::seq<
      type_rule,
      seps,
      var_rule,
      seps,
      pegtl::opt<
        pegtl::one< ',' >
      >,
      seps
    > { };

  struct function_define:
    pegtl::seq<
      function_type_rule,
      seps,
      function_name,
      seps
    >{};

  struct Function_rule:
    pegtl::seq<
      seps,
      str_define,
      seps,
      function_define,
      seps,
      pegtl::one< '(' >,
      seps,
      pegtl::star<
        Vars_rule
      >,
      seps,
      pegtl::one< ')' >,
      seps,
      pegtl::one< '{' >,
      seps,
      Instructions_rule,
      seps,
      pegtl::one< '}' >,
      seps
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
      Functions_rule,
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

  template<> struct action < function_define > {
    template< typename Input >
  static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      currentF->type = parsed_items.back();
      parsed_items.pop_back();
    }
  };

  template<> struct action < Vars_rule > {
    template< typename Input >
  static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      currentF->arguments.push_back(parsed_items.back());
      parsed_items.pop_back();
      currentF->argumentsType.push_back(parsed_items.back());
      parsed_items.pop_back();
    }
  };

  template<> struct action < Args_rule > {
    template< typename Input >
  static void apply( const Input & in, Program & p){
      numOfArgs++;
    }
  };

  template<> struct action < dimensions_rule > {
    template< typename Input >
  static void apply( const Input & in, Program & p){
      numOfDim++;
    }
  };  


  template<> struct action < Label_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      Item i;
      i.labelName = in.string();
      parsed_items.push_back(i);
    }
  };

  template<> struct action < type_int64 > {
    template< typename Input >
  static void apply( const Input & in, Program & p){
      Item i;
      i.isAType = true;
      i.labelName = in.string();
      parsed_items.push_back(i);
    }
  };

  template<> struct action < type_void > {
    template< typename Input >
  static void apply( const Input & in, Program & p){
      Item i;
      i.isAType = true;
      i.labelName = in.string();
      parsed_items.push_back(i);
    }
  };

  template<> struct action < type_tuple > {
    template< typename Input >
  static void apply( const Input & in, Program & p){
      Item i;
      i.isAType = true;
      i.labelName = in.string();
      parsed_items.push_back(i);
    }
  };

  template<> struct action < type_code > {
    template< typename Input >
  static void apply( const Input & in, Program & p){
      Item i;
      i.isAType = true;
      i.labelName = in.string();
      parsed_items.push_back(i);
    }
  };



  template<> struct action < str_print > {
    template< typename Input >
  static void apply( const Input & in, Program & p){
      Item i;
      i.labelName = "print";
      parsed_items.push_back(i);
    }
  };

  template<> struct action < str_allocate > {
    template< typename Input >
  static void apply( const Input & in, Program & p){
      Item i;
      i.labelName = "allocate";
      parsed_items.push_back(i);
    }
  };

  template<> struct action < str_array > {
    template< typename Input >
  static void apply( const Input & in, Program & p){
      Item i;
      i.labelName = "array-error";
      parsed_items.push_back(i);
    }
  };

  template<> struct action < Number_rule > {
    template< typename Input >
	static void apply( const Input & in, Program & p){
      Item i;
      i.isARegister = false;
      i.isANumber = true;
      int temp = stoi(in.string());
      i.labelName = to_string(temp);
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

  template<> struct action < var_rule > {
    template< typename Input >
  static void apply( const Input & in, Program & p){
      Item i;
      i.isAVar = true;
      i.labelName = in.string();
      parsed_items.push_back(i);
    }
  };

  template<> struct action < Instruction_define_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      auto i = new Instruction_define();
      i->var = parsed_items.back();
      parsed_items.pop_back();
      i->type = parsed_items.back();
      parsed_items.pop_back();
      currentF->instructions.push_back(i);
    }
  };

  template<> struct action < Instruction_length_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      auto i = new Instruction_length();
      i->arg = parsed_items.back();
      parsed_items.pop_back();
      i->src = parsed_items.back();
      parsed_items.pop_back();
      i->dest = parsed_items.back();
      parsed_items.pop_back();
      currentF->instructions.push_back(i);
    }
  };

  template<> struct action < Instruction_new_tuple_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      auto i = new Instruction_new_tuple();
      i->arg = parsed_items.back();
      parsed_items.pop_back();
      i->var = parsed_items.back();
      parsed_items.pop_back();
      currentF->instructions.push_back(i);
    }
  };

  template<> struct action < Instruction_new_array_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      auto i = new Instruction_new_array();
      while(numOfArgs > 0){
        i->arguments.push_back(parsed_items.back());
        parsed_items.pop_back();
        numOfArgs--;
      }
      std::reverse(i->arguments.begin(),i->arguments.end());
      i->var = parsed_items.back();
      parsed_items.pop_back();
      currentF->instructions.push_back(i);
    }
  };

  template<> struct action < Instruction_array_dest_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      auto i = new Instruction_array_dest();
      i->src = parsed_items.back();
      parsed_items.pop_back();
      while(numOfDim > 0){
        i->dimensions.push_back(parsed_items.back());
        parsed_items.pop_back();
        numOfDim--;
      }
      std::reverse(i->dimensions.begin(),i->dimensions.end());
      i->var = parsed_items.back();
      parsed_items.pop_back();
      currentF->instructions.push_back(i);
    }
  };

  template<> struct action < Instruction_array_src_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      auto i = new Instruction_array_src();
      while(numOfDim > 0){
        i->dimensions.push_back(parsed_items.back());
        parsed_items.pop_back();
        numOfDim--;
      }
      std::reverse(i->dimensions.begin(),i->dimensions.end());
      i->var = parsed_items.back();
      parsed_items.pop_back();
      i->dest = parsed_items.back();
      parsed_items.pop_back();
      currentF->instructions.push_back(i);
    }
  };

  template<> struct action < Instruction_assignment_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      auto i = new Instruction_assignment_cmp_aop();
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
    }
  }; 


  template<> struct action < Instruction_return_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      auto i = new Instruction_ret();
      currentF->instructions.push_back(i);
    }
  }; 

  template<> struct action < Instruction_return_t_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      auto i = new Instruction_ret_t();
      i->returnVar = parsed_items.back();
      parsed_items.pop_back();
      currentF->instructions.push_back(i);
    }
  }; 

  template<> struct action < Instruction_label_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      auto i = new Instruction_label();
      i->label.labelName = in.string();
      currentF->instructions.push_back(i);
    }
  };

  template<> struct action < Instruction_br_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      auto i = new Instruction_br();
      i->label = parsed_items.back();
      parsed_items.pop_back();
      currentF->instructions.push_back(i);
    }
  };

  template<> struct action < Instruction_br_var_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      auto i = new Instruction_br_var();
      i->labelFalse = parsed_items.back();
      parsed_items.pop_back();
      i->labelTrue = parsed_items.back();
      parsed_items.pop_back();
      i->var = parsed_items.back();
      parsed_items.pop_back();
      currentF->instructions.push_back(i);
    }
  }; 

  template<> struct action < Instruction_call_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      auto i = new Instruction_call();
      while(numOfArgs > 0){
        i->arguments.push_back(parsed_items.back());
        parsed_items.pop_back();
        numOfArgs--;
      }
      std::reverse(i->arguments.begin(),i->arguments.end());
      i->fname = parsed_items.back();
      parsed_items.pop_back();
      currentF->instructions.push_back(i);
    }
  };

  template<> struct action < Instruction_assign_call_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      auto i = new Instruction_assign_call();
      while(numOfArgs > 0){
        i->arguments.push_back(parsed_items.back());
        parsed_items.pop_back();
        numOfArgs--;
      }
      std::reverse(i->arguments.begin(),i->arguments.end());
      i->fname = parsed_items.back();
      parsed_items.pop_back();
      i->var = parsed_items.back();
      parsed_items.pop_back();
      currentF->instructions.push_back(i);
    }
  };
/*
  


  template<> struct action < Instruction_assignment_rule_src_mem_x_M > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      auto i = new instruction_assignment_mem_x_M();
      i->M = parsed_items.back();
      parsed_items.pop_back();
      i->x = parsed_items.back();
      parsed_items.pop_back();
      i->other = parsed_items.back();
      parsed_items.pop_back();
      i->isSrc = true;
      currentF->instructions.push_back(i);
    }
  }; 

  template<> struct action < Instruction_assignment_rule_dest_mem_x_M > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();

      auto i = new instruction_assignment_mem_x_M;
      i->other = parsed_items.back();
      parsed_items.pop_back();
      i->M = parsed_items.back();
      parsed_items.pop_back();
      i->x = parsed_items.back();
      parsed_items.pop_back();
      i->isSrc = false;
      currentF->instructions.push_back(i);
    }
  }; 



  template<> struct action < Instruction_aop_rule > {
    template< typename Input >
      static void apply( const Input & in, Program & p){
      auto currentF = p.functions.back();
      auto i = new instruction_aop;
      i->src = parsed_items.back();
      parsed_items.pop_back();
      i->operation = parsed_items.back();
      parsed_items.pop_back();
      i->dest = parsed_items.back();
      parsed_items.pop_back();
      currentF->instructions.push_back(i);
    }
  }; 


/*
instruction_sop_rule
*/


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


