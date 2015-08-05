#pragma once

#include "utility_internal.h"
#include "__swap.h"
#include "__allocator.h"
#include "compress_pair.h"
#include "compare_funtor.h"
#include "iterator.h"
#include <numeric>
#include <cinttypes>

namespace Yupei
{
	template<typename Type,
		typename UType,
		typename = void>
	struct has_rebind : false_type
	{
		//no rebind...
	};

	template<typename Type, typename UType>
	struct has_rebind<Type,
		UType,
		void_t<typename Type::template rebind<UType>>
	> : true_type
	{

	};

	template<typename Type,
		typename UType,
		bool HasRebind = has_rebind<Type, UType>::value>
	struct get_rebind
	{
		//no rebind...
	};

	template<typename Type, typename UType>
	struct get_rebind
		<
		Type,
		UType,
		true
		>
	{
		using type = typename Type::template rebind<UType>;
	};

	template<template<typename, typename...> typename Type,
		typename T1,
		typename... Args,
		typename UType>
	struct get_rebind<
		Type<T1, Args...>, UType,
		false>
	{
		using type = Type<UType, Args...>;
	};

	template<typename Type,
		typename = void>
	struct has_element_type : false_type
	{

	};

	template<typename Type>
	struct has_element_type<Type, void_t<typename Type::element_type>> : true_type
	{

	};

	template<typename Type,
		bool HasRebind = has_element_type<Type>::value>
	struct get_element_type
	{
		//no rebind...
		static_assert(has_element_type<Type>::value, "no element_type!");
	};

	template<typename Type>
	struct get_element_type
		<
		Type,
		true
		>
	{
		using type = typename Type::element_type;
	};

	template
		<
		template<typename, typename...> typename Type,
		typename T1,
		typename... Args
		>
	struct get_element_type<
		Type<T1, Args...>,
		false>
	{
		using type = T1;
	};


	template<class Ptr>
	struct pointer_traits
	{
		using pointer = Ptr;
		using element_type = typename get_element_type<Ptr>::type;
		using difference_type = get_difference_type_t<Ptr>;
		template<class U>
		using rebind = typename get_rebind<Ptr, U>::type;

		static pointer pointer_to(conditional_t<is_void<element_type>::value,
			void*,
			element_type&> r)
		{
			return pointer::pointer_to(r);
		}
	};

	template<class T>
	struct pointer_traits<T*>
	{
		using pointer = T*;
		using element_type = T;
		using difference_type = std::ptrdiff_t;
		template<class U>
		using rebind = U*;

		static pointer pointer_to(conditional_t<is_void<element_type>::value,
			void*,
			element_type&> r) noexcept
		{
			return Yupei::addressof(r);
		}
	};

	template<typename Type,
		typename Ptr,
		typename _Value,
		typename = void>
	struct get_const_pointer
	{
		using type = typename pointer_traits<Ptr>::template rebind<const _Value>;
	};

	template<typename Type,
		typename Ptr,
		typename _Value
	>
	struct get_const_pointer<Type, Ptr, _Value, void_t<typename Type::const_pointer>>
	{
		using type = typename Type::const_pointer;
	};

	template<typename Type,
		typename Ptr,
		typename = void>
	struct get_void_pointer
	{
		using type = typename pointer_traits<Ptr>::template rebind<void>;
	};

	template<typename Type,
		typename Ptr
	>
	struct get_void_pointer<Type, Ptr, void_t<typename Type::void_pointer>>
	{
		using type = typename Type::void_pointer;
	};

	template<typename Type,
		typename Ptr,
		typename = void>
	struct get_const_void_pointer
	{
		using type = typename pointer_traits<Ptr>::template rebind<const void>;
	};

	template<typename Type,
		typename Ptr
	>
	struct get_const_void_pointer<Type, Ptr, void_t<typename Type::const_void_pointer>>
	{
		using type = typename Type::const_void_pointer;
	};

	template<typename Type,
		typename Ptr,
		typename = void>
	struct get_allocator_diff
	{
		using type = typename pointer_traits<Ptr>::difference_type;
	};

	template<typename Type,
		typename Ptr
	>
	struct get_allocator_diff<Type, Ptr, void_t<typename Type::difference_type>>
	{
		using type = typename Type::difference_type;
	};

	template<typename Type,
		typename DiffType,
		typename = void>
	struct get_allocator_size_type
	{
		using type = make_unsigned_t<DiffType>;
	};

	template<typename Type,
		typename DiffType
	>
	struct get_allocator_size_type<Type, DiffType, void_t<typename Type::size_type>>
	{
		using type = typename Type::size_type;
	};

	template<typename Alloc,
		typename = void>
	struct is_always_equal_traits
	{
		using type = typename is_empty<Alloc>::type;
	};

	template<typename Alloc>
	struct is_always_equal_traits<Alloc, void_t<typename Alloc::is_always_equal>>
	{
		using type = typename Alloc::is_always_equal;
	};


	template<typename Alloc,
		typename = void>
	struct propagate_on_container_copy_assignment_traits
	{
		using type = false_type;
	};

	template<typename Alloc>
	struct propagate_on_container_copy_assignment_traits<Alloc, void_t<typename Alloc::propagate_on_container_copy_assignment>>
	{
		using type = typename Alloc::propagate_on_container_copy_assignment;
	};

	template<typename Alloc,
		typename = void>
	struct propagate_on_container_move_assignment_traits
	{
		using type = false_type;
	};

	template<typename Alloc>
	struct propagate_on_container_move_assignment_traits<Alloc, void_t<typename Alloc::propagate_on_container_move_assignment>>
	{
		using type = typename Alloc::propagate_on_container_move_assignment;
	};

	template<typename Alloc,
		typename = void>
	struct propagate_on_container_swap_traits
	{
		using type = false_type;
	};

	template<typename Alloc>
	struct propagate_on_container_swap_traits<Alloc, void_t<typename Alloc::propagate_on_container_swap>>
	{
		using type = typename Alloc::propagate_on_container_swap;
	};

	namespace Internal
	{
		
		template<typename Alloc,
			typename SizeType,
			typename HintType>
			static inline auto allocate_impl(Alloc& a, SizeType n, HintType hint, int)
			->decltype(a.allocate(n, hint))
		{
			return a.allocate(n, hint);
		}

		template<typename Alloc,
			typename SizeType,
			typename HintType>
			static inline auto allocate_impl(Alloc& a, SizeType n, HintType hint, Internal::WrapInt)
			->decltype(a.allocate(n))
		{
			return a.allocate(n);
		}

		template <class Alloc, class T, class... Args>
		static inline auto construct_impl(int, Alloc& a, T* p, Args&&... args)->decltype(a.construct(a.construct(p, Yupei::forward<Args>(args)...)))
		{
			return a.construct(a.construct(p, Yupei::forward<Args>(args)...));
		}

		template <class Alloc, class T, class... Args>
		static inline void construct_impl(Internal::WrapInt, Alloc& a, T* p, Args&&... args)
		{
			::new (static_cast<void*>(p)) T(Yupei::forward<Args>(args)...);
		}

		template <class Alloc, class T>
		static auto destroy_impl(Alloc& a, T* p, int)->decltype(a.destroy(p))
		{
			return a.destroy(p);
		}

		template <class Alloc, class T>
		static void destroy_impl(Alloc& a, T* p, Internal::WrapInt)
		{
			p->~T();
		}

		template<class Alloc>
		static auto max_size_impl(const Alloc& a, int) noexcept
			->decltype(a.max_size())
		{
			return a.max_size();
		}

		template<class SizeType, class Alloc>
		static SizeType max_size_impl(const Alloc& a, Internal::WrapInt) noexcept
		{
			return std::numeric_limits<SizeType>::max();
		}

		template<class Alloc>
		static auto select_on_container_copy_construction_impl(const Alloc& rhs, int)
			->decltype(rhs.select_on_container_copy_construction())
		{
			return rhs.select_on_container_copy_construction();
		}

		template<class Alloc>
		static Alloc select_on_container_copy_construction_impl(const Alloc& rhs, Internal::WrapInt)
		{
			return rhs;
		}
	}



	//minimal allocator_traits support
	template<typename Alloc>
	struct allocator_traits
	{
		using allocator_type = Alloc;
		using value_type = typename allocator_type::value_type;
		using pointer = get_pointer_t<allocator_type>;
		using const_pointer = typename get_const_pointer<allocator_type, pointer, value_type>::type;
		using void_pointer = typename get_void_pointer<allocator_type, pointer>::type;
		using const_void_pointer = typename get_const_void_pointer<allocator_type, pointer>::type;
		using difference_type = typename get_allocator_diff<allocator_type, pointer>::type;
		using size_type = typename get_allocator_size_type<allocator_type, difference_type>::type;
		using is_always_equal = typename is_always_equal_traits<Alloc>::type;
		using propagate_on_container_copy_assignment = typename propagate_on_container_copy_assignment_traits<allocator_type>::type;
		using propagate_on_container_move_assignment = typename propagate_on_container_move_assignment_traits<allocator_type>::type;
		using propagate_on_container_swap = typename propagate_on_container_swap_traits<allocator_type>::type;
		template<class T>
		struct rebind_alloc
		{
			using other = typename get_rebind<allocator_type, T>::type;
		};
		template <class T>
		struct rebind_traits
		{
			using other = allocator_traits<typename rebind_alloc<T>::other >;
		};

		static pointer allocate(Alloc& a, size_type n)
		{
			return a.allocate(n);
		}
		static pointer allocate(Alloc& a, size_type n, const_void_pointer hint)
		{
			return Internal::allocate_impl(a, n, hint, 0);
		}

		static void deallocate(Alloc& a, pointer p, size_type n)
		{
			a.deallocate(p, n);
		}
		template <class T, class... Args>
		static void construct(Alloc& a, T* p, Args&&... args)
		{
			Internal::construct_impl(0, a, p, Yupei::forward<Args>(args)...);
		}
		template <class T>
		static void destroy(Alloc& a, T* p)
		{
			Internal::destroy_impl(a, p, 0);
		}
		static size_type max_size(const Alloc& a) noexcept
		{
			return Internal::max_size_impl<size_type>(a, 0);
		}
		static Alloc select_on_container_copy_construction(const Alloc& rhs)
		{
			return Internal::select_on_container_copy_construction_impl(rhs, 0);
		}

	};

	//Effects: If it is possible to fit size bytes of storage aligned by alignment into the buffer pointed to by
	//ptr with length space, the function updates ptr to point to the first possible address of such storage
	//and decreases space by the number of bytes used for alignment.Otherwise, the function does nothing.
	inline void* align(std::size_t alignment, std::size_t size,
		void*& ptr, std::size_t& space)
	{
		std::size_t offset = static_cast<std::size_t>(reinterpret_cast<std::uintptr_t>(ptr) & (alignment - 1));
		if (offset > 0) offset = alignment - offset;
		if (offset > space || space - offset > size) return nullptr;
		else
		{
			ptr = static_cast<char*>(ptr) - static_cast<std::ptrdiff_t>(offset);
			space -= offset;
			return ptr;
		}
	}

	template <class T>
	class allocator;
	// specialize for void:
	template <> class allocator<void> {
	public:
		typedef void* pointer;
		typedef const void* const_pointer;
		// reference-to-void members are impossible.
		typedef void value_type;
		template <class U> struct rebind { typedef allocator<U> other; };
	};
	template <class T>
	class allocator
	{
	public:
		typedef size_t size_type;
		typedef ptrdiff_t difference_type;
		typedef T* pointer;
		typedef const T* const_pointer;
		typedef T& reference;
		typedef const T& const_reference;
		typedef T value_type;
		template <class U> struct rebind { typedef allocator<U> other; };
		typedef true_type propagate_on_container_move_assignment;
		typedef true_type is_always_equal;
		allocator() noexcept = default;
		allocator(const allocator&) noexcept = default;
		template <class U> allocator(const allocator<U>&) noexcept = default;
		~allocator() = default;
		pointer address(reference x) const noexcept
		{
			return Yupei::addressof(x);
		}
		const_pointer address(const_reference x) const noexcept
		{
			return Yupei::addressof(x);
		}
		pointer allocate(
			size_type sz, allocator<void>::const_pointer hint = 0)
		{
			return static_cast<pointer>(::operator new(sz * sizeof(value_type)));
		}
		void deallocate(pointer p, size_type n)
		{
			return ::operator delete(p, n);
		}
		size_type max_size() const noexcept
		{
			return static_cast<size_type>(-1) / sizeof(value_type);
		}
		//template<class U, class... Args>
		//void construct(U* p, Args&&... args);
		//template <class U>
		//void destroy(U* p);
	};

	template <class T1, class T2>
	bool operator==(const allocator<T1>&, const allocator<T2>&) noexcept
	{
		return true;
	}
	template <class T1, class T2>
	bool operator!=(const allocator<T1>&, const allocator<T2>&) noexcept
	{
		return false;
	}
	template <class OutputIterator, class T>
	class raw_storage_iterator {
	public:
		typedef output_iterator_tag iterator_category;
		typedef void value_type;
		typedef void difference_type;
		typedef void pointer;
		typedef void reference;
		explicit raw_storage_iterator(OutputIterator x)
			:iter(x)
		{

		}
		raw_storage_iterator& operator*()
		{
			return *this;
		}
		raw_storage_iterator& operator=(const T& element)
		{
			new (static_cast<void*>(&*iter)) T(element)
		}
		raw_storage_iterator& operator++()
		{
			++iter;
			return *this;
		}
		raw_storage_iterator operator++(int)
		{
			auto temp = iter;
			++iter;
			return temp;
		}
		OutputIterator base() const
		{
			return iter;
		}
	private:
		OutputIterator iter;
	};
	//no get_temporary_buffer/return support

	template <class InputIterator, class ForwardIterator>
	ForwardIterator uninitialized_copy(InputIterator first, InputIterator last,
		ForwardIterator result)
	{
		for (; first != last; ++result, ++first)
			::new (static_cast<void*>(addressof(*result)))
			typename iterator_traits<ForwardIterator>::value_type(*first);
		return result;
	}
	template <class InputIterator, class Size, class ForwardIterator>
	ForwardIterator uninitialized_copy_n(InputIterator first, Size n,
		ForwardIterator result)
	{
		for (; n > 0; ++result, ++first, --n) {
			::new (static_cast<void*>(addressof(*result)))
				typename iterator_traits<ForwardIterator>::value_type(*first);
		}
		return result;
	}

	template <class ForwardIterator, class T>
	void uninitialized_fill(ForwardIterator first, ForwardIterator last,
		const T& x)
	{
		for (; first != last; ++first)
			::new (static_cast<void*>(addressof(*first)))
			typename iterator_traits<ForwardIterator>::value_type(x);
	}

	template <class ForwardIterator, class Size, class T>
	ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x)
	{
		for (; n--; ++first)
			::new (static_cast<void*>(addressof(*first)))
			typename iterator_traits<ForwardIterator>::value_type(x);
		return first;
	}

	template<typename Type>
	struct default_delete
	{
		constexpr default_delete() noexcept = default;

		template<typename UType,
			typename = enable_if_t <
			is_convertible<Type*, UType*>::value >>
			default_delete(const default_delete<UType>&) noexcept = default;

		void operator()(Type* ptr) const noexcept
		{
			delete ptr;
		}
	};

	template<typename Type>
	struct default_delete<Type[]>
	{
		constexpr default_delete() noexcept = default;

		template<typename UType,
			typename = enable_if_t <
			is_convertible<Type(*)[], UType(*)[]>{} >>
			default_delete(const default_delete<UType>&) noexcept = default;

		template<typename UType,
			typename = enable_if_t <
			is_convertible<Type(*)[], UType(*)[]>::value >>
			void operator()(UType* ptr)
		{
			delete[] ptr;
		}

	};

	namespace Internal
	{
		template<typename E, typename D,
			typename = void>
		struct GetPtrForUniquePtr
		{
			using type = E*;
		};

		template<typename E,
			typename D>
		struct GetPtrForUniquePtr < E, D,
			Yupei::void_t<typename Yupei::remove_reference_t<D>::pointer >>
		{
			using type = typename Yupei::remove_reference_t<D>::pointer;
		};

		template<typename D>
		struct SignatureForUniquePtr
		{
			using type1 = const D&;
			using type2 = D&&;
		};

		template<typename D>
		struct SignatureForUniquePtr<D&>
		{
			using type1 = D&;
			using type2 = D&&;
		};

		template<typename D>
		struct SignatureForUniquePtr<const D&>
		{
			using type1 = const D&;
			using type2 = const D&&;
		};
	}

	template<
		typename T,
		typename Deleter
	>
	struct unique_ptr_base
	{
	public:
		using pointer = typename Internal::GetPtrForUniquePtr<T, Deleter>::type;
		template<typename U>
		unique_ptr_base(U&& u)
			:internalData(Yupei::compress_value_initialize_first_arg,
				Yupei::forward<U>(u))
		{

		}
		template<typename U,
			typename D>
			unique_ptr_base(U&& u, D&& d)
			:internalData(Yupei::compress_value_initialize_both_args,
				Yupei::forward<D>(d),
				Yupei::forward<U>(u))
		{

		}

		Deleter& deleter() noexcept
		{
			return internalData.first();
		}

		const Deleter& deleter() const noexcept
		{
			return internalData.first();
		}

		pointer& raw_pointer() noexcept
		{
			return internalData.second();
		}

		const pointer& raw_pointer() const noexcept
		{
			return internalData.second();
		}

	private:
		compress_pair<Deleter, pointer> internalData;
	};

	template <class T, 
	class Deleter = Yupei::default_delete<T>>
	class unique_ptr : 
		public unique_ptr_base<T,Deleter>
	{
	public:
		
		typedef T element_type;
		typedef Deleter deleter_type;
		using base_type = unique_ptr_base<T, Deleter>;
		using pointer =typename base_type::pointer;
		// 20.8.1.2.1, constructors
		constexpr unique_ptr() noexcept
			:base_type(pointer())
		{

		}
		explicit unique_ptr(pointer p) noexcept
			: base_type(p)
		{

		}

		//if D is a ref(& or const &),just add &&
		//else const & & &&

		using _lref_type_for_unique_ptr = conditional_t
			<
			is_reference<deleter_type>::value,
			deleter_type,
			const deleter_type&
			>;
		unique_ptr(pointer p,
			_lref_type_for_unique_ptr d1) noexcept
			:base_type(p,d1)
		{

		}
		unique_ptr(pointer p, Yupei::add_rvalue_reference_t <
			Yupei::remove_reference_t<Deleter>> d2) noexcept
			: base_type(p, 
				Yupei::forward<Deleter>(u.get_deleter()))
		{

		}
		unique_ptr(unique_ptr&& u) noexcept
			: base_type(u.release(),
				Yupei::forward<Deleter>(u.get_deleter()))
		{

		}

		constexpr unique_ptr(nullptr_t) noexcept
			: unique_ptr() 
		{ 
		}

		//Remarks: This constructor shall not participate in overload resolution unless:
//(19.1) ！ unique_ptr<U, E>::pointer is implicitly convertible to pointer,
//(19.2) ！ U is not an array type, and
//(19.3) ！ either D is a reference type and E is the same type as D, or D is not a reference type and E is
//implicitly convertible to D.
		//now we start
		template < class U,
		class E,
			typename = enable_if_t <
			is_convertible<typename unique_ptr<U, E>::pointer, pointer>::value
		&& !is_array<U>::value
		&& conditional_t <
			is_reference<D>::value,
			is_same<E, D>,
			is_convertible<E, D>
			>::value >
		>
			unique_ptr(unique_ptr<U, E>&& u) noexcept
			:base_type(u.release(),Yupei::forward<E>(u.get_deleter()))
		{

		}
		// 20.8.1.2.2, destructor
		~unique_ptr()
		{
			if (get() != pointer{})
				this->deleter()(this->get());
		}
		// 20.8.1.2.3, assignment
		
		unique_ptr& operator=(unique_ptr&& u) noexcept
		{
			if (this != &u)
			{
				this->get_deleter() = Yupei::forward<E>(u.get_deleter());
				reset(u.release());
			}
			return *this;
		}
		//(5.1) ！ unique_ptr<U, E>::pointer is implicitly convertible to pointer, and
		//(5.2) ！ U is not an array type, and
		//(5.3) ！ is_assignable<D&, E&&>::value is true.
		template <class U, class E,
		typename = 
			enable_if_t
			<
				is_convertible<
								typename unique_ptr<U,E>::pointer,
								pointer
							>::value 
			&& !is_array<U>::value
			&& is_assignable
				<
					deleter_type&,
					E&&
				>::value
			>
		>
		unique_ptr& operator=(unique_ptr<U, E>&& u) noexcept
		{
			this->get_deleter() = Yupei::forward<E>(u.get_deleter());
			reset(u.release());
			return *this;
		}
		unique_ptr& operator=(nullptr_t) noexcept
		{
			reset();
		}
		add_lvalue_reference_t<T> operator*() const
		{
			return *get();
		}
		pointer operator->() const noexcept
		{
			return get();
		}
		pointer get() const noexcept
		{
			return this->raw_pointer();
		}
		deleter_type& get_deleter() noexcept
		{
			return this->deleter();
		}
		const deleter_type& get_deleter() const noexcept
		{
			return this->deleter();
		}
		explicit operator bool() const noexcept
		{
			return get() != pointer{};
		}
		// 20.8.1.2.5 modifiers
		pointer release() noexcept
		{
			auto old_p = this->get();
			this->raw_pointer() = pointer{};
			return old_p;
		}
		void reset(pointer p = pointer{}) noexcept
		{
			auto old_p = this->raw_pointer();
			this->raw_pointer() = p;
			if (old_p != pointer{})
			{
				this->get_deleter()(old_p);
			}
		}
		void swap(unique_ptr& u) noexcept
		{
			Yupei::swap(this->raw_pointer(), u.raw_pointer());
			Yupei::swap(this->get_deleter(), u.get_deleter());
		}
		// disable copy from lvalue
		unique_ptr(const unique_ptr&) = delete;
		unique_ptr& operator=(const unique_ptr&) = delete;

	};

	template <class T,
	class Deleter>
	class unique_ptr<T[],Deleter> :
		public unique_ptr_base<T, Deleter>
	{
	public:

		typedef T element_type;
		typedef Deleter deleter_type;
		using base_type = unique_ptr_base<T, Deleter>;
		using pointer = typename base_type::pointer;
		// 20.8.1.3.1, constructors
		constexpr unique_ptr() noexcept
			:base_type(pointer())
		{

		}
		template<typename U>
		using _can_be_contructed = 
			enable_if_t
			<
				is_same<U, pointer>::value
				|| (
					is_same<element_type*, pointer>::value
					&& is_convertible
						<
							typename Yupei::pointer_traits<U>::element_type(*)[],
							typename element_type(*)[]
						>::value
					)
			>;
		template <class U,
		typename = _can_be_contructed<U>>
		explicit unique_ptr(U p) noexcept
			:base_type(p)
		{

		}
		

		//if D is a ref(& or const &),just add &&
		//else const & & &&

		using _lref_type_for_unique_ptr = 
			conditional_t
			<
				is_reference<deleter_type>::value,
				deleter_type,
				const deleter_type&
			>;
		template <class U,
			typename = _can_be_contructed<U>>
		unique_ptr(U p, _lref_type_for_unique_ptr d) noexcept
		{

		}
		template <class U,
			typename = _can_be_contructed<U>>
		unique_ptr(U p, add_rvalue_reference_t <
			remove_reference_t<Deleter>> d2) noexcept
			: base_type(p,
				Yupei::forward<Deleter>(u.get_deleter()))
		{

		}
		unique_ptr(unique_ptr&& u) noexcept
			: base_type(u.release(),
				Yupei::forward<Deleter>(u.get_deleter()))
		{

		}

		constexpr unique_ptr(nullptr_t) noexcept
			: unique_ptr()
		{
		}

		//(2.1) ！ U is an array type, and
		//(2.2) ！ pointer is the same type as element_type*, and
		//(2.3) ！ UP::pointer is the same type as UP::element_type*, and
		//(2.4) ！ UP::element_type(*)[] is convertible to element_type(*)[], and
		//(2.5) ！ either D is a reference type and E is the same type as D, or D is not a reference type and E is
		//implicitly convertible to D.
		//now we start
		template <class U,class E>
		using _temp_copy_type = bool_constant
			<
				is_array<U>::value
				&&	is_same
				<
					pointer,
					element_type*
				>::value
			&&	is_same
				<
					typename unique_ptr<U, E>::pointer,
					typename unique_ptr<U, E>::element_type*
				>::value
			&& is_convertible
				<
					typename unique_ptr<U, E>::element_type(*)[],
					element_type(*)[]
				>::value
			>;
			template
			<
				typename U,
				typename E,
				typename = enable_if_t
				<
					_temp_copy_type<U,E>::value
					&&	conditional_t
						<
							is_reference<D>::value,
							is_same<D, E>,
							is_convertible<E, D>
						>::value
				>
			>
			unique_ptr(unique_ptr<U, E>&& u) noexcept
			:base_type(u.release(), 
				Yupei::forward<E>(u.get_deleter()))
		{

		}
		// 20.8.1.2.2, destructor
		~unique_ptr()
		{
			if (get() != pointer{})
				this->deleter()(this->get());
		}
		// 20.8.1.2.3, assignment

		unique_ptr& operator=(unique_ptr&& u) noexcept
		{
			this->get_deleter() = Yupei::forward<E>(u.get_deleter());
			reset(u.release());
		}
		//(5.1) ！ unique_ptr<U, E>::pointer is implicitly convertible to pointer, and
		//(5.2) ！ U is not an array type, and
		//(5.3) ！ is_assignable<D&, E&&>::value is true.
		template
		<
			typename U,
			typename E,
			typename = enable_if_t
						<
							_temp_copy_type<U, E>::value
							&&	is_assginable<Deleter&,E&&>::value
						>
		>
		unique_ptr& operator=(unique_ptr<U, E>&& u) noexcept
		{
			this->get_deleter() = Yupei::forward<E>(u.get_deleter());
			reset(u.release());
		}
		unique_ptr& operator=(nullptr_t) noexcept
		{
			reset(pointer{});
		}
		pointer get() const noexcept
		{
			return this->raw_pointer();
		}
		deleter_type& get_deleter() noexcept
		{
			return this->deleter();
		}
		const deleter_type& get_deleter() const noexcept
		{
			return this->deleter();
		}
		explicit operator bool() const noexcept
		{
			return get() != pointer{};
		}
		// 20.8.1.2.5 modifiers
		pointer release() noexcept
		{
			auto old_p = this->get();
			this->raw_pointer() = pointer{};
			return old_p;
		}
		template 
			<
				class U,
				typename = _can_be_contructed<U>
			> 
		void reset(U p) noexcept
		{
			auto old_p = this->raw_pointer();
			this->raw_pointer() = p;
			if (old_p != pointer{})
			{
				this->get_deleter()(old_p);
			}
		}
		void reset(std::nullptr_t = nullptr)
		{
			reset(pointer{});
		}
		T& operator[](size_t i) const
		{
			return get()[i];
		}
		void swap(unique_ptr& u) noexcept
		{
			swap(this->raw_pointer(), u.raw_pointer());
			swap(this->get_deleter(), u.get_deleter());
		}
		// disable copy from lvalue
		unique_ptr(const unique_ptr&) = delete;
		unique_ptr& operator=(const unique_ptr&) = delete;

	};

	namespace Internal
	{
		template<typename T>
		struct is_incomplete_array_type : false_type {};

		template<typename T>
		struct is_incomplete_array_type<T[]> : true_type {};
	}

	template<typename T,
		typename... Args,
	typename = enable_if_t<!is_array<T>::value>>
		unique_ptr<T> make_unique(Args&&... args)
	{
		return unique_ptr<T>(new T(Yupei::forward<Args>(args)...));
	}

	template<typename T,
		typename... Args,
		typename = enable_if_t<Internal::is_incomplete_array_type<T>::value >>
		unique_ptr<T> make_unique(std::size_t n)
	{
		return unique_ptr<T>(new remove_extent_t<T>[n]());
	}

	template <class T, 
	class... Args,
		typename = enable_if_t<is_array<T>::value && sizeof(T) != 0 >>
		T make_unique(Args&&...) = delete;

	template<class T, class D> 
	void swap(unique_ptr<T, D>& x, unique_ptr<T, D>& y) noexcept
	{
		x.swap(y);
	}
	template<class T1, class D1, class T2, class D2>
	bool operator==(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y)
	{
		return x.get() == y.get();
	}
	template<class T1, class D1, class T2, class D2>
	bool operator!=(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y)
	{
		return x.get() != y.get();
	}
	template<class T1, class D1, class T2, class D2>
	bool operator<(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y)
	{
		return less < common_type_t<
			typename unique_ptr<T1, D1>::pointer,
			typename unique_ptr<T2, D2>::pointer
			>> {}(x, y);
	}
	template<class T1, class D1, class T2, class D2>
	bool operator<=(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y)
	{
		return !(y < x);
	}
	template<class T1, class D1, class T2, class D2>
	bool operator>(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y)
	{
		return y < x;
	}
	template<class T1, class D1, class T2, class D2>
	bool operator>=(const unique_ptr<T1, D1>& x, const unique_ptr<T2, D2>& y)
	{
		return !(x < y);
	}
	template <class T, class D>
	bool operator==(const unique_ptr<T, D>& x, std::nullptr_t) noexcept
	{
		return !x;
	}
	template <class T, class D>
	bool operator==(std::nullptr_t, const unique_ptr<T, D>& y) noexcept
	{
		return !y;
	}
	template <class T, class D>
	bool operator!=(const unique_ptr<T, D>& x, std::nullptr_t) noexcept
	{
		return static_cast<bool>(x);
	}
	template <class T, class D>
	bool operator!=(std::nullptr_t, const unique_ptr<T, D>& y) noexcept
	{
		return static_cast<bool>(y);
	}
	template <class T, class D>
	bool operator<(const unique_ptr<T, D>& x, std::nullptr_t)
	{
		return less<typename unique_ptr<T, D>::pointer>{}(x.get(), nullptr);
	}
	template <class T, class D>
	bool operator<(std::nullptr_t, const unique_ptr<T, D>& y)
	{
		return less<typename unique_ptr<T, D>::pointer>{}(nullptr,y.get());
	}
	template <class T, class D>
	bool operator<=(const unique_ptr<T, D>& x, std::nullptr_t)
	{
		return !(nullptr < x);
	}
	template <class T, class D>
	bool operator<=(std::nullptr_t, const unique_ptr<T, D>& y)
	{
		return !(y < nullptr);
	}
	template <class T, class D>
	bool operator>(const unique_ptr<T, D>& x, std::nullptr_t)
	{
		return nullptr < x;
	}
	template <class T, class D>
	bool operator>(std::nullptr_t, const unique_ptr<T, D>& y)
	{
		return y < nullptr;
	}
	template <class T, class D>
	bool operator>=(const unique_ptr<T, D>& x, std::nullptr_t)
	{
		return !(x < nullptr);
	}
	template <class T, class D>
	bool operator>=(std::nullptr_t, const unique_ptr<T, D>& y)
	{
		return !(nullptr < y);
	}
}

