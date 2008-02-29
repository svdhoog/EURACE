//HERE: function to initialize the rule_detail_system.
//Contains arrays for:
//NR_TYPES: number of different types of rules (typically NR_TYPES=3)
//NRRULES_PER_TYPE[NR_TYPES]: number of rules per type (typically the same, equal to 10)
//NRRULES_PER_TYPE=[10,10,10];
//rule_type: type identifier for each rule
//nr_params_per_type: number of parameters for each rule type
//nr_params: no. of parameters for each rule
//parameters: parameter setting for each rule 
//rule_execution:  function name to execute the rule
//my_function_names: the names of the functions to call to execute a rule
//my_function_name[1]='Markowitz';
//my_function_name[2]='ProspectTheory';
//my_function_name[3]='RandomRule';

int initialize_ruledetailsystem()
{
	int i;
	
 	//Getting the size of the system:
	int NR_TYPES=CLASSIFIERSYSTEM->nr_types;

	//dynamic array with number of rules in each type (size of subpopulations)
	int * NRRULES_PER_TYPE=CLASSIFIERSYSTEM->nr_rules_per_type;

	//total number of rules:
	int NRRULES=CLASSIFIERSYSTEM->nr_rules;
	
	//dynamic array with number of parameters in each type
	int nr_params_per_type[NR_TYPES];
	for (i=0; i<NRRULES; i++)
	{	
		nr_params_per_type[i]=CLASSIFIERSYSTEM->array[i]->nr_params_per_type;
	}
	
	//Local vars:
	int 		ids[NRRULES];
	
	//Here: nr_params is the number of parameters for a rule of certain type
	//For each type we should have a prototype and then fill in the dynamic array
	int			rule_type[NRRULES];
	int			nr_params[NRRULES];
	double**	parameters;			//parameter[NRRULES][MAX_PARAMS];
	
	double 		p0[NRRULES];
	double 		dp[NRRULES];
	double 		p1[NRRULES];
	
	char**		rule_execution;	    //rule_execution[NRRULES]
	
	word_array * my_function_names = get_my_function_names(); //list of function names
	
	double		param_value;
	int			j,jmax,k,m, count, par_resolution;

	//Getting parameter settings
	for (i=0; i<NRRULES; i++)
	{
		p0[i] = CLASSIFIERSYSTEM->array[i]->param_start_values;
		dp[i] = CLASSIFIERSYSTEM->array[i]->param_increment_values;
		p1[i] = CLASSIFIERSYSTEM->array[i]->param_end_values;
	}
	
	//First we set the field 'rule_type':
	count = 0;
	for (i=0;i<NR_TYPES;i++)
	{
		//Set nr_rules in the current type
		jmax = CLASSIFIERSYSTEM->array[i]->nr_rules_per_type;
		
		//For type i, traverse the rule ids from 0+count to jmax+count
		//such that we go through all rules of type i
		for (j=count;j<count+jmax;j++)
		{
			//We are now in rule id j
			CLASSIFIERSYSTEM->array[j]->rule_type = i;
			CLASSIFIERSYSTEM->array[j]->nr_params = CLASSIFIERSYSTEM->array[i]->nr_params_per_type;
			
		}
		//Set counter to current rule id
		count=count+jmax;
	}
	
	//Filling in random parameter settings
	for (i=0; i<NRRULES; i++)
	{
		//I want to add an empty double_array to the double2D_array parameter
		add_double_array(double2D_parameter_dynamic_array->array[i], {});
		
		//Filling the just added double_array parameter[i] with the parameter values of rule i
		for (j=0; j<nr_params[i]; j++)
		{
			//Filling the parameters[i][j]
			//A random value is chosen from the parameter resolution given by:
			//[p0:dp:p1] where dp is the step size.
				
			par_resolution = (p1[i]-p0[i])/dp[i]; //total possibilities
			k=rand_unif();
			m=(int)(k*par_resolution); //random uniform in [0,par_res]: selecting a random param. setting
	
			//Can we add a double directly inside the function add_double?
			add_double(parameters[i][j], p0[i] + m*dp[i]);		

			//param_value = p0[i] + m*dp[i];
			//add_double(parameters[i][j], param_value);
		}		

		//We set the field 'rule_execution':
		//First possibility: we test which function name the rule has
		j=CLASSIFIERSYSTEM->array[i]->rule_type;
		CLASSIFIERSYSTEM->array[i]->rule_execution[i]=sprintf('my_function_names[%d]',j);
	}

	//ALTERNATIVE CODE for rule_execution
	//we loop over the different rule types
	//this avoids the rule_type[i]	
	count = 0;
	for (i=0;i<NR_TYPES;i++)
	{
		//Set nr_rules in the current type
		jmax = CLASSIFIERSYSTEM->array[i]->nr_rules_per_type;
		
		//For type i, traverse the rule ids from 0+count to jmax+count
		for (j=count;j<count+jmax;j++)
		{
			//We are now in rule id j
			//Set the function call for rule j, which is just my_function_names[i]
			CLASSIFIERSYSTEM->array[j]->rule_execution=sprintf('my_function_names[%d]', i);
		}
		//Set counter to current rule id
		count=count+jmax;
	}
	
    return 0;
}


//*********** BEGIN AUXILIARY FUNCTIONS ****************************


//double sum(double * p, int size)
//Sum of elements of a vector p.
//
//USAGE:
//Before calling:
//double * p;
//int size
double sum(double * p, int size)
{
	int i;
	double sum=0.0;
	
	for (i=0;i<size;i++)
	{
		sum+=p[i];
	}	
	
    return sum;
}

//\brief: Cumulative sum of elements of a vector p.
//\fn: cumsum(double * p, int size, double * cumsum)
//Call: cumsum(p, size, cumsum);
//
//USAGE:
//Before calling:
//double * p;
//double * cumsum;
//int size;
//cumsum = malloc(sizeof((double)*size);
//Call: cumsum(p, size, cumsum);
//After calling:
//free(cumsum);
//
//UNIT TEST:
//Input: p={0.6 0.2 0.8 0.4}
//cumsum = cumsum(p);
//Outcome: cumsum={0.6 0.8 1.6 2.0}
void cumsum(double * p, int size, double * cumsum)
{
	//Before calling this function you need to have done:
	//cumsum = malloc(sizeof((double)*size);

	int i;
	
	//Cummulative sum
	cumsum[0]=p[0];
	for (i=0;i<size-2;i++)
	{
		cumsum[i+1]=cumsum[i]+p[i+1];
	}
	cumsum[size-1]=cumsum[size-2]+p[size-1];
}

//double * cumpdf(int N, double * p)
//Cummulative probability density function.
//p: probability vector (can be non-scaled, i.e. probabilities need not add to 1).
//size: number of elements
//Given a vector of probabilities p, the cumulative pdf is given by
//the normalized values in the cummulative sum:
//  cpdf = cumsum(p)/sum(p);
//
//USAGE:
//Before calling:
//double * p;
//double * cpdf;
//int size;
//cpdf = malloc(sizeof((double)*size);
//Call: cumpdf(p, size, cumpdf);
//After calling:
//free(cpdf);
//
//UNIT TEST:
//Input: p={0.6 0.2 0.8 0.4}
//cpdf = malloc(sizeof((double)*size);
//cumpdf(p, 4, cpdf);
//Outcome: cpdf={0.3 0.4 0.8 1.0}
//free(cpdf);
void cumpdf(double * p, int size, double * cpdf)
{
	//Before calling this function you need to have done:
	//cpdf = malloc(sizeof((double)*size);
	
	int i;
	double sum_p;		//this is the sum of p
	double * cumsum_p;	//this is the cumulative sum of p	
	
	cumsum_p = malloc(sizeof((double)*size);
	
	//The sum is passed to sum_p:
	sum_p = sum(p, size);
	
	//The cumulative sum is passed to cumsum_p:
	cumsum(p, size, cumsum_p);
    
	//The cumulative pdf is:
	for (i=0;i<size;i++)
	{    	
     	cpdf[i] = cumsum_p[i]/sum_p;
	}	
}

//int draw(int N, double * cpdf)
//N: number of elements
//cpdf: vector of probabilities
//Drawing a random number from the cummulative probability density function cpdf.
//Returns the bin number of a randomly chosen number u.
//Given a cummulative pdf F(.), the random number u belongs to bin j
//if and only if F(j-1)<= u < F(j).
int draw(int N, double * cpdf)
{	
	int j,u, nr_selected_bin;
	int imax = N; //was: cpdf->size;

    //Random number generator:
    u=rand_unif();
    
    nr_selected_bin=0;
    
    //Case 1: u<F(1)
        if (0<=u && u<cpdf[0])
        {
            nr_selected_bin=1;
        }
    
    //Case 2: Travers the cpdf until u >= F(j-1)
	    for (j=2;j++;j<imax)
	    {
	        if (cpdf[j-1]<=u && u<cpdf[j])
	        {
	            nr_selected_bin=j;
	            break;
	    	}
	    }
	    
    //Case 3: u>=F(J)
        if (cpdf[imax]<=u)
        {
            nr_selected_bin=imax;
        }

    return nr_selected_bin;
}

//int ismember(int i, int * x, int n)
//Check if i is a member of the array x, with size n.
//Returns 1 if i is a member, 0 else.
int ismember(int i, int * x, int n)
{
	int k, ans=0;
	for (k=0;k<n;k++)
	{
		if(i==x[k])
		{
			ans=1;
			break;
		}
	}

	return ans;
}


//int * draw_without_replacement(int N, double * cpdf)
//Drawing N random numbers (integers) without replacement from the cummulative probability density function cpdf.
//UNIT TEST:
//cpdf={0.1 0.2 0.3 0.4 0.5};
//draw_without_replacement(5, cpdf)
//Outcome: array 'draws' should contain all values 1-5

int * draw_without_replacement(int N, double * cpdf)
{
	double draws[N];
	int i,k,count=0;
	
	for (k=0;k<N;k++)
	{
		draws[k]=-1;
	}
	
	//Continue drawing until N different numbers have been drawn
	while(count!=N)
	{
		i = draw(cpdf);
		
		//Check membership, only add if not yet a member
		if (ismember(i,draws,N)==0)
		{
			draws[count]=i;
			count += 1;
		}
	}
	
    return (&draws);//CHECK: is this proper way of returning the address of the start of array?
}

//int * draw_with_replacement(int N, double * cpdf)
//Drawing N random numbers (integers) with replacement
//from the cummulative probability density function cpdf.
//UNIT TEST:
//Do 1000 draws from 4 bins with probabilities according to a given pdf.
//Input: 
//pdf={0.3 0.1 0.4 0.2};
//cpdf={0.3 0.4 0.8 1.0};
//draw_with_replacement(1000, cpdf)
//Outcome: approx. 300, 100, 400, 200 in each bin.

int * draw_with_replacement(int N, double * cpdf)
{
	double draws[N];
	int k=0;

	for (k=0;k<N;k++)
	{
		draws[k]=draw(cpdf);
	}
	
    return (&draws);//CHECK: is this proper way of returning the address of the start of array cumsum?
}

// *********** END AUXILIARY FUNCTIONS ****************************
