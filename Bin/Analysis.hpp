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
#include <complex>

Eigen::VectorXf OP_Analysis(std::vector<CirElement> circuit, std::vector<CirSrc> sources);

#endif