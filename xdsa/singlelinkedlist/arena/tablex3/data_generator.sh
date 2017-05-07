#!/bin/bash
#
# Copyright (C) 2017, Vector Li (idorax@126.com). All rights reserved.
#

#
# Create sample datum for testing
#

D_DATA=$(egrep "#define.*D_DATA" foo.h | \
	awk '{print $NF}' | sed 's/"//g')
F_DATA_STUDENTS=$(egrep "#define.*F_DATA_STUDENTS" foo.h | \
	awk '{print $NF}' | sed 's/"//g')
F_DATA_COURSES=$(egrep "#define.*F_DATA_COURSES"   foo.h | \
	awk '{print $NF}' | sed 's/"//g')
F_DATA_SCORES=$(egrep "#define.*F_DATA_SCORES"     foo.h | \
	awk '{print $NF}' | sed 's/"//g')

[[ -d $D_DATA ]] || mkdir $D_DATA

cat > $D_DATA/$F_DATA_STUDENTS <<- EOF
	#sid	#name
	1	John
	2	Smith
	3	Tom
	4	Amy
	5	Alice
EOF

cat > $D_DATA/$F_DATA_COURSES <<- EOF
	#name		#cid
	math		4
	english		1
	art		2
EOF

cat > $D_DATA/$F_DATA_SCORES <<- EOF
	#cid	#sid	#score
	1	1	80
	1	2	85
	1	3	90
	1	4	85
	1	5	50

	2	1	50
	2	2	85
	2	3	90
	2	4	85
	2	5	50

	4	1	80
	4	2	85
	4	3	90
	4	4	85
	4	5	80
EOF
