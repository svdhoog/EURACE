/*********************************
 * FinancialAgent_aux_functions.c 
 * GA auxiliary functions.
 * *********************************
 * History:
 * 24/07/08 Sander: Checked that all malloc pointers are freed at the end of functions. 
 * 16/07/08 Sander 
 *********************************/
#include <limits.h> //required to test for max. double: LONG_MAX
#include "header.h"
#include "FinancialAgent_agent_header.h"
#include "FinancialAgent_aux_header.h"
#include "some_new_functions.h"


/* \fn void single_point_cross_over(int size, double * string_a, double * string_b, int cross_point)
 * \brief Genetic operator: single_point_cross_over
 */
void single_point_cross_over(int size, double * string_a, double * string_b, int cross_point)
{
	int k;
	double tmp;
	
	if (cross_point>size)
	{
		printf("Error in single_point_cross_over: cross_point > size of string");
		return;
	}

    //Crossover the bitstrings using single-point cross-over from the cross_point
    for (k=cross_point;k<size;k++)
    {
        tmp = string_a[k];
        string_a[k]= string_b[k];
        string_b[k]= tmp;
    }
}

/* \fn void two_point_cross_over(int size, double * string_a, double * string_b, int cross_point, int cross_length)
 * \brief Genetic operator: two_point_cross_over
 */
void two_point_cross_over(int size, double * string_a, double * string_b, int cross_point, int cross_length)
{
	int k, count;
	double tmp;
	
	if ((cross_point+cross_length)<size)
	{
	    // perform normal cross-over between parent pair
		for (k=cross_point; k<(cross_point+cross_length); k++)
		{
			tmp = string_a[k];
			string_a[k]=string_b[k];
			string_b[k]=tmp;	
		}
	}
	else
	{
		//Case: cross-over crossed the bitstring boundary, so wrap-around is needed
	
	    //first, perform cross-over starting from the cross-point to end of string:
		count=0;
		for (k=cross_point; k<size; k++)
		{
			tmp = string_a[k];
			string_a[k]=string_b[k];
			string_b[k]=tmp;
			count++;
		}
	    //second, perform cross-over from the beginning of string until the count reaches the cross_length
		k=0;
		while (count!=cross_length)
		{
			tmp = string_a[k];
			string_a[k]=string_b[k];
			string_b[k]=tmp;
			count++;
		}
	}
}

/* \fn void two_point_cross_over_alt(int size, double * string_a, double * string_b, int cross_point, int cross_length)
 * \brief Genetic operator: two_point_cross_over. Alternative code for the 2-point crossover operation.
 */
void two_point_cross_over_alt(int size, double * string_a, double * string_b, int cross_point, int cross_length)
{
	int k, start, end;
	double tmp;
	
	//Set start and end bits
	if (cross_point+cross_length<=size)
	{
		start = cross_point;
		end   = cross_point+cross_length;
	}
	//Prevent overrun on right-hand side and cut to length
	/*
	if (cross_point+cross_length>size)
	{
	    start=cross_point;
	    end=size;            	
	}
	*/
	//Prevent overrun on right-hand side by making strings circular
	//Here we switch the start and end points: the substring that is crossed over is the negative of the actual substring
	//that should have been crossed over, but the result is the same. 
	if (cross_point+cross_length>size)
	{
		start=(cross_point+cross_length)%size;
		end=cross_point;
	}

	//Crossover the bitstrings
    for (k=start;k<end;k++)
    {
        tmp = string_a[k];
        string_a[k]=string_b[k];
        string_b[k]=tmp;
    }
    
}

/* \fn void mutation(int size, double * string, double * stepsize, double prob_mut)
 * \brief Genetic operator: Mutation of real-valued bitstrings.
 * size: size of the string
 * string: pointer to the string
 * min_value[k]: array of minimum values for each bit
 * max_value[k]: array of maximum values for each bit
 * stepsize[k]: array of stepsizes for each bit 
 * delta_min: lower range for the mutation
 * delta_max: upper range for the mutation 
 * prob_mut: mutation probability
 * The mutation is delta*stepsize[k], where: 
 * delta: is a random number between (delta_min, delta_max)
 */
void mutation(int size, double * string, double * stepsize, double delta_min, double delta_max, double * min_values, double * max_values, double prob_mut)
{
	int k;
	double delta, mutation_size;
	
	for (k=0; k<size; k++)
	{
		// apply mutation to the bit of the string
		if (random_unif() < prob_mut)
		{	
			//Set integer units to mutate in the interval delta_min*stepsize and delta_max*stepsize
			 delta = random_unif_interval(delta_min, delta_max);
				  
			//mutate the value at position k by a random number between -10*stepsize and +10*stepsize from the previous value
			 mutation_size = delta*stepsize[k];
		}
		
		// check that min_value and max_value are not being violated
		if (string[k] + mutation_size >= min_values[k] && string[k] + mutation_size <= max_values[k])
		{
			string[k] = string[k] + mutation_size;
		}
		else if (string[k] + mutation_size < min_values[k])
			 {
				string[k] = min_values[k];
		     }
			 else if (string[k] + mutation_size > max_values[k])
			 	  {
				 		string[k] = max_values[k];
			 	  }
	}    
}

/* \fn void election(int size, double * offspring_1, double * offspring_2, double * parent_1, double * parent_2)
 * \brief Genetic operator: Election of 2 out of 4 best bitstrings 
 */

void election(int size, double * offspring_1, double * offspring_2, double * parent_1, double * parent_2)
{

}



void GA_selection(int N_pairs, int * parent_index_1, int * parent_index_2, int * rule_id_1, int * rule_id_2)
{
	int j, index;
	int nr_rules, N_rep;
	double avg_performance, sum;
	
	double * p;
	double * cpdf;
	int * draws;
		
	// *********************** Start of Selection function **************************************
	// 1. Selection/Reproduction
			    
    //Computing fitness-based probabilities using multi-logit probabilities
	nr_rules = PUBLIC_CLASSIFIERSYSTEM.nr_rules;
	sum=0.0;
    for (j=0;j<nr_rules;j++)
    {
    	avg_performance  = PUBLIC_CLASSIFIERSYSTEM.ruletable[j].avg_performance;
    	if(PRINT_DEBUG) printf("\n In GA_selection: avg_performance=%f\n", avg_performance);

    	if(PRINT_DEBUG) 
    	{
	    	if (EWA_PARAMETERS.EWA_beta*avg_performance > log(LONG_MAX))
	    	{
	    		printf("\n In GA_selection, line 188: error computing sum.\n");    		
	    		printf("\n Maximum value exceeded: EWA_PARAMETERS.EWA_beta * avg_performance > log(LONG_MAX).\n");
	    		//printf("\n LONG_MAX = %f.\n", LONG_MAX);
	    		printf("\n log(LONG_MAX) = %f.\n", log(LONG_MAX));
	    		printf("\n avg_performance=%f\n", avg_performance);
	    		printf("\n EWA_PARAMETERS.EWA_beta=%f\n", EWA_PARAMETERS.EWA_beta);
	    		printf("\n EWA_PARAMETERS.EWA_beta*avg_performance=%f\n", EWA_PARAMETERS.EWA_beta*avg_performance);    		
	    	}
    	}
        sum += exp(EWA_PARAMETERS.EWA_beta*avg_performance);
        //if(PRINT_DEBUG) printf("\n In GA_selection: sum=%f\n", sum);
    }
    
    p = malloc(sizeof(double)*nr_rules);
    for (j=0;j<nr_rules;j++)
    {
    	avg_performance  = PUBLIC_CLASSIFIERSYSTEM.ruletable[j].avg_performance;
        p[j] = exp(EWA_PARAMETERS.EWA_beta * avg_performance)/sum;
        //if(PRINT_DEBUG) printf("\n In GA_selection: p[%d]=%f\n", j, p[j]);
    }

    // Construct cumulative probability density function: cpdf
     cpdf = malloc(sizeof(double)*nr_rules);
     cumpdf(p, nr_rules, cpdf);
     
    //print prob. vector:
     if(PRINT_DEBUG) 
     {
	     printf("\n In GA_selection: prob: [ ");
	     for (j=0;j<nr_rules;j++){printf("%2.2f ", p[j]);}
	     printf("]\n");
     }
    //print cpdf:
     if(PRINT_DEBUG) 
     {
	     printf("\n In GA_selection: cpdf: [ ");
	     for (j=0;j<nr_rules;j++){printf("%2.2f ", cpdf[j]);}
	     printf("]\n"); 
     }     
     
     // Drawing N_rep random copies (without replacement) from the entire population
     // using the fitness-based probabilities.
     N_rep = (int) 2*floor((GA_PARAMETERS.reproduction_proportion * GA_PARAMETERS.pop_size)/2);
     
     //draws is an array containing indices for the selected rules
     draws = malloc(sizeof(int)*N_rep);
     
     //draw N_rep times without_replacement from density cpdf, and store results in draws 
     //draw_without_replacement(nr_rules, cpdf, N_rep, draws);
     
     //testing: draw_with_replacement
     //this most likely results in selecting the rule with the highest probability only, for all draws
     draw_with_replacement(nr_rules, cpdf, N_rep, draws);
	     
     // For the random matching, drawing is WITH replacement using uniform probabilities
     // from the discrete interval [0, N_pairs+1].
     // Drawing with replacement allows for both parents to be a copy of the same bitstring.
     // We use (N_pairs+0.999) below to allow also the last string to be drawn (casting as int takes the floor of the double).
     for (j=0; j<N_pairs; j++)
     {
    	 //parent_index_1[j] = (int)((N_pairs+0.999)*random_unif());
    	 //parent_index_2[j] = (int)((N_pairs+0.999)*random_unif());
     
    	 //equivalent code:
    	  parent_index_1[j] = (int)(random_unif_interval(0.0, N_pairs+0.999));
    	  parent_index_2[j] = (int)(random_unif_interval(0.0, N_pairs+0.999));

     // Now we have selected the indices of N_pairs parent pairs that will undergo crossover and mutation.
     // To retrieve the 2 rule_ids associated to these two indices parent_index_1, parent_index_2
     // we need to get them from 'draws'.

	 // Rule indices id1, id2 are stored in the vector 'draws':
		index=parent_index_1[j];
		rule_id_1[j]=draws[index];
		
		index=parent_index_2[j];
		rule_id_2[j]=draws[index];
     }
     
     //Testing:
     if (PRINT_DEBUG) 
     {
	     printf("\n In GA_selection:\n draws=[ ");
	     for (j=0;j<N_rep;j++){printf("%d ", draws[j]);}
	     printf("]\n Completed drawing %d potential parents from population.\n", N_rep); 

	     printf("\n In GA_selection:\n parent_index_1=[ ");
	     for (j=0;j<N_pairs;j++){printf("%d ", parent_index_1[j]);}
	     printf("]\n Completed drawing %d indices for parent_index_1 from potential parent subset.\n", N_pairs); 

	     printf("\n In GA_selection:\n parent_index_2=[ ");
	     for (j=0;j<N_pairs;j++){printf("%d ", parent_index_2[j]);}
	     printf("]\n Completed drawing %d indices for parent_index_2 from potential parent subset.\n", N_pairs);  

	     printf("\n In GA_selection:\n rule_id_1=[ ");
	     for (j=0;j<N_pairs;j++){printf("%d ", rule_id_1[j]);}
	     printf("]\n Completed associating %d rule_ids to parent_index_1.\n", N_pairs); 

	     printf("\n In GA_selection:\n rule_id_2=[ ");
	     for (j=0;j<N_pairs;j++){printf("%d ", rule_id_2[j]);}
	     printf("]\n Completed associating %d rule_ids to parent_index_2.\n", N_pairs); 
     }
     
	 //Now we have 2 arrays of rule_ids that are randomly matched, and these can be used in the next functions.
     // *********************** End of Selection function **************************************

    free(p);
 	free(cpdf);
 	free(draws);
}


void GA_reproduction(int size, int id1, int id2, double * offspring_1, double * offspring_2)
{
	int k, j;
	int cross_point, cross_length;

    /*********************** Start of Reproduction function **************************************************/
	 //void GA_reproduction() : applies to each parent pair, nr_pair=0,...,N_pairs
     //2. Genetic operators: cross-over
		
		if(PRINT_DEBUG) printf("\n In GA_reproduction: copying rule id1=%d and rule id2=%d to offspring strings.\n", id1, id2);
		if(PRINT_DEBUG) printf("\n size=%d\n", size);
		for (k=0; k<size; k++)
		{
			offspring_1[k]=PUBLIC_CLASSIFIERSYSTEM.ruletable[id1].parameters[k];
			offspring_2[k]=PUBLIC_CLASSIFIERSYSTEM.ruletable[id2].parameters[k];
		}
	     if(PRINT_DEBUG)
	     {
		     printf("\n In GA_reproduction, before cross-over:\n offspring_1=[ ");
		     for (j=0;j<size;j++){printf("%1.1f ", offspring_1[j]);}
		     printf("]\n"); 

		     printf("\n In GA_reproduction, before cross-over:\n offspring_2=[ ");
		     for (j=0;j<size;j++){printf("%1.1f ", offspring_2[j]);}
		     printf("]\n"); 
	     }


		//now cross-over the strings
		if (random_unif() < GA_PARAMETERS.prob_cross)
		{
			// 2a. Cross-over: 1-point cross-over
			if (GA_PARAMETERS.single_point_cross_over)
			{
	   			// draw random cross-over point between $[1,L-1]$
	   			cross_point = (int)random_unif_interval(1, size-1);	
	   			single_point_cross_over(size, offspring_1, offspring_2, cross_point);
	   			
	   	     if(PRINT_DEBUG)
	   	     {
	   		     printf("\n In GA_reproduction: cross_point = %d\n", cross_point);
	   	     }
			}
			else
			{
			    // 2b. Cross-over: 2-point cross-over
				// draw random cross-over point between $[1,L-1]$
				cross_point = (int)random_unif_interval(1, size-1);
				
				// draw random cross-over length between $[1,L-1]$
				cross_length = (int)random_unif_interval(1, size-1);

		   	     if(PRINT_DEBUG)
		   	     {
		   		     printf("\n In GA_reproduction: cross_point = %d, cross_length = %d\n", cross_point, cross_length);
		   	     }

				two_point_cross_over(size, offspring_1, offspring_2, cross_point, cross_length);
			}
		}
		else
		{
	        //No cross-over occurs: 2 offspring remain identical copies of the parents
		}
		
	     if(PRINT_DEBUG)
	     {
		     printf("\n In GA_reproduction, after cross-over:\n offspring_1=[ ");
		     for (j=0;j<size;j++){printf("%1.1f ", offspring_1[j]);}
		     printf("]\n"); 

		     printf("\n In GA_reproduction, after cross-over:\n offspring_2=[ ");
		     for (j=0;j<size;j++){printf("%1.1f ", offspring_2[j]);}
		     printf("]\n"); 
	     }

	/*********************** End of Reproduction function **************************************************/
}

void GA_mutation(int size, double * offspring_1, double * offspring_2)
{
	int j;
	
	/*********************** Start of Mutation function ****************************************************/
	//void GA_mutation(int size, double * offspring_1, double * offspring_2): applies to each pair, nr_pair j=0,...,N_pairs
	// 3. Mutation: each bit has a probability of mutating by stepsize
 	//For each of the strings offspring_1 and offspring_2 that have just undergone cross-over, now perform mutation

    //void mutation(int size, double * offspring_1, double * offspring_2);
	mutation(size, offspring_1, GA_PARAMETERS.stepsize, GA_PARAMETERS.delta_min, GA_PARAMETERS.delta_max, GA_PARAMETERS.min_values, GA_PARAMETERS.max_values, GA_PARAMETERS.prob_mut);
	mutation(size, offspring_2, GA_PARAMETERS.stepsize, GA_PARAMETERS.delta_min, GA_PARAMETERS.delta_max, GA_PARAMETERS.min_values, GA_PARAMETERS.max_values, GA_PARAMETERS.prob_mut);

    if(PRINT_DEBUG)
    {
	     printf("\n In GA_mutation, after mutation:\n offspring_1=[ ");
	     for (j=0;j<size;j++){printf("%1.1f ", offspring_1[j]);}
	     printf("]\n"); 

	     printf("\n In GA_mutation, after mutation:\n offspring_2=[ ");
	     for (j=0;j<size;j++){printf("%1.1f ", offspring_2[j]);}
	     printf("]\n"); 
    }

	/*********************** End of Mutation function ****************************************************/
}

void GA_election(int size, int id1, int id2, double * offspring_1, double * offspring_2)
{
	int k;
	double * parent_1;
	double * parent_2;

    /*********************** Start of Election function ****************************************************/		
	//void GA_election() : applies to each pair, nr_pair j=0,...,N_pairs
	// 4. Election: before 2 offsprings are added to the gene pool, they are tested against their parents.
	// Out ot the 4 candidates (2 parents, 2 offsprings) the best 4 are retained.
	if (GA_PARAMETERS.election)
	{
		//1. Make 2 local copies of the parents
		
		 parent_1 = malloc(sizeof(double)*NR_PARAMS);
		 parent_2 = malloc(sizeof(double)*NR_PARAMS);

		for (k=0; k<size; k++)
		{
 		    parent_1[k]=PUBLIC_CLASSIFIERSYSTEM.ruletable[id1].parameters[k];
 		    parent_2[k]=PUBLIC_CLASSIFIERSYSTEM.ruletable[id2].parameters[k];
		}
		
		
		//2. Compare the 2 offspring to their 2 parents: test for higher fitness between 2 offspring and 2 parents
		// void election(int size, double * offspring_1, double * offspring_2, double * parent_1, double * parent_2)
		 election(NR_PARAMS, offspring_1, offspring_2, parent_1, parent_2);
		
		//3. Add 2 out of 4 best bitstrings to new generation
		
	}
    /*********************** End of Election function ****************************************************/
 	free(parent_1);
 	free(parent_2);
}

void GA_reinsertion(int size, int id1, int id2, double * offspring_1, double * offspring_2)
{
	int k;
	
    /*********************** Start of Reinsertion function ****************************************************/
	//void GA_reinsertion() : applies to each pair
	//5. Finally, add the new strings to the population to replace the old ones
	//This means: copy the parameters into the classifier system
	for (k=0; k<size; k++)
	{
		PUBLIC_CLASSIFIERSYSTEM.ruletable[id1].parameters[k] = offspring_1[k];
		PUBLIC_CLASSIFIERSYSTEM.ruletable[id2].parameters[k] = offspring_2[k];
	}
	/*********************** End of Reinsertion function ****************************************************/
}

int FinancialAgent_print_public_classifiersystem()
{	
	char str[10];
	char * filename;
	FILE * file;

	int i, j, rule_id, counter;
	double performance, avg_performance;

 	//Set the output file:
 	i = sprintf(str, "%d", iteration_loop);
 	
 	//Start an empty string for the filename
 	filename = malloc(40*sizeof(char));
 	filename[0]=0;
 	
 	//Concatenate
 	strcpy(filename, "./log/CS_FinancialAgent_");
 	strcat(filename, str);
 	strcat(filename, ".txt");

 	//Open a file pointer: FILE * file 
 	file = fopen(filename,"w");


    //Print FinancialAdvisor classifier system:
    fprintf(file,"=============================================================================================\n");
    fprintf(file,"FinancialAdvisor:\n");
    fprintf(file,"rule\t performance\t counter\t avg_performance\t rule details\n");
    fprintf(file,"=============================================================================================\n"); 

    for (i=0;i<PUBLIC_CLASSIFIERSYSTEM.nr_rules;i++)
    {
         rule_id 		= PUBLIC_CLASSIFIERSYSTEM.ruletable[i].id;
         performance 	= PUBLIC_CLASSIFIERSYSTEM.ruletable[i].performance;
		 counter 		= PUBLIC_CLASSIFIERSYSTEM.ruletable[i].counter;         
         avg_performance = PUBLIC_CLASSIFIERSYSTEM.ruletable[i].avg_performance;

         fprintf(file,"%d\t|\t %f\t|\t %7d\t|\t %f\t | ", rule_id, performance, counter, avg_performance);

         fprintf(file,"[ ");
	     for (j=0;j<GA_PARAMETERS.string_size;j++){fprintf(file,"%02.1f ", PUBLIC_CLASSIFIERSYSTEM.ruletable[i].parameters[j]);}
	     fprintf(file,"]\n"); 
    }
     fprintf(file,"=============================================================================================\n");

    fprintf(file,"\n");
	fclose(file);
	
	free(filename);
    return 0;
}
// *********** END GA AUXILIARY FUNCTIONS ****************************
