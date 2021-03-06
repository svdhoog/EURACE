/*********************************
 * Mall_aux_functions.c 
 * Mall auxiliary functions.
 * *********************************
 * History:
 * 29/10/08 Sander 
 *********************************/
#include "../header.h"
#include "../Mall_agent_header.h"
#include "Mall_aux_header.h"

//#define NO_REGIONS 2 //number of regions (hard-coded here, but should be a environment constant)


/* \fn: void Mall_add_export_data()
 * \brief: Function to add data to the export matrix (during every transaction).
 * export_volume: Q_t
 * export_value: P_t*Q_t
 * export_previous_value: P_t-1*Q_t
 */
void Mall_add_export_data(int firm_region, int household_region, double export_volume, double export_value, double export_previous_value)
{
	int index;
	
	//add value to export matrix
	index=(firm_region-1)*NO_REGIONS+(household_region-1);
	EXPORT_VOLUME_MATRIX[index] = export_volume;
	EXPORT_VALUE_MATRIX[index] = export_value;
	EXPORT_PREVIOUS_VALUE_MATRIX[index] = export_previous_value;
}


