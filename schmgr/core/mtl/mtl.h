#ifndef CORE_MTL_MTL_H_
#define CORE_MTL_MTL_H_

#include <string>
#include <map>
#include <array>
#include <vector>
#include <utility>
#include <list>
#include <queue>
#include <unordered_map>

#include "schmgr/core/mtl/Vector.h"
#include "schmgr/core/mtl/Map.h"
#include "schmgr/core/mtl/multiMap.h"
#include "schmgr/core/mtl/unordered_Map.h"
#include "schmgr/core/mtl/List.h"



namespace core::mtl
{
using string = std::string;

template <class Kty, class Ty>
// using map = std::map<Kty, Ty>;
using map = core::mtl::Map<Kty, Ty>;

template <class T>
// using vector = std::vector<T>;
using vector = core::mtl::Vector<T>;

template <class T>
// using list = std::list<T>;
using list = core::mtl::List<T>;

template <class Kty, class Ty>
using unordered_map = std::unordered_map<Kty, Ty>;
// using unordered_map = core::mtl::unordered_Map<Kty, Ty>;

template <class Kty, class Ty>
using multimap = std::multimap<Kty, Ty>;
// using multimap = core::mtl::multiMap<Kty, Ty>;

template <class T, class P, class C>
using priority_queue = std::priority_queue<T, P, C>;

} // namespace core::mtl

#endif // CORE_MTL_MTL_H_
