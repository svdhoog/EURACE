/* $Id: delete.c 687 2008-02-27 16:57:25Z lsc $ */
/*!
 * \file serial/delete.c
 * \code
 *      Author: Lee-Shawn Chin 
 *      Date  : Feb 2008 
 *      Copyright (c) 2008 STFC Rutherford Appleton Laboratory
 * \endcode
 * 
 * \brief Serial implementation of MB_Delete()
 * 
 */

#include "mb_serial.h"

/*!
 * \brief Deletes a MessageBoard
 * \ingroup MB_API
 * \param[in,out] mb_ptr Address of MessageBoard handle
 * 
 * Upon successful removal of the reference to the board from the 
 * ::MBI_OM_mboard ObjectMap, we first delete the pooled-list associated 
 * with the board and then deallocate the board object.
 * 
 * \note It is valid to delete a null board (::MB_NULL_MBOARD). The routine
 * will return immediately with ::MB_SUCCESS
 * 
 * Possible return codes:
 *  - ::MB_SUCCESS 
 *  - ::MB_ERR_INVALID (invalid board given) 
 *  - ::MB_ERR_LOCKED (message board is locked for synchronisation)
 *  - ::MB_ERR_INTERNAL (possible bug. Recompile and run in debug mode for hints)
 */
int MB_Delete(MBt_Board *mb_ptr) {
    
    int rc;
    void *obj;
    MBIt_Board *board;
    
    /* Check for NULL message board */
    if (*mb_ptr == MB_NULL_MBOARD) return MB_SUCCESS;
    
    /* get reference to mboard object */
    board = (MBIt_Board *)MBI_getMBoardRef(*mb_ptr);
    if (board == NULL) return MB_ERR_INVALID;
    assert(board->data != NULL);
    
    /* If message board locked by other process */
    if (board->locked == MB_TRUE) return MB_ERR_LOCKED;

    /* Delete object from map*/
    obj = MBI_objmap_pop(MBI_OM_mboard, (OM_key_t)*mb_ptr);
    assert(obj == (void*)board);
    
    /* free object memory */
    rc = pl_delete(&(board->data));
    assert(rc == PL_SUCCESS);
    
    /* deallocate board object */
    free(obj);
    
    /* set mb to NULL */
    *mb_ptr = MB_NULL_MBOARD;
    
    /* indicate internal error if any */
    if (rc != PL_SUCCESS) return MB_ERR_INTERNAL;
    
    return MB_SUCCESS;
}

