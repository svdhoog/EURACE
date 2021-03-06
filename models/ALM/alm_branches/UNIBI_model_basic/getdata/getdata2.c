#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*************************************
* Struct Type Definitions            *
*************************************/

struct firm
{
	int id;
	int region_id;
	double wage_offer;
	double technology;
	int no_employees;
	int vacancies;
	double average_g_skill;

	double production_quantity;
	double capital_stock;
	double revenue;
	double earnings;
	double price;
	double price_last_month;
	double mean_specific_skills;

	double total_sold_quantity;
	double planned_production_quantity;
	double account;

	struct firm * next;
};

struct household
{
	int id;
	int region_id;
	int employee_firm_id;
	int employer_region_id;
	int general_skill;
	double specific_skill;
	double wage;
	double wage_reservation;
	double demand;
	double savings;

	struct household * next;
};

struct mall
{
	int id;
	int region_id;
	double total_supply;

	struct mall * next;


};
 struct IGfirm
{
	int id;
	int region_id;
	double productivity;
	double revenue_per_day;	

	struct IGfirm * next;

};



typedef struct firm firm;
typedef struct household household;

typedef struct mall mall;
typedef struct IGfirm IGfirm;


	
/*****************************************************************
* FUNCTIONS: linked list functions                               *
* PURPOSE: to allocate and free memory in linked lists           *
*****************************************************************/
void freefirms(firm * head)
{
	/* Tempory element needed for loop */
	firm * tmp;

	/* Loop while head is not NULL */
	while(head)
	{
		/* Make next in list tmp */
		tmp = head->next;
		/* Free memory of head */
		free(head);
		/* Make head the next in the list */
		head = tmp;
	}
}

void freehouseholds(household * head)
{
	/* Tempory element needed for loop */
	household * tmp;
	
	/* Loop while head is not NULL */
	while(head)
	{
		/* Make next in list tmp */
		tmp = head->next;
		/* Free memory of head */
		free(head);
		/* Make head the next in the list */
		head = tmp;
	}
}

void freemalls(mall * head)
{
	/* Tempory element needed for loop */
	mall * tmp;

	/* Loop while head is not NULL */
	while(head)
	{
		/* Make next in list tmp */
		tmp = head->next;
		/* Free memory of head */
		free(head);
		/* Make head the next in the list */
		head = tmp;
	}
}

void freeIGfirms(IGfirm * head)
{
	/* Tempory element needed for loop */
	IGfirm * tmp;

	/* Loop while head is not NULL */
	while(head)
	{
		/* Make next in list tmp */
		tmp = head->next;
		/* Free memory of head */
		free(head);
		/* Make head the next in the list */
		head = tmp;
	}
}



firm * addfirm(firm ** pointer_to_firms, firm * current)
{
	/* The new tail of the linked list */
	firm * tail;

	/* Allocate memory for new neighbour data */

	tail = (firm *)malloc(sizeof(firm));

	/* Check if current is not NULL */
	if(current)
	{
		/* Current exists therefore make its next point to tail */
		current->next = tail;
	}
	else
	{
		/* Current is NULL therefore make the cell neighbour_head point to tail */
		*pointer_to_firms = tail;
	}
	/* Point next to NULL */
	tail->next = NULL;
	/* Return new neighbour data */
	return tail;
}

household * addhousehold(household ** pointer_to_households, household * current)
{
	/* The new tail of the linked list */
	household * tail;

	/* Allocate memory for new neighbour data */

	tail = (household *)malloc(sizeof(household));

	/* Check if current is not NULL */
	if(current)
	{
		/* Current exists therefore make its next point to tail */
		current->next = tail;
	}
	else
	{
		/* Current is NULL therefore make the cell<name>Firm neighbour_head point to tail */
		*pointer_to_households = tail;
	}
	/* Point next to NULL */
	tail->next = NULL;
	/* Return new neighbour data */
	return tail;
}

mall * addmall(mall ** pointer_to_malls, mall * current)
{
	/* The new tail of the linked list */
	mall * tail;

	/* Allocate memory for new neighbour data */

	tail = (mall *)malloc(sizeof(mall));

	/* Check if current is not NULL */
	if(current)
	{
		/* Current exists therefore make its next point to tail */
		current->next = tail;
	}
	else
	{
		/* Current is NULL therefore make the cell neighbour_head point to tail */
		*pointer_to_malls = tail;
	}
	/* Point next to NULL */
	tail->next = NULL;
	/* Return new neighbour data */
	return tail;
}

IGfirm * addIGfirm(IGfirm ** pointer_to_IGfirms, IGfirm * current)
{
	/* The new tail of the linked list */
	IGfirm * tail;

	/* Allocate memory for new neighbour data */

	tail = (IGfirm *)malloc(sizeof(IGfirm));

	/* Check if current is not NULL */
	if(current)
	{
		/* Current exists therefore make its next point to tail */
		current->next = tail;
	}
	else
	{
		/* Current is NULL therefore make the cell neighbour_head point to tail */
		*pointer_to_IGfirms = tail;
	}
	/* Point next to NULL */
	tail->next = NULL;
	/* Return new neighbour data */
	return tail;
}





/* Print all cell data */
/*void printagentdata(xmachine ** pointer_to_cells)
{
	xmachine * current;
	
	current = *pointer_to_cells;

	printf("Cell Data\n");
	while(current)
	{
		printf("cell:%d\txp:%f\ty:%f\tz:%f\n", current->id, current->x, current->y, current->z);
		current = current->next;
	}

	printf("\n");
}*/

/*****************************************************************
* FUNCTION: getIteration                                         *
* PURPOSE: read iteration xml file                               *
*****************************************************************/
int getiteration(char * filepath, int itno, firm ** pointer_to_firms, household ** pointer_to_households,IGfirm ** pointer_to_IGfirms,mall ** pointer_to_malls)
{
	/* Pointer to file */
	FILE *file;
	/* Char and char buffer for reading file to */
	char c = ' ';
	char buffer[1000000];
	/* Variable to keep reading file */
	int reading = 1;

	
	
		

	/* Variables for checking tags */
	int i, instate, intag, initeration, inagent, inid, inregion_id, inname, ina;
	/*Firm*/
	int inwage_offer, intechnology, inno_employees, invacancies, inaverage_g_skill;
	int inproduction_quantity, incapital_stock, inrevenue, inearnings, inprice;
	int intotal_sold_quantity, inplanned_production_quantity, inaccount, inprice_last_month, inmean_specific_skills;
	/*Household*/
	int  inwage, inwage_reservation, ingeneral_skill, inspecific_skill, inemployee_firm_id;
	int inemployer_region_id, insavings;

	/*Mall*/
	int intotal_supply;

	/*IGFirm*/
	int inproductivity, inrevenue_per_day;

	/* Variables for model data */
	int state, id, region_id;

	/*Firm*/
	int  no_employees, vacancies; 
	double wage_offer, technology, average_g_skill, production_quantity, capital_stock;
	double revenue, earnings, price, total_sold_quantity, planned_production_quantity;
	double account,  price_last_month, mean_specific_skills;

	/*Household*/
	int  general_skill, employee_firm_id, employer_region_id;
	double a, wage, wage_reservation, specific_skill, savings;

	/*Mall*/
	double total_supply;

	/*IGFirm*/
	double productivity, revenue_per_day;
	
	char name[100000];
	
	firm * current_firm, * tail_firm;
	tail_firm = *pointer_to_firms;
	current_firm = NULL;
	
	household * current_household, * tail_household;
	tail_household = *pointer_to_households;
	current_household = NULL;
	
	mall * current_mall, * tail_mall;
	tail_mall = *pointer_to_malls;
	current_mall = NULL;


	IGfirm * current_IGfirm, * tail_IGfirm;
	tail_IGfirm = *pointer_to_IGfirms;
	current_IGfirm = NULL;
	
	/* Open config file to read-only */
	char data[10000];
	sprintf(data, "%s%i%s", filepath, itno, ".xml");
	printf("%s", data);
	if((file = fopen(data, "r"))==NULL)
	{
		printf(" nent\n");
		return 0;
		/*exit(0);*/
	}
	else
	{

	printf("\n");
	/* Initialise variables */
	i = 0;
	instate = 0;
	intag = 0;
	initeration = 0;
	inagent = 0;
	inname = 0;
	inid = 0;
	inregion_id = 0;
	ina = 0;
	/*Firm*/
	inwage_offer = 0;
	intechnology = 0;
	inno_employees = 0;
	invacancies = 0;
	inaverage_g_skill = 0;
	inproduction_quantity = 0;
	incapital_stock = 0;
	inrevenue = 0;
	inearnings = 0;
	inprice = 0;
	intotal_sold_quantity = 0;
	inplanned_production_quantity = 0;
	inaccount  = 0;
	inprice_last_month=0;
	inmean_specific_skills=0;
	/*Household*/
	inwage = 0;
	inwage_reservation = 0;
	ingeneral_skill = 0;
	inspecific_skill =0;
	inemployee_firm_id = 0;
	inemployer_region_id = 0;
	insavings = 0.0;
	/*Mall*/
	intotal_supply =0;
	/*IGFirm*/
	inproductivity=0;
	inrevenue_per_day=0;

	state = 0;
	id = 0;
	region_id = 0;
	a = 0.0;
	
	wage_offer = 0.0;
	technology = 0.0;
	no_employees = 0;
	vacancies = 0;
	average_g_skill = 0.0;
	production_quantity = 0.0;
	capital_stock = 0.0;
	revenue = 0;
	earnings = 0;
	price = 0;
	total_sold_quantity = 0.0;
	planned_production_quantity = 0.0;
	account = 0.0;
	price_last_month=0.0;
	mean_specific_skills=0.0;
	
	wage = 0.0;
	wage_reservation = 0.0;
	general_skill = 0;
	specific_skill = 0.0;
	employee_firm_id = 0;
	employer_region_id = 0;
	savings = 0.0;
		
	total_supply =0;
		
	inproductivity=0;
	revenue_per_day=0;
		
	/* Read characters until the end of the file */
	/*while(c != EOF)*/
	/* Read characters until end of xml found */
	while(reading == 1)
	{
		/* Get the next char from the file */
		c = fgetc(file);
		
		
		
		/* If the end of a tag */
		if(c == '>')
		{
			/* Place 0 at end of buffer to make chars a string */
			buffer[i] = 0;
			if(strcmp(buffer, "states") == 0) { initeration = 1; }
			if(strcmp(buffer, "/states") == 0) { initeration = 0; reading = 0; }
			if(strcmp(buffer, "xagent") == 0) { inagent = 1; }
			if(strcmp(buffer, "/xagent") == 0)
			{
				inagent = 0;
				
				//printf("Found agent ... ");
				
				if(strcmp(name, "Firm") == 0)
				{
					//printf("Adding agent\n");
					
					/* check if tail is NULL */
					if(tail_firm == NULL)
					{
						//printf("tail is null allocate more memory\n");
						/* Allocate memory */
						tail_firm = addfirm(pointer_to_firms, current_firm);
					}
					//else printf("tail exisits\n");
					
					/* Make tail the current element to add to */
					current_firm = tail_firm;
					
					current_firm->id = id;
					current_firm->region_id = region_id;
					current_firm->wage_offer = wage_offer;
					current_firm->technology = technology;
					current_firm->no_employees = no_employees;
					current_firm->vacancies = vacancies;
					current_firm->average_g_skill = average_g_skill;
					current_firm->production_quantity = production_quantity;
					current_firm->capital_stock = capital_stock;
					current_firm->revenue = revenue;
					current_firm->earnings = earnings;
					current_firm->price = price;
					current_firm->total_sold_quantity = total_sold_quantity;
			current_firm->planned_production_quantity = planned_production_quantity;
					current_firm->account = account;
					current_firm->price_last_month=price_last_month;
					current_firm->mean_specific_skills=mean_specific_skills;
					
					
					
					
					//printf("Firm %d, ", id);
					
					/* Make tail the next element in the linked list */
					tail_firm = current_firm->next;
				}
				
				if(strcmp(name, "Household") == 0)
				{
					//printf("Adding agent\n");
					
					/* check if tail is NULL */
					if(tail_household == NULL)
					{
						//printf("tail is null allocate more memory\n");
						/* Allocate memory */
						tail_household = addhousehold(pointer_to_households, current_household);
					}
					//else printf("tail exisits\n");
					
					/* Make tail the current element to add to */
					current_household = tail_household;
					
					current_household->id = id;
					current_household->region_id = region_id;
					current_household->wage = wage;
					current_household->wage_reservation = wage_reservation;
					current_household->general_skill = general_skill;
					current_household->specific_skill = specific_skill;
					current_household->employee_firm_id = employee_firm_id;
				     current_household->employer_region_id = employer_region_id;
					current_household->savings = savings;
					
					//printf("Household %d, ", id);
					
					/* Make tail the next element in the linked list */
					tail_household = current_household->next;
				}
				
			
			if(strcmp(name, "Mall") == 0)
				{
					//printf("Adding agent\n");
					
					/* check if tail is NULL */
					if(tail_mall == NULL)
					{
						//printf("tail is null allocate more memory\n");
						/* Allocate memory */
						tail_mall = addmall(pointer_to_malls, current_mall);
					}
					//else printf("tail exisits\n");
					
					/* Make tail the current element to add to */
					current_mall = tail_mall;
					
					current_mall->id = id;
					current_mall->region_id = region_id;
					current_mall->total_supply =total_supply;
					
					
					
					
					
					//printf("mall %d, ", id);
					
					/* Make tail the next element in the linked list */
					tail_mall = current_mall->next;
				}
			


			if(strcmp(name, "IGFirm") == 0)
				{
					//printf("Adding agent\n");
					
					/* check if tail is NULL */
					if(tail_IGfirm == NULL)
					{
						//printf("tail is null allocate more memory\n");
						/* Allocate memory */
						tail_IGfirm = addIGfirm(pointer_to_IGfirms, current_IGfirm);
					}
					//else printf("tail exisits\n");
					
					/* Make tail the current element to add to */
					current_IGfirm = tail_IGfirm;
					
					current_IGfirm->id = id;
					current_IGfirm->region_id = region_id;
					current_IGfirm->productivity = productivity;
					current_IGfirm->revenue_per_day = revenue_per_day;
					
					
					
					
					
					//printf("IGfirm %d, ", id);
					
					/* Make tail the next element in the linked list */
					tail_IGfirm = current_IGfirm->next;
				}
						//else printf("Not adding agent\n");
			}
			if(strcmp(buffer, "name") == 0) { inname = 1; }
			if(strcmp(buffer, "/name") == 0) { inname = 0; }
			if(strcmp(buffer, "id") == 0) { inid = 1; }
			if(strcmp(buffer, "/id") == 0) { inid = 0; }
			if(strcmp(buffer, "region_id") == 0) { inregion_id = 1; }
			if(strcmp(buffer, "/region_id") == 0) { inregion_id = 0; }
			
			if(strcmp(buffer, "wage_offer") == 0) { inwage_offer = 1; }
			if(strcmp(buffer, "/wage_offer") == 0) { inwage_offer = 0; }
			if(strcmp(buffer, "technology") == 0) { intechnology = 1; }
			if(strcmp(buffer, "/technology") == 0) { intechnology = 0; }
			if(strcmp(buffer, "no_employees") == 0) { inno_employees = 1; }
			if(strcmp(buffer, "/no_employees") == 0) { inno_employees = 0; }
			if(strcmp(buffer, "vacancies") == 0) { invacancies = 1; }
			if(strcmp(buffer, "/vacancies") == 0) { invacancies = 0; }
			if(strcmp(buffer, "average_g_skill") == 0) { inaverage_g_skill = 1; }
			if(strcmp(buffer, "/average_g_skill") == 0) { inaverage_g_skill = 0; }
			if(strcmp(buffer, "production_quantity") == 0) { inproduction_quantity = 1; }
			if(strcmp(buffer, "/production_quantity") == 0) { inproduction_quantity = 0; }

			if(strcmp(buffer, "mean_specific_skills") == 0) { inmean_specific_skills = 1; }
			if(strcmp(buffer, "/mean_specific_skills") == 0) { inmean_specific_skills = 0; }
			if(strcmp(buffer, "capital_stock") == 0) { incapital_stock = 1; }
			if(strcmp(buffer, "/capital_stock") == 0) { incapital_stock = 0; }
			if(strcmp(buffer, "revenue") == 0) { inrevenue = 1; }
			if(strcmp(buffer, "/revenue") == 0) { inrevenue = 0; }
			if(strcmp(buffer, "earnings") == 0) { inearnings = 1; }
			if(strcmp(buffer, "/earnings") == 0) { inearnings = 0; }
			
			if(strcmp(buffer, "price_last_month") == 0) { inprice_last_month = 1; }
			if(strcmp(buffer, "/price_last_month") == 0) { inprice_last_month = 0; }

			if(strcmp(buffer, "price") == 0) { inprice = 1; }
			if(strcmp(buffer, "/price") == 0) { inprice = 0; }
		     if(strcmp(buffer, "total_sold_quantity") == 0) { intotal_sold_quantity = 1; }
		     if(strcmp(buffer, "/total_sold_quantity") == 0) { intotal_sold_quantity = 0; }
	if(strcmp(buffer, "planned_production_quantity") == 0) { inplanned_production_quantity = 1; }
	if(strcmp(buffer, "/planned_production_quantity") == 0) { inplanned_production_quantity = 0; }
			if(strcmp(buffer, "account") == 0) { inaccount = 1; }
			if(strcmp(buffer, "/account") == 0) { inaccount = 0; }

		

			if(strcmp(buffer, "wage") == 0) { inwage = 1; }
			if(strcmp(buffer, "/wage") == 0) { inwage = 0; }
			if(strcmp(buffer, "wage_reservation") == 0) { inwage_reservation = 1; }
			if(strcmp(buffer, "/wage_reservation") == 0) { inwage_reservation = 0; }
			if(strcmp(buffer, "general_skill") == 0) { ingeneral_skill = 1; }
			if(strcmp(buffer, "/general_skill") == 0) { ingeneral_skill = 0; }
			if(strcmp(buffer, "specific_skill") == 0) { inspecific_skill = 1; }
			if(strcmp(buffer, "/specific_skill") == 0) { inspecific_skill = 0; }
			if(strcmp(buffer, "employee_firm_id") == 0) { inemployee_firm_id = 1; }
			if(strcmp(buffer, "/employee_firm_id") == 0) { inemployee_firm_id = 0; }
		        if(strcmp(buffer, "employer_region_id") == 0) { inemployer_region_id = 1; }
		       if(strcmp(buffer, "/employer_region_id") == 0) { inemployer_region_id = 0; }
			if(strcmp(buffer, "savings") == 0) { insavings = 1; }
			if(strcmp(buffer, "/savings") == 0) { insavings = 0; }


			if(strcmp(buffer, "total_supply") == 0) { intotal_supply = 1; }
		       if(strcmp(buffer, "/total_supply") == 0) { intotal_supply = 0; }
			if(strcmp(buffer, "productivity") == 0) { inproductivity = 1; }
		       if(strcmp(buffer, "/productivity") == 0) { inproductivity= 0; }	
			if(strcmp(buffer, "revenue_per_day") == 0) { inrevenue_per_day = 1; }
		       if(strcmp(buffer, "/revenue_per_day") == 0) { inrevenue_per_day= 0; }	
			
			/* End of tag and reset buffer */
			intag = 0;
			i = 0;
		}
		/* If start of tag */
		else if(c == '<')
		{
			/* Place /0 at end of buffer to end numbers */
			buffer[i] = 0;
			/* Flag in tag */
			intag = 1;
			/* If just read data into buffer retrieve it */
			if(inagent && inname)  { strcpy(name, buffer); }
			if(inagent && inid) { id = atoi(buffer); }
			if(inagent && inregion_id) { region_id = atoi(buffer); }
			

			if(inagent && inwage_offer)  { wage_offer  = atof(buffer); }
			if(inagent && intechnology)  { technology  = atof(buffer); }
			if(inagent && inno_employees)  { no_employees  = atoi(buffer); }
			if(inagent && invacancies)  { vacancies  = atof(buffer); }
			if(inagent && inaverage_g_skill)  { average_g_skill  = atof(buffer); }
			if(inagent && inproduction_quantity)  { production_quantity  = atof(buffer); }
			if(inagent && incapital_stock)  { capital_stock  = atof(buffer); }
			if(inagent && inrevenue)  { revenue  = atof(buffer); }
			if(inagent && inearnings)  { earnings  = atof(buffer); }
			if(inagent && inprice)  { price  = atof(buffer); }
			if(inagent && intotal_sold_quantity)  { total_sold_quantity  = atof(buffer); }
if(inagent && inplanned_production_quantity)  { planned_production_quantity  = atof(buffer); }
			if(inagent && inaccount)  { account  = atof(buffer); }
			if(inagent && inprice_last_month)  { price_last_month  = atof(buffer); }
			if(inagent && inmean_specific_skills)  { mean_specific_skills  = atof(buffer); }
			
			if(inagent && inwage)  { wage  = atof(buffer); }
			if(inagent && inwage_reservation)  { wage_reservation  = atof(buffer); }
			if(inagent && ingeneral_skill)  { general_skill  = atoi(buffer); }
			if(inagent && inspecific_skill)  { specific_skill  = atof(buffer); }
			if(inagent && inemployee_firm_id)  { employee_firm_id = atoi(buffer); }
			if(inagent && inemployer_region_id)  { employer_region_id = atoi(buffer); }
			if(inagent && insavings)  { savings  = atof(buffer); }
			
			if(inagent && inrevenue_per_day)  {revenue_per_day  = atof(buffer); }
			if(inagent && inproductivity)  { productivity  = atof(buffer); }
			if(inagent && intotal_supply)  { total_supply  = atof(buffer); }
				
			
			
			/* Reset buffer */
			i = 0;
		}
		/* If in tag put read char into buffer */
		else if(intag)
		{
			buffer[i] = c;
			i++;
		}
		/* If in data read char into buffer */
		else// if(inname || inid || ina || inproductivity || inprice || insold || inproduction || inwage || insavings || infirmid || inworkers)
		{
			buffer[i] = c;
			i++;
		}
	}
	
	/* Free memory for unused linked list elements */
	if(tail_firm)
	{
		freefirms(tail_firm);
		/* Make pointer to tail equal NULL */
		if(current_firm) { current_firm->next = NULL; }
	}
	
	/* Free memory for unused linked list elements */
	if(tail_household)
	{
		freehouseholds(tail_household);
		/* Make pointer to tail equal NULL */
		if(current_household) { current_household->next = NULL; }
	}

	if(tail_mall)
	{
		freemalls(tail_mall);
		/* Make pointer to tail equal NULL */
		if(current_mall) { current_mall->next = NULL; }
	}

	if(tail_IGfirm)
	{
		freeIGfirms(tail_IGfirm);
		/* Make pointer to tail equal NULL */
		if(current_IGfirm) { current_IGfirm->next = NULL; }
	}
	
	/* Close the file */
	fclose(file);
	
	/*printagentdata(pointer_to_cells);*/
	
	return 1;
	}
}

void savedatatofile(int itno, firm ** pointer_to_firms, household ** pointer_to_households,IGfirm ** pointer_to_IGfirms,mall ** pointer_to_malls)
{

	
	FILE *file;
	char data[10000];
	firm * current_firm;
	household * current_household;
	IGfirm * current_IGfirm;
	mall* current_mall;
	current_firm = *pointer_to_firms;
	current_household = *pointer_to_households;
	current_mall = *pointer_to_malls;
	current_IGfirm = *pointer_to_IGfirms;
	

	int no_firms = 0;
	int no_households = 0;
	int num_regions=0;
	while(current_mall)
	{
	num_regions++;
	current_mall=current_mall->next;
	}

	int no_firm_in_regions[num_regions];
	int no_households_in_region[num_regions];

	double total_output=0;
	double regional_output[num_regions];

	int no_employed=0;
	int no_employed_at_home=0;
	int no_employed_off_home=0;



	int regional_no_employed[num_regions];
	int regional_no_employed_at_home[num_regions];
	int regional_no_employed_off_home[num_regions];


	int i;
	for(i=0;i<num_regions;i++)
	{
	 regional_output[i]=0;
	 regional_no_employed[i]=0;
	 regional_no_employed_at_home[i]=0;
	 regional_no_employed_off_home[i]=0;
	 no_households_in_region[i]=0;	
	}

	while(current_firm)
	{
	
	no_firms++;
		
	total_output+=current_firm->production_quantity;
	regional_output[current_firm->region_id-1]+=current_firm->production_quantity;

	current_firm=current_firm->next;
	}
	
			

		if(itno%20 == 1)	
			{
			file = fopen("data-global-output.csv", "a");
			sprintf(data, "%i", itno);
			fputs(data, file);
			fputs("\t", file);
			
			
			sprintf(data, "%f",total_output) ;
			fputs(data, file);
	
			fputs("\n", file);
			fclose(file);


			
	
			file = fopen("data-regional-output.csv", "a");
			sprintf(data, "%i", itno);
			fputs(data, file);
			fputs("\t", file);
			for(i=0;i<num_regions;i++)
			{
			sprintf(data, "%f",regional_output[i]) ;
			fputs(data, file);
			fputs("\t", file);
			}
			fputs("\n", file);
			fclose(file);


			}
//>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>Households<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
	
	while(current_household)
	{

	no_households++;
	
	no_households_in_region[current_household->region_id - 1]++;

	if(current_household->employee_firm_id>-1)
	{
	no_employed++;
	
	regional_no_employed[current_household->region_id - 1]++;
	if(current_household->employer_region_id == current_household-> region_id)
	{

	no_employed_at_home++;
		
	regional_no_employed_at_home[current_household->region_id - 1]++;
	}else
	{
	no_employed_off_home++;
	regional_no_employed_off_home[current_household->region_id - 1]++;
	}


	}
	


	current_household=current_household->next;
	}

	

			
			
			file = fopen("data-labour-market-statistics.csv", "a");
			sprintf(data, "%i", itno);
			fputs(data, file);
			fputs("\t", file);
			//Unemployment rate total
			
			
			sprintf(data, "%f",1-((double)no_employed/(double)no_households)) ;
			fputs(data, file);
			fputs("\t", file);
			//fraction of commuting households
			if(no_employed_off_home==0 && no_employed ==0)
				
			sprintf(data, "0.0") ;
			else
			sprintf(data, "%f",  (double)no_employed_off_home / (double)no_employed) ;
			fputs(data, file);
			fputs("\t", file);
		
			fputs("\n", file);
			fclose(file);

	
			file = fopen("data-regional-labour-market-statistics.csv", "a");
			sprintf(data, "%i", itno);
			fputs(data, file);
			fputs("\t", file);
			
			for(i=0;i<num_regions;i++)
			{
			
			sprintf(data, "%f",1-((double)regional_no_employed[i]/(double)no_households_in_region[i]));
			fputs(data, file);
			fputs("\t", file);
			}
		
			fputs("\n", file);
			fclose(file);
	

		



}

int main( int argc, char **argv )
{
	int position = 0;
	int lastd = 0;
	int i;
	char * filepath;
	firm * firms, * current_firm;
	household * households, * current_household;
	IGfirm * IGfirms, * current_IGfirm;
	mall * malls, * current_mall;
	firm ** p_firms;
	household ** p_household;
	IGfirm ** p_IGfirms;
	mall ** p_malls;
	int iteration_number = 0;
	int stilldata = 1;
	



	FILE *file;
	
	//file = fopen("data-v-u.csv", "w");
	//fclose(file);
	
	file = fopen("data-regional-output.csv", "w");
	fclose(file);

	file = fopen("data-global-output.csv", "w");
	fclose(file);


	file = fopen("data-labour-market-statistics.csv", "w");
	fclose(file);

	file = fopen("data-regional-labour-market-statistics.csv", "w");
	fclose(file);	
	
	
	/* Read initial states of x-machines */
	/* advance argument pointer to next argument */
	*argv++;
	if(*argv == NULL)
	{
		printf("No intial states defined\n");
		exit(0);
	}
	printf("Initial states: %s\n", *argv);
	/*readinitialstates(*argv, p_iteration_number, p_xmemory);*/
	while(**argv != 0)
	{
		/* For windows directories */
		if(**argv == '\\') lastd=position;
		/* For windows directories */
		if(**argv == '/') lastd=position;
		(*argv)++;
		position++;
	}
	for(i=position; i>0; i--)
	{
		if(i==lastd+1) **argv = 0;
		(*argv)--;
	}
	filepath = *argv;
	printf("Ouput dir: %s\n", filepath);
	
	/* Initialise pointers */
	firms = NULL;
	p_firms = &firms;
	
	households = NULL;
	p_household = &households;

	IGfirms = NULL;
	p_IGfirms = &IGfirms;
	
	malls = NULL;
	p_malls = &malls;
	
	while(stilldata)
	{
		stilldata = getiteration(filepath, iteration_number, p_firms, p_household,p_IGfirms, p_malls );
		
		if(stilldata) savedatatofile(iteration_number, p_firms, p_household,p_IGfirms, p_malls);
		
		iteration_number++;
		
		/*current = *p_agents;
		
		printf("Cell Data\n");
		while(current)
		{
			printf("cell:%d\ts:%d\tt:%d\n", current->id, current->state, current->type);
			current = current->next;
		}
		
		printf("\n");*/
	}
	
	/* Should never get here */
	return( 0 );
}
