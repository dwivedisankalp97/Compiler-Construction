#!/bin/bash

# Fetch the inputs
if test $# -lt 3 ; then
  echo "USAGE: `basename $0` EXTENSION_FILE COMPILER TESTS_DIR" ;
  exit 1;
fi
extFile=$1 ;
compiler=$2 ;
testsDir=$3 ;

# Define the variables
origDir=`pwd` ;

# Check the tests directory
if ! test -d ${testsDir} ; then
  echo "The directory \"${testsDir}\" does not exist. Please create it and place your new tests there." ;
  exit 1 ;
fi

# Run the tests
passed=0 ;
failed=0 ;
testNumber=0 ;
alltests=0 ;
testsFailed="" ;
cd ${testsDir} ; 
for i in *.${extFile} ; do
  let alltests=$alltests+1 ;

  # If the output already exists, skip the current test
  if ! test -f ${i}.out ; then
    continue ;
  fi
  echo -n -e "\rTest ${testNumber}: $i                          " ;
  let testNumber=${testNumber}+1 ;

  # Generate the binary
  pushd ./ &> /dev/null ;
  cd ${origDir} ;
  didSucceed=0 ;
  ./${compiler} ${testsDir}/${i} ;
  if test $? -eq 0 ; then
    ./a.out &> ${testsDir}/${i}.out.tmp ;
    cmp ${testsDir}/${i}.out.tmp ${testsDir}/${i}.out ;
    if test $? -eq 0 ; then
      didSucceed=1 ;
    fi
  fi
  if test $didSucceed == "1" ; then
    let passed=$passed+1 ;
  else
    let failed=$failed+1 ;
    testsFailed="${i} ${testsFailed}";
  fi
  popd &> /dev/null ; 
done
let total=$passed+$failed ;
echo "" ;
echo "" ;

# Check corner cases
if test $total -eq 0 -a $alltests -gt 0; then
  echo "Warning: there are tests in \"${testsDir}\" but they do not have their oracle output" ;
  echo "" ;
  echo "Please create their oracle outputs in the following way. ";
  echo "Create a text file called like the test name with the \".out\" suffix and store it in \"${testsDir}\". ";
  echo "For example, if a test is called \"mytest.${extFile}\" then you need to have the oracle file \"${testsDir}/mytest.${extFile}.out\"" ;
  exit 0;
fi

# Print summary
echo "########## SUMMARY" ;
if test "${testsFailed}" != "" ; then
  echo "Failed tests: ${testsFailed}" ;
fi
echo "Test passed: $passed out of $total"
