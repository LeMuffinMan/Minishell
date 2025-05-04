/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   history.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: oelleaum <oelleaum@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/04 20:47:32 by oelleaum          #+#    #+#             */
/*   Updated: 2025/05/04 21:00:16 by oelleaum         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

int add_history(char *cmd)
{
  //on ajoute une ligne en append au fichier history
  //si le fichier depasse les lignes / taille max : on suprrime la premiere ligne 
}

int read_history(t_var **env)
{
  //si on trouve une variable HisTFILE : on l'utilise
  //sinon on prend l'emplacement par defaut
  //on open
  //pour chaque ligne en commencant par en haut, on add_history
}

int save_history()
{
  //normal et avec signaux !
  //a l'arret :
  //on copie en trunc sur le fichier d'historique toute la liste chainee de la fin au debut jusqu'a la limite HIST_SIZE ou une limite par defaut
}

int builtin_history()
{
  //on open le fichier history et on affiche chaque ligne avec un numerottage au debut 
  //history N : Affiche les N dernières commandes (ex: history 10).
  //    history -c : Efface tout l'historique en mémoire (mais pas le fichier ~/.bash_history).

/*     history -d N : Supprime la commande à la ligne N de l'historique. */
/**/
/*     history -a : Écrit immédiatement les nouvelles commandes dans le fichier d'historique (~/.bash_history). */
/**/
/*     history -w : Écrase le fichier d'historique avec l'historique actuel en mémoire. */
/**/
/*     history -r : Recharge l'historique depuis le fichier ~/.bash_history. */
/**/
/**/
/*     !N : Exécute la commande avec le numéro N dans l'historique (ex: !42). */
/**/
/*     !! : Répète la dernière commande. */
/**/
/*     !-N : Exécute la commande qui était il y a N commandes (ex: !-2 pour l'avant-dernière). */
/**/
/*     !string : Exécute la dernière commande commençant par string (ex: !git). */
/**/
/*     !?string? : Exécute la dernière commande contenant string. */
/*       HISTFILE : Chemin du fichier sauvegardant l'historique (par défaut ~/.bash_history). */
/**/
/*     HISTSIZE : Nombre maximal de commandes conservées en mémoire pour la session courante. */
/**/
/*     HISTFILESIZE : Nombre maximal de commandes conservées dans le fichier d'historique. */
/**/
/*     HISTCONTROL : Contrôle le comportement de l'historique (ex: ignorespace pour ignorer les commandes commençant par un espace, ignoredups pour ignorer les doublons). */
/**/
/*     HISTIGNORE : Permet de définir un motif pour exclure des commandes de l'historique (ex: export HISTIGNORE="ls:cd:exit"). */
}
