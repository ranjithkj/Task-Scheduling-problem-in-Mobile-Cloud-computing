/* --------------------------------------------------------------
   MIP for the Cloud Computing Problems
   ---------------------------------------------------------------- */
// Header Files

#include <vector>
#include "problem.h"
#include <math.h>
#include <ilcplex/ilocplex.h>
#include <ilcp/cp.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <algorithm>
#include <time.h>
#include <ctime>

using namespace std;

ILOSTLBEGIN


// Functions to read input from a text file
vector<string> findParameters(string line){
  int nPos = 0;
  int n = 0;
  while(nPos!=-1){
    nPos=line.find(" ",nPos+1);
    n++;
  }
  vector<string> toReturn(n);
  nPos = 0;
  int i=0;
  while(nPos!=-1){
    int extra = i==0?0:1;
    int nPrevious = nPos+extra;
    nPos=line.find(" ",nPos+1);
    string tmp =line.substr(nPrevious,nPos-nPrevious);
    if (nPos==-1){
      tmp=tmp.substr(0,tmp.length());
    }
    //cout << tmp << endl;;
    toReturn[i]=tmp;
    i++;
  }
   
  if(toReturn[0].find(" ")){
    toReturn[0]=toReturn[0].substr(0,toReturn[0].find(" "));
  }
   
  return toReturn;
}

vector<vector<string> > parseFile(string file){
   
  //cout << "reading " << parameters << endl;
  vector<vector<string> > toReturn;
  string line;
  ifstream parametersFile(file.c_str());
  if (parametersFile.is_open()){
    while (getline(parametersFile,line)){
      vector<string> tokens = findParameters(line);
      toReturn.push_back(tokens);
    }
  } else{
    cerr << file << " not found" << endl;
  }
   
  return toReturn;
}
///////////////////////////////
// Problem Solver
void Problem::solveProblem() {
		

  IloModel model(env);
  IloInt i = 0;   //Counters for Input
  IloInt j = 0;   //Counters for Input
  IloInt r = 0;   //Counters for Input
  IloInt p = 0;   //Counters for Input
  IloInt k = 0;   //Counters for Input
  IloInt q = 0;   //Counters for Input
  IloInt user = 0;   //Counters for Input
  IloInt counter = 0;   //Counters for Input
  IloInt I = 10000; //I as a large integer
  IloInt num_users = 0; // M-number of users
  IloInt num_tasks = 0; // N-number of tasks
  IloInt num_proc = 0; // Number of processors
  IloInt sum8 = 0;  // For calculating t
 // IloNumArray speed_fcr(env,num_proc+1);
		
  //Reading the INPUT file
  		
  vector<vector<string> > parsed = parseFile("instances3.txt");//Input File
  num_users = atoi(parsed[0][0].c_str());                           // Number of users
  num_proc = atoi(parsed[0][1].c_str());		                  // NUmber of processors(Not including the Local Processor)
  num_tasks = atoi(parsed[2][0].c_str());  // Total number of tasks ( Sum of tasks of all users)
  cout << "we have this number of users "<< num_users << endl;        // Dont remove
  cout << "we have this number of processors "<< num_proc << endl;    // Dont remove
  cout << "we have this number of tasks "<< num_tasks << endl;	    // Dont remove	
		

  IloNumArray speed_fcr(env,num_proc+num_users);
  for (i=0; i<num_proc+num_users; i++)
    {	
      speed_fcr[i] = atof(parsed[4][i].c_str());               // Speed factor for the cloud processors 
      cout << "speed_fcr " << i << " "<< speed_fcr[i] << " "; 
    } 
                 // The Local processor's speed factor is set to 1

    
   for(int i = 0; i < (num_proc)+1; i++)
     {
          cout << speed_fcr[i]<< " " << endl;
     }
     
      
  IloNumArray s(env, num_users);               // Number of tasks per user. s[1] -> Number of tasks for user 1
  for( i=0; i<num_users; i++)
    {	
      s[i] = atoi(parsed[6][i].c_str()); 
      //num_tasks = num_tasks+s[i]; // N- number of tasks} // number of the tasks per user
      cout << "Number of Tasks for user"<<i+1 <<": "<< s[i] << endl; 
    }
                 
  IloNumArray list(env, num_tasks);   // List contains the indices of all the tasks. list[k] represesents the task k
  for ( int k = 0; k <  num_tasks; k++)
    {
      list[k] = k+1;
    }
                
  IloNumArray t(env, num_tasks);      // Array containing the processing times for the tasks.
  for (int j=0; j<num_tasks;j++) // Processing times for the tasks
    {
      t[j] = atoi(parsed[8][j].c_str());
      cout << "Processing times for task"<<j+1<<": "<< t[j] << endl;
    } // getting the processing times 

  IloNumArray  B (env, num_users);  // Budget Array. Getting the information of budgets from the user
                
  for (int k=0;k<num_users;k++)// Getting the budget from the users
    {
      B[k] = atoi(parsed[10][k].c_str());
      cout << "Budget for user"<<k+1<<": "<< B[k] << endl;
    }
 IloNumArray  beta (env, num_proc+num_users);
    //beta[0] = 0;
cout <<"BETA VALUES "<<endl;
     for (int k=0;k < num_proc+num_users; k++)// Getting the beta data
    {     
       beta[k] = atoi(parsed[12][k].c_str());
       cout << beta[k] << " ";
    }
  // 2D Vector containing the list of tasks for user. J[i][j] refers to user i and his task j 
  // create a vector for each user
  vector<vector<int> > J;
  vector <int> litasks; // 1D Vector for pushing elements into 2D Vector
                     
  for(int l = 0; l < num_users; l++)
    {
      litasks.resize(0);
      counter = s[l];
      for(int i = 0;i<counter;i++)
	{
	  litasks.push_back(list[q]);
	  q = q + 1;
	}
      J.push_back(litasks);
    }
            
  // Displaying the vector
  cout << " displaying the vector " << endl;
  for(unsigned int l = 0; l < J.size(); l++)

    {
      for( unsigned int m = 0; m < J[l].size(); m++)
	{
	  cout << J[l][m] << " ";
	}
      cout << endl;
    }
              

 // 2D Vector containing the list of tasks for user. J[i][j] refers to user i and his task j 
  // create a vector for each user
  vector<vector<int> > R;
  vector <int> liproc; // 1D Vector for pushing elements into 2D Vector
                     
  for(int l = 0; l < num_users; l++)
    {
      liproc.resize(0);
      
      for(int i = 0;i<num_proc;i++)
	{
	  liproc.push_back(i);
	  
	}
         liproc.push_back(num_proc+l);
      R.push_back(liproc);
    }
            
  // Displaying the vector
  cout << " displaying the vector " << endl;
  for(unsigned int l = 0; l < R.size(); l++)

    {
      for( unsigned int m = 0; m < R[l].size(); m++)
	{
	  cout << R[l][m] << " ";
	}
      cout << endl;
    }
            
  //Variable declaration
  IloArray < IloArray < IloBoolVarArray > > x(env);// x is the decision variable. x[i][j][k] represents the boolean value for a task i run on processor j at position k

  for (int i = 0; i < num_tasks; i++)         // Allocating size for the decision variables
    {
      IloArray < IloBoolVarArray > tmp2(env);
      for (int j = 0; j < num_proc+num_users; j++)
	{
	  IloBoolVarArray tmp(env);
	  for (int k = 0; k < num_tasks; k++)
	    {
		stringstream name;
    		name << "x_" << i << "_" << j << "_" << k;
	      tmp.add(IloBoolVar(env,name.str().c_str()));
	    }
	  tmp2.add(tmp);
	}
      x.add(tmp2);
    }
                 
  // T is the Completion Time. T[i] refers to the completion time of user i 

  IloNumVarArray T(env); 
  for (int i = 0; i < num_users; i++){
    stringstream name;
    name << "T_" << i;
    T.add(IloNumVar(env, 0, 100, name.str().c_str()));
  }
  IloNumVarArray C(env);         // Completion time of tasks. C[1] -> Completion time for task 1
  for (int i = 0; i < num_tasks; i++){
    stringstream name;
    name << "C_" << i;
    C.add(IloNumVar(env, 0, 100, name.str().c_str())); 
}   
  // Sum of the Constraints

  //Constraints modelling -Modelled according to last formulation!!
       
  // CONSTRAINT 1
           
  for (int i=0; i < num_users; i++)
    {
      IloNumExpr sum1(env, 0);
      
	  for (int p=0; p < num_tasks; p++) 
	    { 
              for (unsigned int j=0; j < J[i].size(); j++) 
	       {
            sum1 += speed_fcr[num_proc+i]*t[J[i][j]-1]*x[J[i][j]-1][num_proc+i][p];
	       }
	    }
	model.add(T[i] >= sum1);           
    } // constraint 1 added to IloModel

	 
  // CONSTRAINT 2

  for (int j=0; j<num_tasks; j++) 
    {
        
      	sum8 = 0;
	
	for(int i = 0;i < num_users; i++)
	{
	sum8=sum8+s[i]-1;
	 if (j<=sum8)
	{user=i; break;}
	
	}

      IloNumExpr sum2(env, 0);
      for (p=0; p<num_tasks; p++) 
	{
	  for(unsigned int r=0; r< num_proc; r++)
	    {
	       sum2 += x[j][r][p];
        }
		sum2 += x[j][num_proc+user][p];
	}
	 model.add(1 >= sum2);   
	 model.add(1 <= sum2);   
    } // constraint 2 added to model
    

  // CONSTRAINT 3
 
	for (p=0; p< num_tasks; p++)
       {
              for(r=0; r<num_proc+num_users; r++)
            {
               IloNumExpr sum3(env, 0);
               for (unsigned int j=0; j<num_tasks; j++)
                   {
                     sum3= sum3 + x[j][r][p];
                   }
	           model.add( 1 >= sum3);
            }
     }
  // constraint 3 added to model
    
// CONSTRAINT 4

 for ( int i = 0; i < num_proc + 1; i++)
  {
       cout << speed_fcr[i] << " ";
   }
      
  for (int j=0; j<num_tasks; j++) 
    {  
       for (int k=0; k<num_tasks; k++)
	 {
	     for (int p=1; p<num_tasks; p++)
	    {
	          for(int r=0; r<num_proc+num_users; r++)
		    {
                    if(j!=k)       
                    {
                       model.add( (C[j]-C[k] + I*(2 - x[j][r][p] - x[k][r][p-1]) ) >= speed_fcr[r]*t[j] );
                    }
            }
        }
	  }
    } 
      
  // constraint 4 added to model
      
  // CONSTRAINT 5
 
  for (int i=0; i<num_users; i++)
    { 
      IloNumExpr sum6(env, 0);
      for (unsigned int j=0; j< J[i].size(); j++) 
	{
	   model.add(T[i] >= C[J[i][j]-1]);
	}
        
    }

  // CONSTRAINT 6

  for (int j=0; j<num_tasks; j++) 
    {
      IloNumExpr sum4(env, 0);
      for (int r=0; r <num_proc+num_users; r++)
	{
         for ( int p = 0; p < num_tasks; p++)
         {
	       sum4 = sum4 + speed_fcr[r]*t[j]*x[j][r][p];
         }
	}
      model.add( C[j] >= sum4);	
    } 
  // constraint 6 added to model 

  // CONSTRAINT 7
     
  for(int i=0; i <num_users; i++)
    {
      IloNumExpr sum5(env, 0);
      for(int r=0; r <num_proc+num_users; r++)
	{
	  for(int p=0; p <num_tasks; p++)
	    {
	       for( unsigned int j=0; j <J[i].size(); j++)
		  {
		    sum5 = sum5 + speed_fcr[r]*beta[r]*t[J[i][j]-1]*x[J[i][j]-1][r][p];
          }
	    }
	}
	   model.add(B[i] >= sum5);              
    }

   
  // Minimization of sum of completion times
  IloNumExpr sumT(env, 0);
  //sumT = 0;
  cout << "we have this number of users "<< num_users << endl;
  for (int i=0; i<num_users; i++)
    {
      sumT= sumT + T[i]; 
      cout<<T[i]<<endl;
    }

  model.add(IloMinimize(env, sumT));  //Dont Remove

  // Optimize class
  IloCplex cplex(model);
  cplex.exportModel("model.lp");
  cplex.solve();
  env.out() << "Sum of completion times: " << cplex.getValue(sumT) << endl; //list solution
  for ( int i = 0; i < num_users; i++)
     {
        env.out() << "User Completion time[" << i+1 <<"] : "<< cplex.getValue(T[i]) << endl;
     }
       for (int i = 0; i < num_tasks; i++)
    {
      
    for ( int i = 0; i < num_tasks; i++)
     {
        env.out() << "Completion time[" << i+1 <<"] : "<< cplex.getValue(C[i]) << endl;
     }

}

void Problem::createModelCP()
{
  clock_t start_time; // end_time;
  start_time = clock();
  model = IloModel(env);
  IloCP cp(model);
  cp.setParameter(IloCP::Workers, 1);
  //cp.setParameter(IloCP::TimeLimit, timeLimit);
  cp.setParameter(IloCP::LogVerbosity, IloCP::Terse);
}
