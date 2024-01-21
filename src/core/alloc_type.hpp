/*
 * This file is part of OpenTTD.
 * OpenTTD is free software; you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software Foundation, version 2.
 * OpenTTD is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details. You should have received a copy of the GNU General Public License along with OpenTTD. If not, see <http://www.gnu.org/licenses/>.
 */

/** @file alloc_type.hpp Helper types related to the allocation of memory */

#ifndef ALLOC_TYPE_HPP
#define ALLOC_TYPE_HPP

#include "alloc_func.hpp"
#include <memory>

/**
 * A reusable buffer that can be used for places that temporary allocate
 * a bit of memory and do that very often, or for places where static
 * memory is allocated that might need to be reallocated sometimes.
 *
 * Every time Allocate or ZeroAllocate is called previous results of both
 * functions will become invalid.
 */
template <typename T>
class ReusableBuffer {
private:
	T *buffer;    ///< The real data buffer
	size_t count; ///< Number of T elements in the buffer

public:
	/** Create a new buffer */
	ReusableBuffer() : buffer(nullptr), count(0) {}
	/** Clear the buffer */
	~ReusableBuffer() { free(this->buffer); }

	/**
	 * Get buffer of at least count times T.
	 * @note the buffer might be bigger
	 * @note calling this function invalidates any previous buffers given
	 * @param count the minimum buffer size
	 * @return the buffer
	 */
	T *Allocate(size_t count)
	{
		if (this->count < count) {
			free(this->buffer);
			this->buffer = MallocT<T>(count);
			this->count = count;
		}
		return this->buffer;
	}

	/**
	 * Get buffer of at least count times T with zeroed memory.
	 * @note the buffer might be bigger
	 * @note calling this function invalidates any previous buffers given
	 * @param count the minimum buffer size
	 * @return the buffer
	 */
	T *ZeroAllocate(size_t count)
	{
		if (this->count < count) {
			free(this->buffer);
			this->buffer = CallocT<T>(count);
			this->count = count;
		} else {
			memset(this->buffer, 0, sizeof(T) * count);
		}
		return this->buffer;
	}

	/**
	 * Get the currently allocated buffer.
	 * @return the buffer
	 */
	inline const T *GetBuffer() const
	{
		return this->buffer;
	}
};

/**
 * Base class that provides memory initialization on dynamically created objects.
 * All allocated memory will be zeroed.
 */
class ZeroedMemoryAllocator
{
public:
	ZeroedMemoryAllocator() {}
	virtual ~ZeroedMemoryAllocator() = default;

	/**
	 * Memory allocator for a single class instance.
	 * @param size the amount of bytes to allocate.
	 * @return the given amounts of bytes zeroed.
	 */
	inline void *operator new(size_t size) { return CallocT<byte>(size); }

	/**
	 * Memory allocator for an array of class instances.
	 * @param size the amount of bytes to allocate.
	 * @return the given amounts of bytes zeroed.
	 */
	inline void *operator new[](size_t size) { return CallocT<byte>(size); }

	/**
	 * Memory release for a single class instance.
	 * @param ptr  the memory to free.
	 */
	inline void operator delete(void *ptr) { free(ptr); }

	/**
	 * Memory release for an array of class instances.
	 * @param ptr  the memory to free.
	 */
	inline void operator delete[](void *ptr) { free(ptr); }
};

struct FreeDeleter
{
	void operator()(const void* ptr) { free(ptr); }
};

struct NoOpDeleter
{
	void operator()(const void* ptr) {}
};

/**
 * A wrapper around std::unique_ptr<T[]> which also stores the size.
 */
template <typename T>
class UniqueBuffer {
private:
	std::unique_ptr<T[]> buffer; ///< The real data buffer
	size_t buffer_size = 0;      ///< Number of T elements in the buffer

public:
	void swap(UniqueBuffer &other) noexcept
	{
		std::swap(this->buffer, other.buffer);
		std::swap(this->buffer_size, other.buffer_size);
	}

	void reset(size_t size = 0)
	{
		if (size > 0) {
			this->buffer.reset(new T[size]);
		} else {
			this->buffer.reset();
		}
		this->buffer_size = size;
	}

	T *get() const noexcept
	{
		return this->buffer.get();
	}

	size_t size() const noexcept
	{
		return this->buffer_size;
	}

	T& operator[](size_t i) const noexcept
	{
		return this->buffer[i];
	}

	UniqueBuffer() noexcept {}

	UniqueBuffer(const UniqueBuffer &other) = delete;

	UniqueBuffer(UniqueBuffer &&other) noexcept
	{
		this->swap(other);
	}

	UniqueBuffer(size_t size)
	{
		this->reset(size);
	}

	UniqueBuffer(std::unique_ptr<T[]> buffer, size_t size) : buffer(std::move(buffer)), buffer_size(size) {}

	UniqueBuffer& operator=(const UniqueBuffer &other) = delete;

	UniqueBuffer& operator=(UniqueBuffer &&other) noexcept
	{
		this->swap(other);
		return *this;
	}

	bool operator==(std::nullptr_t) const noexcept
	{
		return this->buffer == nullptr;
	}

	bool operator!=(std::nullptr_t) const noexcept
	{
		return this->buffer != nullptr;
	}

	explicit operator bool() const noexcept
	{
		return (bool)this->buffer;
	}
};

#endif /* ALLOC_TYPE_HPP */
