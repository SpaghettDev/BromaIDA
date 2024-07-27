// taken from: https://github.com/geode-sdk/geode/blob/main/loader/include/Geode/c++stl/string.hpp

#if !defined(BROMAIDA_DONT_USE_CUSTOM_GNUSTL) && (defined(BROMAIDA_PLATFORM_ANDROID32) || defined(BROMAIDA_PLATFORM_ANDROID64))

#include <cstdint>
#include <cstddef>

using va_list = char*;

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


namespace std
{
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

	template <>
	class vector<bool> {
	protected:
		stl::_bit_iterator m_start;
		stl::_bit_iterator m_end;
		std::uintptr_t* m_capacity_end;
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
		void* m_[6];
	};

	template <class K, class V>
	using unordered_map = void*[7];

	template <class V>
	using unordered_set = void*[7];
}

#endif
