set -e

g++ -I /path/to/Eigen/ OP_Analysis.cpp ../../ELEC40006/Bin/Parser.cpp ../../ELEC40006/Bin/Analysis.cpp  -o OP_Analysis


./OP_Analysis < ../../ELEC40006/Tests/Data_struct_input.txt 
