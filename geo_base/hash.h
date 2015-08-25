#pragma once

#include "algo.h"
#include "blob.h"
#include "vector.h"

namespace troll {

struct hash64_t {
	std::hash<uint64_t> hash;

	template<typename x_t>
	uint64_t operator () (x_t const &x) const
	{
		static_assert(sizeof(x) == sizeof(uint64_t), "sizeof(x) != sizeof(uint64_t)");
		return hash(*((uint64_t const *) &x));
	}
};

struct blob_hash_t {
	std::hash<blob_t> hash;

	uint64_t operator () (blob_t const &b) const
	{
		return hash(b);
	}
};

template<uint64_t const BASE = 337>
struct poly_hash_t {
	uint64_t operator () (byte_t const *bytes, count_t count) const
	{
		uint64_t hash = 0, power = 1;
		for (count_t i = 0; i < count; ++i) {
			hash += (bytes[i] + 1) * power;
			power *= BASE;
		}
		return hash;
	}

	template<typename val_t>
	uint64_t operator () (val_t const &val) const
	{
		return (*this)((byte_t const *) &val, sizeof(val_t));
	}

	template<typename val_t>
	uint64_t operator () (vector_t<val_t> const &a) const
	{
		return (*this)((byte_t const *) &(a[0]), sizeof(val_t) * a.size());
	}
};

}
