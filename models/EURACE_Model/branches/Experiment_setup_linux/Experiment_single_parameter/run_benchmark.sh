#!/bin/bash

######### RUNNING THE INITIAL PHASE ###############################################################
# Script to run the first stage simulation:
# Runs the transient phase, and then saves the final iteration as transient.xml in the top folder
# Steps:
# - run in the top-level folder: ./its/0.xml (see output_last.xml)
# - save a full snapshot in the last iteration only (see output_last.xml)
# - join the node-*.xml files to a single file
# - the final xml is renamed to transient.xml
###################################################################################################
# Usage:
# - NR_NODES: number of nodes to use
# - ITS: number of iterations
# - MAIN: location of executable file
# - run
# - remove

echo '  Starting benchmark experiment...'

mkdir -p 'its'
cd ./its
for run in $RUNS; do
    mkdir -p 'run'$run
    cd 'run'$run
    cp $BASE/output_benchmark.xml ./
    echo '      Copied output file from' $BASE'/output_benchmark.xml to folder: run'$run
    #cp $BASE/output_benchmark_snapshot.xml ./
    #echo '      Copied output file from' $BASE'/output_benchmark_snapshot.xml to folder: run'$run
    echo '      Starting run for benchmark experiment...'

	#Run serial:
	#$MAIN_S $ITS output_benchmark_snapshot.xml 
	#Run parallel:
	#mpiexec -n $NR_NODES $MAIN_P $ITS 'output_benchmark_snapshot.xml' -r
	mpiexec -n $NR_NODES $MAIN_P $ITS 'output_benchmark.xml' -r

#	If the program exits:
#	if [ $? -ne 0 ]; then
#		echo $MAIN_S $ITS output_benchmark_snapshot.xml 
#		bash
#	fi 

	#Concatenating the node files
	echo '      Starting concatenation of the node output files'
	bash $BASE/join.sh ./ $NR_NODES
	echo '      Finished concatenation'

	#Removing the node files
	rm -f node*.xml
	echo '      Removed node output files'
    cd ..
done
cd ..

echo '  Finished benchmark experiment.'
