/*
 * chunk_operations.c
 *
 *  Created on: Oct 12, 2022
 *      Author: HP
 */

#include <kern/trap/fault_handler.h>
#include <kern/disk/pagefile_manager.h>
#include "kheap.h"
#include "memory_manager.h"


/******************************/
/*[1] RAM CHUNKS MANIPULATION */
/******************************/

//===============================
// 1) CUT-PASTE PAGES IN RAM:
//===============================
//This function should cut-paste the given number of pages from source_va to dest_va
//if the page table at any destination page in the range is not exist, it should create it
//Hint: use ROUNDDOWN/ROUNDUP macros to align the addresses
int cut_paste_pages(uint32* page_directory, uint32 source_va, uint32 dest_va, uint32 num_of_pages)
{
	//TODO: [PROJECT MS2] [CHUNK OPERATIONS] cut_paste_pages
	// Write your code here, remove the panic and write your code
	//panic("cut_paste_pages() is not implemented yet...!!");
	/*uint32 *ptr_page_table;
	uint32 x=ROUNDDOWN(source_va,PAGE_SIZE);
	uint32 i=ROUNDDOWN(dest_va,PAGE_SIZE);
	uint32 size=ROUNDUP(dest_va+PAGE_SIZE*num_of_pages,PAGE_SIZE) ;
	struct FrameInfo*ptr_frame_info;
		for(;i<size;i+=PAGE_SIZE)
		{
			ptr_frame_info=NULL;
			ptr_frame_info=get_frame_info(page_directory,i,&ptr_page_table);
			if (ptr_frame_info!=NULL)
				return -1;
		}
		ptr_page_table=NULL;
		x=ROUNDDOWN(source_va,PAGE_SIZE);
		i=ROUNDDOWN(dest_va,PAGE_SIZE);
		size=ROUNDUP(dest_va+num_of_pages*PAGE_SIZE,PAGE_SIZE) ;
		for(;i<size;i+=PAGE_SIZE){

			if (get_page_table(page_directory,i,&ptr_page_table)!=TABLE_IN_MEMORY)
				create_page_table(page_directory,i);
			ptr_page_table=NULL;
			struct FrameInfo*pptr_frame_info=get_frame_info(page_directory,x,&ptr_page_table);
			map_frame(page_directory,pptr_frame_info,i,pt_get_page_permissions(page_directory,x));
			unmap_frame(page_directory,x);

			x+=PAGE_SIZE;
	   }
		return 0;



	uint32 size=ROUNDUP(num_of_pages *PAGE_SIZE,PAGE_SIZE);
	uint32 *ptr_page_table=NULL;
	uint32 i=ROUNDDOWN(dest_va,PAGE_SIZE);

	struct FrameInfo*ptr_frame_info =get_frame_info(ptr_page_directory,dest_va,&ptr_page_table);
	for( i;i<size;i+PAGE_SIZE)
	{
		get_page_table(ptr_page_directory,i,&ptr_page_table);
		ptr_frame_info=get_frame_info(ptr_page_directory,i,&ptr_page_table);
		if (ptr_page_table==NULL)
		{
			create_page_table(ptr_page_directory,i);
		}
		if(ptr_frame_info!=NULL)
			{
				return -1;
			}
	}
	for( i;i<size;i+PAGE_SIZE)
		{
		  ptr_frame_info=get_frame_info(ptr_page_directory,i,&ptr_page_table);
		  if(ptr_frame_info==NULL)
			{
				get_frame_info(ptr_page_directory,source_va,&ptr_page_table);
				map_frame(ptr_page_directory,ptr_frame_info,i,PERM_USED|PERM_WRITEABLE);
				unmap_frame(ptr_page_directory,source_va);
			}


		}
	return 0;



	//uint32 i=ROUNDDOWN(dest_va,PAGE_SIZE);
	uint32 size=ROUNDUP(dest_va*PAGE_SIZE,PAGE_SIZE);
	dest_va=ROUNDDOWN(dest_va,PAGE_SIZE);
	source_va=ROUNDDOWN(source_va,PAGE_SIZE);
	uint32* ptr_page_table=NULL;
	struct FrameInfo* Ptr_Frame_Info;
	for (uint32 i=dest_va;i<size;(i+=PAGE_SIZE))
	{

	    Ptr_Frame_Info= get_frame_info(page_directory,i,&ptr_page_table);
		if (Ptr_Frame_Info!=NULL)
		{
			return -1;
		}
	}

	for (uint32 i=dest_va;i<size;(i+=PAGE_SIZE))
	{

		 get_page_table(page_directory,i,&ptr_page_table);
		 if(ptr_page_table==NULL)
		 {
			 create_page_table(ptr_page_directory,i);
		 }
		 else
		 {
			 if(Ptr_Frame_Info==NULL)
			 {
				Ptr_Frame_Info=get_frame_info(ptr_page_directory,source_va,&ptr_page_table);
				map_frame(ptr_page_directory,Ptr_Frame_Info,i,pt_get_page_permissions(page_directory,source_va));
				unmap_frame(ptr_page_directory,source_va);
			 }
		 }
   }
	return 0;
*/
	    uint32 *ptr_page_table;
		source_va=ROUNDDOWN(source_va,PAGE_SIZE);
		dest_va=ROUNDDOWN(dest_va,PAGE_SIZE);
		ptr_page_table=NULL;
		uint32 size=ROUNDUP(dest_va+PAGE_SIZE*num_of_pages,PAGE_SIZE) ;
	    struct FrameInfo*ptr_frame_info;
		for(uint32 i=dest_va;i<size;i+=PAGE_SIZE)
		{
			ptr_frame_info=NULL;
			ptr_frame_info=get_frame_info(page_directory,i,&ptr_page_table);
			if (ptr_frame_info!=NULL)
						return -1;
		}
		source_va=ROUNDDOWN(source_va,PAGE_SIZE);
		dest_va=ROUNDDOWN(dest_va,PAGE_SIZE);
		//uint32 size=ROUNDUP(dest_va+PAGE_SIZE*num_of_pages,PAGE_SIZE) ;
		for(uint32 i=dest_va;i<size;i+=PAGE_SIZE)
		{

			if (get_page_table(page_directory,i,&ptr_page_table)!=TABLE_IN_MEMORY)
			{
			create_page_table(page_directory,i);
			}
			ptr_page_table=NULL;
			struct FrameInfo*pptr_frame_info=get_frame_info(page_directory,source_va,&ptr_page_table);
			map_frame(page_directory,pptr_frame_info,i,pt_get_page_permissions(page_directory,source_va));
			unmap_frame(page_directory,source_va);
			source_va+=PAGE_SIZE;
		}
	return 0;
}

//===============================
// 2) COPY-PASTE RANGE IN RAM:
//===============================
//This function should copy-paste the given size from source_va to dest_va
//if the page table at any destination page in the range is not exist, it should create it
//Hint: use ROUNDDOWN/ROUNDUP macros to align the addresses
int copy_paste_chunk(uint32* page_directory, uint32 source_va, uint32 dest_va, uint32 size)
{
	/*//TODO: [PROJECT MS2] [CHUNK OPERATIONS] copy_paste_chunk
	// Write your code here, remove the panic and write your code
	//panic("copy_paste_chunk() is not implemented yet...!!");
	uint32 source=ROUNDDOWN(source_va+size,PAGE_SIZE);
	uint32 destin=ROUNDDOWN(dest_va+size,PAGE_SIZE);
	uint32 x=ROUNDUP(dest_va+size,PAGE_SIZE);
	uint32* ptr_page_table=NULL;
	struct FrameInfo* Ptr_Frame_Info;
	for (uint32 i=destin;i<x;(i=i+PAGE_SIZE))
	{
	 Ptr_Frame_Info= get_frame_info(page_directory,i,&ptr_page_table);
	 //int ret=pt_get_page_permissions(page_directory,destin);
		if (Ptr_Frame_Info!=NULL&&~PERM_WRITEABLE)
		{

			return -1;
		}
	}

	for (uint32 i=dest_va;i<x;(i+=PAGE_SIZE))
	{

		 get_page_table(page_directory,i,&ptr_page_table);
		 if(ptr_page_table==NULL)
		 {
			 create_page_table(ptr_page_directory,i);
		 }
		 else
		 {
			struct FrameInfo*Ptr_Frame_Info=get_frame_info(page_directory,source_va,&ptr_page_table);
			map_frame(page_directory,Ptr_Frame_Info,i,PERM_USED|PERM_WRITEABLE);
			//map_frame(page_directory,Ptr_Frame_Info,source_va,PERM_USED|PERM_WRITEABLE);
			source_va+=PAGE_SIZE;
		 }
	}
return 0;*/

	uint32 x=ROUNDUP(dest_va+size,PAGE_SIZE);
			uint32* ptr_page_table=NULL;
			uint8* dest=(uint8*)dest_va;
			uint8* sour=(uint8*)source_va;

			struct FrameInfo* Ptr_Frame_Info=NULL;
			for (uint32 i=dest_va;i<x;i++)
			{
			 Ptr_Frame_Info= get_frame_info(page_directory,i,&ptr_page_table);
			 int ret=pt_get_page_permissions(page_directory,dest_va);
				if (Ptr_Frame_Info!=NULL)
				{
					if(ret!= PERM_WRITEABLE)
								return -1;
					else if(ret== PERM_WRITEABLE)
					{
						int all=allocate_frame(&Ptr_Frame_Info);
						if(all==E_NO_MEM)
						{
							//allocate_frame(&Ptr_Frame_Info);
							//map_frame(page_directory,Ptr_Frame_Info,i,ret);
						}
						//struct FrameInfo*Ptr_Frame_Info=get_frame_info(page_directory,source_va,&ptr_page_table);
						//get_frame_info(page_directory,source_va,&ptr_page_table);
						//map_frame(page_directory,Ptr_Frame_Info,i,PERM_WRITEABLE);
					}
					allocate_frame(&Ptr_Frame_Info);
					map_frame(page_directory,Ptr_Frame_Info,i,ret);
				}

				source_va+=PAGE_SIZE;
			 //dest_va+=PAGE_SIZE;
			}
		for(uint32 i=dest_va;i<x;i++)
			{get_page_table(page_directory,i,&ptr_page_table);

			 if(ptr_page_table!=NULL){
				 Ptr_Frame_Info= get_frame_info(page_directory,source_va,&ptr_page_table);
				 map_frame(page_directory,Ptr_Frame_Info,i,PERM_WRITEABLE);

				 }
				 else
				 {
					 create_page_table(page_directory,i);

					 Ptr_Frame_Info= get_frame_info(page_directory,source_va,&ptr_page_table);
					 map_frame(page_directory,Ptr_Frame_Info,i,PERM_WRITEABLE);
				 }
			 allocate_frame(&Ptr_Frame_Info);
			 *dest=*sour;
			 source_va+=PAGE_SIZE;
			 dest_va+=PAGE_SIZE;
			}
		return 0;



		/*for (uint32 i=dest_va;i<x;i+=PAGE_SIZE)
		{
			int ret= get_page_table(page_directory,i,&ptr_page_table);
			 if (ret!=TABLE_IN_MEMORY)
			 {
					 create_page_table(ptr_page_directory,i);
			 }
			 else
			 {
				 return -1;
			 }
			 Ptr_Frame_Info= get_frame_info(page_directory,i,&ptr_page_table);
			 if(Ptr_Frame_Info==NULL)
			 {
				 int all=allocate_frame(&Ptr_Frame_Info);
				 	if(all==E_NO_MEM)
				 	{
				 		allocate_frame(&Ptr_Frame_Info);
				 	}

				// int sou=pt_get_page_permissions(page_directory,source_va);
				 //if(sou==PERM_USER&&PERM_WRITEABLE)
				 //{
					//pt_set_page_permissions(page_directory,dest_va,PERM_USER,0);


				 //}

			 }
			 else
			 {
				 struct FrameInfo*Ptr_Frame_Info=get_frame_info(page_directory,source_va,&ptr_page_table);
				get_frame_info(page_directory,source_va,&ptr_page_table);
				 map_frame(page_directory,Ptr_Frame_Info,i,PERM_WRITEABLE);

			 }



			*dest=*sour;
			source_va+=PAGE_SIZE;
			//dest_va+=PAGE_SIZE;
		}*/


}

//===============================
// 3) SHARE RANGE IN RAM:
//===============================
//This function should share the given size from dest_va with the source_va
//Hint: use ROUNDDOWN/ROUNDUP macros to align the addresses
int share_chunk(uint32* page_directory, uint32 source_va,uint32 dest_va, uint32 size, uint32 perms)
{
	//TODO: [PROJECT MS2] [CHUNK OPERATIONS] share_chunk
	// Write your code here, remove the panic and write your code
	//panic("share_chunk() is not implemented yet...!!");

	/*int i=ROUNDDOWN(dest_va,PAGE_SIZE);
		int x=ROUNDUP(dest_va+size,PAGE_SIZE);
		int y=ROUNDDOWN(dest_va,PAGE_SIZE);
		int z=ROUNDUP(dest_va+size,PAGE_SIZE);
		uint32 sourceva=ROUNDDOWN(source_va,PAGE_SIZE);
		uint32* ptr_page_table=NULL;
		struct FrameInfo* Ptr_Frame_Info;
		for (i;i<x;(i=i+PAGE_SIZE))
		{

			Ptr_Frame_Info= get_frame_info(page_directory,i,&ptr_page_table);
			if (Ptr_Frame_Info!=NULL){
			return -1;
			}
		}

	for (y;y<z;(y=y+PAGE_SIZE))
	{

		 get_page_table(page_directory,y,&ptr_page_table);

		 if(ptr_page_table!=NULL){
			 Ptr_Frame_Info= get_frame_info(page_directory,sourceva,&ptr_page_table);
			 map_frame(page_directory,Ptr_Frame_Info,y,perms);

			 }
			 else
			 {
				 create_page_table(page_directory,y);
				 Ptr_Frame_Info= get_frame_info(page_directory,sourceva,&ptr_page_table);
				 map_frame(page_directory,Ptr_Frame_Info,y,perms);
			 }
			 sourceva+=PAGE_SIZE;
		}

		return 0;*/

	int i=ROUNDDOWN(dest_va,PAGE_SIZE);
	int x=ROUNDUP(dest_va+size,PAGE_SIZE);
	int y=ROUNDDOWN(dest_va,PAGE_SIZE);
	int z=ROUNDUP(dest_va+size,PAGE_SIZE);
	uint32 sourceva=ROUNDDOWN(source_va,PAGE_SIZE);
	uint32* ptr_page_table=NULL;
	struct FrameInfo* Ptr_Frame_Info;
	for (i;i<x;(i=i+PAGE_SIZE))
	{

		Ptr_Frame_Info= get_frame_info(page_directory,i,&ptr_page_table);
		if (Ptr_Frame_Info!=NULL){
		return -1;
		}
	}

		for (y;y<z;(y=y+PAGE_SIZE)){

			 get_page_table(page_directory,y,&ptr_page_table);

			 if(ptr_page_table!=NULL){
				 Ptr_Frame_Info= get_frame_info(page_directory,sourceva,&ptr_page_table);
				int exist= map_frame(page_directory,Ptr_Frame_Info,y,perms);
				if (exist==E_NO_MEM){
					return -1;
				}

				 }
			 else
			 {
				 create_page_table(page_directory,y);
				 Ptr_Frame_Info= get_frame_info(page_directory,sourceva,&ptr_page_table);
				 map_frame(page_directory,Ptr_Frame_Info,y,perms);
			 }
			 sourceva=sourceva+PAGE_SIZE;
		}

		return 0;

}

//===============================
// 4) ALLOCATE CHUNK IN RAM:
//===============================
//This function should allocate in RAM the given range [va, va+size)
//Hint: use ROUNDDOWN/ROUNDUP macros to align the addresses
int allocate_chunk(uint32* page_directory, uint32 va, uint32 size, uint32 perms)
{
	//TODO: [PROJECT MS2] [CHUNK OPERATIONS] allocate_chunk
	// Write your code here, remove the panic and write your code
	//panic("allocate_chunk() is not implemented yet...!!");
	uint32 i=ROUNDDOWN(va,PAGE_SIZE);
		uint32 x=ROUNDUP(va+size,PAGE_SIZE);
		uint32* ptr_page_table=NULL;
		struct FrameInfo* Ptr_Frame_Info;
		for (i;i<x;(i=i+PAGE_SIZE))
		{
		Ptr_Frame_Info= get_frame_info(page_directory,i,&ptr_page_table);
			if (Ptr_Frame_Info!=NULL)
			{
				return -1;
			}
		}
			uint32 y=ROUNDDOWN(va,PAGE_SIZE);
			uint32 z=ROUNDUP(va+size,PAGE_SIZE);
			for (y;y<z;(y=y+PAGE_SIZE)){

				 get_page_table(page_directory,y,&ptr_page_table);
				 if(ptr_page_table!=NULL){
					 allocate_frame(&Ptr_Frame_Info);
					 map_frame(page_directory,Ptr_Frame_Info,y,perms);
					 Ptr_Frame_Info->va = y ;
					 }
				 else
				 {
					 create_page_table(page_directory,y);
					 allocate_frame(&Ptr_Frame_Info);
					 map_frame(page_directory,Ptr_Frame_Info,y,perms);
				 }
			}

	 return 0;
}

/*BONUS*/
//=====================================
// 5) CALCULATE ALLOCATED SPACE IN RAM:
//=====================================
void calculate_allocated_space(uint32* page_directory, uint32 sva, uint32 eva, uint32 *num_tables, uint32 *num_pages)
{
	//TODO: [PROJECT MS2 - BONUS] [CHUNK OPERATIONS] calculate_allocated_space
	// Write your code here, remove the panic and write your code
	//panic("calculate_allocated_space() is not implemented yet...!!");
	/*uint32 *ptr_page_table=NULL;
	struct FrameInfo* Ptr_Frame_Info=NULL;
	//int count_page=0;
	for (uint32 i=sva;i<eva;i+=(PAGE_SIZE*1024))
	{
		uint32 *ptr_page_table=NULL;
		uint32 ret=get_page_table(ptr_page_directory,i,&ptr_page_table);
		  if(ret==TABLE_IN_MEMORY)
		 {
			 num_tables++;
		 }
	}
	for (uint32 i=sva;i<eva;i+=PAGE_SIZE)
	{ //struct frames_info* Ptr_Frame_Info=NULL;
		struct FrameInfo* Ptr_Frame_Info=NULL;
	   Ptr_Frame_Info= get_frame_info(ptr_page_directory,i,&ptr_page_table);
		if(Ptr_Frame_Info!=NULL)
				 {
			 num_pages++;
				 }

		}*


	// uint32 S_va=ROUNDDOWN(sva,PAGE_SIZE);
	//uint32 E_va=ROUNDUP(eva,PAGE_SIZE);
   for(uint32 i=sva;i<eva+PAGE_SIZE;i=(PAGE_SIZE*1024)){
		uint32 * ptr_page_table = NULL;
		int rett= get_page_table(page_directory,i,&ptr_page_table);
		if(rett==TABLE_IN_MEMORY){
					*num_tables+=1;

		}
	}
	for(uint32 i=sva;i<eva;i+=PAGE_SIZE){
		uint32 * ptr_page_table = NULL;
		struct FrameInfo* ptr_frame_info=get_frame_info(page_directory,i,&ptr_page_table);
		if(ptr_frame_info!=NULL){
			*num_pages+=1;
		}
	}*/

    uint32 end_va_tables=eva+(5*1024*1024);
    uint32 end_va_pages=eva+PAGE_SIZE;
    uint32 increment=(PAGE_SIZE*1024);
    uint32 * page_table = NULL;
    struct FrameInfo* frame=NULL;
    uint32 *tables=0;
    uint32 *pages=0;
    for(uint32 i=sva;i<end_va_pages;i+=PAGE_SIZE)
    {
    	page_table = NULL;
        frame=NULL;
    	frame=get_frame_info(page_directory,i,&page_table);
		if(frame!=NULL){
			*num_pages+=1;
			//*num_pages=*pages;
		}
    }
   // *num_pages=*pages;
	for(uint32 i=sva;i<end_va_tables;i+=increment)
	{  page_table = NULL;
	    frame=NULL;
		 get_page_table(page_directory,i,&page_table);
			if(page_table!=NULL){
				*num_tables+=1;
			//*num_tables=*tables;
			}
	}


}

/*BONUS*/
//=====================================
// 6) CALCULATE REQUIRED FRAMES IN RAM:
//=====================================
// calculate_required_frames:
// calculates the new allocation size required for given address+size,
// we are not interested in knowing if pages or tables actually exist in memory or the page file,
// we are interested in knowing whether they are allocated or not.
uint32 calculate_required_frames(uint32* page_directory, uint32 sva, uint32 size)
{
	//TODO: [PROJECT MS2 - BONUS] [CHUNK OPERATIONS] calculate_required_frames
	// Write your code here, remove the panic and write your code
	//panic("calculate_required_frames() is not implemented yet...!!");
	uint32 new_size=ROUNDUP(sva+size,PAGE_SIZE);
	uint32 * page_table = NULL;
	struct FrameInfo* frame=NULL;
    uint32 *req_frame=0;
	for(int i=sva;i<new_size+PAGE_SIZE;i+PAGE_SIZE)
	{  page_table = NULL;
	   frame=NULL;
		frame=get_frame_info(page_directory,i,&page_table);
				if(frame!=NULL){
					*req_frame+=1;

				}
	}
	return *req_frame;
}

//=================================================================================//
//===========================END RAM CHUNKS MANIPULATION ==========================//
//=================================================================================//

/*******************************/
/*[2] USER CHUNKS MANIPULATION */
/*******************************/

//======================================================
/// functions used for USER HEAP (malloc, free, ...)
//======================================================

//=====================================
// 1) ALLOCATE USER MEMORY:
//=====================================
void allocate_user_mem(struct Env* e, uint32 virtual_address, uint32 size)
{
	// Write your code here, remove the panic and write your code
	panic("allocate_user_mem() is not implemented yet...!!");
}

//=====================================
// 2) FREE USER MEMORY:
//=====================================
void free_user_mem(struct Env* e, uint32 virtual_address, uint32 size)
{

	//TODO: [PROJECT MS3] [USER HEAP - KERNEL SIDE] free_user_mem
	// Write your code here, remove the panic and write your code
	//panic("free_user_mem() is not implemented yet...!!");
	uint32 *ptr_page_table=NULL;
	struct FrameInfo* ptr_frame_info = NULL;
	size =ROUNDUP(size,PAGE_SIZE);
	//uint32 i=ROUNDDOWN(virtual_address,PAGE_SIZE);
	uint32 y=ROUNDUP(virtual_address+size,PAGE_SIZE);
	uint32 new_size=ROUNDUP(size, PAGE_SIZE)/PAGE_SIZE;
	uint32 virtual_ws;
	 int check;
	 uint32 size_of_WS=e->page_WS_max_size;
	for(int incr=0; incr<new_size;incr+=PAGE_SIZE)
	{
		pf_remove_env_page(e,incr);

	}
	// Free pages resident in ws
	for(int index=0; index<size_of_WS; index++)
	{
		virtual_ws=env_page_ws_get_virtual_address(e,index);
		if((virtual_ws>=virtual_address) && (virtual_ws<(virtual_address+size)))
		{
			ptr_frame_info =get_frame_info(e->env_page_directory,virtual_ws,&ptr_page_table);
			if(ptr_frame_info != NULL)
			{
			unmap_frame(e->env_page_directory,virtual_ws);
			env_page_ws_clear_entry(e,index);
			}
		}
	}
	// remove empty table
	for(int i=0; i<new_size; i++)
	{
		 check=1;
		ptr_page_table=NULL;
		get_page_table(e->env_page_directory,virtual_address,&ptr_page_table);
		if(ptr_page_table !=NULL)
		{
			for(int j=0; j<1024; j++)
			{
				if(ptr_page_table[j] != 0) {
					check=0;
					break;
				}
			}
			if(check)
			{
				kfree((void*)ptr_page_table);
				pd_clear_page_dir_entry(e->env_page_directory,(uint32)virtual_address);
			}
		}
		virtual_address+=PAGE_SIZE;
	}
	//Refresh the cache memory
	tlbflush();

	//This function should:
	//1. Free ALL pages of the given range from the Page File
	//2. Free ONLY pages that are resident in the working set from the memory
	//3. Removes ONLY the empty page tables (i.e. not used) (no pages are mapped in the table)

}

//=====================================
// 2) FREE USER MEMORY (BUFFERING):
//=====================================
void __free_user_mem_with_buffering(struct Env* e, uint32 virtual_address, uint32 size)
{
	// your code is here, remove the panic and write your code
	panic("__free_user_mem_with_buffering() is not implemented yet...!!");

	//This function should:
	//1. Free ALL pages of the given range from the Page File
	//2. Free ONLY pages that are resident in the working set from the memory
	//3. Free any BUFFERED pages in the given range
	//4. Removes ONLY the empty page tables (i.e. not used) (no pages are mapped in the table)
}

//=====================================
// 3) MOVE USER MEMORY:
//=====================================
void move_user_mem(struct Env* e, uint32 src_virtual_address, uint32 dst_virtual_address, uint32 size)
{
	//TODO: [PROJECT MS3 - BONUS] [USER HEAP - KERNEL SIDE] move_user_mem
	//your code is here, remove the panic and write your code
	panic("move_user_mem() is not implemented yet...!!");

	// This function should move all pages from "src_virtual_address" to "dst_virtual_address"
	// with the given size
	// After finished, the src_virtual_address must no longer be accessed/exist in either page file
	// or main memory

	/**/
}

//=================================================================================//
//========================== END USER CHUNKS MANIPULATION =========================//
//=================================================================================//

