#include <CUnit/Basic.h>

#include "../../header.h"
#include "../../Firm_agent_header.h"
#include "../../my_library_header.h"

/************Firm Role: Financial Management Role ********************************/

/************ Unit tests ********************************/

/*
 * \fn: void unittest_Firm_compute_financial_payments()
 * \brief: Unit test for: Firm_compute_financial_payments.
 * Status: Tested OK
 */
void unittest_Firm_compute_financial_payments()
{
	int rc;
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Firm_agent();
	
    /***** Variables: Memory pre-conditions **************************/
    /*
    LOANS[0].bank_id =1;
    LOANS[0].loan_value=100.0;
    LOANS[0].interest_rate=0.01;
    LOANS[0].installment_amount=20.0;
    LOANS[0].var_per_installment=0.0;
    LOANS[0].residual_var=0.0;
    LOANS[0].bad_debt=0.0;
    LOANS[0].nr_periods_before_repayment=5;

    LOANS[1].bank_id=2;
    LOANS[1].loan_value=200.0;
    LOANS[1].interest_rate=0.02;
	LOANS[1].installment_amount=50.0;
    LOANS[1].var_per_installment=0.0;
    LOANS[1].residual_var=0.0;
    LOANS[1].bad_debt=0.0;
    LOANS[1].nr_periods_before_repayment=4;
*/
    reset_debt_item_array(&LOANS);
    add_debt_item(&LOANS, 1, 100.0, 0.01, 20.0, 0.0, 0.0, 0.0, 5);
    add_debt_item(&LOANS, 2, 200.0, 0.02, 50.0, 0.0, 0.0, 0.0, 4);

    TOTAL_INTEREST_PAYMENT=0.0;
    TOTAL_DEBT_INSTALLMENT_PAYMENT=0.0;
    TOTAL_DEBT=0.0;
    
	/***** Messages: initialize message boards **********************************/

	/***** Messages: pre-conditions **********************************/
    //add_<message>_message();
    	    
    /***** Message: Adding message iterators ***************************************/

    /***** Function evaluation ***************************************/
	Firm_compute_financial_payments();
    
    /***** Variables: Memory post-conditions *****/

        CU_ASSERT_EQUAL(LOANS.array[0].bank_id, 1);
        CU_ASSERT_DOUBLE_EQUAL(LOANS.array[0].loan_value, 100.0, 1e-3);
        CU_ASSERT_DOUBLE_EQUAL(LOANS.array[0].interest_rate, 0.01, 1e-3);
        CU_ASSERT_DOUBLE_EQUAL(LOANS.array[0].installment_amount, 20.0, 1e-3);
        CU_ASSERT_EQUAL(LOANS.array[0].nr_periods_before_repayment, 5);

        CU_ASSERT_EQUAL(LOANS.array[1].bank_id, 2);
        CU_ASSERT_DOUBLE_EQUAL(LOANS.array[1].loan_value, 200.0, 1e-3);
        CU_ASSERT_DOUBLE_EQUAL(LOANS.array[1].interest_rate, 0.02, 1e-3);
        CU_ASSERT_DOUBLE_EQUAL(LOANS.array[1].installment_amount, 50.0, 1e-3);
        CU_ASSERT_EQUAL(LOANS.array[1].nr_periods_before_repayment, 4);
        
        CU_ASSERT_DOUBLE_EQUAL(TOTAL_INTEREST_PAYMENT, 5.0, 1e-3);
        CU_ASSERT_DOUBLE_EQUAL(TOTAL_DEBT_INSTALLMENT_PAYMENT, 70.0, 1e-3);
        CU_ASSERT_DOUBLE_EQUAL(TOTAL_DEBT, 300.0, 1e-3);

    /***** Messages: Message post-conditions *****/

    /************* At end of unit test, free the agent **************/
	unittest_free_Firm_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Firm_compute_income_statement()
 * \brief: Unit test for: Firm_compute_income_statement.
 * Status: Tested OK
 */
void unittest_Firm_compute_income_statement()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Firm_agent();
	
    /***** Variables: Memory pre-conditions **************************/

	CUM_REVENUE = 130.0;
	TOTAL_INTEREST_PAYMENT = 20.0;
	PRODUCTION_COSTS = 10.0;
	EARNINGS =0.0;
    TAX_PAYMENT =0.0;
    TAX_RATE_CORPORATE = 0.25;

	PAYMENT_ACCOUNT=0.0;
    PREVIOUS_NET_EARNINGS =1.0;
    NET_EARNINGS =2.0;
    
    PREVIOUS_EARNINGS_PER_SHARE =0.0;
    CURRENT_SHARES_OUTSTANDING =75;
    EARNINGS_PER_SHARE =2.0;
    
	/***** Messages: initialize message boards **********************************/

	/***** Messages: pre-conditions **********************************/
    	    
    /***** Message: Adding message iterators ***************************************/

    /***** Function evaluation ***************************************/
	Firm_compute_income_statement();
    
    /***** Variables: Memory post-conditions *****/
    CU_ASSERT_DOUBLE_EQUAL(EARNINGS, 100.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(PAYMENT_ACCOUNT, 0.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(TAX_PAYMENT, 25.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(PREVIOUS_NET_EARNINGS, 2.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(NET_EARNINGS, 75.0, 1e-3);
    
    CU_ASSERT_DOUBLE_EQUAL(PREVIOUS_EARNINGS_PER_SHARE, 2.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(EARNINGS_PER_SHARE, 1.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(NET_EARNINGS, 75.0, 1e-3);
    /***** Messages: Message post-conditions *****/

    /************* At end of unit test, free the agent **************/
	unittest_free_Firm_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Firm_compute_dividends()
 * \brief: Unit test for: Firm_compute_dividends.
 * Status: Tested OK
 * Case: option 4: keep earnings per share constant
 */
void unittest_Firm_compute_dividends()
{	
    /************* At start of unit test, add one agent **************/
	unittest_init_Firm_agent();
	
    /***** Variables: Memory pre-conditions **************************/
	TOTAL_DIVIDEND_PAYMENT =1.0;
	
	EARNINGS_PER_SHARE = 2.0;
	PREVIOUS_EARNINGS_PER_SHARE = 1.0;
	CURRENT_SHARES_OUTSTANDING = 200;
	PREVIOUS_SHARES_OUTSTANDING = 100;
	
	CURRENT_DIVIDEND_PER_SHARE =0.0;

	EARNINGS = 4.0;
	PREVIOUS_DIVIDEND_PER_EARNINGS = 0.0;
	CURRENT_DIVIDEND_PER_EARNINGS = 1.0;

	/***** Messages: initialize message boards **********************************/

	/***** Messages: pre-conditions **********************************/
    	    
    /***** Message: Adding message iterators ***************************************/

    /***** Function evaluation ***************************************/
	Firm_compute_dividends();
    
    /***** Variables: Memory post-conditions *****/
	CU_ASSERT_DOUBLE_EQUAL(TOTAL_DIVIDEND_PAYMENT, 4.0, 1e-3);
	CU_ASSERT_DOUBLE_EQUAL(PREVIOUS_DIVIDEND_PER_SHARE, 0.0, 1e-3);
	CU_ASSERT_DOUBLE_EQUAL(CURRENT_DIVIDEND_PER_SHARE, 0.02, 1e-3);
	
	CU_ASSERT_DOUBLE_EQUAL(PREVIOUS_DIVIDEND_PER_EARNINGS, 1.0, 1e-3);
	CU_ASSERT_DOUBLE_EQUAL(CURRENT_DIVIDEND_PER_EARNINGS, 1.0, 1e-3);
	
    /***** Messages: Message post-conditions *****/

    /************* At end of unit test, free the agent **************/
	unittest_free_Firm_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Firm_compute_total_financial_payments()
 * \brief: Unit test for: Firm_function.
 * Status: Tested OK
 */
void unittest_Firm_compute_total_financial_payments()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Firm_agent();
	
    /***** Variables: Memory pre-conditions **************************/
	TOTAL_PAYMENTS = 0.0;
	
	TOTAL_INTEREST_PAYMENT = 1.0;
	TOTAL_DEBT_INSTALLMENT_PAYMENT = 1.0;
	TOTAL_DIVIDEND_PAYMENT = 1.0; 
	TAX_PAYMENT = 1.0;
	PRODUCTION_COSTS = 1.0;
	
	/***** Messages: initialize message boards **********************************/

	/***** Messages: pre-conditions **********************************/
    	    
    /***** Message: Adding message iterators ***************************************/

    /***** Function evaluation ***************************************/
	Firm_compute_total_financial_payments();
    
    /***** Variables: Memory post-conditions *****/
	CU_ASSERT_DOUBLE_EQUAL(TOTAL_PAYMENTS, 5.0, 1e-3);

    /***** Messages: Message post-conditions *****/

    /************* At end of unit test, free the agent **************/
	unittest_free_Firm_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Firm_compute_balance_sheet()
 * \brief: Unit test for: Firm_compute_balance_sheet.
 * Status: Tested OK
 */
void unittest_Firm_compute_balance_sheet()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Firm_agent();
	
    /***** Variables: Memory pre-conditions **************************/
/*
    CURRENT_MALL_STOCKS[0].mall_id =1;
    CURRENT_MALL_STOCKS[0].critical_stock=1.0;
    CURRENT_MALL_STOCKS[0].current_stock=2.0;

    CURRENT_MALL_STOCKS[1].mall_id =2;
    CURRENT_MALL_STOCKS[1].critical_stock=1.0;
    CURRENT_MALL_STOCKS[1].current_stock=2.0;
*/
    reset_mall_info_array(&CURRENT_MALL_STOCKS);
    add_mall_info(&CURRENT_MALL_STOCKS, 1, 1.0, 2.0);
    add_mall_info(&CURRENT_MALL_STOCKS, 2, 1.0, 2.0);

    PRICE = 1.0;
    TOTAL_VALUE_LOCAL_INVENTORY =0.0;
    TOTAL_ASSETS = 0.0;
    PAYMENT_ACCOUNT = 10.0;
    TOTAL_VALUE_CAPITAL_STOCK = 1.0;
    TOTAL_DEBT = 7.5;
    
	/***** Messages: initialize message boards **********************************/

	/***** Messages: pre-conditions **********************************/
    	    
    /***** Message: Adding message iterators ***************************************/

    /***** Function evaluation ***************************************/
	Firm_compute_balance_sheet();
    
    /***** Variables: Memory post-conditions *****/
	CU_ASSERT_EQUAL(CURRENT_MALL_STOCKS.size, 2);
	CU_ASSERT_DOUBLE_EQUAL(TOTAL_VALUE_LOCAL_INVENTORY, 4.0, 1e-3);
	CU_ASSERT_DOUBLE_EQUAL(TOTAL_ASSETS, 15.0, 1e-3);
	CU_ASSERT_DOUBLE_EQUAL(EQUITY, 7.5, 1e-3);
	CU_ASSERT_DOUBLE_EQUAL(DEBT_EQUITY_RATIO, 1.0, 1e-3);
	
    /***** Messages: Message post-conditions *****/

    /************* At end of unit test, free the agent **************/
	unittest_free_Firm_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Firm_compute_total_liquidity_needs()
 * \brief: Unit test for: Firm_compute_total_liquidity_needs.
 * Status: NOT Tested, gives a problem
 */
void unittest_Firm_compute_total_liquidity_needs()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Firm_agent();
	
    /***** Variables: Memory pre-conditions **************************/
	FINANCIAL_LIQUIDITY_NEEDS = 0.0; 
	PRODUCTION_LIQUIDITY_NEEDS = 0.0; 
		
	PLANNED_PRODUCTION_COSTS = 1.0;
	TOTAL_DIVIDEND_PAYMENT = 1.0;
	TOTAL_INTEREST_PAYMENT = 1.0;
	TOTAL_DEBT_INSTALLMENT_PAYMENT = 1.0;
	TAX_PAYMENT = 1.0;
	
	PAYMENT_ACCOUNT = 2.0;
	TOTAL_FINANCIAL_NEEDS = 0.0;
	EXTERNAL_FINANCIAL_NEEDS = 0.0;
	
	/***** Messages: initialize message boards **********************************/

	/***** Messages: pre-conditions **********************************/
    	    
    /***** Message: Adding message iterators ***************************************/

    /***** Function evaluation ***************************************/
	Firm_compute_total_liquidity_needs();
    
    /***** Variables: Memory post-conditions *****/
	CU_ASSERT_DOUBLE_EQUAL(TOTAL_FINANCIAL_NEEDS, 5.0, 1e-3);
	CU_ASSERT_DOUBLE_EQUAL(EXTERNAL_FINANCIAL_NEEDS, 3.0, 1e-3);
	
    /***** Messages: Message post-conditions *****/

    /************* At end of unit test, free the agent **************/
	unittest_free_Firm_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest1_Firm_check_financial_and_bankruptcy_state()
 * \brief: Unit test for: Firm_check_financial_and_bankruptcy_state.
 * Status: Tested OK
 */
void unittest1_Firm_check_financial_and_bankruptcy_state()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Firm_agent();
	
    /***** Variables: Memory pre-conditions **************************/	
    BANKRUPTCY_ILLIQUIDITY_STATE=1;
    FINANCIAL_CRISIS_STATE=1;

	PAYMENT_ACCOUNT = 2.0;
	TOTAL_FINANCIAL_NEEDS = 5.0;
	
	TOTAL_INTEREST_PAYMENT = 1.0;
	TOTAL_DEBT_INSTALLMENT_PAYMENT = 1.0;
	TAX_PAYMENT = 1.0;

	/***** Messages: initialize message boards **********************************/

	/***** Messages: pre-conditions **********************************/
    	    
    /***** Message: Adding message iterators ***************************************/

    /***** Function evaluation ***************************************/
	Firm_check_financial_and_bankruptcy_state();
    
    /***** Variables: Memory post-conditions *****/
	CU_ASSERT_EQUAL(BANKRUPTCY_ILLIQUIDITY_STATE, 1);
	CU_ASSERT_EQUAL(FINANCIAL_CRISIS_STATE, 0);
	
    /***** Messages: Message post-conditions *****/

    /************* At end of unit test, free the agent **************/
	unittest_free_Firm_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest2_Firm_check_financial_and_bankruptcy_state()
 * \brief: Unit test for: Firm_check_financial_and_bankruptcy_state.
 * Status: Tested OK
 */
void unittest2_Firm_check_financial_and_bankruptcy_state()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Firm_agent();
	
    /***** Variables: Memory pre-conditions **************************/	
    BANKRUPTCY_ILLIQUIDITY_STATE=1;
    FINANCIAL_CRISIS_STATE=1;

	PAYMENT_ACCOUNT = 3.0;
	TOTAL_FINANCIAL_NEEDS = 5.0;
	
	TOTAL_INTEREST_PAYMENT = 1.0;
	TOTAL_DEBT_INSTALLMENT_PAYMENT = 1.0;
	TAX_PAYMENT = 1.0;

	/***** Messages: initialize message boards **********************************/

	/***** Messages: pre-conditions **********************************/
    	    
    /***** Message: Adding message iterators ***************************************/

    /***** Function evaluation ***************************************/
	Firm_check_financial_and_bankruptcy_state();
    
    /***** Variables: Memory post-conditions *****/
	CU_ASSERT_EQUAL(BANKRUPTCY_ILLIQUIDITY_STATE, 0);
	CU_ASSERT_EQUAL(FINANCIAL_CRISIS_STATE, 1);
	
    /***** Messages: Message post-conditions *****/

    /************* At end of unit test, free the agent **************/
	unittest_free_Firm_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_Firm_in_financial_crisis()
 * \brief: Unit test for: Firm_in_financial_crisis.
 * Status: Tested OK
 */
void unittest_Firm_in_financial_crisis()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Firm_agent();
	
    /***** Variables: Memory pre-conditions **************************/
	PAYMENT_ACCOUNT = 14.0;
	TOTAL_INTEREST_PAYMENT = 1.0;
	TOTAL_DEBT_INSTALLMENT_PAYMENT = 1.0;
	TAX_PAYMENT = 1.0;
	
	TOTAL_DIVIDEND_PAYMENT = 100.0;
	PLANNED_PRODUCTION_COSTS = 10.0;;
	/***** Messages: initialize message boards **********************************/

	/***** Messages: pre-conditions **********************************/
    	    
    /***** Message: Adding message iterators ***************************************/

	/***** Function evaluation ***************************************/
	Firm_in_financial_crisis();
    
    /***** Variables: Memory post-conditions *****/
	CU_ASSERT_DOUBLE_EQUAL(TOTAL_DIVIDEND_PAYMENT, 1.0, 1e-3);
	CU_ASSERT_EQUAL(FINANCIAL_CRISIS_STATE, 0);
	CU_ASSERT_EQUAL(BANKRUPTCY_STATE, 0);

    /***** Messages: Message post-conditions *****/

    /************* At end of unit test, free the agent **************/
	unittest_free_Firm_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest1_Firm_execute_financial_payments()
 * \brief: Unit test for: Firm_execute_financial_payments.
 * Status: Tested, Error: Loan 2 should have been removed , but isnt
 */
void unittest1_Firm_execute_financial_payments()
{
	int rc;
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Firm_agent();
	
    /***** Variables: Memory pre-conditions **************************/
/*
    LOANS[0].bank_id =1;
    LOANS[0].loan_value=100.0;
    LOANS[0].interest_rate=0.01;
    LOANS[0].installment_amount=20.0;
    LOANS[0].var_per_installment=20.0;
    LOANS[0].residual_var=0.0;
    LOANS[0].bad_debt=0.0;
    LOANS[0].nr_periods_before_repayment=5;

    LOANS[1].bank_id=2;
    LOANS[1].loan_value=50.0;
    LOANS[1].interest_rate=0.02;
	LOANS[1].installment_amount=50.0;
    LOANS[1].var_per_installment=50.0;
    LOANS[1].residual_var=0.0;
    LOANS[1].bad_debt=0.0;
    LOANS[1].nr_periods_before_repayment=1;
*/
    reset_debt_item_array(&LOANS);
    add_debt_item(&LOANS, 1, 100.0, 0.01, 20.0, 20.0, 0.0, 0.0, 5);
    add_debt_item(&LOANS, 2, 50.0, 0.02, 50.0, 50.0, 0.0, 0.0, 1);

    PAYMENT_ACCOUNT = 100.0;
    TOTAL_INTEREST_PAYMENT=0.0;
    TOTAL_DEBT_INSTALLMENT_PAYMENT=0.0;
    TOTAL_DEBT=0.0;
    
	/***** Messages: initialize message boards **********************************/

	rc = MB_Create(&b_installment, sizeof(m_installment));
    	    #ifdef ERRCHECK
    	    if (rc != MB_SUCCESS)
    	    {
    	       fprintf(stderr, "ERROR: Could not create 'installment' board\n");
    	       switch(rc) {
    	           case MB_ERR_INVALID:
    	               fprintf(stderr, "\t reason: Invalid message size\n");
    	               break;
    	           case MB_ERR_MEMALLOC:
    	               fprintf(stderr, "\t reason: out of memory\n");
    	               break;
    	           case MB_ERR_INTERNAL:
    	               fprintf(stderr, "\t reason: internal error. Recompile libmoard in debug mode for more info \n");
    	               break;
    	       }
    	    }
    	    #endif
	
	/***** Messages: pre-conditions **********************************/
    //add_installment_message();
    	    
    /***** Message: Adding message iterators ***************************************/
	rc = MB_Iterator_Create(b_installment, &i_installment);
			
	if (rc != MB_SUCCESS)
			{
			   fprintf(stderr, "ERROR: Could not create Iterator for 'installment'\n");
			   switch(rc) {
			       case MB_ERR_INVALID:
			           fprintf(stderr, "\t reason: 'installment' board is invalid\n");
			           break;
			       case MB_ERR_LOCKED:
		               fprintf(stderr, "\t reason: 'installment' board is locked\n");
		               break;
		           case MB_ERR_MEMALLOC:
		               fprintf(stderr, "\t reason: out of memory\n");
		               break;
		           case MB_ERR_INTERNAL:
		               fprintf(stderr, "\t reason: internal error. Recompile libmoard in debug mode for more info \n");
		               break;
			   }
			}

    /***** Function evaluation ***************************************/
	Firm_execute_financial_payments();
    
    /***** Variables: Memory post-conditions *****/
    CU_ASSERT_EQUAL(LOANS.array[0].bank_id, 1);
    CU_ASSERT_DOUBLE_EQUAL(LOANS.array[0].loan_value, 80.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(LOANS.array[0].interest_rate, 0.01, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(LOANS.array[0].installment_amount, 20.0, 1e-3);
    CU_ASSERT_EQUAL(LOANS.array[0].nr_periods_before_repayment, 4);

    CU_ASSERT_DOUBLE_EQUAL(PAYMENT_ACCOUNT, 28.0, 1e-3);
    CU_ASSERT_DOUBLE_EQUAL(TOTAL_DEBT, 80.0, 1e-3);
    CU_ASSERT_EQUAL(LOANS.size, 1);
    
    /***** Messages: Message post-conditions *****/
	//start a reading loop
    //add_debt_installment_message(bank_id, installment_amount, interest_amount, credit_refunded, var_per_installment)

    START_INSTALLMENT_MESSAGE_LOOP
    if(installment_message->bank_id==1)
    {
    	CU_ASSERT_DOUBLE_EQUAL(installment_message->installment_amount, 20.0, 1e-3);
	    CU_ASSERT_DOUBLE_EQUAL(installment_message->interest_amount, 1.0, 1e-3);
	    CU_ASSERT_DOUBLE_EQUAL(installment_message->var_per_installment, 20.0, 1e-3);
    }
    if(installment_message->bank_id==2)
    {
    	CU_ASSERT_DOUBLE_EQUAL(installment_message->installment_amount, 50.0, 1e-3);
	    CU_ASSERT_DOUBLE_EQUAL(installment_message->interest_amount, 1.0, 1e-3);
	    CU_ASSERT_DOUBLE_EQUAL(installment_message->var_per_installment, 50.0, 1e-3);
    }
	FINISH_INSTALLMENT_MESSAGE_LOOP
	
    /************* At end of unit test, free the agent **************/
	unittest_free_Firm_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest2_Firm_execute_financial_payments()
 * \brief: Unit test for: Firm_execute_financial_payments.
 * Status: NOT Tested
 */
void unittest2_Firm_execute_financial_payments()
{
	int rc;
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Firm_agent();
	
    /***** Variables: Memory pre-conditions **************************/
	GOV_ID=1;
	ID =2;
	TAX_PAYMENT=100.0;
	PAYMENT_ACCOUNT = 200.0;
	
	CURRENT_DIVIDEND_PER_SHARE = 0.0;
	TOTAL_DIVIDEND_PAYMENT = 100.0;
	CURRENT_SHARES_OUTSTANDING = 100;
	
	/***** Messages: initialize message boards **********************************/

	rc = MB_Create(&b_tax_payment, sizeof(m_tax_payment));
    	    #ifdef ERRCHECK
    	    if (rc != MB_SUCCESS)
    	    {
    	       fprintf(stderr, "ERROR: Could not create 'tax_payment' board\n");
    	       switch(rc) {
    	           case MB_ERR_INVALID:
    	               fprintf(stderr, "\t reason: Invalid message size\n");
    	               break;
    	           case MB_ERR_MEMALLOC:
    	               fprintf(stderr, "\t reason: out of memory\n");
    	               break;
    	           case MB_ERR_INTERNAL:
    	               fprintf(stderr, "\t reason: internal error. Recompile libmoard in debug mode for more info \n");
    	               break;
    	       }
    	    }
    	    #endif
    	    
		rc = MB_Create(&b_dividend_per_share, sizeof(m_dividend_per_share));
	    	    #ifdef ERRCHECK
	    	    if (rc != MB_SUCCESS)
	    	    {
	    	       fprintf(stderr, "ERROR: Could not create 'dividend_per_share' board\n");
	    	       switch(rc) {
	    	           case MB_ERR_INVALID:
	    	               fprintf(stderr, "\t reason: Invalid message size\n");
	    	               break;
	    	           case MB_ERR_MEMALLOC:
	    	               fprintf(stderr, "\t reason: out of memory\n");
	    	               break;
	    	           case MB_ERR_INTERNAL:
	    	               fprintf(stderr, "\t reason: internal error. Recompile libmoard in debug mode for more info \n");
	    	               break;
	    	       }
	    	    }
	    	    #endif
			
	/***** Messages: pre-conditions **********************************/
    	    
    /***** Message: Adding message iterators ***************************************/
	rc = MB_Iterator_Create(b_tax_payment, &i_tax_payment);
			
	if (rc != MB_SUCCESS)
			{
			   fprintf(stderr, "ERROR: Could not create Iterator for 'tax_payment'\n");
			   switch(rc) {
			       case MB_ERR_INVALID:
			           fprintf(stderr, "\t reason: 'tax_payment' board is invalid\n");
			           break;
			       case MB_ERR_LOCKED:
		               fprintf(stderr, "\t reason: 'tax_payment' board is locked\n");
		               break;
		           case MB_ERR_MEMALLOC:
		               fprintf(stderr, "\t reason: out of memory\n");
		               break;
		           case MB_ERR_INTERNAL:
		               fprintf(stderr, "\t reason: internal error. Recompile libmoard in debug mode for more info \n");
		               break;
			   }
			}

	rc = MB_Iterator_Create(b_dividend_per_share, &i_dividend_per_share);
			
	if (rc != MB_SUCCESS)
			{
			   fprintf(stderr, "ERROR: Could not create Iterator for 'dividend_per_share'\n");
			   switch(rc) {
			       case MB_ERR_INVALID:
			           fprintf(stderr, "\t reason: 'dividend_per_share' board is invalid\n");
			           break;
			       case MB_ERR_LOCKED:
		               fprintf(stderr, "\t reason: 'dividend_per_share' board is locked\n");
		               break;
		           case MB_ERR_MEMALLOC:
		               fprintf(stderr, "\t reason: out of memory\n");
		               break;
		           case MB_ERR_INTERNAL:
		               fprintf(stderr, "\t reason: internal error. Recompile libmoard in debug mode for more info \n");
		               break;
			   }
			}
	
    /***** Function evaluation ***************************************/
	Firm_execute_financial_payments();
    
    /***** Variables: Memory post-conditions *****/
	CU_ASSERT_DOUBLE_EQUAL(PAYMENT_ACCOUNT, 0.0, 1e-3);

    /***** Messages: Message post-conditions *****/
	//start a reading loop

    START_TAX_PAYMENT_MESSAGE_LOOP
	     CU_ASSERT_EQUAL(tax_payment_message->gov_id, 1);
	     CU_ASSERT_DOUBLE_EQUAL(tax_payment_message->tax_payment, 100.0, 1e-3);
	FINISH_TAX_PAYMENT_MESSAGE_LOOP
	
    START_DIVIDEND_PER_SHARE_MESSAGE_LOOP
	     CU_ASSERT_EQUAL(dividend_per_share_message->firm_id, 2);
	     CU_ASSERT_DOUBLE_EQUAL(dividend_per_share_message->current_dividend_per_share, 1.0, 1e-3);
	FINISH_DIVIDEND_PER_SHARE_MESSAGE_LOOP

	/************* At end of unit test, free the agent **************/
	unittest_free_Firm_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_Firm_bankruptcy_illiquidity_procedure()
 * \brief: Unit test for: Firm_bankruptcy_illiquidity_procedure.
 * Status: NOT Tested
 */
void unittest_Firm_bankruptcy_illiquidity_procedure()
{
	int rc;
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Firm_agent();
	
    /***** Variables: Memory pre-conditions **************************/
	ID=1;
	//employee(worker_id, region_id, wage, general_skill, specific_skill);
    reset_employee_array(&EMPLOYEES);
    add_employee(&EMPLOYEES, 2, 1, 1.0,1,1.0);
	
	/***** Messages: initialize message boards **********************************/

	rc = MB_Create(&b_firing, sizeof(m_firing));
    	    #ifdef ERRCHECK
    	    if (rc != MB_SUCCESS)
    	    {
    	       fprintf(stderr, "ERROR: Could not create 'firing' board\n");
    	       switch(rc) {
    	           case MB_ERR_INVALID:
    	               fprintf(stderr, "\t reason: Invalid message size\n");
    	               break;
    	           case MB_ERR_MEMALLOC:
    	               fprintf(stderr, "\t reason: out of memory\n");
    	               break;
    	           case MB_ERR_INTERNAL:
    	               fprintf(stderr, "\t reason: internal error. Recompile libmoard in debug mode for more info \n");
    	               break;
    	       }
    	    }
    	    #endif
	
	/***** Messages: pre-conditions **********************************/
    	    
    /***** Message: Adding message iterators ***************************************/
	rc = MB_Iterator_Create(b_firing, &i_firing);
			
	if (rc != MB_SUCCESS)
			{
			   fprintf(stderr, "ERROR: Could not create Iterator for 'firing'\n");
			   switch(rc) {
			       case MB_ERR_INVALID:
			           fprintf(stderr, "\t reason: 'firing' board is invalid\n");
			           break;
			       case MB_ERR_LOCKED:
		               fprintf(stderr, "\t reason: 'firing' board is locked\n");
		               break;
		           case MB_ERR_MEMALLOC:
		               fprintf(stderr, "\t reason: out of memory\n");
		               break;
		           case MB_ERR_INTERNAL:
		               fprintf(stderr, "\t reason: internal error. Recompile libmoard in debug mode for more info \n");
		               break;
			   }
			}

    /***** Function evaluation ***************************************/
	Firm_bankruptcy_illiquidity_procedure();
    
    /***** Variables: Memory post-conditions *****/
//	CU_ASSERT_DOUBLE_EQUAL(var, result, 1e-3);

    /***** Messages: Message post-conditions *****/
	//start a reading loop

    START_FIRING_MESSAGE_LOOP
	     CU_ASSERT_EQUAL(firing_message->firm_id,1);
    	 CU_ASSERT_EQUAL(firing_message->worker_id,2);
	FINISH_FIRING_MESSAGE_LOOP
	
    /************* At end of unit test, free the agent **************/
	unittest_free_Firm_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}


/*
 * \fn: void unittest_Firm_bankruptcy_idle_counter()
 * \brief: Unit test for: Firm_bankruptcy_idle_counter.
 * Status: NOT Tested
 */
void unittest_Firm_bankruptcy_idle_counter()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Firm_agent();
	
    /***** Variables: Memory pre-conditions **************************/
	BANKRUPTCY_IDLE_COUNTER = 10;
	
	/***** Messages: initialize message boards **********************************/

	/***** Messages: pre-conditions **********************************/
    	    
    /***** Message: Adding message iterators ***************************************/

    /***** Function evaluation ***************************************/
	Firm_bankruptcy_idle_counter();
    
    /***** Variables: Memory post-conditions *****/
	CU_ASSERT_EQUAL(BANKRUPTCY_IDLE_COUNTER, 9);

    /***** Messages: Message post-conditions *****/

    /************* At end of unit test, free the agent **************/
	unittest_free_Firm_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}

/*
 * \fn: void unittest_Firm_reset_bankruptcy_flags()
 * \brief: Unit test for: Firm_reset_bankruptcy_flags.
 * Status: NOT Tested
 */
void unittest_Firm_reset_bankruptcy_flags()
{
	
    /************* At start of unit test, add one agent **************/
	unittest_init_Firm_agent();
	
    /***** Variables: Memory pre-conditions **************************/
	ACTIVE=0;
	BANKRUPTCY_INSOLVENCY_STATE  = 1;
	BANKRUPTCY_ILLIQUIDITY_STATE = 0;
	BANKRUPTCY_IDLE_COUNTER = -1;
	EXTERNAL_FINANCIAL_NEEDS = 0.0;
	
	/***** Messages: initialize message boards **********************************/

	/***** Messages: pre-conditions **********************************/
    	    
    /***** Message: Adding message iterators ***************************************/

    /***** Function evaluation ***************************************/
	Firm_reset_bankruptcy_flags();
    
    /***** Variables: Memory post-conditions *****/
	CU_ASSERT_EQUAL(ACTIVE, 1);
	CU_ASSERT_EQUAL(BANKRUPTCY_INSOLVENCY_STATE, 0);
	CU_ASSERT_EQUAL(BANKRUPTCY_ILLIQUIDITY_STATE, 0);
	
    /***** Messages: Message post-conditions *****/

	/************* At end of unit test, free the agent **************/
	unittest_free_Firm_agent();
    /************* At end of unit tests, free all Messages **********/
    free_messages();
}
