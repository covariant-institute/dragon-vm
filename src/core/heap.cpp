//
// Created by Mike Lee on 2017/8/29.
//

#define DVM_DEBUG
#include <core/debug.hpp>
#include <core/heap.hpp>
namespace dvm{
    namespace core{
        void* heap::malloc(const mem_block_info& mbi)
        {
            DVM_LOG("Malloc");
            if(!free_list.empty()) {
                free_list.sort(compare);
                auto it=free_list.begin();
                for(; it!=free_list.end();)
                    if(reinterpret_cast<mem_block_info*>(*it)->data_size>=mbi.data_size)
                        break;
                if(it!=free_list.end()) {
                    Byte* cur=*it;
                    reinterpret_cast<mem_block_info*>(cur)->type_id=mbi.type_id;
                    void* ptr= static_cast<void*>(cur+ sizeof(mem_block_info));
                    free_list.erase(it);
                    return ptr;
                }
            }
            else {
                memcpy(heap_pointer, (void *) &mbi, sizeof(mem_block_info));
                move_forward(sizeof(mem_block_info));
                void* ptr=static_cast<void*>(heap_pointer);
                move_forward(mbi.data_size);
                return ptr;
            }
        }
        void heap::free(void* ptr)
        {
            DVM_LOG("FREE");
            Byte * orig=static_cast<Byte *>(ptr)- sizeof(mem_block_info);
            memset(ptr,'\0',reinterpret_cast<mem_block_info*>(orig)->data_size);
            free_list.push_back(orig);
        }
        void heap::gc(const stack &s) {
            Byte* stack_begin= reinterpret_cast<Byte*>(s.allocated_start);
            Byte* stack_end=s.stack_pointer;
            // 如果栈为空则退出
            if(stack_begin==stack_end)
                return;
            Byte* heap_begin= reinterpret_cast<Byte*>(this->allocated_start);
            Byte* heap_end=this->heap_pointer;
            // 如果堆为空则退出
            if(heap_begin==heap_end)
                return;
            // 取消堆上的所有标记
            for(Byte* ptr=heap_begin;ptr<heap_end;)
            {
                mem_block_info* mbi= reinterpret_cast<mem_block_info*>(ptr);
                mbi->reachable=False;
                ptr+= sizeof(mem_block_info)+mbi->data_size;
            }
            // 遍历栈查找Link并进行标记
            for(Byte* ptr=stack_end;ptr>stack_begin;)
            {
                ptr-= sizeof(mem_block_info);
                mem_block_info* mbi= reinterpret_cast<mem_block_info*>(ptr);
                ptr-=mbi->data_size;
                if(mbi->type_id==type_identifier::TYPE_ID_LINK)
                    reinterpret_cast<mem_block_info*>(*reinterpret_cast<Link*>(ptr))->reachable=True;
            }
            // 标记已释放的内存块
            for(auto& ptr:free_list)
                reinterpret_cast<mem_block_info*>(ptr)->reachable=True;
            // 释放不可达的内存块
            for(Byte* ptr=heap_begin;ptr<heap_end;)
            {
                mem_block_info* mbi= reinterpret_cast<mem_block_info*>(ptr);
                ptr+= sizeof(mem_block_info);
                if(mbi->reachable==False)
                    this->free(reinterpret_cast<void*>(ptr));
                ptr+=mbi->data_size;
            }
        }
    }
}