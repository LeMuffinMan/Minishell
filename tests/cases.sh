#!/bin/bash

# The case book: every line below is one input sequence replayed through
# minishell and through bash by compare.sh, which compares what comes out.
#
#   ./cases.sh                 run the suite
#   TEST_LEAKS=1 ./cases.sh    run it under valgrind too (much slower)

# shellcheck source-path=SCRIPTDIR
source "$(dirname "${BASH_SOURCE[0]}")/lib.sh"

resolve_minishell || exit 1
clean_stale_sandboxes

# valgrind runs the shell tens of times slower, so the budget follows the
# tool: at 10s a pipeline through grep times out under load and the suite
# reports a failure that is only the machine being busy.
if [[ ${TEST_LEAKS:-0} == 1 ]]; then
	TIMEOUT_DURATION="${TIMEOUT_DURATION:-60}"
else
	TIMEOUT_DURATION="${TIMEOUT_DURATION:-10}"
fi

# print what we have so far if the run is interrupted
trap 'print_summary; exit 130' INT

# check "pwd" "cd" "pwd"

#so make sure to gather arguments
# check "ls -l" "cd .." "ls -l"

#test pipe as an argument 
# check "ls -l | wc -l" "pwd"

#or even a redirection, but dont forget the -r flag first !
# check "-r" "echo -e '180g milk' > log/outfile" 

#valgrind is a switch now : TEST_LEAKS=1 ./cases.sh
# check "-r" "echo -e '5g vanilla' >> log/outfile"

#to use quotes, use \" or ' or \' ... 
# check "echo -e \"two eggs\"" "echo -e '100g sugar'" 

#test by yourself !
# check "your" "own"
# check "t" "e" "s" "t" "S"

# You can either add your own tests here manualy, or you can use the --add-case command, 
# the sequence in buffer will be added after this line, ready to run with ./cases.sh or --suite in the CLI.

# I recommend to keep this file clean and organized by sorting tests in categories here 
# Comments are your friends !
# check "pwd" "cd .." "pwd" "cd .." "pwd" "cd .." "pwd" "cd .." "pwd" "cd .." 
#
#
#

echo 
echo -e "${YELLOW} == Booleans operators ==$NC"
echo 
check "true && whoami" 
check "true || whoami" 
check "false && whoami" 
check "false || whoami" 
echo 
echo -e "${YELLOW}basics$NC"
echo 
check "true && echo ok"      
check "false || echo ok"        
check "true && echo A || echo B"        
check "false || echo A && echo B"     
check "false && echo A || echo B"   
echo 
echo -e "${YELLOW}natural priority tests$NC"
echo 
check "true || echo A && echo B"      
check "false && echo A || echo B"     
check "true && false || echo Fallback"  
check "ls && echo LS_OK"                
check "notacommand || echo FAILED"      
check "mkdir test && rm -r test"       
# echo  MANUAL !!
echo -e "${YELLOW}parenthesis$NC"
echo 
check "(true && echo A) || echo B"      
check "(false || echo A) && echo B"     
check "true && (false || echo NESTED)"  
echo 
echo -e "${YELLOW}edges cases : syntax error$NC"
echo 
check "true &&"                         
check "true && true && true &&"                         
check "&& true"                         
check "&& true && true && true"                         
check "|| true"                         
check "|| true || true || true"                         
check "true ||"                         
check "true || true ||true ||"                         
# check "(true && echo OK"               
check "true && echo OK)"               
check "echo A && echo B || echo C"      
check "false || false || echo LAST"    
echo 
echo -e "${YELLOW}boolop and exit_code$NC"
echo 
check "false || echo \$?"               
check "true && echo \$?"
check "cd /root || echo \$?"
check "cd /root && echo \$?"
check "cd /no_exiting_folder && echo \$?"
check "(false && true) || echo \$?"               
check "(false || true) || echo \$?"               
check "(false || true) && echo \$?"               

# echo MANUAL TESTS !!
# echo -e "${YELLOW}syntax error : not required$NC"
# echo 
# check "(true && )(false || echo NESTED) > out"  
# check "(true || false) | (false || echo NESTED)"  
# check "(true && false) | (false || echo NESTED)"  
# check "(false && echo) ls (false || echo NESTED)"  
# check "true (false || echo NESTED)"  
# check "true > (false || echo NESTED)"  
# check "true >> (false || echo NESTED)"  
# check "true | (false || echo NESTED)"  
# check "cat < (false || echo NESTED)"  
# check "< (false || echo NESTED) cat"  
echo 
echo -e "${YELLOW}combined$NC"
echo
echo -e "${YELLOW}Usage: Use parenthesis for priorities of '&&' and '||'$NC"
check "true && (false || (echo L1 && echo L2))"  # Devrait afficher "L1" puis "L2"
check "(false && echo A) || (true && echo B)"    # Devrait afficher "B"
check "false || (true && (false || echo DEEP))"  # Devrait afficher "DEEP"
check "false || echo \$?" 
echo -e "${YELLOW}======= BUILT_INS ======$NC"
echo
# ------------exit------------
echo
echo -e "${YELLOW}exit$NC"
echo
check "exit ghernjfw gthrnejwm" 
check "exit 256" 
check "exit 256999999999999999999999999999999999999999999999999999999999999999999999999999999999999999" 
check "exit 42" 
check "exit -1" 
check "exit -256" 
check "exit 1 2" 
check "exit not_numeric_argument" 
check "exit 1 not_numeric_argument" 
check "export EXIT=\"123\"" "exit \$EXIT"

echo -e "${YELLOW}export$NC"
echo

# odd identifiers
check "-r" "export HOL}A=bonjour" 
# check "-r" "export HO\`LA=bonjour" # backticks are out of the subject scope 
check "export TEST" "export | grep TEST" 
#Tom tests : 
check "export fdsfds fsdfdf 8 sdfsf"
# Abidolet tests :
check "export VAR=VAR @=VAR VAR1=VAR1 @=RAV" 
check "export VAR=VAR @=VAR VAR1=VAR1" 
check "export SHLVL+=1 | export grep SHLVL" 
check "export HOME=" "export | grep HOME" 
check "export VA1=\"COUCOU\"" 
check "export VAR=VAR" "export | grep VAR" 
check "export VAR=\"\"" "export | grep VAR" 
check "export VAR=\"\"" "export VAR+=\"123\"" "export | grep VAR" 
check "export VAR=\"123\"" "export | grep VAR" 
check "export VAR=\"123\"" "export | grep VAR" 
check "export VA1R=VAR" "export | grep VA1R" 
check "export VA+R=VAR" "export | grep VA+R" 
check "export VA*R=VAR" "export | grep VA*R" 
check "export VA-R=VAR" "export | grep VA-R" 
check "export VAR=VAR" "export | grep VAR" "unset VAT" "export | grep VAR" 
check "export VAR=VAR" "export | grep VAR" "unset VAR" "export | grep VAR" 
check "export VAR=VAR" "export VAR+=VAR" "export | grep VAR"
check "export SHLVL+=1" "export | grep SHLVL"
check "export VAR1=\"\" VAR2= VAR3=\"\$HOME\$USER\"" "export | grep VAR1" "export | grep VAR2" "export | grep VAR3"
check "export VAR=\"    .   V  A R    .   \"" "export | grep VAR" 
check "export VAR+=\"lolilol\"" "export | grep VAR" 
check "export =LOL" 
echo
echo -e "${YELLOW}unset$NC"
echo
echo -e "${YELLOW}expand avec UNSET ?$NC"
check "unset NOT_KNOWN_VALUE" 
check "unset USER" 
check "unset USER HOME SHLVL" 
check "unset USER HOME NOT_KNOWN_VALUE SHLVL" 
check "unset" 
echo 
echo -e "${YELLOW}cmd$NC"
echo 
check "whoami"
check "uname -a"
check "uname -m -n -r -s"
check "/usr/bin/uname -a"
check "non_existing_cmd"
check "/usr/bin/sudo"
check "/usr/bin/sudo -A"
check "/usr/bin/sudo -A apt update"
# known: when execve fails with 2, bash says "No such file or directory"
# where minishell says "Is a directory"
check "./usr/bin/sudo -A apt update"
check "cp /usr/bin/ls ." "./ls" "rm -rf ls"
# ------------echo------------
echo
echo -e "${YELLOW}echo$NC"
echo
check "echo" 
# check "echo -n -nn e" 
# known: two spaces printed where there should be one
# check "echo -n -nn -z o" 
# check "echo -nn o" 
# echo -e "${YELLOW}manque un espace derriere -nna$NC"
# check "echo -n -nna a" 
check "echo -nnnnnnnnnnnnnnnnnnnnnnnnnnn" 
# check "echo -n nnnnnnnnnnnnnnnnnnnnnnnnn" 
check "echo -c" 
check "echo -n" 
# check "echo -n hello" 
check "echo -n -n -n " 
# check "echo -n -n hello" 
check "echo hello" 
# known: two spaces printed where there should be one
check "echo hello world" 
check "echo \$HOME \$USER" 
check "echo \"\$HOME \$USER\"" 
check "echo \$HOME\$USER" 
check "echo \"\$HOME\$USER\""
check "echo \"\$HOME\$USER\""
check "echo \"\$HOME\"\$USER"
# a tester manuellement
# check "echo \'\$HOME\'\$USER"
check "echo \$NOTEXITING"
check "echo \"\$NOTEXITING\""
check "export VAR=\"    .    V  A  R    .    \"" "echo \"\$NOTEXITING\""
# ------------pwd------------
echo
echo -e "${YELLOW}pwd$NC"
echo
check "pwd" 
check "pwd with args" 
# ------------cd------------
echo
echo -e "${YELLOW}cd$NC"
echo
check "cd" 
check "cd \$HOME/.." "pwd" 
check "cd \$HOME/../." "pwd" 
check "cd ./././././././././././././log" "pwd" 
check "pwd .. ... ." 
check "cd test" "cat *.c" 
check "cd /" "pwd" "cd /home" "pwd" "cd $HOME" "pwd" 
check "cd /non_existing_folder"
check "cd /egerg" 
check "cd .." "pwd" "cd .." "pwd" "cd .." "pwd" "cd .." "pwd" "cd .." "pwd" "cd .." "pwd" "cd .." "pwd" 
check "pwd" "cd .." "pwd" 
check "cd .." "pwd" "cd " "pwd" 
check "cd log/ls_cpy" 
check "cd fwef feww" 
check "cd .." "cd -" 
check "cd -"
check "cd -" "cd -" 
check "cd .." "pwd" "cd -" "pwd" "cd .." "pwd" "cd -" "pwd" 
check "cd /home" "cd -" "cd -" 
check "cd .." "cd \$HOME" "pwd" 
echo
echo -e "${YELLOW}combined builtins$NC"
echo
check "export VAR=VAR" "unset VAR" "export | grep VAR" "env | grep VAR"
check "export HOME=\"/tmp\"" "cd \$HOME" "unset HOME" "export | grep HOME"
#
echo 
echo -e "${YELLOW}======= Pipes =======$NC"
echo 
check "whoami | cat"
check "uname -a | cat"
echo
check "whoami | grep"
check "whoami | cat | cat -e | cat | cat | cat"
check "whoami | cat | cat | cat | cat | cat"
check "whoami | cat | cat | wc -l | cat | cat"
check "whoami | cat | cat | wc -l | grep | cat"
check "whoami | cat | uname -a | wc -l | grep | cat"
check "|"
check "| wc -l"
check "uname -a | cat |"

echo 
echo -e "${YELLOW}======= Redirections =======$NC"
echo 
echo
echo -e "${YELLOW}syntax$NC"
check "-r" "> log/outfile"
check "-r" ">"
check "-r" "whoami >"
check "-r" "uname -a >>"
echo
echo -e "${YELLOW}permissions$NC"
check "-r" "whoami > log"
check "-r" "whoami > log/file_without_permissions"
check "-r" "whoami >> log/file_without_permissions"
check "-r" "whoami < log/file_without_permissions"
check "-r" "< log/file_without_permissions whoami"
echo
echo -e "${YELLOW}trunc$NC"
check "-r" "whoami > log/outfile"
echo
echo -e "${YELLOW}infile$NC"
check "-r" "< log/infile cat"
check "-r" "cat < log/infile"
check "-r" "< log/infile"
echo
echo -e "${YELLOW}append$NC"
# check "-r" "whoami >> log/outfile"
check "-r" "whoami > log/outfile" "whoami >> log/outfile"
echo
echo -e "${YELLOW}multiple redirections$NC"
check "-r" "whoami > log/outfile" "whoami >> log/outfile"
check "-r" "< log/non_existing_file cat < log/infile"
check "-r" "< log/file_without_permissions cat < log/infile"
check "-r" "< log/infile cat < log/non_existing_file"
check "-r" "< log/infile cat < log/file_without_permissions"
check "-r" "< log/outfile < log/file1 < log/infile cat"
check "-r" "< log/outfile < log/non_existing_file < log/infile cat"
check "-r" "< log/non_existing_file < log/outfile < log/infile cat"
check "-r" "< log/file_without_permissions < log/outfile < log/infile cat"
check "-r" "< log/outfile < log/file_without_permissions < log/infile cat"
echo
check "-r" "whoami > log/file1 > log/file2 > log/outfile" "whoami > log/file1 > log/file2 >> log/outfile"
# check "-r" "whoami > log/file1 > log/file_without_permissions > log/outfile" "whoami > log/file1 > log/file2 >> log/outfile"
check "-r" "whoami > log/file1 > log/file_without_permissions > log/outfile" "whoami > log/file_without_permissions > log/file2 >> log/outfile"
echo
check "-r" "< log/infile cat > log/outfile"
check "-r" "cat < log/infile > log/outfile"

# echo 
# echo "Environnement"
# echo 
#
echo 
echo -e "${YELLOW}combined pipes and redirections$NC"
echo 
check "-r" "< log/outfile < log/file_without_permissions < log/infile cat | cat | cat | cat | wc -l | cat > log/file1 > log/outfile"
check "-r" "< log/infile cat | cat > log/outfile | cat | cat | wc -l | cat > log/file1"
check "-r" "< log/file1 cat | cat | < log/infile cat | cat | wc -l | cat > log/file1"
check "-r" "< log/infile cat | cat | < log/infile cat | cat | wc -l | cat > log/outfile"
check "-r" "cat < log/infile > log/outfile | wc -l"
check "-r" "whoami | cat | cat | cat < log/infile > log/outfile"
check "-r" "whoami | cat < log/infile > log/outfile"
# echo 
# echo "Expands"
# echo 
echo 
echo -e  "${YELLOW}TO FIX$NC"
echo 
check "mkdir a" "mkdir b" "cd a" "cd ../b" "rm -rf ../a" "cd -" "cd .." "rm -rf b" 
check "mkdir -p a/b/c" "cd a/b/c" "rm -rf ../../../a" "cd .." "pwd" "cd .." "pwd" "cd .." "pwd" "cd .." "pwd" 
check "." 
check ".." 
check "../.." 
check "../../" 
check "< log/infile ." 
check "< log/infile .." 
check "mkdir a" "mkdir b" "cd a" "cd ../b" "rm -rf ../a" "cd -" "rm -rf ../b" 
echo 
echo -e "${YELLOW} == EXPAND | WILDCARD ==$NC"
echo 
check "touch test test1 test2 test3 salut test4 coucou test5" "mkdir dir_test1 dir_test2 dir_test3" "echo *dir"
check "touch test test1 test2 test3 salut test4 coucou test5" "mkdir dir_test1 dir_test2 dir_test3" "echo *1"
check "touch test test1 test2 test3 salut test4 coucou test5" "mkdir dir_test1 dir_test2 dir_test3" "echo te*3"
check "echo *" 
# check "echo *." 
check "export a= b=hey c= d=\" ca va\"" "echo \$a\$b\$c\$d" 
check "export a=ls" "\$a" 
# check "export a=|<>" "\$a" 
echo 
echo -e "${YELLOW}basics$NC"
echo 
check "echo \$" 
check "unset PWD" "cd " 
check  "echo test lol" 
# echo -e "${YELLOW}apres un unset de PATH, les builtins ne sont plus reconnus correctement$NC"
check "unset PATH" "unset USER" 
# echo -e "${YELLOW}/usr/bin/cp: cannot create regular file '/usr/bin/cat': Permission denied$NC"
check "cp minishell cat" "cat Makefile" "rm -rf cat"
# echo -e "${YELLOW}ls est considere comme file$NC"
check "cp /usr/bin/ls ." "chmod 000 ls" "ls" "chmod 777 ls" "rm -rf ls" 
check "export PATH=\$PWD\":\"\$PATH" 
# check "export VAR=\"echo hi | sleep 3\"" "export | grep VAR" "\$VAR" 
check "export a=\"o hello\"" "ech\$a" 
# echo -e "${YELLOW}STDERR custom !$NC"
# check "echo | \"d\" | \"\"" 
echo 
echo -e  "${YELLOW}TOO MUCH ?$NC"
echo 
check "cmd_not_found" "echo \$?" 
check "cmd_not_found || echo \$?" 
check "/usr/bin/sudo " "echo \$?" 
check "/usr/bin/sudo || echo \$?" 
check "echo 'a''b'" 
check "export=|><" 
check "ls > out error_to_print_for_this_arg" 
check "ls|ls" 
check "-r" "echo>log/outfile" 
check "-r" "ls < > log/outfile" 
check "/////////ls" 
check "minishell" 
check "/" 
check "/./../../../../.." 
check "-" 
check "\"\"''echo hola\"\"'''' que\"\"'' tal\"\"''" 
check "echo \$? | echo \$? | echo \$?" 
check "echo > <" 
check "echo | |" 
check "echo \$*" 
check "echo \"\$HO\"ME" 
check "echo \"\$HO\"\"ME\"" 
check "echo hola\"\"\"\"\"\"\"\"\"\"\"\"" 
check "echo hola''''''''''''" 
check "e\"cho hola\"" 
check "echo \"hola     \" | cat -e" 
check "\"e\"'c'ho 'b'\"o\"nj\"o\"'u'r" 
check "\"\"e\"'c'ho 'b'\"o\"nj\"o\"'u'r\"" 
check "echo \"\$DONTEXIST\"Makefile" 
check "\$?" 
check "\$?\$?" 
check "?\$HOME" 
check "echo \"\$\"\"\"" 
echo 
echo -e  "${YELLOW}Mandatory ?$NC"
echo 
# check "echo \$\"\"" 
# check "echo \"\" \$HOME" 
# check "./minishell" 
# check "echo \$:\$= | cat -e" 
# check "echo \" \$ \" | cat -e" 
check "echo '*'" 
check "echo *" 
check "echo t*" 
# check "echo \$HOME *" 
check "echo \"'\"h'o'la\"'\"" 
check "echo '''ho\"''''l\"a'''" 
check "\$HOME" 
check "mkdir a && cd a && mkdir b && cd b && mkdir c && cd c" "cd ../../../ && rm -rf a" 

print_summary
