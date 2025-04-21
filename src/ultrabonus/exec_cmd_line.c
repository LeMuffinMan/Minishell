
//si on dit qu'on doit d'abord parser toute la ligne comme bash : 
//
//export VAR=VAR && echo $VAR
//
//devrait ne rien afficher
//Alors que bash affiche VAR
//
//Si on fait un premier ast pour export VAR=VAR
//qu'on l'execute et qu'on stock son exit code
//
//on peut comparer avec l'ordre des seps si la suite est a executer ou non
//si oui : 
  //execute la cmd line qui suit
//si non :
 //on stop l'exec et on retourne le dernier exit code
//
//Je pense que les delirs de quotes et d'expands rendent les expands plus simple a gerer au parsing. apres je me rend pas compte.
//Et ca permet de maintenir l'environnement a jour entre deux blocs de commandes, 
//donc que ce soit au parsing ou a l'exec qu'on fera les expands, on peut garder notre logique actuelle

typedef struct s_exec_order 
{
  char **cmd_lines;
  char **boolean_operators;
} t_exec_order;

int exec_sequence(void)
{
  char *line = "echo coucou && ls -l || echo cat | cat | cat | cat | cat | cat | cat | cat && sleep 1";
  char **cmd_lines = {"echo coucou", "ls -l", "echo cat | cat | cat | cat | cat | cat | cat", "sleep 1"};
  //supposons qu'on ait pu produire la liste des separateurs qui conditionnent chaque cmd line dans l'ordre d'exec
  char **boolean_operators = {"&&", "||", "&&"};
  int i;
  t_tree *line_ast;
  int exit

  line_ast = NULL;
  i = 0;
  //sep a une case de moins logiquement, donc on boucle sur sep[i] pour pas segfault
  while (sep[i])
  {
    line_ast = parse(cmd_lines[i]);
    exit_code = exec_ast(&line_ast, env, /* toutes nos lists necessaires a l'exec .. */);
    free_tree(line_ast);
    line_ast = NULL;
    if ((exit_code != 0 && sep[i] == "&&") || (exit_code == 0 && sep[i] == "||"))
    {
      //Si la cmd line a echoue et que son operateur conditionnel etait && 
      //OU si la cmd line a marche et son operateur conditionnell etait ||
      break;
      //on stop l'execution de la ligne entiere et on rend le prompt
    }
    //sinon, on passe a la cmd line et son operateur conditionnell qui suivent en iterant avec i 
    i++;
  }
  //vu qu'on bouclait sur sep, on n'a pas execute la dernier cmd line 
  if ((exit_code == 0 && sep[last_sep] == "&&") || (exit_code != 0 && sep[last_sep] == "||"))
  {
    line_ast = parse(cmd_lines[i]);
    exit_code = exec_ast(&line_ast, env, /* toutes nos lists necessaires a l'exec .. */);
    free_tree(line_ast);
    line_ast = NULL;
  }
  return (exit_code);
}

int main (void)
{
  //si la ligne recue contient des && || ; :
  //exec_order = get_execution_order(line)
  exec_sequence(exec_order);
  //si la ligne recue ne contient pas de && || ;
  //on appelle parse et exec_ast comme on le fait deja
  //
  return (0);
}
