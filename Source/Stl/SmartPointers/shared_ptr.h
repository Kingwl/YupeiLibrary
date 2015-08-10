#pragma once

#include "..\type_traits.h"
#include "..\utility_internal.h"
#include "..\__swap.h"
#include "..\compress_pair.h"
#include "default_delete.h"
#include <exception> //for std::exception
#include <cassert>


namespace Yupei
{
	class bad_weak_ptr
		:public std::exception
	{
	public:
		bad_weak_ptr() noexcept {}

		virtual const char* what() const override
		{
			return "bad_weak_ptr";
		}
	};

	//This is an unsynchronized of std::shared_ptr called unsynchronized_ptr.
	//IncRefPolicy should have following member functions:
	//void IncreaseRefCount(std::ptrdiff_t&)
	//bool DecreaseRefCount(std::ptrdiff_t& ref_count) (after the decreasing,if ref_count == 0,return true)
	//bool TryIncreaseRefCount(std::ptrdiff_t& ref_count)
	//Before we write the real ptr,we need some contracts on IncRefPolicy.
	//Maybe we can't validate the signature because of the template functions.
	
	template<typename Policy,typename = void>
	struct IncRefPolicyContracts : false_type
	{
		
	};

	template<typename Policy>
	struct IncRefPolicyContracts<Policy,
		void_t<decltype(&Policy::IncreaseRefCount),
		decltype(&Policy::DecreaseRefCount),
		decltype(&Policy::TryIncreaseRefCount)>> : true_type
	{

	};

	template<typename IncRefPolicy>
	struct RefCount
	{
	public:
		//static_assert(IncRefPolicyContracts<IncRefPolicy>::value, "The policy doesn't meet the require.");
		RefCount()
			:weakCount(1),
			refCount(1)
		{

		}
		void IncRefCount()
		{
			IncRefPolicy().IncreaseCount(refCount);
		}
		bool DecRefCount()
		{
			return IncRefPolicy().DecreaseRefCount(refCount);
		}
		void IncWeakCount()
		{
			IncRefPolicy().IncreaseCount(weakCount);
		}
		bool DecWeakCount()
		{
			if (IncRefPolicy().DecreaseRefCount(weakCount))
			{
				delete this;
				return true;
			}
			return false;
		}
		std::ptrdiff_t GetWeakCount() const noexcept
		{
			return weakCount;
		}
		std::ptrdiff_t GetRefCount() const noexcept
		{
			return refCount;
		}
	private:
		std::ptrdiff_t weakCount;
		std::ptrdiff_t refCount;
	};
	//IncRefPolicy should have following member functions:
	//IncreaseRefCount
	//DecreaseRefCount
	//TryIncreaseRefCount
	
	struct UnsynchronizedPolicy
	{
		constexpr UnsynchronizedPolicy() noexcept = default;
		void IncreaseRefCount(std::ptrdiff_t& num)
		{
			++num;
		}
		bool DecreaseRefCount(std::ptrdiff_t& num)
		{
			if (--num == 0) return true;
			return false;
		}
		bool TryIncreaseRefCount(std::ptrdiff_t& num)
		{
			if (num == 0) return false;
			else ++num;
			return true;
		}
	};

	template<typename Type,
		typename IncRefCountPolicy,
		typename Deleter
		>
	class PtrBase
	{
	public:
		using element_type = remove_extent_t<Type>;
		using ref_count_type = RefCount<IncRefCountPolicy>;
		constexpr PtrBase() noexcept
			:refCountObj(),
			deleterWithPtr()
		{

		}
		template<typename Y>
		explicit PtrBase(Y* p)
			:deleterWithPtr(Yupei::compress_value_initialize_first_arg,p)
		{
			refCountObj = new ref_count_type();
		}
		PtrBase(PtrBase&& r) noexcept
			: deleterWithPtr{ Yupei::compress_value_initialize_both_args,
			Yupei::move(r.GetDeleter()),
			r.GetRawPtr() },
			refCountObj(r.refCountObj)
		{
			r.refCountObj = nullptr;
			r.GetRawPtr() = nullptr;
		}
		template<typename Y,
			typename D>
		PtrBase(Y* p,D&& d)
			:deleteWithPtr{
				Yupei::compress_value_initialize_both_args,
				Yupei::forward<D>(d),
				p }
		{
			refCountObj = new ref_count_type();
		}
		std::size_t use_count() const noexcept
		{
			return static_cast<std::size_t>(refCountObj->GetRefCount());
		}
		void swap(PtrBase& rhs)
		{
			Yupei::swap(refCountObj, rhs.refCountObj);
			Yupei::swap(deleterWithPtr, rhs.deleterWithPtr);
		}
	protected:
		/*template<typename Policy>
		void ResetResources(element_type* _ptr, RefCount<IncRefCountPolicy>& _refCount)
		{
			refCount.DecRefCount();
		}*/
		void Reset(element_type* p, ref_count_type* _ref_count)
		{
			if (this->refCountObj != nullptr)
			{
				refCountObj->DecRefCount();
			}
			this->refCountObj = _ref_count;
			this->GetRawPtr() = p;
			_ref_count->IncRefCount();
		}
		void IncRef()
		{
			if (refCountObj)
			{
				refCountObj->IncRefCount();
			}
		}
		void DecRef()
		{
			if (refCountObj)
			{
				if (refCountObj->DecRefCount())
				{
					this->GetDeleter()(this->GetRawPtr());
					this->GetRawPtr() = nullptr;
					refCountObj->DecWeakCount();
				}
			}
		}
		std::size_t UseCount() const noexcept
		{
			if (refCountObj)
			{
				return refCountObj->GetRefCount();
			}
			return{};
		}
		Deleter& GetDeleter() noexcept
		{
			return deleterWithPtr.first();
		}
		const Deleter& GetDeleter() const noexcept
		{
			return deleterWithPtr.first();
		}
		element_type*& GetRawPtr() noexcept
		{
			return deleterWithPtr.second();
		}
		element_type* GetRawPtr() const noexcept
		{
			return deleterWithPtr.second();
		}
		void AssginRv(PtrBase&& r) noexcept
		{
			this->GetRawPtr() = r.GetRawPtr();
			r.GetRawPtr() = nullptr;
			this->refCountObj = r.refCountObj;
			r.refCountObj = nullptr;
			this->GetDeleter() = Yupei::move(r.GetDeleter());
		}
		
	protected:
		RefCount<IncRefCountPolicy>* refCountObj;
		compress_pair<Deleter, element_type*> deleterWithPtr;
	};

	template<typename Type,
		typename IncRefCountPolicy,
		typename Deleter = default_delete<Type>
	>
	class shared_ptr : public PtrBase<Type, IncRefCountPolicy,Deleter>
	{
	public:
		template<typename T,typename Inc,typename D>
		friend class shared_ptr;
		using base_type = PtrBase<Type, Deleter, IncRefCountPolicy>;
		using element_type = typename base_type::element_type;
		using inc_policy = IncRefCountPolicy;
		using this_ptr_type = shared_ptr<Type, IncRefCountPolicy, Deleter>;

		template<typename Type,typename Inc,typename D>
		friend class shared_ptr;

		constexpr shared_ptr() noexcept = default;

		template<typename Y,
		typename = enable_if_t<
		is_convertible<Y*,element_type*>::value>>
		explicit shared_ptr(Y* p)
			:PtrBase(p)
		{
		}
		template<typename Y,
			typename D,
		typename = enable_if_t<is_convertible<Y*,element_type*>::value
		&& is_convertible<D,Deleter>::value>>
			shared_ptr(Y* p, D&& d)
			:PtrBase{ p,Yupei::forward<D>(d) }
		{
			
		}
		template<typename Y,
		typename D,
		typename = enable_if_t<is_convertible<D,Deleter>::value
		&& is_convertible<Y*,element_type*>::value>>
			shared_ptr(const shared_ptr<Y,inc_policy, D>& r, element_type* p)
			:PtrBase{p,r.GetDeleter()}
		{
			this->Reset(p, r.refCount);
		}

		shared_ptr(const shared_ptr& r) noexcept
			:PtrBase{r.GetRawPtr(),r.GetDeleter()}
		{
			this->Reset(r.GetRawPtr(), r.refCount);
		}
		template<typename Y,
			typename D,
			typename = enable_if_t<is_convertible<D, Deleter>::value
			&& is_convertible<Y*, element_type*>::value >>
			shared_ptr(const shared_ptr<Y, inc_policy, D>& r) noexcept
			:PtrBase{ r.GetRawPtr(),r.GetDeleter() }
		{
			this->Reset(r.GetRawPtr(), r.refCount);
		}
		shared_ptr(shared_ptr&& r) noexcept
			:base_type(Yupei::move(r))
		{
			
		}
		template<typename Y,
			typename D,
			typename = enable_if_t<is_convertible<D, Deleter>::value
			&& is_convertible<Y*, element_type*>::value >>
			shared_ptr(shared_ptr<Y, inc_policy, D>&& r) noexcept
			:base_type(Yupei::move(r))
		{
			
		}
		constexpr shared_ptr(nullptr_t) noexcept
			:shared_ptr()
		{

		}
		shared_ptr& operator=(const shared_ptr& r) noexcept
		{
			shared_ptr(r).swap(*this);
			return *this;
		}
		template<typename Y,
			typename D,
			typename = enable_if_t<is_convertible<D, Deleter>::value
			&& is_convertible<Y*, element_type*>::value >>
			shared_ptr& operator=(const shared_ptr<Y, inc_policy, D>& rhs)
		{
			shared_ptr(rhs).swap(*this);
			return *this;
		}
		template<typename Y,
			typename D,
			typename = enable_if_t<is_convertible<D, Deleter>::value
			&& is_convertible<Y*, element_type*>::value >>
		shared_ptr& operator=(shared_ptr<Y, inc_policy, D>&& r) noexcept
		{
			shared_ptr(Yupei::move(r)).swap(*this);
			return *this;
		}
		shared_ptr& operator=(shared_ptr&& r)
		{
			shared_ptr(Yupei::move(r)).swap(*this);
			return *this;
		}
		void reset() noexcept
		{
			shared_ptr().swap(*this);
		}
		template<typename Y>
		void reset(Y* p)
		{
			shared_ptr(p).swap(*this);
		}
		element_type* get() const noexcept
		{
			return this->GetRawPtr();
		}
		element_type& operator*() const noexcept
		{
			assert(get() != 0);
			return *get();
		}
		element_type* operator->() const noexcept
		{
			assert(get() != 0);
			return get();
		}
		std::size_t use_count() const noexcept
		{
			return this->UseCount();
		}
		bool unique() const noexcept
		{
			return use_count() == 1;
		}
		explicit operator bool() const noexcept
		{
			return get() != 0;
		}
		~shared_ptr()
		{
			this->DecRef();
		}
	};

	template<typename Type,typename D = default_delete<Type>>
	using unsynchronized_shared_ptr = Yupei::shared_ptr<Type, UnsynchronizedPolicy,D>;
}

