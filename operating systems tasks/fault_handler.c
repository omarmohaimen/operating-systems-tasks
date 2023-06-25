/*
 * fault_handler.c
 *
 *  Created on: Oct 12, 2022
 *      Author: HP
 */

#include "trap.h"
#include <kern/proc/user_environment.h>
#include "../cpu/sched.h"
#include "../disk/pagefile_manager.h"
#include "../mem/memory_manager.h"

//2014 Test Free(): Set it to bypass the PAGE FAULT on an instruction with this length and continue executing the next one
// 0 means don't bypass the PAGE FAULT
uint8 bypassInstrLength = 0;

//===============================
// REPLACEMENT STRATEGIES
//===============================
//2020
void setPageReplacmentAlgorithmLRU(int LRU_TYPE)
{
	assert(LRU_TYPE == PG_REP_LRU_TIME_APPROX || LRU_TYPE == PG_REP_LRU_LISTS_APPROX);
	_PageRepAlgoType = LRU_TYPE ;
}
void setPageReplacmentAlgorithmCLOCK(){_PageRepAlgoType = PG_REP_CLOCK;}
void setPageReplacmentAlgorithmFIFO(){_PageRepAlgoType = PG_REP_FIFO;}
void setPageReplacmentAlgorithmModifiedCLOCK(){_PageRepAlgoType = PG_REP_MODIFIEDCLOCK;}
/*2018*/ void setPageReplacmentAlgorithmDynamicLocal(){_PageRepAlgoType = PG_REP_DYNAMIC_LOCAL;}
/*2021*/ void setPageReplacmentAlgorithmNchanceCLOCK(int PageWSMaxSweeps){_PageRepAlgoType = PG_REP_NchanceCLOCK;  page_WS_max_sweeps = PageWSMaxSweeps;}

//2020
uint32 isPageReplacmentAlgorithmLRU(int LRU_TYPE){return _PageRepAlgoType == LRU_TYPE ? 1 : 0;}
uint32 isPageReplacmentAlgorithmCLOCK(){if(_PageRepAlgoType == PG_REP_CLOCK) return 1; return 0;}
uint32 isPageReplacmentAlgorithmFIFO(){if(_PageRepAlgoType == PG_REP_FIFO) return 1; return 0;}
uint32 isPageReplacmentAlgorithmModifiedCLOCK(){if(_PageRepAlgoType == PG_REP_MODIFIEDCLOCK) return 1; return 0;}
/*2018*/ uint32 isPageReplacmentAlgorithmDynamicLocal(){if(_PageRepAlgoType == PG_REP_DYNAMIC_LOCAL) return 1; return 0;}
/*2021*/ uint32 isPageReplacmentAlgorithmNchanceCLOCK(){if(_PageRepAlgoType == PG_REP_NchanceCLOCK) return 1; return 0;}

//===============================
// PAGE BUFFERING
//===============================
void enableModifiedBuffer(uint32 enableIt){_EnableModifiedBuffer = enableIt;}
uint8 isModifiedBufferEnabled(){  return _EnableModifiedBuffer ; }

void enableBuffering(uint32 enableIt){_EnableBuffering = enableIt;}
uint8 isBufferingEnabled(){  return _EnableBuffering ; }

void setModifiedBufferLength(uint32 length) { _ModifiedBufferLength = length;}
uint32 getModifiedBufferLength() { return _ModifiedBufferLength;}

//===============================
// FAULT HANDLERS
//===============================

//Handle the table fault
void table_fault_handler(struct Env * curenv, uint32 fault_va)
{
	//panic("table_fault_handler() is not implemented yet...!!");
	//Check if it's a stack page
	uint32* ptr_table;
#if USE_KHEAP
	{
		ptr_table = create_page_table(curenv->env_page_directory, (uint32)fault_va);
	}
#else
	{
		__static_cpt(curenv->env_page_directory, (uint32)fault_va, &ptr_table);
	}
#endif
}

//Handle the page fault

//void placement (struct Env * curenv, uint32 fault_va)

void page_fault_handler(struct Env * curenv, uint32 fault_va)
{

	//TODO: [PROJECT MS3] [FAULT HANDLER] page_fault_handler
		// Write your code here, remove the panic and write your code
		//panic("page_fault_handler() is not implemented yet...!!");
		uint32 sizze=env_page_ws_get_size(curenv);
		fault_va=ROUNDDOWN(fault_va,PAGE_SIZE);

		 // int per=pt_get_page_permissions(curenv->env_page_directory,fault_va);
		 allocate_chunk(curenv->env_page_directory,fault_va,PAGE_SIZE,PERM_USER|PERM_WRITEABLE|PERM_PRESENT);
		  int read=pf_read_env_page(curenv,(void*)fault_va);
		  uint32 *ptr_page_table=NULL;
		  get_page_table(curenv->env_page_directory,fault_va,&ptr_page_table);
		 struct FrameInfo*frame;//=get_frame_info(curenv->env_page_directory,fault_va,&ptr_page_table);
		 // allocate_frame(&frame);
		 // map_frame(curenv->env_page_directory,frame,fault_va,PERM_USER|PERM_WRITEABLE|PERM_PRESENT);
		  if(read==E_PAGE_NOT_EXIST_IN_PF)//page not exist
			 {
				 if((fault_va>=USTACKBOTTOM&&fault_va<USTACKTOP)|(fault_va>=USER_HEAP_START&&fault_va<USER_HEAP_MAX))
				 {
					 //env_page_ws_print(curenv);// to show element in working set
					 //curenv->ptr_pageWorkingSet;
				 }
				 else
				 {
					panic("ILLEGAL MEMORY ACCESS for the given fault virtual address");
				 }
			}
	  if(sizze<curenv->page_WS_max_size )//exist space
	   {
		  uint32 size=(uint32) curenv->page_WS_max_size;
		   for(uint32 i=0;i<=size;i++)
			{
			 uint32 ent=env_page_ws_is_entry_empty(curenv,i);
				if(ent ==1)//entery is empty
				{
					env_page_ws_set_entry(curenv,i,fault_va);
					//if any thing is not correct uncomment line
					//curenv->page_last_WS_index=i;
				  	//if any thing is not correct uncomment two lines
					 curenv->page_last_WS_index++;
					curenv->page_last_WS_index%=curenv->page_WS_max_size;
					break;
				}
			 }
		  }




 else
	{
		uint32 size =curenv->page_WS_max_size ;
		uint32 incr=curenv->page_last_WS_index;
		uint32 index=curenv->page_last_WS_index;
		uint32 victim_va;
		uint32 *ptr_page_table = NULL;
		struct FrameInfo *frame=NULL;
		for(; incr <size; incr++)
		{
			victim_va = env_page_ws_get_virtual_address(curenv,incr);
			uint32 used = pt_get_page_permissions(curenv->env_page_directory,victim_va);
			uint32 new_per_used = used &(PERM_USED);
			 // page not used
			if ( new_per_used == 0 )
			{
				index=incr;
				break;
			}
			else
			{
				pt_set_page_permissions( curenv->env_page_directory,victim_va , 0 , PERM_USED );
				 curenv->page_last_WS_index++;
			}

		}   // End loop
		victim_va = env_page_ws_get_virtual_address(curenv,index);
		uint32 mod=pt_get_page_permissions(curenv->env_page_directory,victim_va);
		uint32 new_per_mod = mod &(PERM_MODIFIED);
		// page not MODIFIED
		if (new_per_mod==0)
		{
			unmap_frame(curenv->env_page_directory,victim_va);
			 env_page_ws_clear_entry(curenv,index);
		}
		else
		{
		 // check and update there is page in page file or not
			ptr_page_table = NULL;
			frame=get_frame_info(curenv->env_page_directory,victim_va,&ptr_page_table);
			pf_update_env_page(curenv,victim_va,frame);
			unmap_frame(curenv->env_page_directory,victim_va);
			 env_page_ws_clear_entry(curenv,index);

		}
		//upfdate working set
		env_page_ws_set_entry(curenv,index,fault_va );
		curenv->page_last_WS_index=index+1;
		if ( curenv->page_WS_max_size == curenv->page_last_WS_index )
		{
			curenv->page_last_WS_index = 0;
		}
	}


}
void __page_fault_handler_with_buffering(struct Env * curenv, uint32 fault_va)
{
	// Write your code here, remove the panic and write your code
	panic("__page_fault_handler_with_buffering() is not implemented yet...!!");

}




