#!/bin/sh
#generate CMakeLists.txt based on files with .cpp suffix. 
#Can only go one layer deep tho, its not recursive and
#i dont really need more than one layer of depth anyway
cd ./src

CMAKELISTS="CMakeLists.txt"
SRC_FILES=$(ls --format=single-column *.cpp)
echo "target_sources(\${PROJECT_NAME} PRIVATE " > $CMAKELISTS
for j in $SRC_FILES 
do
	echo -e "\t${j}" >> $CMAKELISTS
done
echo ")" >> $CMAKELISTS

for i in $(ls -d */)
do
	if [[ "$i" != "old/" ]]; then #exclude the outdated files directory
		cd ${i}
		#redirect stderr to null in case no cpp files exist
		SUBDIR_FILES=$(ls --format=single-column *.cpp 2>/dev/null)
		#if no cpp files are found, dont echo
		if [ -n "$SUBDIR_FILES" ]; then
			echo "add_subdirectory(${i%%/})" >> ../$CMAKELISTS
			echo "target_sources(\${PROJECT_NAME} PRIVATE " > $CMAKELISTS
			for j in $SUBDIR_FILES 
			do
				echo -e "\t$j" >> $CMAKELISTS
			done
			echo ")" >> $CMAKELISTS
		fi
		cd ../
	fi
done
