#!/bin/bash
#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

#
# NOTE: C Operators which are from page#735 of book
#	C Programming: A Modern Approach, Second Edition
#

#
# Usage: ./getCOperators
#    OR  ./getCOperators '->' '&'
#    OR  ./getCOperators '\.'
#    OR  ./getCOperators '\*'
#    OR  ./getCOperators '\?'
#

f_out=/tmp/COperators.txt.$$
trap "rm -f $f_out" exit

cat > $f_out << EOF
APPENDIX  A
C Operators

--------------------------------------------------------------------------------
Precedence              Name                    Symbol(s)       Associativity
--------------------------------------------------------------------------------
    1           Array subscripting              []              Left
    1           Function call                   ()              Left
    1           Structure and union member      .  ->           Left
    1           Increment (postfix)             ++              Left
    1           Decrement (postfix)             --              Left
--------------------------------------------------------------------------------
    2           Increment (prefix)              ++              Right
    2           Decrement (prefix)              --              Right
    2           Address                         &               Right
    2           Indirection                     *               Right
    2           Unary plus                      +               Right
    2           Unary minus                     -               Right
    2           Bitwise complement              ~               Right
    2           Logical negation                !               Right
    2           Size                            sizeof          Right
--------------------------------------------------------------------------------
    3           Cast                            ()              Right
--------------------------------------------------------------------------------
    4           Multiplicative                  *  /  %         Left
--------------------------------------------------------------------------------
    5           Additive                        +  -            Left
--------------------------------------------------------------------------------
    6           Bitwise shift                   <<  >>          Left
--------------------------------------------------------------------------------
    7           Relational                      <  >  <=  >=    Left
--------------------------------------------------------------------------------
    8           Equality                        ==  !=          Left
--------------------------------------------------------------------------------
    9           Bitwise and                     &               Left
--------------------------------------------------------------------------------
   10           Bitwise exclusive or            ^               Left
--------------------------------------------------------------------------------
   11           Bitwise inclusive or            |               Left
--------------------------------------------------------------------------------
   12           Logical and                     &&              Left
--------------------------------------------------------------------------------
   13           Logical or                      ||              Left
--------------------------------------------------------------------------------
   14           Conditional                     ?:              Right
--------------------------------------------------------------------------------
   15           Assignment                      =  *=  /=  %=   Right
                                                += -=  <<= >>=
                                                &= ^=  |=
--------------------------------------------------------------------------------
   16           Comma                           ,               Left
--------------------------------------------------------------------------------

                                                                             735
EOF

if (( $# == 0 )); then
	cat -n $f_out
else
	egrep --color=auto "Precedence|Name|Symbol\(s\)|Associativity" $f_out
	for x in "$@"; do
		egrep --color=auto -- " $x " $f_out
	done
fi

exit 0
