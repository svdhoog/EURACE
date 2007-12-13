#include "header.h"
#include "Firm_agent_header.h"
#include "my_library_header.h"
#include "math.h"



/************************************ Firm agent functions ************************************/


/************Firm Role: Labour Market********************************/

/** \fn Firm_calc_vacancies_and_send()
 * \brief Firms post vacancies and wage offer and send vacancies message
 */
int Firm_send_vacancies()
{
			

	
	int i,j;
	
	/* If day of month to act... */
	if(DAY%MONTH == DAY_OF_MONTH_TO_ACT)
	{
		/* WAGE_OFFER = MEAN_WAGE;	*/
									
		 /*If more employees then send vacancies */
		if(EMPLOYEES_NEEDED > NO_EMPLOYEES)
		{
			VACANCIES = EMPLOYEES_NEEDED - NO_EMPLOYEES;
			add_vacancies_message(ID, REGION_ID, VACANCIES, WAGE_OFFER, MSGDATA);
			
			/*Print to control*/
			/*printf("Firm %d:\t\tsent vacancies message (vacancies = %d, w_offer = %d)\n", ID, VACANCIES, WAGE_OFFER);*/
		
		}
		 /*If less employees needed then send redundencies */
		if(EMPLOYEES_NEEDED < NO_EMPLOYEES)
		{
			VACANCIES = 0;
			/*sorting employees: highest specific skills first*/
	int a, b;
	employee * first_employee, * second_employee;
	employee * temp_employee = (employee *)malloc(sizeof(employee));
	
	/* Using a bubble sort */
	for(a = 0; a<(EMPLOYEES->size-1); a++)
	{
		for(b=0; b<(EMPLOYEES->size-1)-a; b++) 
		{
			first_employee = EMPLOYEES->array[b+1];
			second_employee = EMPLOYEES->array[b];
			
			/* Comparing the values between neighbours */
			if(first_employee->specific_skill > second_employee->specific_skill)
			{
				/* Swap neighbours */
				* temp_employee = * second_employee;
				* second_employee = * first_employee;
				* first_employee = * temp_employee;
			}
		}
	}

			for(i = 0; i < (NO_EMPLOYEES - EMPLOYEES_NEEDED); i++)
			{
				
				/*Firing: last-in-first-out*/
				j = random_int(0,(EMPLOYEES->size-1));//(EMPLOYEES->size)-1;
				add_firing_message(ID, EMPLOYEES->array[j]->id, 					MSGDATA);
			
				/*Print to control*/	
				/*printf("Firm %d:\t\tsent firing message (worker id = %d)\n", 
				ID, EMPLOYEES->array[j]->id);*/
				
				remove_employee(EMPLOYEES, j);
												
			}
			
			NO_EMPLOYEES = EMPLOYEES_NEEDED;
		}
	}
	
	return 0;
}




/** \fn Firm_read_job_applications_send_offer_or_rejection()
 * \brief Read job applications, rank, and send job offers and rejections
 */
int Firm_read_job_applications_send_job_offer_or_rejection()
{	

	/* Create a job application dynamic array */
	job_application_array * job_application_list = init_job_application_array();
	
	/* If day of month to act... */
	if(DAY%MONTH == DAY_OF_MONTH_TO_ACT)
	{
		START_JOB_APPLICATION_MESSAGE_LOOP
			/* Read job application messages for this Firm */
			if(job_application_message->firm_id == ID)
			{
				add_job_application(job_application_list, 
				job_application_message->worker_id,
				job_application_message->region_id,
				job_application_message->general_skill, 
				job_application_message->specific_skill);
				
				/*Print to control*/
				/*printf("Firm %d:\t\tread application mes (worker id=%d, g_skill =%d, s_skill=%d)\n", 
				ID, job_application_message->worker_id, 
				job_application_message->general_skill, 
				job_application_message->specific_skill);*/
			}
		FINISH_JOB_APPLICATION_MESSAGE_LOOP

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	
	/*Shuffle applications before sorting: If two or more applications are equal then they 		will be ranked by chance*/
	int i,k;
	job_application * i_job_application, * k_job_application;
	job_application * temp_job_application = (job_application *)malloc(sizeof(job_application));

	for(i = 0; i < (job_application_list->size);i++)
	{
		k = random_int(i, (job_application_list->size-1));

		i_job_application = job_application_list->array[i];
		k_job_application = job_application_list->array[k];

		/* Swap neighbours */
				* temp_job_application = * i_job_application;
				* i_job_application = * k_job_application;
				* k_job_application = * temp_job_application;
	}

	free(temp_job_application);


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


		 /*Rank job applications (function from my_library_functions) */
		sort_job_application_list(job_application_list);

		
		/*printf("++++++Application List: Firm %d++++++\n", ID);
		printf("++++++Application List Size = %d++++++\n",job_application_list->size);
		 For each job application message... */
		for(i = 0; i < (job_application_list->size); i++)
		{
			/*Print to control*/
                        /*printf("Worker id = %d\n", job_application_list->array[i]->worker_id); 
  
			 If there is a vacancy send a job offer message */
			if(i < VACANCIES)
			{
			add_job_offer_message(ID, 
			job_application_list->array[i]->worker_id, REGION_ID, WAGE_OFFER, MSGDATA);
				
			/*Print to control*/	
			/*printf("Firm %d:\t\tsent job offer message (worker id = %d, w_offer = %d)\n", ID, job_application_list->array[i]->worker_id, WAGE_OFFER);*/
			}
			
			/* If no vacancies left then send a job rejection message */
			else
			{
				add_application_rejection_message(ID, 
				job_application_list->array[i]->worker_id, MSGDATA);
				
				/*Print to control*/
				/*printf("Firm %d:\t\tsent application rejection message (workerid 					=%d)\n" , ID, job_application_list->array[i]->worker_id);*/
			}
		}
	}
	
	/* Free the job application dynamic array */
	free_job_application_array(job_application_list);
	
	return 0;
}



/** \fn Firm_read_job_responses()
 * \brief Read job responses and add employees
 */
int Firm_read_job_responses()
{

	int i;	
			
	/* If day of month to act... */
	if(DAY%MONTH == DAY_OF_MONTH_TO_ACT)
	{
		START_JOB_ACCEPTANCE_MESSAGE_LOOP
			/* Read job acceptance messages for this Firm */
			if(job_acceptance_message->firm_id == ID)
			{
				/*Print to control*/
				/*printf("Firm %d:\t\tread acceptance mes (worker id = %d, g_skill 					= %d, s_skill = %d)\n", ID, job_acceptance_message->worker_id, 
				job_acceptance_message->general_skill, 
				job_acceptance_message->specific_skill);

				 Add employee to employee list */
				add_employee(EMPLOYEES, job_acceptance_message->worker_id,
				job_acceptance_message->region_id, 					WAGE_OFFER, job_acceptance_message->general_skill, 
				job_acceptance_message->specific_skill);
				
				VACANCIES--;
				NO_EMPLOYEES++;
			}
		FINISH_JOB_ACCEPTANCE_MESSAGE_LOOP

	}
		
		/*Firms have to read quitting messages every day*/
		START_QUITTING_MESSAGE_LOOP
			/*Read quittings for this firm*/
			if(quitting_message->firm_id == ID)
			{
				/*Print to control*/
				/*printf("Firm %d reads Quitting of Household %d\n",ID, 				quitting_message->worker_id);*/
				for(i=0; i < (EMPLOYEES->size);i++)
					{
					if(quitting_message->worker_id == EMPLOYEES->array[i]->id)
						{	
						remove_employee(EMPLOYEES,i);
						//printf("Removed =%d\n",i);	
					
						VACANCIES++;
						NO_EMPLOYEES--;
						}
					}
				
			}
		FINISH_QUITTING_MESSAGE_LOOP
		
		
	
	
	return 0;
}



/** \fn Firm_update_wage_offer()
 * \brief Firms update wage_offer to get more applications
 */
int Firm_update_wage_offer()
{
	if(DAY%MONTH == DAY_OF_MONTH_TO_ACT)
	{	
		if(VACANCIES > MIN_VACANCY) 
		//{
		//VACANCY_COUNTER = VACANCY_COUNTER +1;
	
		//
		//}
		//if(VACANCY_COUNTER > WAGE_INTERTIA-1) 
		{
		WAGE_OFFER = WAGE_OFFER*(1+wage_update);
		//VACANCY_COUNTER = 0;
	
		
		}
	}
	return 0;
}



/** \fn Firm_calc_vacancies_and_send_2()
 * \brief Firms which  did not fill all vacancies post vacancies and wage offer and send vacancies message
 */
int Firm_send_vacancies_2()
{

	
	/* If day of month to act... */
	if(DAY%MONTH == DAY_OF_MONTH_TO_ACT)
	{
		
		
		/* If vacancies left */
		if(VACANCIES > 0)
		{
			add_vacancies2_message(ID, REGION_ID, VACANCIES, WAGE_OFFER, MSGDATA);
			
			/*Print to control*/
			/*printf("Firm %d:\t\tsent vacancies mes again (vac = %d, w_offer = %d)\n", 				ID, VACANCIES, WAGE_OFFER);*/
		}
		
	}
	
	return 0;
}




/** \fn Firm_read_job_applications_send_offer_or_rejection()_2
 * \brief Read job applications, rank, and send job offers and rejections
 */
int Firm_read_job_applications_send_job_offer_or_rejection_2()
{

	/* Create a job application dynamic array */
	job_application_array * job_application_list = init_job_application_array();
	
	/* If day of month to act... */
	if(DAY%MONTH == DAY_OF_MONTH_TO_ACT)
	{
		START_JOB_APPLICATION2_MESSAGE_LOOP
			/* Read job application messages for this Firm */
			if(job_application2_message->firm_id == ID)
			{
				add_job_application(job_application_list, 
				job_application2_message->worker_id,
				job_application2_message->region_id, 
				job_application2_message->general_skill, 
				job_application2_message->specific_skill);
				
				/*Print to control*/
				/*printf("Firm %d:\t\tread application mes (worker id = %d, g_skill = %d, s_skill = %d)\n", 
				ID, job_application2_message->worker_id, 
				job_application2_message->general_skill, 
				job_application2_message->specific_skill);*/
			}
		FINISH_JOB_APPLICATION2_MESSAGE_LOOP

/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/
	

	int i,k;
	job_application * i_job_application, * k_job_application;
	job_application * temp_job_application = (job_application *)malloc(sizeof(job_application));

	for(i = 0; i < (job_application_list->size);i++)
	{
		k = random_int(i, (job_application_list->size-1));

		i_job_application = job_application_list->array[i];
		k_job_application = job_application_list->array[k];

		/* Swap neighbours */
				* temp_job_application = * i_job_application;
				* i_job_application = * k_job_application;
				* k_job_application = * temp_job_application;
	}

	free(temp_job_application);


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

		
		
		/* Rank job applications (function from my_library_functions) */
		sort_job_application_list(job_application_list);

		/*printf("++++++Application List-Loop 2: Firm %d++++++\n", ID);
		printf("++++++Application List Size = %d++++++\n",job_application_list->size);
		 For each job application message... */
		for(i = 0; i < job_application_list->size; i++)
		{

			/*Print to control*/
                        //printf("Worker id = %d\n", job_application_list->array[i]->worker_id);
			
			/* If there is a vacancy send a job offer message */
			if(i < VACANCIES)
			{
				add_job_offer2_message(ID, 
				job_application_list->array[i]->worker_id,
				REGION_ID, WAGE_OFFER, MSGDATA);
				
				/*Print to control*/
				/*printf("Firm %d:\t\tsent job offer message again (worker id = %d)\n", ID, job_application_list->array[i]->worker_id);*/
			}
			/* If no vacancies left then send a job rejection message */
			else
			{
				add_application_rejection2_message(ID, 
				job_application_list->array[i]->worker_id, MSGDATA);
				
				/*Print to control*/
				/*printf("Firm %d:\t\tsent job rejection message again (worker id = %d)\n", ID, job_application_list->array[i]->worker_id);*/
			}
		}
	}
	
	/* Free the job application dynamic array */
	free_job_application_array(job_application_list);
	
	return 0;
}




/** \fn Firm_read_job_responses_2()
 * \brief Read job responses and add employees
 */
int Firm_read_job_responses_2()
{
	int i;

/* If day of month to act... */
	if(DAY%MONTH == DAY_OF_MONTH_TO_ACT)
	{
		START_JOB_ACCEPTANCE2_MESSAGE_LOOP
			/* Read job acceptance messages for this Firm */
			if(job_acceptance2_message->firm_id == ID)
			{
				
				/*Print to control*/
				/*printf("Firm %d:\t\tread accept mes again (worker id = %d, g_skill = %d, s_skill = %d)\n", 
				ID, job_acceptance2_message->worker_id, 
				job_acceptance2_message->general_skill, 
				job_acceptance2_message->specific_skill);

				Add employee to employee list*/ 
				add_employee(EMPLOYEES, job_acceptance2_message->worker_id,
				job_acceptance2_message->region_id, 					WAGE_OFFER, job_acceptance2_message->general_skill, 					job_acceptance2_message->specific_skill);

				VACANCIES--;
				NO_EMPLOYEES++;
			}
		FINISH_JOB_ACCEPTANCE2_MESSAGE_LOOP

	}


		/*Firms have to read quitting messages every day*/
		START_QUITTING2_MESSAGE_LOOP
			/*Read quittings for this firm*/
			if(quitting2_message->firm_id == ID)
			{
				/*Print to control*/
				/*printf("Firm %d reads Quitting of Household %d\n",ID, quitting2_message->worker_id);*/
				for(i=0; i < (EMPLOYEES->size);i++)
					{
					if(quitting2_message->worker_id == EMPLOYEES->array[i]->id)
						{	
						remove_employee(EMPLOYEES,i);
						//printf("Removed =%d\n",i);	
					
						VACANCIES++;
						NO_EMPLOYEES--;
						}
					}
				
			}
		FINISH_QUITTING2_MESSAGE_LOOP
	
		
		
		
	
	
	return 0;
}



/** \fn Firm_update_wage_offer_2()
 * \brief Firms update wage_offer to get more applications in next iteration
 */
int Firm_update_wage_offer_2()
{
	int i;
	int j = 0;

	/*Average  general skill at the end of one day*/
	for(i = 0; i < (EMPLOYEES->size); i++)
	{
		j += EMPLOYEES->array[i]->general_skill;
		
	}
	
	if(NO_EMPLOYEES > 0)
	{
	AVERAGE_G_SKILL = (double)((double)j/(double)NO_EMPLOYEES);
	//printf("ags = %f\n", AVERAGE_G_SKILL);
	}
	

	if(DAY%MONTH == DAY_OF_MONTH_TO_ACT)
	{	
		if(VACANCIES > 0) 
		{
			//WAGE_OFFER = WAGE_OFFER*(1 + wage_update);
			//printf("Wage updated:2 Firm_id = %d Vacancies = %d\n", ID, VACANCIES);
		}
	}else{

	VACANCIES=0;
	}
	return 0;
}





/********************Firm Role Consumption Goods market  *******************************/


/** \fn Firm_calc_production_quantity()
 * \brief Firm produces the production volume depending on the current stocks in the malls
 */
int Firm_calc_production_quantity()

{

	double production_volume = 0;
	double prod_vol;

if(DAY%MONTH==DAY_OF_MONTH_TO_ACT)
{

	/*reading current mall stocks   */
	START_CURRENT_MALL_STOCK_INFO_MESSAGE_LOOP

		if(current_mall_stock_info_message->firm_id == ID)
		{
			for(int j=0; j< CURRENT_MALL_STOCKS->size;j++)
			{
				if(current_mall_stock_info_message->mall_id==CURRENT_MALL_STOCKS->array[j]->mall_id)
				{
					CURRENT_MALL_STOCKS->array[j]->current_stock= 						current_mall_stock_info_message->stock;

			
				}		
			}
		}
	FINISH_CURRENT_MALL_STOCK_INFO_MESSAGE_LOOP



	

	


sales_statistics_array * sales_mall_1 = init_sales_statistics_array();

sales_statistics_array * sales_mall_2 = init_sales_statistics_array();

for(int i=0; i < MALLS_SALES_STATISTICS->size;i++)
{

	if(i == 0)
	{
add_sales_statistics(sales_mall_1,MALLS_SALES_STATISTICS->array[i]->mall_id,MALLS_SALES_STATISTICS->array[i]->period, MALLS_SALES_STATISTICS->array[i]->sales);
	}
	else
	{
		if(MALLS_SALES_STATISTICS->array[i]->mall_id == sales_mall_1->array[0]->mall_id)
			{
			add_sales_statistics(sales_mall_1,MALLS_SALES_STATISTICS->array[i]->mall_id, MALLS_SALES_STATISTICS->array[i]->period, MALLS_SALES_STATISTICS->array[i]->sales);
			}
			else
			{
			add_sales_statistics(sales_mall_2,MALLS_SALES_STATISTICS->array[i]->mall_id, MALLS_SALES_STATISTICS->array[i]->period, MALLS_SALES_STATISTICS->array[i]->sales);
			}


	}
}
	/* Sorting the sales lists   */


	sort_mall_sales_list(sales_mall_1);

	sort_mall_sales_list(sales_mall_2);


	/* ***testing the sort algorithm */

	/*printf("Sortierte Liste mall 1:");
	for(int i =0; i<sales_mall_1->size;i++ )
	{
	printf("%f, ",sales_mall_1->array[i]->sales);

	}
	printf("\n");
	printf("Sortierte Liste mall 2:");
	for(int i=0; i<sales_mall_2->size;i++ )
	{
	printf("%f, ",sales_mall_2->array[i]->sales);

	}
	printf("\n");*/


	/*Computing the out-of-stock costs    */

	OUT_OF_STOCK_COSTS = (PRICE - DISCONT_RATE*UNIT_COSTS) / (PRICE + INVENTORY_COSTS_PER_UNIT);
	

	/* Setting the critical values:   */

	for(int i=0; i<CURRENT_MALL_STOCKS->size;i++)
	{
		if(CURRENT_MALL_STOCKS->array[i]->mall_id == sales_mall_1->array[0]->mall_id)
			{
			for(int j= 0; j < 10; j ++)
				{
				double l = (double) j;
				double k = (1+l)/10;
				
				
					if(k < OUT_OF_STOCK_COSTS )
						{
						//printf("Cost: %f vergl mit %f\n", OUT_OF_STOCK_COSTS, k);
						CURRENT_MALL_STOCKS->array[i]->critical_stock = sales_mall_1->array[j]->sales;	
					
			//printf("Critical value Mall %d: %f\n", sales_mall_1->array[0]->mall_id,CURRENT_MALL_STOCKS->array[i]->critical_stock);
						}
				
				}
			}
			else
			{
			for(int j= 0; j < 10; j ++)
				{
				double l = (double) j;
				double k = (1+l)/10;

					if(k < OUT_OF_STOCK_COSTS)
						{
						CURRENT_MALL_STOCKS->array[i]->critical_stock = sales_mall_2->array[j]->sales;	
						//printf("Critical x value Mall %d: %f\n", sales_mall_2->array[0]->mall_id,CURRENT_MALL_STOCKS->array[i]->critical_stock);
						}
				}
			}
	}
	free_sales_statistics_array(sales_mall_1);
	free_sales_statistics_array(sales_mall_2);


	/*checking whether or not the current mall stocks are below the critical values (sS-Rule)
	If this is the case refill the stock up to the max stock */
		for(int i=0;i<CURRENT_MALL_STOCKS->size;i++)
		{

			//printf("Max stock: %f\n",CURRENT_MALL_STOCKS->array[i]->max_stock);
			if(CURRENT_MALL_STOCKS->array[i]->current_stock <= CURRENT_MALL_STOCKS->array[i]->critical_stock)	
			{
				prod_vol= CURRENT_MALL_STOCKS->array[i]->critical_stock - 					CURRENT_MALL_STOCKS->array[i]->current_stock;
	
	
				PLANNED_DELIVERY_VOLUME->array[i]->mall_id= CURRENT_MALL_STOCKS->array[i]->mall_id;
				PLANNED_DELIVERY_VOLUME->array[i]->quantity= prod_vol;
				production_volume = production_volume + prod_vol;
			}
			else
			{
				PLANNED_DELIVERY_VOLUME->array[i]->mall_id= CURRENT_MALL_STOCKS->array[i]->mall_id;
				PLANNED_DELIVERY_VOLUME->array[i]->quantity= 0;
			}
		}

	
	double mean_production_quantity=0;
	for(int i=0; i<LAST_PLANNED_PRODUCTION_QUANTITIES->size;i++)
	{
	mean_production_quantity+=LAST_PLANNED_PRODUCTION_QUANTITIES->array[i];
	}
	mean_production_quantity=mean_production_quantity/LAST_PLANNED_PRODUCTION_QUANTITIES->size;
	
	PLANNED_PRODUCTION_QUANTITY=LAMBDA*production_volume+ (1-LAMBDA)*mean_production_quantity;
//	printf("planned Production quantity: ID: %d %f\n",ID, PLANNED_PRODUCTION_QUANTITY);
}
else 
{
PLANNED_PRODUCTION_QUANTITY = 0;
}
return 0;
}


//Here: we need as input memory variable the total financial resources obtained during the firm's financial planning.
//This is needed when there is a re-planning phase in case the firm could not obtain all financial resources required for the planned production quantity.
//In the computation of the production quantity, the final costs of the inputs has to be taken into account.
//Output of this function: production plan that is consistent with obtained financial resources (set in memory: cash_holdings).
int Firm_calc_input_demands()
{

	
	//printf(" Firm_calc_input_demands()\n");
	

	double temp_labour_demand;
	

	if(DAY%MONTH==DAY_OF_MONTH_TO_ACT)
	{
		
		
		
		START_PRODUCTIVITY_MESSAGE_LOOP

		TECHNOLOGICAL_FRONTIER = productivity_message->cap_productivity;
		ACTUAL_CAP_PRICE = productivity_message->cap_good_price;
		
		FINISH_PRODUCTIVITY_MESSAGE_LOOP
		
		

		if(MEAN_SPECIFIC_SKILLS < TECHNOLOGY)
		{
			temp_labour_demand = PLANNED_PRODUCTION_QUANTITY / (pow((BETA/ALPHA*MEAN_WAGE/	ACTUAL_CAP_PRICE),BETA)*MEAN_SPECIFIC_SKILLS);
			
			NEEDED_CAPITAL_STOCK = temp_labour_demand * BETA/ALPHA*MEAN_WAGE/	ACTUAL_CAP_PRICE; 
			
			//printf("Needed Capital %f\n",NEEDED_CAPITAL_STOCK);
			//printf("PRodQuan %f\n",PRODUCTION_QUANTITY);
			//printf("BRUCH %f\n",pow((BETA/ALPHA*MEAN_WAGE/actual_cap_price),BETA)*MEAN_SPECIFIC_SKILLS);

			//printf("Temp labour 1%f\n",temp_labour_demand);
		}
		else
		{
			temp_labour_demand = PLANNED_PRODUCTION_QUANTITY / (pow(BETA/ALPHA*MEAN_WAGE/ACTUAL_CAP_PRICE,BETA)*TECHNOLOGY);
			//printf("PRodQuan %f\n",PRODUCTION_QUANTITY);
			//printf("Temp labour 2%f",temp_labour_demand);
			//printf("BRUCH %f\n",pow((BETA/ALPHA*MEAN_WAGE/actual_cap_price),BETA)*MEAN_SPECIFIC_SKILLS);
			NEEDED_CAPITAL_STOCK = temp_labour_demand * BETA/ALPHA*MEAN_WAGE/ACTUAL_CAP_PRICE; 
			//printf("Needed Capital %f\n",NEEDED_CAPITAL_STOCK);

		}

		if(NEEDED_CAPITAL_STOCK>(1-DEPRICIATION_RATE)*CAPITAL_STOCK)
		{
		//add_capital_good_request_message(ID,NEEDED_CAPITAL_STOCK - (1-DEPRICIATION_RATE)*CAPITAL_STOCK,MSGDATA);	
			
			CAPITAL_STOCK = (1-DEPRICIATION_RATE)*CAPITAL_STOCK;
			
			if(temp_labour_demand<1 && temp_labour_demand>0)
			{
			EMPLOYEES_NEEDED = 1;
			}else
			{
			EMPLOYEES_NEEDED = (int) temp_labour_demand;
			}
		}
		else
		{

		CAPITAL_STOCK = (1-DEPRICIATION_RATE)*CAPITAL_STOCK;
		

			if(MEAN_SPECIFIC_SKILLS>TECHNOLOGY)
			{
				EMPLOYEES_NEEDED = (int) (pow(PLANNED_PRODUCTION_QUANTITY/(TECHNOLOGY*     pow(CAPITAL_STOCK,BETA)),1/ALPHA));
			}
			else
			{
				EMPLOYEES_NEEDED = (int) (pow(PLANNED_PRODUCTION_QUANTITY/ (MEAN_SPECIFIC_SKILLS*pow(CAPITAL_STOCK,BETA)),1/ALPHA));
			}

		}
		

	}
return 0;
}


int Firm_send_goods_to_mall()
{ 
	//printf(" Firm_send_goods_to_mall()");
	if(DAY%MONTH==DAY_OF_MONTH_TO_ACT)
	{
		double delivery_volume=0;

		for(int i=0; i<PLANNED_DELIVERY_VOLUME->size; i++)
		{
			delivery_volume+=PLANNED_DELIVERY_VOLUME->array[i]->quantity;

		}


		for(int i=0; i<PLANNED_DELIVERY_VOLUME->size; i++)
		{

			for(int j=0; j<DELIVERY_VOLUME->size; j++)

			if(DELIVERY_VOLUME->array[j]->mall_id == PLANNED_DELIVERY_VOLUME->array[i]->mall_id)
	{


			
			if(delivery_volume>PRODUCTION_QUANTITY)
			{
				DELIVERY_VOLUME->array[j]->quantity = PRODUCTION_QUANTITY / delivery_volume * PLANNED_DELIVERY_VOLUME->array[i]->quantity;
			}else if(PRODUCTION_QUANTITY > delivery_volume && delivery_volume !=0)
			{
			DELIVERY_VOLUME->array[j]->quantity = PRODUCTION_QUANTITY / delivery_volume* PLANNED_DELIVERY_VOLUME->array[i]->quantity;

			}else if(PRODUCTION_QUANTITY > delivery_volume && delivery_volume ==0)
{	

			DELIVERY_VOLUME->array[j]->quantity =0;
	}
			else
			{
			DELIVERY_VOLUME->array[j]->quantity = 	 PLANNED_DELIVERY_VOLUME->array[i]->quantity;
			}
			DELIVERY_VOLUME->array[j]->quality=PLANNED_DELIVERY_VOLUME->array[j]->quality;
			DELIVERY_VOLUME->array[j]->price=PLANNED_DELIVERY_VOLUME->array[j]->price;
			
			
			add_update_mall_stock_message(DELIVERY_VOLUME->array[j]->mall_id,ID,DELIVERY_VOLUME->array[j]->quantity,QUALITY,PRICE,MSGDATA);
			//printf("Message: update_mall_stock_message(%d, %f,%f)\n",DELIVERY_VOLUME->array[j]->mall_id,PRICE,DELIVERY_VOLUME->array[i]->quantity);
		}}

	}

return 0;
}


int Firm_calc_revenue()
{
	REVENUE_PER_DAY=0.0;
	TOTAL_SOLD_QUANTITY=0.0;
	double dividend_per_household;
	START_SALES_MESSAGE_LOOP
	
	if(sales_message->firm_id==ID)
	{
		
		for(int i=0; i< SOLD_QUANTITIES->size; i++)
		{
			if(sales_message->mall_id ==  SOLD_QUANTITIES->array[i]->mall_id)
			{
				SOLD_QUANTITIES->array[i]->sold_quantity += sales_message->revenue/PRICE;
				
			
			
			
			REVENUE_PER_DAY += sales_message->revenue;
			
			CUM_REVENUE += sales_message->revenue;
		
			

			TOTAL_SOLD_QUANTITY += sales_message->revenue/PRICE;
			
			if((sales_message->stock_empty == 1) && (SOLD_QUANTITIES->array[i]->stock_empty== 0))
			{
			if(DAY%MONTH>=DAY_OF_MONTH_TO_ACT)
			{
			SOLD_QUANTITIES->array[i]->estimated_demand =SOLD_QUANTITIES->array[i]->sold_quantity + (((SOLD_QUANTITIES->array[i]->sold_quantity)/(DAY%MONTH))*20 - SOLD_QUANTITIES->array[i]->sold_quantity)*ADDITIONAL_PRODUCTION;

			//printf("Menge = %f schätzung = %f\n",SOLD_QUANTITIES->array[i]->sold_quantity, SOLD_QUANTITIES->array[i]->estimated_demand);
			}

			if(DAY%MONTH == 0)
			{
			SOLD_QUANTITIES->array[i]->estimated_demand =SOLD_QUANTITIES->array[i]->sold_quantity;
			//printf("Menge = %f schätzung = %f\n",SOLD_QUANTITIES->array[i]->sold_quantity, SOLD_QUANTITIES->array[i]->estimated_demand);
			}

/*else
			{
			SOLD_QUANTITIES->array[i]->estimated_demand =SOLD_QUANTITIES->array[i]->sold_quantity + (((SOLD_QUANTITIES->array[i]->sold_quantity)/(DAY%MONTH))*20 - SOLD_QUANTITIES->array[i]->sold_quantity)*ADDITIONAL_PRODUCTION;

			}*/

			
			SOLD_QUANTITIES->array[i]->stock_empty = 1;
			}
			

		

			
			}
			
			
		}


	}

	FINISH_SALES_MESSAGE_LOOP
	CUM_TOTAL_SOLD_QUANTITY+=TOTAL_SOLD_QUANTITY;	
	//printf("TOTAL_SOLD_QUANTITY %f \n",TOTAL_SOLD_QUANTITY);


	if(DAY%MONTH==DAY_OF_MONTH_TO_ACT)
	{
		
		
				//+++++++++++++++++++++++
				//inventory rule
			
				int remove_index[MALLS_SALES_STATISTICS->size];
				//int remove_index[2];
				int i=0;
				for(int j=0; j < MALLS_SALES_STATISTICS->size;j++)
			
				{
					
					 
					if(MALLS_SALES_STATISTICS->array[j]->period== 10)
						{
						
						
						remove_index[i]= j;
						i++;
													
						}
					
	
				}


			
			remove_sales_statistics(MALLS_SALES_STATISTICS, remove_index[1]);
			remove_sales_statistics(MALLS_SALES_STATISTICS, remove_index[0]);
			
			
			for(int j=0; j < MALLS_SALES_STATISTICS->size;j++)
			
				{
					//printf("Period: %d\n",MALLS_SALES_STATISTICS->array[j]->period);
					MALLS_SALES_STATISTICS->array[j]->period++;	
				}
				
				
			for(int i=0; i< SOLD_QUANTITIES->size;i++)
			{
			if(SOLD_QUANTITIES->array[i]->stock_empty ==0)
			{
			add_sales_statistics(MALLS_SALES_STATISTICS,SOLD_QUANTITIES->array[i]->mall_id, 1 , SOLD_QUANTITIES->array[i]->sold_quantity);
			}else
			{
			add_sales_statistics(MALLS_SALES_STATISTICS,SOLD_QUANTITIES->array[i]->mall_id, 1 , SOLD_QUANTITIES->array[i]->estimated_demand);

			}
			SOLD_QUANTITIES->array[i]->sold_quantity=0;
			SOLD_QUANTITIES->array[i]->stock_empty=0;
			SOLD_QUANTITIES->array[i]->estimated_demand =0;
			}

		if(PLANNED_PRODUCTION_QUANTITY>PRODUCTION_QUANTITY  && DAY>20)
		{
			double mean_wage =0.0;
					
			for( int i=0; i < EMPLOYEES->size; i++)
			{
				/* EMPLOYEES->array[i]->wage = EMPLOYEES->array[i]->wage*1.0;*/
				mean_wage+= EMPLOYEES->array[i]->wage;
				
			}
			double no_employees = (int) NO_EMPLOYEES;
			MEAN_WAGE = mean_wage/no_employees; 	
		}		
					
	}

	if(DAY%MONTH==DAY_OF_MONTH_TO_ACT)
	{
		CUM_TOTAL_SOLD_QUANTITY=0.0;
		CUM_REVENUE =0.0;
	}
	// BREAK: Thıs ıs the end of the productıon cycle
	// We contınue from here wıth the Fırm::Fınancıal Management Role, functıon: compute_ıncome_statement

	return 0;
}



/*
 * \fn:int Firm_calc_pay_costs()
 * \brief: Thıs functıon computes the actual production costs and sets the actual output produced: production_quantity.
 */	
int Firm_calc_pay_costs()
{
	
	
	double capital_costs;
	double labour_costs;
	//printf("Firm_calc_pay_costs\n");

	if(DAY%MONTH==DAY_OF_MONTH_TO_ACT)
	{

		//Calculate mean wage and mean specific skills:
		double ave_wage =0.0;
		double ave_spec_skills =0.0;
		


		for(int i=0;i<EMPLOYEES->size;i++)
		{

			ave_wage+=EMPLOYEES->array[i]->wage;
			ave_spec_skills += EMPLOYEES->array[i]->specific_skill;

		}
		double no_employees = (double) NO_EMPLOYEES;
		if(no_employees==0)
		{
		MEAN_WAGE = WAGE_OFFER;
		MEAN_SPECIFIC_SKILLS = 1;
		}else
		{
		MEAN_WAGE = ave_wage /( no_employees);
		
		MEAN_SPECIFIC_SKILLS =ave_spec_skills/no_employees;
		}
		//printf("MEAN_SPECIFIC_SKILLS: %f",MEAN_SPECIFIC_SKILLS);
		//printf("MEAN_WAGE: %f",MEAN_WAGE);
		//Update of technology
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
		

			



			START_CAPITAL_GOOD_DELIVERY_MESSAGE_LOOP
		
			TECHNOLOGICAL_FRONTIER = capital_good_delivery_message->productivity;

			if(ID == capital_good_delivery_message->firm_id)
			{
				TECHNOLOGY = CAPITAL_STOCK / (CAPITAL_STOCK + capital_good_delivery_message->capital_good_delivery_volume)*TECHNOLOGY + capital_good_delivery_message->capital_good_delivery_volume/(CAPITAL_STOCK + capital_good_delivery_message->capital_good_delivery_volume)*capital_good_delivery_message->productivity;

				CAPITAL_STOCK += capital_good_delivery_message->capital_good_delivery_volume;

				capital_costs = capital_good_delivery_message->capital_good_delivery_volume* capital_good_delivery_message->capital_good_price;

				ACTUAL_CAP_PRICE =capital_good_delivery_message->capital_good_price;

				TECHNOLOGICAL_FRONTIER = capital_good_delivery_message->productivity;
			}

		FINISH_CAPITAL_GOOD_DELIVERY_MESSAGE_LOOP

		if(PLANNED_PRODUCTION_QUANTITY!=0)
		{
		
		if(MEAN_SPECIFIC_SKILLS<TECHNOLOGY)
		{	
			
			PRODUCTION_QUANTITY = MEAN_SPECIFIC_SKILLS * pow(NO_EMPLOYEES,ALPHA)*pow(CAPITAL_STOCK,BETA); 
		}
		else 
		{
			PRODUCTION_QUANTITY = TECHNOLOGY * pow(NO_EMPLOYEES,ALPHA)*pow(CAPITAL_STOCK,BETA); 
		}

		//printf("Production Quantity in calc_pay_costs: %f\n",PRODUCTION_QUANTITY);

		//Pay cost:

		add_pay_capital_goods_message(ID,capital_costs,MSGDATA );
		labour_costs=0;

		for(int i=0; i<EMPLOYEES->size;i++)
		{
			labour_costs += EMPLOYEES->array[i]->wage;

			
			add_wage_payment_message(ID,EMPLOYEES->array[i]->id,EMPLOYEES->array[i]->wage,TECHNOLOGICAL_FRONTIER,MEAN_SPECIFIC_SKILLS,MSGDATA);
		}
	if(PRODUCTION_QUANTITY!=0 )
		{
		//UNIT_COSTS = (labour_costs + NEEDED_CAPITAL_STOCK *DEPRICIATION_RATE*ACTUAL_CAP_PRICE) / PRODUCTION_QUANTITY;

		UNIT_COSTS = (labour_costs + CAPITAL_STOCK *DEPRICIATION_RATE*ACTUAL_CAP_PRICE) / PRODUCTION_QUANTITY;
		
		
		//UNIT_COSTS  = (capital_costs + labour_costs)/PRODUCTION_QUANTITY;
		PRICE_LAST_MONTH=PRICE;
		PRICE = UNIT_COSTS*(1 + MARK_UP);

		
		//}else 
		//{
		//UNIT_COSTS=100000.0;
		
		}
		COSTS = capital_costs + labour_costs;
		

		}
		printf("Production Quantity : %f\n",PRODUCTION_QUANTITY);

		remove_double(LAST_PLANNED_PRODUCTION_QUANTITIES,0);
		add_double(LAST_PLANNED_PRODUCTION_QUANTITIES,PLANNED_PRODUCTION_QUANTITY);


		//printf("Cap : %f\n",CAPITAL_STOCK);


	}else PRODUCTION_QUANTITY =0;

return 0;
}

/************Firm Role: Fınancıal Management Role ********************************/


int Firm_compute_income_statement()
{
	if(DAY%MONTH==DAY_OF_MONTH_TO_ACT)
	{
		//old: EARNINGS = CUM_TOTAL_SOLD_QUANTITY*PRICE - COSTS;
		EBIT= CUM_TOTAL_SOLD_QUANTITY*PRICE - COSTS;		
		

		CASH_HOLDINGS += CUM_TOTAL_SOLD_QUANTITY*PRICE;
		CASH_HOLDINGS -= TOTAL_INTEREST_PAYMENT;
		CASH_HOLDINGS -= TAX_PAYMENT;

		//Firm_compute_interest_payments()
		//loop over all loans and compute
		//INTEREST_PAYMENT[nrloans]
		//TOTAL_INTEREST_PAYMENT

		EARNINGS = EBIT - TOTAL_INTEREST_PAYMENT;
		TAX_PAYMENT = TAX_RATE_CORPORATE* EARNINGS;
		NET_EARNINGS = EARNINGS -TAX_PAYMENT;
	}

	return 0;
}

int Firm_compute_balance_sheet()
{
	double debt_install_payment;
	int bank_id;
	double sum;

	if(DAY%MONTH==DAY_OF_MONTH_TO_ACT)
	{

		//Code: compute total debt installment payments
		//set: double_array DEBT_INSTALL_PAYMENTS and double TOTAL_DEBT_PAYMENT
		//struct DEBT_INSTALL_PAYMENTS: array of structs wıth each struct a loan
		//int bank_id
		//double loan_value
		//double interest_rate
		//double debt_install_payment
		//int maturity_day

		//Sending debt_installment_payment_msg to all banks at whıch the fırm has a loan 
		imax = DEBT_INSTALL_PAYMENTS->size;
		for (i=0; i<imax;i++)
		{
			debt_install_payment = DEBT_INSTALL_PAYMENTS->array[i]->debt_install_payment;
			bank_id = DEBT_INSTALL_PAYMENTS->array[i]->bank_id;				

			//Code: reduce the value of each loan by the debt_install_payment:

			//pay the installment
			DEBT_INSTALLMENT_PAYMENTS->array[i]->loan_value -= DEBT_INSTALLMENT_PAYMENTS->array[i]->debt_install_payment;
			//tell the bank I paid
			add_debt_installment_payment_message(FİRM_İD, debt_install_payment, bank_id)
		}


		if (CASH_HOLDINGS < TOTAL_DEBT_PAYMENT)
		{
			//Code: transform debt into equıty
			//Code: debt ıs repaid partially
			//Code: compute debt remaining to be paıd

			CASH_HOLDINGS = 0;
		}
		else
		{
			CASH_HOLDINGS -= TOTAL_DEBT_PAYMENT;
			//Code: compute interest_payments, total_interest_payment

		}

		//total_dividend_payment
		//compute dividend_per_share
		//add_dividend_message(dividend_per_share,MSGDATA);

		//compute the equıty of the fırm
		//CASH_HOLDINGS
		//VALUE_CAPITAL_STOCK: estımated value of capital stock

		//VALUE_CAPITAL_STOCK ıs a double array wıth elements for each new purchase of capıtal stock
		//(the capıtal stock ıs dated)
		//DEPRECIATION_VALUE_OF_CAPITAL_STOCK_PER_PERIOD: we assumed that the capıtal deprecıatıon ıs constant
		//over tıme and unıform over the deprecıatıon perıod. Also ıt ıs the same for each new purchase of 			//capıtal. Thıs may change lateron when we let the deprecıatıon_value be a functıon of the value of
		//capıtal stock at the tıme of purchase.

		//We loop over all elements and update the current_value of each dated capıtal stock
		imax = VALUE_CAPITAL_STOCK->sıze;
		for (i=0;i<imax;i++)
		{
			VALUE_CAPITAL_STOCK->array[i]->current_value -= DEPRECIATION_VALUE_OF_CAPITAL_STOCK_PER_PERIOD;
		}

		//VALUE_LOCAL_INVENTORY: estımated value of local ınventory stocks at current mall prıces
		//We loop over all malls and sum the value of all local ınventory stocks
		imax = CURRENT_MALL_STOCKS->size;
		sum=0;
		for (i=0;i<imax;i++)
		{
			sum += PRICE*CURRENT_MALL_STOCKS->array[i]->current_stock;
			//When malls have dıfferent current_price use thıs code:
			//sum += CURRENT_MALL_STOCKS->array[i]->current_prıce * CURRENT_MALL_STOCKS->array[i]->current_stock;
		}
		VALUE_LOCAL_INVENTORY=sum;

		EQUITY = CASH_HOLDINGS + VALUE_CAPITAL_STOCK + VALUE_LOCAL_INVENTORY;
	}

	return 0;
}


