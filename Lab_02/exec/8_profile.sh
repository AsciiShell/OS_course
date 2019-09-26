cat .profile | grep -v '#'

EDITOR=vi;      export EDITOR
PAGER=more;     export PAGER

ENV=$HOME/.shrc; export ENV

if [ -x /usr/games/fortune ] ; then /usr/games/fortune freebsd-tips ; fi
