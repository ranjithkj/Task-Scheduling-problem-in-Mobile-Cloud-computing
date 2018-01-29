/* --------------------------------------------------------------
 Full MIP and Decompositions for the Cloud Computing Problem
 ---------------------------------------------------------------- */

#include <iostream>
#include <string>
#include <vector>
#include <ilcplex/ilocplex.h>
#include <ilcp/cp.h>
#include "problem.h"

using namespace std;

int main(){
	IloEnv env;
        cout<<"Hello;";
	try {
		Problem *prob; // Define problem class		
	    	prob = new (env) Problem(env);
		
		prob->solveProblem();

	} catch(IloException& e){
		env.out() << " ERROR: " << e << std::endl;
	}
        
	env.end();
	return 0;
}
