#!/usr/bin/python
import os

in_folder_name = "generator_in_files/"
list_file_name = "ComponentList.txt"
names_file_name = "ComponentNames.cpp"
includes_file_name = "IncludeComponents.hpp"
serialize_file_name = "ComponentSerialize.hpp"
serialize_cpp_file_name = "ComponentSerialize.cpp"

def findLineMatch(file_lines, match_string):
	counter = 0
	for l in file_lines:
		if match_string in l:
			del lines[counter];
			break;	
		else:
			counter += 1
	return counter

#scan filesystem for components
f_lists = open(in_folder_name + list_file_name, 'r')
lines = f_lists.read().splitlines()

components = []
for l in lines:
	components.append(l.split())

print(components)

#Generate ComponentNames.cpp--------------------------------------------------
f_in = open(in_folder_name+names_file_name+".in", 'r')
lines = f_in.read().splitlines()

match_i = findLineMatch(lines, "//INSERT1")
#insert components in a function must be backwards from actual thingy
lines.insert(match_i, '\t}') #insert final curly brace
for i in range(len(components)):
	c = components[i][0]
	lines.insert(match_i, 
		'\t\tgetToJson<{}>(reg, entity, j, "{}");'.format(c, c))
	if i == len(components)-1:
		lines.insert(match_i, 
			'\tif (type_text == "{}") {{'.format(c))
	else:
		lines.insert(match_i, 
			'\t}} else if (type_text == "{}") {{'.format(c))

match_i = findLineMatch(lines, "//INSERT2")
#insert components in a function must be backwards from actual thingy
lines.insert(match_i, '\t}') #insert final curly brace
for i in range(len(components)):
	c = components[i][0]
	lines.insert(match_i, 
		'\t\tgetFromJson<{}>(reg, entity, j, "{}");'.format(c, c))
	if i == len(components)-1:
		lines.insert(match_i, 
			'\tif (type_text == "{}") {{'.format(c))
	else:
		lines.insert(match_i, 
			'\t}} else if (type_text == "{}") {{'.format(c))

match_i = findLineMatch(lines, "//INSERT3")
for c in components:
	lines.insert(match_i, '\tgetToJson<{}>(reg, entity, j, "{}");'.format(c[0], c[0]))

match_i = findLineMatch(lines, "//INSERT4")
for c in components:
	lines.insert(match_i, '\tgetFromJson<{}>(reg, entity, j, "{}");'.format(c[0], c[0]))

print("writing new " + names_file_name)

f = open('src/' + names_file_name, 'w')
f.truncate(0)
counter = 0
for l in lines:
	print(l, file=f)
	counter += 1

print("wrote " + str(counter) + " lines to " + names_file_name)
f.close()
#Done generating ComponentNames.cpp-------------------------------------------













#Generate ComponentIncludes.hpp-----------------------------------------------
f_in = open(in_folder_name+includes_file_name+".in", 'r')
lines = f_in.read().splitlines()

match_i = findLineMatch(lines, "//INSERT1")

#insert components as includes
for c in components:
	lines.insert(match_i, '#include "components/{}.hpp"'.format(c[0]))

print("writing new " + includes_file_name)

f = open('src/' + includes_file_name, 'w')
f.truncate(0)
counter = 0
for l in lines:
	print(l, file=f)
	counter += 1

print("wrote " + str(counter) + " lines to " + includes_file_name)
f.close()
#Done generating ComponentIncludes.hpp----------------------------------------










#Generate ComponentSerialize.hpp----------------------------------------------
f_in = open(in_folder_name+serialize_file_name+".in", 'r')
lines = f_in.read().splitlines()

match_i = findLineMatch(lines, "//INSERT1")

#insert to and from json function declarations
for c in components:
	lines.insert(match_i, 'void to_json(json&, const {}&);'.format(c[0]))
	lines.insert(match_i, 'void from_json(const json&, {}&);'.format(c[0]))

print("writing new " + serialize_file_name)

f = open('src/' + serialize_file_name, 'w')
f.truncate(0)
counter = 0
for l in lines:
	print(l, file=f)
	counter += 1

print("wrote " + str(counter) + " lines to " + serialize_file_name)
f.close()
#Done generating ComponentSerialize.hpp---------------------------------------






#Generate ComponentSerialize.cpp----------------------------------------------
f_in = open(in_folder_name+serialize_cpp_file_name+".in", 'r')
lines = f_in.read().splitlines()

match_i = findLineMatch(lines, "//INSERT1")

#insert NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE
for c in components:
	to_insert = "NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(" + c[0]
	for v in c[1:]:
		to_insert = to_insert + ", " + v
	to_insert = to_insert + ")"
	lines.insert(match_i, to_insert)

print("writing new " + serialize_cpp_file_name)

f = open('src/' + serialize_cpp_file_name, 'w')
f.truncate(0)
counter = 0
for l in lines:
	print(l, file=f)
	counter += 1

print("wrote " + str(counter) + " lines to " + serialize_cpp_file_name)
f.close()
#Done generating ComponentSerialize.cpp---------------------------------------
