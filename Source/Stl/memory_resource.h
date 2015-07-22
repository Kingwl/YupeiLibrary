#pragma once

#include <cstddef>
#include <new>
#include <cassert>
#include "math.h"
#include "Auto.h"

namespace Yupei
{
	inline namespace Experimental
	{
		namespace Pmr
		{
			class memory_resource {
			protected:
				// For exposition only
				static constexpr size_t max_align = alignof(std::max_align_t);

			public:
				virtual ~memory_resource() {}

				void* allocate(size_t bytes, size_t alignment = max_align)
				{
					return do_allocate(bytes, alignment);
				}
				void deallocate(void* p, size_t bytes,
					size_t alignment = max_align)
				{
					return do_deallocate(p, bytes, alignment);
				}

				bool is_equal(const memory_resource& other) const noexcept
				{
					return do_is_equal(other);
				}

			protected:
				virtual void* do_allocate(size_t bytes, size_t alignment) = 0;
				virtual void do_deallocate(void* p, size_t bytes,
					size_t alignment) = 0;

			//Returns:

			//	A derived class shall implement this function to return true if memory allocated from this can be deallocated from other and vice - versa; 
			//	otherwise it shall return false.[Note:The most - derived type of other might not match the type of this.
			//	For a derived class, D, a typical implementation of this function will compute dynamic_cast<const D*>(&other) 
			//	and go no further(i.e., return false) if it returns nullptr.¡ª end note]
				virtual bool do_is_equal(const memory_resource& other) const noexcept = 0;
			};

			bool inline operator==(const memory_resource& a,
				const memory_resource& b) noexcept
			{
				return &a == &b || a.is_equal(b);
			}
			bool inline operator!=(const memory_resource& a,
				const memory_resource& b) noexcept
			{
				return !(a == b);
			}

			namespace Internal
			{

				struct new_delete_resource : memory_resource
				{
				protected:
					void* do_allocate(size_t bytes, size_t alignment) override
					{
						bytes = (bytes + alignment - 1) & ~( alignment - 1);
						return ::operator new(bytes);
					}
					void do_deallocate(void* p, size_t bytes,
						size_t alignment) override
					{
						(void)bytes;
						(void)alignment;
						::operator delete(p);
					}

					bool do_is_equal(const memory_resource& other) const noexcept override
					{
						return this == &other;
					}
				};

				struct null_memory_resource : memory_resource
				{
					void* do_allocate(size_t bytes, size_t alignment) override
					{
						throw std::bad_alloc();
					}
					void do_deallocate(void* p, size_t bytes,
						size_t alignment) override
					{
						//no-op
					}
					bool do_is_equal(const memory_resource& other) const noexcept override
					{
						return this == &other;
					}
				};
			}

			//// The name resource_adaptor_imp is for exposition only.
			//template <class Allocator> class resource_adaptor_imp;

			//template <class Allocator>
			//using resource_adaptor = resource_adaptor_imp<
			//	allocator_traits<Allocator>::rebind_alloc<char >> ;

			// Global memory resources

			//Returns:
			/*A pointer to a static - duration object of a type derived from memory_resource that 
			can serve as a resource for allocating memory using ::operator new and ::operator delete.The same value is 
			returned every time this function is called.For return value p and memory resource r, p->is_equal(r) 
			returns &r == p.*/
			extern memory_resource* new_delete_resource() noexcept;

			/*Returns:
			A pointer to a static - duration object of a type derived from memory_resource for which allocate()
			always throws bad_alloc and for which deallocate() has no effect.The same value is returned
			every time this function is called.For return value p and memory resource r, p->is_equal(r)
			returns &r == p.*/
			extern memory_resource* null_memory_resource() noexcept;

			// The default memory resource
			extern memory_resource* set_default_resource(memory_resource* r) noexcept;
			extern memory_resource* get_default_resource() noexcept;

			
			struct BlockPtr
			{
			public:
				using size_type = std::size_t;
			private:
				//Blocks consist of :
				//1.the content
				//2.the next block

				/*
				
				the blocks seem like this:												
				  |-------------|
				  | totalSize	|	sizeof(size_type)(the size of content)			
				  |-------------|  									
				  |availableSize|	sizeof(size_type)
				  |-------------|													
				  | next_ptr	|	lcm(sizeof(void*),sizeof(size_type))											---
				  |-------------|
				  |				|
				  | content		|
				  |				|
				  |-------------|
				*/

				char* ptr;

			public:
				BlockPtr(char* _ptr)
					:ptr(_ptr)
				{

				}

				BlockPtr()
					:BlockPtr(nullptr)
				{

				}
				char* begin() const noexcept
				{
					return ptr + (sizeof(size_type) << 1)
						+ StaticGetLcm(sizeof(size_type), sizeof(void*));
				}
				char* end() const noexcept
				{
					return begin() + ContentSize();
				}
				size_type* GetContentSizePtr() const noexcept
				{
					return reinterpret_cast<size_type*>(ptr);
				}
				size_type& ContentSize() const noexcept
				{
					return *GetContentSizePtr();
				}
				size_type* GetAvaliableSizePtr() const noexcept
				{
					return GetContentSizePtr() + 1;
				}
				size_type& AvaliableSize() const noexcept
				{
					return *GetAvaliableSizePtr();
				}
				char*& NextBlockPtr() const noexcept
				{
					return *reinterpret_cast<char**>(ptr + (sizeof(size_type) << 1));
				}
				BlockPtr GetNextBlock() const noexcept
				{
					return BlockPtr(NextBlockPtr());
				}
				char* allocate(size_type sz) const noexcept
				{
					assert(sz <= AvaliableSize());
					auto res = begin() + ContentSize() - AvaliableSize();
					AvaliableSize() -= sz;
					return res;
				}
				void deallocate(size_type sz) const noexcept
				{
					assert(sz <= ContentSize() - AvaliableSize());
					AvaliableSize() += sz;
				}
				bool empty() const noexcept
				{
					return ptr == nullptr;
				}
				bool HasAvailableSize() const noexcept
				{
					return AvaliableSize() == 0;
				}
				void* Head() const noexcept
				{
					return static_cast<void*>(ptr);
				}
				//Warning!!The total size is content+overhead
				void SetArgs(
					size_type totalsz,
					char* _next = nullptr) noexcept
				{
					ContentSize() = AvaliableSize() = totalsz - GetOverheadSize();
					NextBlockPtr() = _next;
				}
				bool IsContain(void* p) const noexcept
				{
					auto cp = static_cast<char*>(p);
					return (cp >= begin() && cp < end());
				}
				bool HasNext() const noexcept
				{
					return ptr != nullptr && NextBlockPtr() != nullptr;
				}
				static size_type GetSizeToAllocate(size_type sz) noexcept
				{
					return sz + GetOverheadSize();
				}
				static size_type GetOverheadSize() noexcept
				{
					return (sizeof(size_type) << 1)
						+ StaticGetLcm(sizeof(size_type), sizeof(void*));
				}
				size_type GetChunkTotalSize() const noexcept
				{
					return ContentSize() + GetOverheadSize();
				}
			};

			constexpr inline size_t RoundUp(size_t sz,
				size_t _alig) noexcept
			{
				return (sz + _alig - 1) & ~(_alig - 1);
			}


			class monotonic_buffer_resource : public memory_resource 
			{
				memory_resource* upstream_rsrc; // exposition only
				void* current_buffer; // exposition only
				size_t next_buffer_size; // exposition only
				static constexpr size_t LeastStartSize = 256;//0.25KB
				static constexpr size_t MaxBlockSize = 1024 * 1024;//1MB
				void CorrectInitialSize() noexcept
				{
					next_buffer_size = RoundUp(next_buffer_size,LeastStartSize);
				}
				void ZeroAll() noexcept
				{
					upstream_rsrc = nullptr;
					next_buffer_size = 0;
				}
			public:
				explicit monotonic_buffer_resource(memory_resource* upstream)
					:monotonic_buffer_resource(LeastStartSize,upstream)
				{

				}
				monotonic_buffer_resource(size_t initial_size,
					memory_resource* upstream)
					:upstream_rsrc(upstream),
					next_buffer_size(initial_size),
					current_buffer(nullptr)
				{
					CorrectInitialSize();
				}
				monotonic_buffer_resource(void* buffer, size_t buffer_size,
					memory_resource* upstream)
					:upstream_rsrc(upstream),
					next_buffer_size(buffer_size << 1)
				{
					if (next_buffer_size < BlockPtr::GetOverheadSize())
					{
						//Screw you guys,I'm going home.
						upstream->deallocate(buffer,buffer_size);
						current_buffer = nullptr;
						next_buffer_size = RoundUp(1,LeastStartSize);
					}
					else
					{
						BlockPtr bptr{ static_cast<char*>(buffer) };
						bptr.SetArgs(buffer_size);
						current_buffer = buffer;
						CorrectInitialSize();
					}
				}

				monotonic_buffer_resource()
					: monotonic_buffer_resource(get_default_resource()) 
				{ 

				}
				explicit monotonic_buffer_resource(size_t initial_size)
					: monotonic_buffer_resource(initial_size,
						get_default_resource()) 
				{ 

				}
				monotonic_buffer_resource(void* buffer, size_t buffer_size)
					: monotonic_buffer_resource(buffer, buffer_size,
						get_default_resource()) 
				{ 

				}

				monotonic_buffer_resource(const monotonic_buffer_resource&) = delete;

				virtual ~monotonic_buffer_resource()
				{
					release();
				}

				monotonic_buffer_resource operator=(
					const monotonic_buffer_resource&) = delete;

				void release()
				{
					BlockPtr ptr = GetCurrentBufferBlockPtr();
					while (!ptr.empty())
					{
						auto temp = ptr.GetNextBlock();
						upstream_rsrc->deallocate(ptr.Head(), ptr.GetChunkTotalSize());
						ptr = temp;
					}
					
				}
				memory_resource* upstream_resource() const
				{
					return upstream_rsrc;
				}

			protected:
				virtual void* do_allocate(size_t bytes, size_t alignment) override
				{
					if (bytes > MaxBlockSize)
					{
						return upstream_rsrc->allocate(bytes, alignment);
					}
					auto realBytes = RoundUp(bytes, alignment);
					BlockPtr cur = GetCurrentBufferBlockPtr();
					if (cur.empty() || cur.AvaliableSize() < realBytes)
					{
						Auto(UpdateNextSize());
						AddNewBlock(AllocateNewBlock(realBytes));
						return GetCurrentBufferBlockPtr().allocate(realBytes);
					}
					else
					{
						return cur.allocate(realBytes);
					}
					
				}
				virtual void do_deallocate(void* p, size_t bytes,
					size_t alignment)
				{
					//Standard tells us no-op?
					if (bytes > MaxBlockSize)
					{
						return upstream_rsrc->deallocate(p,bytes, alignment);
					}
					else
					{
						//no-op
					}
				}

				virtual bool do_is_equal(const memory_resource& other) const noexcept
				{
					return this == dynamic_cast<const monotonic_buffer_resource*>(&other);
				}
				private:
					void UpdateNextSize() noexcept
					{
						if (next_buffer_size < MaxBlockSize)
						{
							next_buffer_size <<= 1;
						}
					}
					BlockPtr GetCurrentBufferBlockPtr() const noexcept
					{
						return BlockPtr{ static_cast<char*>(current_buffer) };
					}
					void AddNewBlock(void* newBlock) noexcept
					{
						auto head = GetCurrentBufferBlockPtr();
						BlockPtr ptr{ static_cast<char*>(newBlock) };
						ptr.NextBlockPtr() = static_cast<char*>(head.Head());
						current_buffer = ptr.Head();
					}
					void* AllocateNewBlock(size_t origSz) const
					{
						origSz = origSz > next_buffer_size ? origSz : next_buffer_size;
						auto ptr = BlockPtr{ static_cast<char*>(
							upstream_rsrc->allocate(
								BlockPtr::GetSizeToAllocate(origSz))) };
						ptr.ContentSize() = ptr.AvaliableSize() = origSz;
						ptr.NextBlockPtr() = nullptr;
						return ptr.Head();
					}
			};

			class FreeList
			{
			public:
				using size_type = std::size_t;
				static constexpr size_type MaxChunksCount = 64;
				FreeList(memory_resource* _upstream,
					size_type _chunkSize,
					size_type _maxChunkCount)
					:upstream(_upstream),
					chunkSize(_chunkSize),
					maxChunksCount(_maxChunkCount)
				{
					CorrectArguments();
				}
				FreeList(size_type _chunkSize,
					size_type _maxChunkCount)
					:FreeList(new_delete_resource(),
						_chunkSize,_maxChunkCount)
				{

				}

				//pre-condition:chunksList == nullptr
				void* ReFillChunks()
				{
					assert(chunksList == nullptr);
					auto totalRequestSize = GetRquiredSize();
					auto rawMemory = static_cast<char*>(upstream->allocate(totalRequestSize));
					//the last chunk needs to be return to client
					for (size_type i = 0;i < requestChunksCount - 1;++i)
					{
						AddToList(rawMemory);
						rawMemory += chunkSize;
					}
					UpdateRequestSize();
					return static_cast<void*>(rawMemory);
				}

				size_type GetMaxChunkCount() const noexcept
				{
					return maxChunksCount;
				}

				void* allocate()
				{
					if (chunksList == nullptr)
					{
						//no chunks left
						return ReFillChunks();
					}
					else
					{
						void* res = static_cast<void*>(chunksList);
						chunksList = chunksList->next_chunk;
						return res;
					}
				}

				void deallocate(void* _chunk)
				{
					AddToList(static_cast<char*>(_chunk));
				}
				~FreeList()
				{
					//Er...
					auto ptr = reinterpret_cast<chunk_block*>(chunksList);
					chunk_block* temp;
					while (ptr != nullptr)
					{
						temp = ptr->next_chunk;
						upstream->deallocate(ptr,chunkSize);
						ptr = temp;
					}
				}
			private:
				void UpdateRequestSize() noexcept
				{
					if(requestChunksCount < maxChunksCount)
						requestChunksCount <<= 1;
				}
				size_type GetRquiredSize() const noexcept
				{
					return requestChunksCount * chunkSize;
				}
				void AddToList(char* _mem)
				{
					auto _chunk = reinterpret_cast<chunk_block*>(_mem);
					_chunk->next_chunk = chunksList;
					chunksList = _chunk;
				}
				size_type GetTotalSize(size_type _chunksCount) const noexcept
				{
					return _chunksCount * chunkSize;
				}
				void CorrectArguments() noexcept
				{
					assert(chunkSize != 0);
					if (maxChunksCount == 0)
					{
						maxChunksCount = MaxChunksCount;
					}
					else
					{
						maxChunksCount = RoundUp(maxChunksCount, 2);
					}
				}
				
				union chunk_block
				{
					chunk_block* next_chunk;
					char real_content[1];
				};

				memory_resource* upstream;
				chunk_block* chunksList = nullptr;
				size_type requestChunksCount = 2;
				size_type maxChunksCount;
				const size_type chunkSize; //my size
			};



			// Standard memory resources
			struct pool_options 
			{
				/*The maximum number of blocks that will be allocated at once 
				from the upstream memory resource to replenish a pool. If the value of 
				max_blocks_per_chunk is zero or is greater than an implementation-defined limit, 
				that limit is used instead. The implementation may choose to use a smaller value than
				is specified in this field and may use different values for different pools. */
				size_t max_blocks_per_chunk = 0;
				/*The largest allocation size that is required to be fulfilled using 
				the pooling mechanism. Attempts to allocate a single block larger than this
				threshold will be allocated directly from the upstream memory resource. 
				If largest_required_pool_block is zero or is greater than an implementation-defined 
				limit, that limit is used instead. The implementation may choose a pass-through 
				threshold larger than specified in this field. */
				size_t largest_required_pool_block = 0;
			};

			/*class synchronized_pool_resource : public memory_resource 
			{
			public:
				synchronized_pool_resource(const pool_options& opts, memory_resource* upstream);

				synchronized_pool_resource()
					: synchronized_pool_resource(pool_options(), get_default_resource()) 
				{ 
				}
				explicit synchronized_pool_resource(memory_resource* upstream)
					: synchronized_pool_resource(pool_options(), upstream)
				{ 
				}
				explicit synchronized_pool_resource(const pool_options& opts)
					: synchronized_pool_resource(opts, get_default_resource()) { }

				synchronized_pool_resource(
					const synchronized_pool_resource&) = delete;
				virtual ~synchronized_pool_resource();

				synchronized_pool_resource& operator=(
					const synchronized_pool_resource&) = delete;

				void release();
				memory_resource* upstream_resource() const;
				pool_options options() const;

			protected:
				virtual void* do_allocate(size_t bytes, size_t alignment);
				virtual void do_deallocate(void* p, size_t bytes, size_t alignment);

				virtual bool do_is_equal(const memory_resource& other) const noexcept;

			private:
				static constexpr std::size_t defaultChunkPerBlock = 64;
				static constexpr std::size_t largestRequiredPoolBlock = 4096;
				memory_resource* upStream;
			};*/

			class unsynchronized_pool_resource : public memory_resource 
			{
			public:
				unsynchronized_pool_resource(const pool_options& opts, memory_resource* upstream)
					:memory_resource(),
					upstream_src(upstream),
					freeLists(nullptr),
					freeListSource(new monotonic_buffer_resource())
				{
					size_t freeListCount, largestPool = opts.largest_required_pool_block;
					//correct the largest_required_pool_block
					if (largestPool == 0)
					{
						largestPool = 128;//0.5KB
					}
					freeListCount = largestPool / 8;


					freeLists = static_cast<FreeList*>(
						::operator new(sizeof(FreeList) * freeListCount));
					
					auto ptr = freeLists;
					size_t chunkSize = 0;
					for (size_t i = 0;i < freeListCount;++i)
					{
						chunkSize += 8;
						new (ptr) FreeList(freeListSource, chunkSize, opts.max_blocks_per_chunk);
						++ptr;
					}
					poolOptions.largest_required_pool_block = largestPool;
					poolOptions.max_blocks_per_chunk = ptr[0].GetMaxChunkCount();
				}

				unsynchronized_pool_resource()
					: unsynchronized_pool_resource(pool_options(), get_default_resource()) 
				{ 
				}
				explicit unsynchronized_pool_resource(memory_resource* upstream)
					: unsynchronized_pool_resource(pool_options(), upstream)
				{ 
				}
				explicit unsynchronized_pool_resource(const pool_options& opts)
					: unsynchronized_pool_resource(opts, get_default_resource()) 
				{
				}

				unsynchronized_pool_resource(
					const unsynchronized_pool_resource&) = delete;
				virtual ~unsynchronized_pool_resource()
				{
					release();
				}

				unsynchronized_pool_resource& operator=(
					const unsynchronized_pool_resource&) = delete;

				void release()
				{
					size_t count = poolOptions.largest_required_pool_block / 8;
					for (size_t i = 0;i < count;++i)
					{
						freeLists[i].~FreeList();
					}
					::operator delete(freeLists);
					delete freeListSource;
				}
				memory_resource* upstream_resource() const
				{
					return upstream_src;
				}
				pool_options options() const
				{
					return poolOptions;
				}

			protected:
				virtual void* do_allocate(size_t bytes, size_t alignment)
				{
					//ignore the alignment
					//because 
					//1.if alignment < 8,I'll give him a chunk which is aligned to 8Bytes
					//2.we(Visual C++) 's max_align is 8.
					if (bytes > poolOptions.largest_required_pool_block)
					{
						return upstream_src->allocate(bytes, alignment);
					}
					(void)alignment;
					auto index = GetFreeListIndex(bytes);
					return freeLists[index].allocate();
				}
				virtual void do_deallocate(void* p, size_t bytes, size_t alignment)
				{
					if (bytes > poolOptions.largest_required_pool_block)
					{
						return upstream_src->deallocate(p,bytes, alignment);
					}
					(void)alignment;
					auto index = GetFreeListIndex(bytes);
					freeLists[index].deallocate(p);
				}

				virtual bool do_is_equal(const memory_resource& other) const noexcept
				{
					return this == dynamic_cast<const unsynchronized_pool_resource*>(&other);
				}

			private:
				FreeList* freeLists;
				memory_resource* upstream_src;
				pool_options poolOptions;
				monotonic_buffer_resource* freeListSource;

				static size_t GetFreeListIndex(size_t sz) noexcept
				{
					return (sz + memory_resource::max_align - 1) / memory_resource::max_align - 1;
				}
			};
			template <class Tp>
			class polymorphic_allocator {
				memory_resource* m_resource; // For exposition only

			public:
				typedef Tp value_type;

				polymorphic_allocator() noexcept
					:m_resource(get_default_resource())
				{

				}
				polymorphic_allocator(memory_resource* r)
					:m_resource(r)
				{

				}

				polymorphic_allocator(const polymorphic_allocator& other) = default;
				

				template <class U>
				polymorphic_allocator(const polymorphic_allocator<U>& other) noexcept	
					:m_resource(other.resource())
				{

				}

				polymorphic_allocator&
					operator=(const polymorphic_allocator& rhs) = default;

				Tp* allocate(size_t n)
				{
					return static_cast<Tp*>(m_resource->allocate(n * sizeof(Tp), alignof(Tp)));
				}
				void deallocate(Tp* p, size_t n)
				{
					return m_resource->deallocate(p, n * sizeof(Tp), alignof(Tp));
				}

				//minimal allocator support

				/*template <class T, class... Args>
				void construct(T* p, Args&&... args);*/

				// Specializations for pair using piecewise construction
				/*template <class T1, class T2, class... Args1, class... Args2>
				void construct(pair<T1, T2>* p, piecewise_construct_t,
					tuple<Args1...> x, tuple<Args2...> y);
				template <class T1, class T2>
				void construct(pair<T1, T2>* p);
				template <class T1, class T2, class U, class V>
				void construct(pair<T1, T2>* p, U&& x, V&& y);
				template <class T1, class T2, class U, class V>
				void construct(pair<T1, T2>* p, const std::pair<U, V>& pr);
				template <class T1, class T2, class U, class V>
				void construct(pair<T1, T2>* p, pair<U, V>&& pr);*/

				//minimal allocator support
				/*template <class T>
				void destroy(T* p);*/

				// Return a default-constructed allocator (no allocator propagation)
				polymorphic_allocator select_on_container_copy_construction() const
				{
					return polymorphic_allocator();
				}

				memory_resource* resource() const
				{
					return m_resource;
				}
			};


			template <class T1, class T2>
			bool operator==(const polymorphic_allocator<T1>& a,
				const polymorphic_allocator<T2>& b) noexcept
			{
				return *a.resource() == *b.resource();
			}

			template <class T1, class T2>
			bool operator!=(const polymorphic_allocator<T1>& a,
				const polymorphic_allocator<T2>& b) noexcept
			{
				return !(a == b);
			}

		}
	}
}
