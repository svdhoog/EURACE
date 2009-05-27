#!/bin/sh

###########################################################################
# This script is automatically generated by libexp.py. You *may* make     #
# changes but the next time libexp.py is run, your changes will get lost. #
###########################################################################

mpiexec -n 4 /media/DOCS/Docs/EURACE/X-models/SVN_linux/models/EURACE_Model/branches/Hybrid_Model_A/main 200 output.xml

NODES="0 1 2 3"

#Construct the ITS list:
ITS=''
LIMIT=10;
i=0
for ((j=0; j<LIMIT; j++)); do
	let i=i+20
	ITS=$ITS' '$i
done
echo $ITS

#Concatenating the node files
for it in $ITS; do
	echo nodeN-$it'.xml''->'$it'.xml'
	for n in $NODES; do
		cat node$n-$it.xml >> $it.xml
	done
done
rm node*.xml