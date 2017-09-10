#include <stdexcept>
#include <cstdint>
#include <cstdlib>
#include <utility>
#include <list>
namespace cov {
	using byte=uint8_t;
	using size_t=uint64_t;
	class heap final {
		// Heap Start
		void* hs=nullptr;
		// Heap Pointer
		byte* hp=nullptr;
		// Heap Limit
		byte* hl=nullptr;
		std::list<byte*> free_list;
		inline size_t& get_size(byte* ptr)
		{
			return *reinterpret_cast<size_t*>(ptr);
		}
		inline void sort_by_size()
		{
			free_list.sort([this](byte* lhs,byte* rhs) {
				return get_size(lhs)<get_size(rhs);
			});
		}
		inline void sort_by_addr()
		{
			free_list.sort([this](byte* lhs,byte* rhs) {
				return lhs<rhs;
			});
		}
		void compress()
		{
			// Sort the spaces by address.
			sort_by_addr();
			std::list<byte*> new_list;
			byte* ptr=nullptr;
			// Compress the free list.
			for(auto p:free_list) {
				if(ptr!=nullptr) {
					size_t& size=get_size(ptr);
					if(ptr+size+sizeof(size_t)==p) {
						size+=get_size(p)+sizeof(size_t);
					}
					else {
						new_list.push_back(ptr);
						ptr=p;
					}
				}
				else
					ptr=p;
			}
			// Connect the final space and remain spaces.
			get_size(ptr)+=hl-hp;
			hp=hl;
			new_list.push_back(ptr);
			// Swap the new list and old list.
			std::swap(new_list,free_list);
		}
		void* find_in_free_list(size_t size)
		{
			if(!free_list.empty()) {
				// Sort the spaces by size.
				sort_by_size();
				// Find the first fit space.
				auto it=free_list.begin();
				for(; it!=free_list.end(); ++it)
					if(get_size(*it)>=size)
						break;
				if(it!=free_list.end()) {
					// Remove from free list.
					void* ptr=reinterpret_cast<void*>(*it+sizeof(size_t));
					free_list.erase(it);
					return ptr;
				}
			}
			return nullptr;
		}
	public:
		heap()=delete;
		heap(const heap&)=delete;
		explicit heap(size_t size):hs(::malloc(size))
		{
			hp=reinterpret_cast<byte*>(hs);
			hl=hp+size;
		}
		~heap()
		{
			::free(hs);
		}
		void* malloc(size_t size)
		{
			// Try to find usable spaces in free list
			void* ptr=find_in_free_list(size);
			// If found,return.
			if(ptr!=nullptr)
				return ptr;
			// Checkout remain spaces,if enough,return.
			if(hl-hp>=size+sizeof(size_t)) {
				get_size(hp)=size;
				ptr=reinterpret_cast<void*>(hp+sizeof(size_t));
				hp+=size+sizeof(size_t);
				return ptr;
			}
			// Compress the memory spaces
			compress();
			// Try to find usable spaces in free list again.
			ptr=find_in_free_list(size);
			// If found,return.
			if(ptr!=nullptr)
				return ptr;
			else // There have no usable spaces,throw bad alloc exception.
				throw std::runtime_error("Bad alloc.");
		}
		void free(void* ptr)
		{
			free_list.push_back(reinterpret_cast<byte*>(ptr)-sizeof(size_t));
		}
	};
	template<typename T,typename...ArgsT>
	void construct(T* ptr,ArgsT&&...args)
	{
		if(ptr!=nullptr)
			::new(ptr) T(std::forward<ArgsT>(args)...);
	}
	template<typename T>void destroy(T* ptr)
	{
		if(ptr!=nullptr)
			ptr->~T();
	}
}
