/*
 * dyn_block_management.c
 *
 *  Created on: Sep 21, 2022
 *      Author: HP
 */
#include <inc/assert.h>
#include <inc/string.h>
#include "../inc/dynamic_allocator.h"


//==================================================================================//
//============================== GIVEN FUNCTIONS ===================================//
//==================================================================================//

//===========================
// PRINT MEM BLOCK LISTS:
//===========================

void print_mem_block_lists()
{
 cprintf("\n=========================================\n");
 struct MemBlock* blk ;
 struct MemBlock* lastBlk = NULL ;
 cprintf("\nFreeMemBlocksList:\n");
 uint8 sorted = 1 ;
 LIST_FOREACH(blk, &FreeMemBlocksList)
 {
  if (lastBlk && blk->sva < lastBlk->sva + lastBlk->size)
   sorted = 0 ;
  cprintf("[%x, %x)-->", blk->sva, blk->sva + blk->size) ;
  lastBlk = blk;
 }
 if (!sorted) cprintf("\nFreeMemBlocksList is NOT SORTED!!\n") ;

 lastBlk = NULL ;
 cprintf("\nAllocMemBlocksList:\n");
 sorted = 1 ;
 LIST_FOREACH(blk, &AllocMemBlocksList)
 {
  if (lastBlk && blk->sva < lastBlk->sva + lastBlk->size)
   sorted = 0 ;
  cprintf("[%x, %x)-->", blk->sva, blk->sva + blk->size) ;
  lastBlk = blk;
 }
 if (!sorted) cprintf("\nAllocMemBlocksList is NOT SORTED!!\n") ;
 cprintf("\n=========================================\n");

}

//******//
//******//

//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//

//===============================
// [1] INITIALIZE AVAILABLE LIST:
//===============================
void initialize_MemBlocksList(uint32 numOfBlocks)
{
 //TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] initialize_MemBlocksList


 //int arr[]={};
 //int n=0;
    //int min =1;
 struct MemBlock *iteration;
 //struct MemBlock *element2;
 //uint32 n=numOfBlocks;
 LIST_FOREACH(iteration, &(AvailableMemBlocksList))
  {     //if(n==0){}
        LIST_REMOVE(&AvailableMemBlocksList,iteration);
     }
 //if(min==1){}
 LIST_INIT(&AvailableMemBlocksList);
 for(int i=0;i<numOfBlocks;i++)
 {   //if(n==0){}
  LIST_INSERT_HEAD(&AvailableMemBlocksList,&(MemBlockNodes[i]));

 }
 //if(min==1){}
 // Write your code here, remove the panic and write your code
 //panic("initialize_MemBlocksList() is not implemented yet...!!");

}

//===============================
// [2] FIND BLOCK:
//===============================
struct MemBlock *find_block(struct MemBlock_List *blockList, uint32 va)
{
 //int arr[]={};
 //int n=0;
 //int min =0;
 struct MemBlock *element_free;
 struct MemBlock *find;
    int flag=0;
  LIST_FOREACH(element_free, (blockList))
   {
         if(va==element_free->sva)
    {   find=element_free;
        //if(n==1){}
     flag=1;
     break;
    }
         else
         {
        	 //if(n==0){}
          find=NULL;
         }
   }
  //arr[0]=1;
  return find;
 //TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] find_block
 // Write your code here, remove the panic and write your code
 //panic("find_block() is not implemented yet...!!");
}


//=========================================
// [3] INSERT BLOCK IN ALLOC LIST [SORTED]:
//=========================================
void insert_sorted_allocList(struct MemBlock *blockToInsert)
{
	int size = LIST_SIZE(&AllocMemBlocksList);

	if(size==0)
	{
		blockToInsert->prev_next_info.le_prev=NULL;
		blockToInsert->prev_next_info.le_next=NULL;
		LIST_INSERT_HEAD(&AllocMemBlocksList, blockToInsert);
		//size = LIST_SIZE(&AllocMemBlocksList);
		//panic("%d",size);
	}
	else
	{
		struct MemBlock *block = LIST_FIRST(&AllocMemBlocksList);
		struct MemBlock *block_last = LIST_LAST(&AllocMemBlocksList);


		if( block->sva > blockToInsert->sva )
		{
			LIST_INSERT_HEAD(&AllocMemBlocksList, blockToInsert);
		}
		else if( block_last->sva < blockToInsert->sva )
		{
			LIST_INSERT_TAIL(&AllocMemBlocksList, blockToInsert);
		}
		else{
		 struct MemBlock *iteration;
		 LIST_FOREACH(iteration, &AllocMemBlocksList)
		 {
			 if(iteration->sva<blockToInsert->sva)
				 block=iteration;
			 else if(iteration->sva>blockToInsert->sva)
			 {
				 LIST_INSERT_BEFORE(&AllocMemBlocksList, iteration, blockToInsert);
				 break;
			 }
		 }
		}
		/*size = LIST_SIZE(&AllocMemBlocksList);
		panic("%d",size);*/
	}

 //TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] insert_sorted_allocList
 // Write your code here, remove the panic and write your code
 //panic("insert_sorted_allocList() is not implemented yet...!!");
}

//=========================================
// [4] ALLOCATE BLOCK BY FIRST FIT:
//=========================================

struct MemBlock *alloc_block_FF(uint32 size)
{   //int arr[]={};
 //int n=0;
 //int min =1;
 int flag=1;
 struct MemBlock *iteration;
 struct MemBlock *bool;
 struct MemBlock* new_block=LIST_LAST(&AvailableMemBlocksList);
 LIST_FOREACH(iteration, &(FreeMemBlocksList))
  {

     if(size==iteration->size)
     { //if(n==0){}
      new_block=iteration;
      LIST_REMOVE(&FreeMemBlocksList,iteration);
      return new_block;
     }
     else if(size<iteration->size)
     {   //if(n==0){}
      new_block->sva=iteration->sva;
      new_block->size=size;
      iteration->size=iteration->size-size;
      iteration->sva=iteration->sva+size;
      LIST_REMOVE(&AvailableMemBlocksList,new_block);
      //if(min==1){}
      return new_block;
      //break;
     }

  }
 return NULL;
 //TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] alloc_block_FF
 // Write your code here, remove the panic and write your code
 //panic("alloc_block_FF() is not implemented yet...!!");
}

//=========================================
// [5] ALLOCATE BLOCK BY BEST FIT:
//=========================================
struct MemBlock *alloc_block_BF(uint32 size)
{
 int arr[]={};
 int n=0;
 int min =1;
 int flag=1;
 int compare_size=0;
 int found_block_size;
 struct MemBlock *iteration;
 struct MemBlock *bollean;
 struct MemBlock* new_block=LIST_LAST(&AvailableMemBlocksList);
 LIST_FOREACH(iteration, &(FreeMemBlocksList))
  {compare_size=iteration->size-size;
     if(size==iteration->size)
     { if(n==0){}
      flag=0;
      new_block=iteration;
      LIST_REMOVE(&FreeMemBlocksList,iteration);
      return new_block;

     }
     if(compare_size>0)
     {
      if(iteration->size<found_block_size)
      {if(n==0){}
       found_block_size=iteration->size;
       flag=1;
      }
     }


  }
 if(min==1){}
LIST_FOREACH(iteration, &(FreeMemBlocksList))
  {
  if(found_block_size==iteration->size)
  {
      //  arr[0]=1;
   if(min==1){}
     new_block->sva=iteration->sva;
     new_block->size=size;
     iteration->size=iteration->size-size;
     iteration->sva=iteration->sva+size;
     LIST_REMOVE(&AvailableMemBlocksList,new_block);
     return new_block;

      }
 }
return NULL;
 //TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] alloc_block_BF
 // Write your code here, remove the panic and write your code
 //panic("alloc_block_BF() is not implemented yet...!!");
}


//=========================================
// [7] ALLOCATE BLOCK BY NEXT FIT:
//=========================================
struct MemBlock *alloc_block_NF(uint32 size)
{

 //TODO: [PROJECT MS1 - BONUS] [DYNAMIC ALLOCATOR] alloc_block_NF
 // Write your code here, remove the panic and write your code
 panic("alloc_block_NF() is not implemented yet...!!");

}

//===================================================
// [8] INSERT BLOCK (SORTED WITH MERGE) IN FREE LIST:
//===================================================
void insert_sorted_with_merge_freeList(struct MemBlock *blockToInsert)
{
	int size = LIST_SIZE(&FreeMemBlocksList);

		if(size==0)
		{
			//blockToInsert->prev_next_info.le_prev=NULL;
			//blockToInsert->prev_next_info.le_next=NULL;
			LIST_INSERT_HEAD(&FreeMemBlocksList, blockToInsert);
			//size = LIST_SIZE(&AllocMemBlocksList);
			//panic("%d",size);
		}
		else
		{
			struct MemBlock *block = LIST_FIRST(&FreeMemBlocksList);
			struct MemBlock *block_last = LIST_LAST(&FreeMemBlocksList);

			if( block->sva > blockToInsert->sva )
			{
				if(blockToInsert->size+blockToInsert->sva==block->sva)
				{
					block->sva=blockToInsert->sva;
					block->size=block->size+blockToInsert->size;
					 blockToInsert->size=0;
					 blockToInsert->sva=0;
					 LIST_INSERT_TAIL(&AvailableMemBlocksList, blockToInsert);
				}
				else {
					LIST_INSERT_HEAD(&FreeMemBlocksList, blockToInsert);
				}

			}

			else if( block_last->sva < blockToInsert->sva )
			{
				 if( block_last->size +block_last->sva == blockToInsert->sva)
				{
					 block_last->size=block_last->size+blockToInsert->size;
					 blockToInsert->size=0;
					 blockToInsert->sva=0;
					 LIST_INSERT_TAIL(&AvailableMemBlocksList, blockToInsert);
					 //panic("%d",LIST_SIZE(&AvailableMemBlocksList));
				}
				else{
					LIST_INSERT_TAIL(&FreeMemBlocksList, blockToInsert);
				}
			}
			else if(block_last->sva > blockToInsert->sva&&blockToInsert->sva+blockToInsert->size==block_last->sva )
			{
				block_last->sva=blockToInsert->sva;
				block_last->size=block_last->size+blockToInsert->size;
				//panic("%d",LIST_SIZE(&FreeMemBlocksList));
			}
			else{
			 struct MemBlock *iteration;
			 LIST_FOREACH(iteration, &FreeMemBlocksList)
			 {
				 struct MemBlock *block_l=LIST_NEXT(iteration);
				 if(iteration->sva<blockToInsert->sva&&iteration->size+iteration->sva==blockToInsert->sva&&blockToInsert->sva+blockToInsert->size==LIST_NEXT(iteration)->sva)
				 {
					 //merge with pre,next
					 iteration->size=iteration->size+blockToInsert->size+LIST_NEXT(iteration)->size;
					 LIST_REMOVE(&FreeMemBlocksList,block_l);
					 blockToInsert->size=0;
					 blockToInsert->sva=0;
					 block_l->size=0;
					 block_l->sva=0;
					 LIST_INSERT_TAIL(&AvailableMemBlocksList, blockToInsert);
					 LIST_INSERT_TAIL(&AvailableMemBlocksList, block_l);
					 break;
				 }
				 else if(iteration->sva<blockToInsert->sva&&iteration->size+iteration->sva==blockToInsert->sva&&blockToInsert->size+blockToInsert->sva<LIST_NEXT(iteration)->sva)
				 {
					 //merge with next
					 iteration->size=iteration->size+blockToInsert->size;
					 blockToInsert->size=0;
					 blockToInsert->sva=0;
					 LIST_INSERT_TAIL(&AvailableMemBlocksList, blockToInsert);
					 break;
				 }
				 else if(iteration->sva<blockToInsert->sva&&iteration->size+iteration->sva<blockToInsert->sva&&blockToInsert->sva+blockToInsert->size==LIST_NEXT(iteration)->sva)
				 {
					 //merge with prev
					 LIST_NEXT(iteration)->sva=blockToInsert->sva;
					 LIST_NEXT(iteration)->size=LIST_NEXT(iteration)->size+blockToInsert->size;
					 blockToInsert->size=0;
					 blockToInsert->sva=0;
					 LIST_INSERT_TAIL(&AvailableMemBlocksList, blockToInsert);
					 break;
				 }
				 else if(iteration->sva<blockToInsert->sva&&blockToInsert->sva+blockToInsert->size<LIST_NEXT(iteration)->sva)
				 {
					 LIST_INSERT_AFTER(&FreeMemBlocksList, iteration, blockToInsert);
					 break;
				 }

					 //LIST_INSERT_BEFORE(&FreeMemBlocksList, iteration, blockToInsert);
			 }
			}
			/*size = LIST_SIZE(&AllocMemBlocksList);
			panic("%d",size);*/
		}
 //cprintf("BEFORE INSERT with MERGE: insert [%x, %x)\n=====================\n", blockToInsert->sva, blockToInsert->sva + blockToInsert->size);
 //print_mem_block_lists() ;

 //TODO: [PROJECT MS1] [DYNAMIC ALLOCATOR] insert_sorted_with_merge_freeList
 // Write your code here, remove the panic and write your code
 //panic("insert_sorted_with_merge_freeList() is not implemented yet...!!");



 //cprintf("\nAFTER INSERT with MERGE:\n=====================\n");
 //print_mem_block_lists();

}
