#include "../header.h"
#include "../Household_agent_header.h"
#include "../my_library_header.h"



/************************************ Household agent functions ************************************/
/*************************************Household Role: Statistics *********************************/

/** \fn Household_read_tax_rates()
 * \brief Household reads the tax_rates_messages from Governments
 */
int Household_read_tax_rates()
{
	//Message send by Government:
	START_GOVERNMENT_TAX_RATES_MESSAGE_LOOP			
		if(government_tax_rates_message->gov_id == GOV_ID)
		{
			TAX_RATE_HH_LABOUR = government_tax_rates_message->tax_rate_hh_labour;
			TAX_RATE_HH_CAPITAL = government_tax_rates_message->tax_rate_hh_capital;
		}
	FINISH_GOVERNMENT_TAX_RATES_MESSAGE_LOOP

	return 0;
}
