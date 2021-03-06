#include "../header.h"
#include "../Household_agent_header.h"
#include "../my_library_header.h"
#include "Labour_aux_headers.h"

/**********************************Household Role: Labour Market**********************************/
/** \fn Household_receive_wage()
 * \brief Household receives wage if the household is employed
 */

int Household_receive_wage()
{

    double mean_income = 0.0;
    
    /*Household reads the wage messages if employed*/
    START_WAGE_PAYMENT_MESSAGE_LOOP

    
        WAGE = wage_payment_message->payment;
    /*Simon: 16.06.2009*/
        WAGE_RESERVATION = WAGE;
        remove_double(&LAST_INCOME,0);
        
        TOTAL_INCOME= wage_payment_message->payment +  CUM_TOTAL_DIVIDENDS + MONTHLY_BOND_INTEREST_INCOME;
             
        add_double(&LAST_INCOME,TOTAL_INCOME);

        /*Compute a mean income of the last four month*/
        int i;
        for(i = 0; i < 4;i++)
        {
            mean_income += LAST_INCOME.array[i];
        }

        MEAN_INCOME= mean_income/4;

        /*Add wage on account   */
        PAYMENT_ACCOUNT += wage_payment_message->payment;
        
   
    CURRENT_PRODUCTIVITY_EMPLOYER = wage_payment_message-> productivity;
    CURRENT_MEAN_SPECIFIC_SKILLS_EMPLOYER =wage_payment_message->average_specific_skills;
    FINISH_WAGE_PAYMENT_MESSAGE_LOOP


    return 0;

}

/** \fn Household_update_specific_skills()
 * \brief Household's specific skills are updated if the household is employed
 */

int Household_update_specific_skills()
{


    if(SPECIFIC_SKILL < CURRENT_PRODUCTIVITY_EMPLOYER)
    {



        SPECIFIC_SKILL = SPECIFIC_SKILL + (CURRENT_PRODUCTIVITY_EMPLOYER - SPECIFIC_SKILL)*((1-pow(0.5,1/(20+0.25*(GENERAL_SKILL-1)*(4-20))))+ 0*CURRENT_MEAN_SPECIFIC_SKILLS_EMPLOYER);


        add_specific_skill_update_message(ID,EMPLOYEE_FIRM_ID,SPECIFIC_SKILL);

    }

    return 0;

}

/** \fn Household_read_firing_messages()
 * \brief Read firing messages
 */
int Household_read_firing_messages()
{

    /* If employed... */

    /* Check for firing message */
    START_FIRING_MESSAGE_LOOP

        /*If employee is fired*/
        if(firing_message->worker_id == ID)
        {
            EMPLOYEE_FIRM_ID = -1;
            LAST_LABOUR_INCOME = WAGE;
            WAGE = 0;
            ON_THE_JOB_SEARCH = 0;
            //DAY_OF_MONTH_RECEIVE_INCOME = 0;
        }

    FINISH_FIRING_MESSAGE_LOOP


    return 0;
}



/** \fn Household_UNEMPLOYED_read_job_offers_send_response()
 * \brief Read job offers, rank, and send response
 */
int Household_UNEMPLOYED_read_job_vacancies_and_send_applications()
{
    /* Create a vacancy dynamic array to store vacancies*/
    vacancy_array  vacancy_list;
    init_vacancy_array(&vacancy_list);
    int i=0;
    int j=0;
    double wage_offer=0.0;
    
    /*if unemployed: search for vacancies*/

    START_VACANCIES_MESSAGE_LOOP
 
    /*Unemployed take into account only the vacancy messages with the wage offer for            the correspondent general skill level*/
        if(GENERAL_SKILL == 1)
        {
            wage_offer = vacancies_message->firm_wage_offer_for_skill_1;
        }
    
        if(GENERAL_SKILL == 2)
        {
            wage_offer = vacancies_message->firm_wage_offer_for_skill_2;
        }
    
        if(GENERAL_SKILL == 3)
        {
            wage_offer =  vacancies_message->firm_wage_offer_for_skill_3;
        }
    
        if(GENERAL_SKILL == 4)
        {
            wage_offer =  vacancies_message->firm_wage_offer_for_skill_4;
        }
    
        if(GENERAL_SKILL == 5)
        {
            wage_offer =  vacancies_message->firm_wage_offer_for_skill_5;
        }
    
        /*wage offer has to be equal or higher than the reservation wage*/
        if(wage_offer >= WAGE_RESERVATION)
        {
            /*same region: Firm and Household*/
            if(REGION_ID == vacancies_message->region_id)
            {
                add_vacancy(&vacancy_list,
                vacancies_message->firm_id, vacancies_message->region_id,
                wage_offer);
            }
            else /*take into account the costs of a different region:                           Firm and Household; Households can only apply in                            neighboring regions*/
            {
                /*For every neighboring region*/
                
                for(i = 0;i < NEIGHBORING_REGION_IDS.size; i++)
                {
                    /*If vacancy is in a neighboring region*/
                    if(vacancies_message->region_id
                    ==NEIGHBORING_REGION_IDS.array[i])
                    {

			/* if((wage_offer -
                         REGION_COST*COMMUTING_COSTS_PRICE_LEVEL_WEIGHT) >= WAGE_RESERVATION)*/
                        if((wage_offer -
                         REGION_COST) >= WAGE_RESERVATION)
                        {
                            add_vacancy(&vacancy_list, vacancies_message->firm_id,                                     vacancies_message->region_id,
                            (wage_offer - REGION_COST));


			/*add_vacancy(&vacancy_list, vacancies_message->firm_id,                                     vacancies_message->region_id,
                            (wage_offer - REGION_COST*COMMUTING_COSTS_PRICE_LEVEL_WEIGHT));*/
                        }
                        break;
                    }
                 }
            }
        }
    

    FINISH_VACANCIES_MESSAGE_LOOP


    /*Maximum number of applications for each person*/
    if(vacancy_list.size > NUMBER_APPLICATIONS)
    {
        /*remove vacancies from the list randomly until the list contains as many               vacancies as a household can send applications*/
        while(vacancy_list.size > NUMBER_APPLICATIONS)
        {
            j = random_int(0, (vacancy_list.size-1));
            remove_vacancy(&vacancy_list, j);
        }
    }

    /* Rank the vacancy list (function from my_library_functions) */
    //sort_vacancy_list(&vacancy_list);

    qsort(vacancy_list.array, vacancy_list.size, sizeof(vacancy),
    vacancy_list_rank_wage_offer_function);

    /* If the vacancy list is bigger than zero then send a job application to every         vacancy on the list */
    for(i = 0; i < (vacancy_list.size); i++)
    {
        add_job_application_message(ID, vacancy_list.array[i].firm_id,              REGION_ID, GENERAL_SKILL, SPECIFIC_SKILL);
    }



    /* Free the vacancy dynamic array */
    free_vacancy_array(&vacancy_list);

    return 0;
}

/** \fn Household_read_job_offers_send_response()
 * \brief Read job offers, rank, and send response
 */
int Household_read_job_offers_send_response()
{
    /* Create a job offer dynamic array */
    job_offer_array job_offer_list;
    init_job_offer_array(&job_offer_list);

    /* If unemployed...or on the job search */

    START_JOB_OFFER_MESSAGE_LOOP

        /* Read job offer messages for this Household */
        if(job_offer_message->worker_id == ID)
        {
            /*Job offers of firms in the same region*/
            if(REGION_ID == job_offer_message->region_id)
            {
                add_job_offer(&job_offer_list,
                job_offer_message->firm_id, job_offer_message->region_id,
                job_offer_message->wage_offer);
            }
            else/*Job offers of firms in different regions*/
            {
                add_job_offer(&job_offer_list,
                job_offer_message->firm_id, job_offer_message->region_id,
                (job_offer_message->wage_offer - REGION_COST));

		 /*add_job_offer(&job_offer_list,
                job_offer_message->firm_id, job_offer_message->region_id,
                (job_offer_message->wage_offer - REGION_COST*COMMUTING_COSTS_PRICE_LEVEL_WEIGHT));*/
            }
        }

    FINISH_JOB_OFFER_MESSAGE_LOOP
/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    /*Shuffle job offers before sorting: two or more equal offers will be ranked be         chance*/
    /*int i,k;
    job_offer * i_job_offer, * k_job_offer;
    job_offer * temp_job_offer = (job_offer *)malloc(sizeof(job_offer));

    for(i = 0; i < (job_offer_list.size);i++)
    {
        k = random_int(i, (job_offer_list.size-1));

        i_job_offer = &job_offer_list.array[i];
        k_job_offer = &job_offer_list.array[k];*/

            /* Swap neighbours */
            /** temp_job_offer = * i_job_offer;
            * i_job_offer = * k_job_offer;
            * k_job_offer = * temp_job_offer;
    }

    free(temp_job_offer);*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    /* Rank job offers (function from my_library_functions) */
    //sort_job_offer_list(&job_offer_list);

    /* Rank job offers with random for equal elements (function from
     my_library_functions) */
    qsort(job_offer_list.array, job_offer_list.size, sizeof(job_offer),
    job_offer_list_rank_wage_offer_function);

    /* Accept best job: first on the list (array[0]) */
    if(job_offer_list.size > 0)
    {
        add_job_acceptance_message(ID, job_offer_list.array[0].firm_id,                 REGION_ID, GENERAL_SKILL, SPECIFIC_SKILL);

        /*If on the job search: send quitting message*/
        if(ON_THE_JOB_SEARCH == 1)
        {
            add_quitting_message(ID, EMPLOYEE_FIRM_ID);
        }

        EMPLOYEE_FIRM_ID = job_offer_list.array[0].firm_id;
        EMPLOYER_REGION_ID = job_offer_list.array[0].region_id;
        DAY_OF_MONTH_RECEIVE_INCOME = DAY%MONTH;

        /*update some memory variables because of the new job*/
        if(REGION_ID == job_offer_list.array[0].region_id)
        {
            WAGE = job_offer_list.array[0].wage_offer;
            WAGE_RESERVATION = WAGE;
            ON_THE_JOB_SEARCH = 0;
        }
        else
        {   /*Have to add the region cost: region costs are important for                   ranking but are part of the wage*/

           // WAGE = (job_offer_list.array[0].wage_offer + REGION_COST*COMMUTING_COSTS_PRICE_LEVEL_WEIGHT);
            //WAGE_RESERVATION = WAGE - REGION_COST*COMMUTING_COSTS_PRICE_LEVEL_WEIGHT;

	WAGE = (job_offer_list.array[0].wage_offer + REGION_COST);
        WAGE_RESERVATION = WAGE - REGION_COST;

            ON_THE_JOB_SEARCH = 0;
        }
    }

    /* Free the job offer dynamic array */
    free_job_offer_array(&job_offer_list);

    return 0;
}



/** \fn Household_read_application_rejection_update_wage_reservation()
 * \brief Read job application rejections and update wage reservation
 */
int Household_read_application_rejection_update_wage_reservation()
{
    /* If unemployed... */

    
    WAGE_RESERVATION = WAGE_RESERVATION - WAGE_RESERVATION*WAGE_RESERVATION_UPDATE;
    

    /* Don't let wage reservation be below the current unemplyoment benefit pct*/
    if(WAGE_RESERVATION < LAST_LABOUR_INCOME*UNEMPLOYMENT_BENEFIT_PCT)
    {
        WAGE_RESERVATION = LAST_LABOUR_INCOME*UNEMPLOYMENT_BENEFIT_PCT;
    }
    


    return 0;
}



int Household_UNEMPLOYED_read_job_vacancies_and_send_applications_2()
{
    /* Create a vacancy dynamic array */
    vacancy_array  vacancy_list;
    init_vacancy_array(&vacancy_list);
    int i=0;
    int j=0;
    double wage_offer=0.0;

    /*If unemployed*/
    START_VACANCIES2_MESSAGE_LOOP
    /*read vacancy messages with the wage offer for the correspondent general skill         level of the household*/
        if(GENERAL_SKILL == 1)
        {
            wage_offer = vacancies2_message->firm_wage_offer_for_skill_1;
        }
    
        if(GENERAL_SKILL == 2)
        {
            wage_offer = vacancies2_message->firm_wage_offer_for_skill_2;
        }
    
        if(GENERAL_SKILL == 3)
        {
            wage_offer =  vacancies2_message->firm_wage_offer_for_skill_3;
        }
    
        if(GENERAL_SKILL == 4)
        {
            wage_offer =  vacancies2_message->firm_wage_offer_for_skill_4;
        }
    
        if(GENERAL_SKILL == 5)
        {
            wage_offer =  vacancies2_message->firm_wage_offer_for_skill_5;
        }

        if(wage_offer >= WAGE_RESERVATION)
        {
            /*Firm and Household are in the same region*/
            if(REGION_ID == vacancies2_message->region_id)
            {
                add_vacancy(&vacancy_list, vacancies2_message->firm_id, vacancies2_message->region_id,
                wage_offer);
            }
            else /*Firm and Household are in different region: take                     into account the region costs*/
            {
                /*For every neighboring region*/
                int i;
                for(i = 0; i < NEIGHBORING_REGION_IDS.size;i++)
                {
                    /*If vacancy is in a neighboring region*/
                    if(vacancies2_message->region_id
                    ==NEIGHBORING_REGION_IDS.array[i])
                    {
                      /*  if((wage_offer -
                        REGION_COST*COMMUTING_COSTS_PRICE_LEVEL_WEIGHT) >= WAGE_RESERVATION)*/
			 if((wage_offer -
                        REGION_COST) >= WAGE_RESERVATION)
                        {
                            add_vacancy(&vacancy_list,
                            vacancies2_message->firm_id, vacancies2_message->region_id,
                            (wage_offer - REGION_COST));

			/*add_vacancy(&vacancy_list,
                            vacancies2_message->firm_id, vacancies2_message->region_id,
                            (wage_offer - REGION_COST*COMMUTING_COSTS_PRICE_LEVEL_WEIGHT));*/
                        }
                        break;

                    }
                 }
            }
        }
    

    FINISH_VACANCIES2_MESSAGE_LOOP

    /*Maximum number of applications for each person*/
    if(vacancy_list.size > NUMBER_APPLICATIONS)
    {
        /*remove vacancies from the list randomly until the list contains as many               vacancies as a household can send applications*/
        while(vacancy_list.size > NUMBER_APPLICATIONS)
        {
            j = random_int(0, (vacancy_list.size-1));
            remove_vacancy(&vacancy_list, j);
        }
    }

    /* Rank the vacancy list (function from my_library_functions) */
    //sort_vacancy_list(&vacancy_list);

    qsort(vacancy_list.array, vacancy_list.size, sizeof(vacancy),
    vacancy_list_rank_wage_offer_function);

    /* If the vacancy list is bigger than zero then send  job applications to every         vacancy on the list */
    for(i = 0; i < (vacancy_list.size); i++)
    {
        add_job_application2_message(ID, vacancy_list.array[i].firm_id,                 REGION_ID, GENERAL_SKILL, SPECIFIC_SKILL);
    }


    /* Free the vacancy dynamic array */
        free_vacancy_array(&vacancy_list);


        return 0;
}

/** \fn Household_read_job_offers_send_response_2()
 * \brief Read job offers, rank, and send response
 */
int Household_read_job_offers_send_response_2()
{
    /* Create a job offer dynamic array */
    job_offer_array  job_offer_list;
    init_job_offer_array(&job_offer_list);

    /* If unemployed... or on the job search*/

    START_JOB_OFFER2_MESSAGE_LOOP
        /* Read job offer messages for this Household */
        if(job_offer2_message->worker_id == ID)
        {
            /*Job offers of firms in the same region*/
            if(REGION_ID == job_offer2_message->region_id)
            {
                add_job_offer(&job_offer_list,
                job_offer2_message->firm_id,job_offer2_message->region_id,
                job_offer2_message->wage_offer);

            }
            else/*Job offers of firms in different region*/
            {
                /*add_job_offer(&job_offer_list,
                job_offer2_message->firm_id,job_offer2_message->region_id,
                (job_offer2_message->wage_offer - REGION_COST*COMMUTING_COSTS_PRICE_LEVEL_WEIGHT));*/

		add_job_offer(&job_offer_list,
                job_offer2_message->firm_id,job_offer2_message->region_id,
                (job_offer2_message->wage_offer - REGION_COST));
            }
        }

    FINISH_JOB_OFFER2_MESSAGE_LOOP


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/


    /*int i,k;
    job_offer * i_job_offer, * k_job_offer;
    job_offer * temp_job_offer = (job_offer *)malloc(sizeof(job_offer));

    for(i = 0; i < (job_offer_list.size);i++)
    {
        k = random_int(i, (job_offer_list.size-1));

        i_job_offer = &job_offer_list.array[i];
        k_job_offer = &job_offer_list.array[k];*/

            /* Swap neighbours */
            /** temp_job_offer = * i_job_offer;
            * i_job_offer = * k_job_offer;
            * k_job_offer = * temp_job_offer;
    }

    free(temp_job_offer);*/


/*+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++*/

    /* Rank job offers (function from my_library_functions) */
    //sort_job_offer_list(&job_offer_list);

    qsort(job_offer_list.array, job_offer_list.size, sizeof(job_offer),
    job_offer_list_rank_wage_offer_function);


    /* Accept best job: first on the list (array[0]) */
    if(job_offer_list.size > 0)
    {
        add_job_acceptance2_message(ID, job_offer_list.array[0].firm_id,                REGION_ID, GENERAL_SKILL, SPECIFIC_SKILL);

        /*If on the job search add quitting message*/
        if(ON_THE_JOB_SEARCH == 1)
        {
            add_quitting2_message(ID, EMPLOYEE_FIRM_ID);
        }

        EMPLOYEE_FIRM_ID = job_offer_list.array[0].firm_id;
        EMPLOYER_REGION_ID = job_offer_list.array[0].region_id;
        DAY_OF_MONTH_RECEIVE_INCOME = DAY%MONTH;

        /*Update some memory variables because of the new job*/
        if(REGION_ID == job_offer_list.array[0].region_id)
        {
            WAGE = job_offer_list.array[0].wage_offer;
            WAGE_RESERVATION = WAGE;
            ON_THE_JOB_SEARCH = 0;
        }
        else
        {   /*Have to add the region cost: region costs are important for                   ranking but are part of the wage*/
            WAGE = (job_offer_list.array[0].wage_offer + REGION_COST);

	/*WAGE = (job_offer_list.array[0].wage_offer + REGION_COST*COMMUTING_COSTS_PRICE_LEVEL_WEIGHT);*/
            WAGE_RESERVATION = WAGE - REGION_COST;
	/*WAGE_RESERVATION = WAGE - REGION_COST*COMMUTING_COSTS_PRICE_LEVEL_WEIGHT;*/
            ON_THE_JOB_SEARCH = 0;
        }
    }

    /* Free the job offer dynamic array */
        free_job_offer_array(&job_offer_list);

    return 0;
}



/** \fn Household_read_application_rejection_update_wage_reservation_2()
 * \brief Read job application rejections and update wage reservation
 */
int Household_read_application_rejection_update_wage_reservation_2()
{

    /* If unemployed... */

    WAGE_RESERVATION = WAGE_RESERVATION-WAGE_RESERVATION*WAGE_RESERVATION_UPDATE;
    
    /* Don't let wage reservation be below 1 */
    if(WAGE_RESERVATION < LAST_LABOUR_INCOME*UNEMPLOYMENT_BENEFIT_PCT)
    {
        WAGE_RESERVATION = LAST_LABOUR_INCOME*UNEMPLOYMENT_BENEFIT_PCT;
    }
    

    return 0;
}

int Household_idle()
{
    return 0;
}

int Household_finish_labour_market()
{
    return 0;
}

/**********************************Household Role: Public Sector *********************/

/** \fn Household_send_unemployment_benefit_notification()
 * \brief This function sends a message to the government in case of being unemployed that contains the unemployment benefit.
 * The government aggregates the unemployment benefit payments and subtract the amount from its payment account.
 * The household uses the own memory variable UNEMPLOYMENT_BENEFIT_PCT to calculate its own unemployment benefit.
 */
int Household_send_unemployment_benefit_notification()
{
    double mean_income = 0.0;
    
    /*Compute unemployment_benefit*/
    //Sander, 23.10.09: Transferred this code from the government to the household
    //Now the household does the complete computation and only sends the outcome to government
            
    //Compute the individual unemployment benefit payment as a fraction of the last labour income       
    //if unemployment benefit is larger than the mean wage:
        if(LAST_LABOUR_INCOME*UNEMPLOYMENT_BENEFIT_PCT > REGION_WIDE_MEAN_WAGE*0.5 )
        {       
            UNEMPLOYMENT_PAYMENT = LAST_LABOUR_INCOME*UNEMPLOYMENT_BENEFIT_PCT;  
        }
        else    
        {
            //if unemployment benefit is below the mean wage: pay 0.5 * MEAN_WAGE
            UNEMPLOYMENT_PAYMENT =  REGION_WIDE_MEAN_WAGE*0.5; 
        }
    
    /*Add unemployment_benefit message */
    add_unemployment_notification_message(GOV_ID, UNEMPLOYMENT_PAYMENT );
    
    /*Add unemployment_benefit to account */
    PAYMENT_ACCOUNT +=  UNEMPLOYMENT_PAYMENT;

    
  //  if (DAY>238)
   // printf("\n LAST_LABOUR_INCOME: %f",LAST_LABOUR_INCOME);        
    //printf("\n PAYMENT_ACCOUNT before: %f",PAYMENT_ACCOUNT);
    //PAYMENT_ACCOUNT +=  UNEMPLOYMENT_BENEFIT_PCT * LAST_LABOUR_INCOME;
   //  printf("\n PAYMENT_ACCOUNT after: %f",PAYMENT_ACCOUNT);
  //  printf("\n UNEMPLOYMENT_BENEFIT_PCT: %f LAST_LABOUR_INCOME: %f",UNEMPLOYMENT_BENEFIT_PCT,LAST_LABOUR_INCOME);

    TOTAL_INCOME=  UNEMPLOYMENT_PAYMENT + CUM_TOTAL_DIVIDENDS + MONTHLY_BOND_INTEREST_INCOME;
    
    remove_double(&LAST_INCOME,0);
    add_double(&LAST_INCOME, TOTAL_INCOME);

     /*Compute a mean income of the last four month*/
    int i;
    for(i = 0; i < 4;i++)
    {
        mean_income += LAST_INCOME.array[i];
    }

    MEAN_INCOME = mean_income/4;

    //Set the benefit reception day
    DAY_OF_MONTH_RECEIVE_BENEFIT = DAY_OF_MONTH_RECEIVE_INCOME;

    return 0;

}


/** \fn Household_send_subsidy_notification()
 * \brief This function sends a message to the government in case the household applies for a subsidy.
 */
int Household_send_subsidy_notification()
{
    if (POLICY_EXP_STABILIZATION_SUBSIDY==1)
    {
        SUBSIDY_PAYMENT = SUBSIDY_PCT*MONTHLY_CONSUMPTION_EXPENDITURE;
    
        /*Add subsidy message */
        add_hh_subsidy_notification_message(GOV_ID, SUBSIDY_PAYMENT);
        PAYMENT_ACCOUNT += SUBSIDY_PAYMENT;
    }
    return 0;
}

/** \fn Household_send_transfer_notification()
 * \brief This function sends a message to the government in case the household applies for a transfer.
 */
int Household_send_transfer_notification()
{
    /*Add transfer message */
    add_hh_transfer_notification_message(GOV_ID);
    PAYMENT_ACCOUNT += TRANSFER_PAYMENT;

    return 0;
}

/** \fn Household_send_tax_payment()
 * \brief Household pays the income taxes
 */
int Household_send_tax_payment()
{
    // #ifdef _DEBUG_MODE  
        FILE *file1=NULL;
        char *filename="";
   // #endif
    double additional_tax;
    additional_tax =0.0;

    //Additional tax: repayment of the already received monthly unemployment benefits if recently re-employed
    if (DAY_OF_MONTH_RECEIVE_BENEFIT != DAY_OF_MONTH_RECEIVE_INCOME )
    {
        additional_tax = ((DAY_OF_MONTH_RECEIVE_BENEFIT + (20-DAY_OF_MONTH_RECEIVE_INCOME)%20)/20.0)
                            * UNEMPLOYMENT_PAYMENT;
        //Reset
        DAY_OF_MONTH_RECEIVE_BENEFIT = DAY_OF_MONTH_RECEIVE_INCOME;
    }


    /*Compute the total taxes*/
    TAX_PAYMENT = additional_tax + CUM_TOTAL_DIVIDENDS*TAX_RATE_HH_CAPITAL + WAGE*TAX_RATE_HH_LABOUR;

    /*Send a message to the government*/
    add_tax_payment_message(GOV_ID, TAX_PAYMENT);

    /*Reduce the payment account*/
    PAYMENT_ACCOUNT-=TAX_PAYMENT;

    /*Setting the counter of monthly dividends = 0*/
    CUM_TOTAL_DIVIDENDS=0;
    
    if (PRINT_DEBUG_FILE_EXP1)
        {                       
            filename = malloc(40*sizeof(char));
            filename[0]=0;
            strcpy(filename, "its/households_tax_payments.txt");      
            file1 = fopen(filename,"a");
            fprintf(file1,"\n %d %d %f %f",DAY,ID,additional_tax,TAX_PAYMENT);
            fclose(file1);
            free(filename);
        }  


    return 0;

}
