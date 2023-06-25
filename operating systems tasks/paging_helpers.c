/*
 * paging_helpers.c
 *
 *  Created on: Sep 30, 2022
 *      Author: HP
 */
#include "memory_manager.h"

/*[2.1] PAGE TABLE ENTRIES MANIPULATION */
inline void pt_set_page_permissions(uint32* page_directory, uint32 virtual_address, uint32 permissions_to_set, uint32 permissions_to_clear)
{
	//TODO: [PROJECT MS2] [PAGING HELPERS] pt_set_page_permissions
	// Write your code here, remove the panic and write your code
	//panic("pt_set_page_permissions() is not implemented yet...!!");
	/*uint32 *ptr_page_table=NULL;
		   get_page_table(ptr_page_directory,virtual_address,&ptr_page_table);
			if(ptr_page_table==NULL)
		    {
				panic("invalid va");
			}
			ptr_page_table[PTX(virtual_address)] =
			ptr_page_table[PTX(virtual_address)] | (permissions_to_set);
			ptr_page_table[PTX(virtual_address)] =
			ptr_page_table[PTX(virtual_address)] & (~permissions_to_clear);
*/


	uint32 *ptr_page_table = NULL;
		get_page_table(page_directory,virtual_address,&ptr_page_table);
		if(ptr_page_table!=NULL)
		{
			ptr_page_table[PTX(virtual_address)] =
			ptr_page_table[PTX(virtual_address)] | (permissions_to_set);

			ptr_page_table[PTX(virtual_address)] =
			ptr_page_table[PTX(virtual_address)] & (~permissions_to_clear);

		}
		else
		{
			panic("invalid va ");
		}




}

inline int pt_get_page_permissions(uint32* page_directory, uint32 virtual_address )
{
	//TODO: [PROJECT MS2] [PAGING HELPERS] pt_get_page_permissions
	// Write your code here, remove the panic and write your code
	//panic("pt_get_page_permissions() is not implemented yet...!!");
		/*uint32 *ptr_page_table=NULL;
		uint32 pt_indix=PTX(virtual_address);
		 get_page_table(ptr_page_directory,virtual_address,&ptr_page_table);
		if (ptr_page_table==NULL){
			return -1;
		 }

		int frame=(ptr_page_table[pt_indix]>>12)*PAGE_SIZE;
		ptr_page_table[PTX(virtual_address)] = ptr_page_table[PTX(virtual_address)] & (~frame);
		//return ptr_page_table[PTX(virtual_address)]&0x00000FFF;
		int val =ptr_page_table[PTX(virtual_address)];
		return val;*/
	uint32 *ptr_page_table=NULL;
	uint32 pt_indix=PTX(virtual_address);
	uint32 pt_indix2=PDX(virtual_address);
	    get_page_table(page_directory,virtual_address,&ptr_page_table);
			if (ptr_page_table==NULL){
				return -1;
			 }
			//int val =ptr_page_table[PTX(virtual_address)]&0x00000FFF;
			else
			{
				uint32 val = ptr_page_table[PTX(virtual_address)];
				return val & 0x00000fff ;
			}
	/*uint32 *ptr_page_table = NULL;
		get_page_table(page_directory,virtual_address,&ptr_page_table);
		if(ptr_page_table!=NULL)
		{
			uint32 entry = ptr_page_table[PTX(virtual_address)];
			return entry & 0x00000fff ;
		}
		else
		{
			return -1 ;
		}*/

}

inline void pt_clear_page_table_entry(uint32* page_directory, uint32 virtual_address)
{
	//TODO: [PROJECT MS2] [PAGING HELPERS] pt_clear_page_table_entry
	// Write your code here, remove the panic and write your code
	//panic("pt_clear_page_table_entry() is not implemented yet...!!");
	uint32 *ptr_page_table=NULL;
	get_page_table(ptr_page_directory,virtual_address,&ptr_page_table);
			if (ptr_page_table!=TABLE_IN_MEMORY){
				panic("invalid virtual address");

			}
			else{
			ptr_page_table[PTX(virtual_address)]=0;
						tlb_invalidate((void*)NULL,(void*)virtual_address);
			}
}

/***********************************************************************************************/

/*[2.2] ADDRESS CONVERTION*/
inline int virtual_to_physical(uint32* page_directory, uint32 virtual_address)
{
	//TODO: [PROJECT MS2] [PAGING HELPERS] virtual_to_physical
	// Write your code here, remove the panic and write your code
	//panic("virtual_to_physical() is not implemented yet...!!");
	uint32 *ptr_page_table =NULL;
	uint32 va =PTX(virtual_address);
	uint32 va2 =PDX(virtual_address);
	int frame;
	int pa;
	get_page_table(ptr_page_directory,virtual_address,&ptr_page_table);
	if (ptr_page_table==NULL)
		{
		    return -1;
		}
	else
	{
		uint32 page_entry=ptr_page_directory[va2];
		frame=page_entry>>12;
		page_entry=ptr_page_table[va];
		frame =page_entry>>12;
		pa=frame*PAGE_SIZE;
		return pa;
	}


}

/***********************************************************************************************/

/***********************************************************************************************/
/***********************************************************************************************/
/***********************************************************************************************/
/***********************************************************************************************/
/***********************************************************************************************/

///============================================================================================
/// Dealing with page directory entry flags

inline uint32 pd_is_table_used(uint32* page_directory, uint32 virtual_address)
{
	return ( (page_directory[PDX(virtual_address)] & PERM_USED) == PERM_USED ? 1 : 0);
}

inline void pd_set_table_unused(uint32* page_directory, uint32 virtual_address)
{
	page_directory[PDX(virtual_address)] &= (~PERM_USED);
	tlb_invalidate((void *)NULL, (void *)virtual_address);
}

inline void pd_clear_page_dir_entry(uint32* page_directory, uint32 virtual_address)
{
	page_directory[PDX(virtual_address)] = 0 ;
	tlbflush();
}
