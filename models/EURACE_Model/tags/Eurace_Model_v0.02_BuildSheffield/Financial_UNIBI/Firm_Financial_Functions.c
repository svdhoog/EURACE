#include "../header.h"
#include "../Firm_agent_header.h"
#include "../my_library_header.h"
#include "../Firm_Library_Functions.h"


/************Firm Role: Financial Market Role ********************************/

/*
 * \fn: Firm_compute_and_send_bond_orders()
 * \brief: This function computes the firm's bond orders and sends a bond_order_message to the clearinghouse.
 */
int Firm_compute_and_send_bond_orders()
{
    /*TESTING*/
    int bond_id=1;
    double limit_price=1.0;
    double limit_quantity=1.0;
    
    add_bond_order_message(ID, bond_id, limit_price, limit_quantity);
    
    return 0;
}

/*
 * \fn: Firm_read_bond_transactions()
 * \brief: This function reads a bond_transaction_message from the clearinghouse, and updates the firm's trading account.
 */
int Firm_read_bond_transactions()
{
	double finances;
	
    START_BOND_TRANSACTION_MESSAGE_LOOP
    if(bond_transaction_message->trader_id==ID)
    {
        //bond_transaction_message->bond_id
        //bond_transaction_message->transaction_price
        //bond_transaction_message->transaction_quantity
    	
    	//Finances obtained: positive quantity is demand, negative quantity is selling
    	finances = (-1)*bond_transaction_message->transaction_price * bond_transaction_message->transaction_quantity;
    	
    	//Increase payment account with the finances obtained
    	PAYMENT_ACCOUNT += finances;
    	
    	//Decrease external financial needs with the finances obtained
    	EXTERNAL_FINANCIAL_NEEDS -= finances;
    }
    FINISH_BOND_TRANSACTION_MESSAGE_LOOP
    return 0;
}

/*
 * \fn: Firm_compute_and_send_stock_orders()
 * \brief: This function computes the firm's stock orders (share emmision) and sends a stock_order_message to the clearinghouse.
 */
int Firm_compute_and_send_stock_orders()
{
	double limit_price=CURRENT_SHARE_PRICE*0.99;
    int quantity = -1*(1+EXTERNAL_FINANCIAL_NEEDS/limit_price);
    
    
    //Firm tries to sell stock_units shares:
    //add_order_message(trader_id, asset_id, limit_price, quantity)
    add_order_message(ID, ID, limit_price, quantity);

    return 0;
}

/*
 * \fn: Firm_read_stock_transactions()
 * \brief: This function reads a stock_transaction_message from the clearinghouse, and updates the firm's trading account.
 */
int Firm_read_stock_transactions()
{
	double finances;
	
    START_ORDER_STATUS_MESSAGE_LOOP
    if(order_status_message->trader_id==ID)
    {
        //order_status_message->asset_id
        //order_status_message->price
        //order_status_message->quantity
    	    	
    	//Finances obtained: positive quantity is demand, negative quantity is selling
    	finances = (-1)*order_status_message->price * order_status_message->quantity;
    	
    	//Increase payment account with the finances obtained
    	PAYMENT_ACCOUNT += finances;
    	
    	//Decrease external financial needs with the finances obtained
    	EXTERNAL_FINANCIAL_NEEDS -= finances;
    }
    FINISH_ORDER_STATUS_MESSAGE_LOOP
    return 0;
}

/*
 * \fn: Firm_compute_and_send_bond_orders()
 * \brief: This function computes the firm's gov_bond orders and sends a gov_bond_order_message to the clearinghouse.
 */
int Firm_compute_and_send_gov_bond_orders()
{
    /*TESTING*/
    int gov_bond_id=1;
    double limit_price=1.0;
    double limit_quantity=1.0;
    
    add_gov_bond_order_message(ID, gov_bond_id, limit_price, limit_quantity);
    
    return 0;
}

/*
 * \fn: Firm_read_gov_bond_transactions()
 * \brief: This function reads a gov_bond_transaction_message from the clearinghouse, and updates the firm's trading account.
 */
int Firm_read_gov_bond_transactions()
{
    START_GOV_BOND_TRANSACTION_MESSAGE_LOOP
    if(gov_bond_transaction_message->trader_id==ID)
    {
        //gov_bond_transaction_message->bond_id;
        //gov_bond_transaction_message->transaction_price;
        //gov_bond_transaction_message->transaction_quantity;
    	
      	//Convention: positive quantity is demand, negative quantity is selling
        //PAYMENT_ACCOUNT -= gov_bond_transaction_message->transaction_price * gov_bond_transaction_message->transaction_quantity;    	
    }
    FINISH_GOV_BOND_TRANSACTION_MESSAGE_LOOP
    return 0;
}
