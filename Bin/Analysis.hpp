/*
# ===============================================================
# About the code
# ===============================================================
# This code forms the 'Analysis' module' # 
# ===============================================================
*/

#ifndef Analysis_hpp
#define Analysis_hpp

#include <iostream>
#include <string>
#include "Eigen/Core"
#include "Eigen/Dense"
#include "Parser.hpp"
#include <vector>

Eigen::VectorXf Analysis(std::vector<CirElement> circuit);

#endif