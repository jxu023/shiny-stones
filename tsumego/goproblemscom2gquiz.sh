#!/bin/sh

# goproblemscom2gquiz.sh
# Get Go-problems from goproblems.com and put them to right directories
# according to difficulty and genre:
# http://www.goproblems.com/

# After that you can easily use them with gquiz (and cgoban1):
# http://nis.acs.uci.edu/~strombrg/software/gquiz.html
# http://cgoban1.sourceforge.net/

##################################################################
#
# Folding used. Get folding.el for your emacs here:
# http://www.csd.uu.se/~andersl/emacs.shtml
# (folding.el comes with latest versions of XEmacs)
#
# To use folding under Vim put this to your ~/.vimrc
# set foldmethod=marker
#
##################################################################

# Copyright (C) 20003 Juhapekka Tolvanen
# http://iki.fi/juhtolv
# juhtolv (at) iki (dot) fi

# This program is free software; you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation; either version 2 of the License, or (at
# your option) any later version.

# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
# General Public License for more details.

# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111-1307
# USA

##################################################################
# System requirements:
#
# basename
# egrep
# mkdir
# sed
# seq
# tail (GNU-version)
# wget, curl, dog, lynx, links or w3m. (You can change it to pavuk,
# getleft etc., because you have this source code)
#

# Usage: Change that MYDIR if needed and then run this script.

##################################################################
# You may want to change this:

MYDIR="${HOME}/seka/go/problems/goproblemscom2gquiz"

##################################################################

mkdir -p $MYDIR 2> /dev/null
cd $MYDIR

# {{{ Environment variables

if gtail --help 2>&1 | grep -i "gnu.org" > /dev/null 2>&1
then
    TAIL_BIN="gtail"
elif tail --help 2>&1 | grep -i "gnu.org" > /dev/null 2>&1
then
    TAIL_BIN="tail"
else
    echo "You do not have GNU tail"
    exit 1
fi

# AFAIK star is faster and better than GNU tar
# http://www.fokus.gmd.de/research/cc/glone/employees/joerg.schilling/private/star.html
if star --version 2>&1 | grep -i "Schilling" > /dev/null 2>&1
then
    TAR_BIN='star -time -fifostats '
    echo "Cool... You have star, not just GNU tar or plain tar"
elif gtar --help 2>&1 | grep -i "gnu.org" > /dev/null 2>&1
then
    TAR_BIN="gtar"
    echo "Cool... You have GNU tar, not just plain tar"
elif tar --help 2>&1 | grep -i "gnu.org" > /dev/null 2>&1
then
    TAR_BIN="tar"
    echo "Cool... You have GNU tar, not just plain tar"
else
    echo "star or GNU tar is not installed."
    echo "Support for other tar-implementations is not yet available."
    exit 1
fi

# }}}

# {{{ Get and unpack files

if type wget
then
    wget http://www.goproblems.com/mkoff/goproblems.tgz http://www.goproblems.com/mkoff/goproblemsSGF.tgz    
elif type curl > /dev/null 2>&1
then
    curl http://www.goproblems.com/mkoff/goproblems.tgz > goproblems.tgz 
    curl http://www.goproblems.com/mkoff/goproblemsSGF.tgz > goproblemsSGF.tgz
elif dog --help 2>&1 | grep no-header > /dev/null 2>&1
then
    dog --no-header http://www.goproblems.com/mkoff/goproblems.tgz > goproblems.tgz
    dog --no-header http://www.goproblems.com/mkoff/goproblemsSGF.tgz > goproblemsSGF.tgz
elif type lynx > /dev/null 2>&1
then
    lynx --source --dump http://www.goproblems.com/mkoff/goproblems.tgz > goproblems.tgz
    lynx --source --dump http://www.goproblems.com/mkoff/goproblemsSGF.tgz > goproblemsSGF.tgz
elif type elinks > /dev/null 2>&1
then
    links -dump -source http://www.goproblems.com/mkoff/goproblems.tgz > goproblems.tgz
    links -dump -source http://www.goproblems.com/mkoff/goproblemsSGF.tgz > goproblemsSGF.tgz
elif type w3m > /dev/null 2>&1
then
    w3m -dump_source http://www.goproblems.com/mkoff/goproblems.tgz > goproblems.tgz
    w3m -dump_source http://www.goproblems.com/mkoff/goproblemsSGF.tgz > goproblemsSGF.tgz
else
    echo "I am confused: It seems you do not have software for retrieving files installed."
    echo "I tried: wget, curl, dog, lynx, links and w3m."
    echo "Please, install at least one of them or ask your sys admin to install at least one of them"
    exit 1
fi

for f in goproblems.tgz goproblemsSGF.tgz
do
    gzip -dc "$f" | $TAR_BIN -x -v -v -v -v -f -
    rm -f "$f"
done

# }}}

cd goproblemsSGF

# {{{ Move files in right directories

LASTNUMBER="$(basename `ls ????.sgf | $TAIL_BIN -n 1` .sgf)"

echo "LASTNUMBER=${LASTNUMBER}"

cd ..

for n in `seq 1 $LASTNUMBER`
do
    if test -f goproblemsSGF/${n}.sgf
    then
        DIFFICULTY="`egrep -A 2 '^\ \ Difficulty\:$' goproblems.com/prob${n}.html | $TAIL_BIN -n 1 | sed 's/^.*(//;s/).*$//'`"
        GENRE="`egrep -A 3 '^\ \ Genre\:$' goproblems.com/prob${n}.html | $TAIL_BIN -n 1 | sed 's/\ /_/g'`"
        echo "${n}.sgf : ${DIFFICULTY} : ${GENRE}"
        if test ! -d "./${DIFFICULTY}"
        then
            echo mkdir "./${DIFFICULTY}"
            mkdir "./${DIFFICULTY}"
        fi
        if test ! -d "./${DIFFICULTY}/${GENRE}"
        then
            echo mkdir "./${DIFFICULTY}/${GENRE}"
            mkdir "./${DIFFICULTY}/${GENRE}"
        fi
        mv goproblemsSGF/${n}.sgf "./${DIFFICULTY}/${GENRE}/"
    else
        echo "${n}.sgf do not exist"
    fi
done

# }}}

rm -rf goproblems.com/  goproblemsSGF/


