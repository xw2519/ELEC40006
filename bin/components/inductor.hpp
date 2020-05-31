/*
# ===================================================================================
# About the code
# ===================================================================================
# This code forms the 'Analysis' module' # 
# ===================================================================================
*/

#ifndef inductor_hpp
#define inductor_hpp

#include "edge.hpp"

class inductor : public edge
{
    private: // ---------------------------------------------------------------------

        double 
            L_value,
            integral_value, // Current value in this case
            g_value,
            Next_edge_I;
        
    public: //-----------------------------------------------------------------------

        /* Class operations */
        inductor(std::string in_name,int in_p_N,int in_n_N,double in_l_value)
        {
            ID='l'; name=in_name;
            p_N=in_p_N; 
            n_N=in_n_N;
            L_value=in_l_value;
            Prev_edge_I=0;
            g_value=0;
            Next_edge_I=0;
        };

        ~inductor(){delete this;};

        /* Get operations for base variables */
        double Get_L(){return this->L_value;}

        /* Simulation-related functions */
        void Set_g_value(double delta){g_value=(delta/L_value);};
        double Get_g(){return g_value;};

        void Set_next_I(double Prev_edge_V){Next_edge_I = Prev_edge_I + (g_value*Prev_edge_V);}
        double Get_next_I(){return Next_edge_I;};

        void Set_prev_I(){Prev_edge_I = Next_edge_I;};
        double Get_prev_I(){return Prev_edge_I;};

        /* Output operations */
        void print_edge()
        {
            std::cout<<"ID: "<<ID<<" name: "<<name<< " P terminal: "<<p_N
            <<" N terminal: "<<n_N<<" Edge voltage: "<<Prev_edge_V<<" Edge current: "
            <<Prev_edge_I<<" Inductance: "<<L_value<<std::endl;
        };
};

#endif