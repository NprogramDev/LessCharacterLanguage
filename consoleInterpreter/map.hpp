#ifndef LCLS_MAP_HPP_
#define LCLS_MAP_HPP_
#include <map>
namespace LCLS{
/*	template<
	class Key,
	class T,
	class Compare = std::less<Key>,
	class Allocator = std::allocator<std::pair<const Key, T>>> 
	class Map : public std::map<Key,T> {
		
	};*/
	template <class K,class T>
	using Map = std::map<K,T>;
}

#endif
