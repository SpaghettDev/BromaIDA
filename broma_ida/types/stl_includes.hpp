// custom gnustl taken from: https://github.com/geode-sdk/geode/blob/main/loader/include/Geode/c++stl/string.hpp

#include <cstdint>

namespace stl
{
	// string
	struct StringData {
		struct Internal {
			size_t m_size;
			size_t m_capacity;
			int m_refcount;
		};
		Internal* m_data = nullptr;
	};

	// vector<bool> (quirky)
	struct _bit_iterator {
		std::uintptr_t* m_bitptr;
		unsigned int m_offset;
	};

	// map
	struct _rb_tree_base {
		bool m_isblack;
		_rb_tree_base* m_parent;
		_rb_tree_base* m_left;
		_rb_tree_base* m_right;
	};

	template <typename T>
	struct _rb_tree_node : public _rb_tree_base {
		T m_value;
	};

	template <typename T>
	struct _rb_tree_iterator {
		typedef T value_type;
		typedef T& reference;
		typedef T* pointer;

		typedef _rb_tree_iterator<T> _Self;
		typedef _rb_tree_base* _Base_ptr;
		typedef _rb_tree_node<T>* _Link_type;

		_Base_ptr m_node;
	};
}

#if !BROMAIDA_USE_CUSTOM_GNUSTL

#include <utility> // IWYU pragma: keep
#include <vector> // IWYU pragma: keep
#include <map> // IWYU pragma: keep
#include <unordered_map> // IWYU pragma: keep
#include <set> // IWYU pragma: keep
#include <unordered_set> // IWYU pragma: keep

#if !BROMAIDA_IS_PLATFORM_ANDROID
#include <string> // IWYU pragma: keep
#endif

#else

#include <cstddef>

#if BROMAIDA_IS_PLATFORM_ANDROID
using va_list = char*;
#endif

namespace std
{
	template <typename T>
	class vector {
	public:
		void assign(const T*, const T*) {}
		T& front() { return *m_start; }
		std::size_t size() { return m_finish - m_start; }

	protected:
		T* m_start;
		T* m_finish;
		T* m_reserveEnd;
	};

#if BROMAIDA_IS_PLATFORM_ANDROID
	template <>
	class vector<bool> {
	protected:
		stl::_bit_iterator m_start;
		stl::_bit_iterator m_end;
		std::uintptr_t* m_capacity_end;
	};

	class string {
	public:
		string() {}
		string(const char*) {}

		const char* c_str() const;
		std::size_t size();

	private:
		stl::StringData m_data;
		// friend stl::StringImpl;
	};

	template <typename K, typename V>
	class map {
	protected:
		// std::less is 1 byte, but idaclang thinks its 0????
		// std::less<K> compare;
		bool compare;
		stl::_rb_tree_base m_header;
		std::size_t m_nodecount;
	};

	template <class V>
	class set {
	public:
		using iterator = V*;

	private:
		void* m_pad[6];
	};

	template <class K, class V>
	using unordered_map = void*[7];

	template <class V>
	using unordered_set = void*[7];

#elif BROMAIDA_IS_PLATFORM_MACHO

	// prevter's headers make vector<bool> have the same size as a regular vector, somehow
	// template <>
	// class vector<bool> {
	// protected:
	// 	stl::_bit_iterator m_start;
	// 	stl::_bit_iterator m_end;
	// 	std::uintptr_t* m_capacity_end;
	// };

	class string {
	public:
		string() {}
		string(const char*) {}

		const char* c_str() const;
		std::size_t size();

	private:
		void* m_pad[3];
	};

	template <typename K, typename V>
	class map {
	protected:
		void* m_pad[3];
	};


	template <class V>
	class set {
	public:
		using iterator = V*;

	private:
		void* m_pad[3];
	};

	template <class K, class V>
	using unordered_map = void*[5];

	template <class V>
	using unordered_set = void*[5];

#endif
}

#endif // !BROMAIDA_USE_CUSTOM_GNUSTL


#if !BROMAIDA_USE_CUSTOM_GNUSTL && (BROMAIDA_IS_PLATFORM_ANDROID || BROMAIDA_IS_PLATFORM_MACHO)

// idaclang thinks empty classes occupy 0 bytes of space therefore breaking the
// implementation of std::map/std::set (caused by the empty class std::less)
template <typename T>
struct custom_less : std::less<T>
{
	bool pad;
};

namespace std
{
	// providing custom specializations for standard library templates is
	// technically UB but clang allows it

	template <typename Key, typename T, typename Alloc>
	class map<Key, T, less<Key>, Alloc> : public map<Key, T, custom_less<Key>, Alloc> {};

	template <typename Key, typename Alloc>
	class set<Key, less<Key>, Alloc> : public set<Key, custom_less<Key>, Alloc> {};

	// std::vector implementation of gnustl breaks idaclang (errors when determining
	// the size in local types)
	template <typename T>
	class vector<T, allocator<T>>
	{
	public:
		void assign(const T*, const T*) {}
		T& front() { return *_M_start; }
		std::size_t size() { return _M_finish - _M_start; }

	private:
		T* _M_start;
		T* _M_finish;
		T* _M_end_of_storage;
	};

#if BROMAIDA_IS_PLATFORM_ANDROID
	template <>
	class vector<bool, allocator<bool>>
	{
		stl::_bit_iterator _M_start;
		stl::_bit_iterator _M_finish;
		void* _M_end_of_storage;
	};


	template <typename Char>
	class char_traits {};

	template <typename Char, typename Traits, typename Allocator>
	class basic_string
	{
	public:
		basic_string() {}
		basic_string(const Char*) {}

		const Char* c_str() const;
		std::size_t size();

	private:
		stl::StringData m_data;
		// friend stl::StringImpl;
	};

	using string = basic_string<char, char_traits<char>, allocator<char>>;
#endif
}

#endif // !BROMAIDA_USE_CUSTOM_GNUSTL
