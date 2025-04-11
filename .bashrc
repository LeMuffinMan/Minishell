#
# ~/.bashrc
#

alias ls='ls --color=auto'
alias grep='grep --color=auto'
alias echo='echo -n'
alias gs='git status'            
alias ga='git add .'            
alias gc='git commit -m'       
alias gp='git push'              
alias gpo='git push origin'      
alias gst='git stash'            
alias gpl='git pull'              

PS1='[\u@\h \W]\$ '
PS2='>'

# export VAR=var
# unset HOME

cdmk() 
{
  mkdir -p $1
  cd $1
}

# --- Navigation et gestion de répertoires ---
mkcd() {
    mkdir -p "$1" && cd "$1"
}

# --- Raccourci Git ---
gitpush() {
    git add . && git commit -m "$1" && git push
}



# --- Gestion des processus ---
# killp() {
#     pid=$(ps aux | grep "$1" | grep -v grep | awk '{print $2}')
#     if [ -n "$pid" ]; then
#         kill -9 "$pid"
#         echo "Processus $1 (PID: $pid) tué."
#     else
#         echo "Aucun processus trouvé avec ce nom."
#     fi
# }

# --- Météo (nécessite curl) ---
meteo() {
    curl "wttr.in/$1"
}

# --- Conversion de devises (nécessite jq et curl) ---
currency() {
    curl -s "https://api.exchangerate-api.com/v4/latest/$2" | jq ".rates.$3 * $1"
}

# --- Sauvegarde rapide ---
backup() {
    cp "$1" "$1.bak"
}

# --- Calculatrice simple ---
calc() {
    echo "$1" | bc -l
}

# --- Récupérer l'IP publique ---
myip() {
    curl ifconfig.me
}
