// Copyright (c) 2015 Urtashev Arslan. All rights reserved.
// Contacts: <urtashev@gmail.com>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software
// and associated documentation files (the "Software"), to deal in the Software without
// restriction, including without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the
// Software is furnished to do so, subject to the following conditions:
//
//   The above copyright notice and this permission notice shall be included in all copies or
//   substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
// BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
// DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#pragma once

#include <geo_base/core/system.h>
#include <geo_base/util/allocator.h>
#include <geo_base/util/exception.h>

#include <algorithm>

namespace NGeoBase
{

template<typename TData>
class TDynArray
{
public:
    TDynArray() noexcept
        : Size_(0)
        , Capacity_(0)
        , Data_(nullptr)
        , Allocator_(nullptr)
    {
    }

    TDynArray(size_t capacity, TAllocator *Allocator)
        : Size_(0)
        , Capacity_(capacity)
        , Data_(nullptr)
        , Allocator_(Allocator)
    {
        Data_ = (TData*) Allocator_->Allocate(capacity * sizeof(TData));
    }

    TDynArray(size_t size, size_t capacity, TAllocator *Allocator)
        : Size_(0)
        , Capacity_(capacity)
        , Data_(nullptr)
        , Allocator_(Allocator)
    {
        Data_ = (TData*) Allocator_->Allocate(capacity * sizeof(TData));
        Resize(size);
    }

    const TData& Back() const noexcept
    {
        return Data_[Size_ - 1];
    }

    const TData& Front() const noexcept
    {
        return Data_[0];
    }

    TData& operator [] (size_t index)
    {
#ifndef NDEBUG
        if (index >= Size_)
            throw TException("Index %lu out of bounds %lu", index, Size_);
#endif
        return Data_[index];
    }

    const TData& operator [] (size_t index) const
    {
#ifndef NDEBUG
        if (index >= Size_)
            throw TException("Index %lu out of bounds %lu", index, Size_);
#endif
        return Data_[index];
    }

    TData* Begin() noexcept
    {
        return Data_;
    }

    const TData* Begin() const noexcept
    {
        return Data_;
    }

    TData* End() noexcept
    {
        return Data_ + Size_;
    }

    const TData* End() const noexcept
    {
        return Data_ + Size_;
    }

    TData* Data() noexcept
    {
        return Data_;
    }

    const TData* Data() const noexcept
    {
        return Data_;
    }

    size_t Size() const noexcept
    {
        return Size_;
    }

    void Resize(size_t size) noexcept
    {
        ResizeDown(size);
        ResizeUp(size);
    }

    void Clear() noexcept
    {
        Resize(0);
    }

    bool Empty() const noexcept
    {
        return Size_ == 0;
    }

    void PushBack(const TData& Data)
    {
#ifndef NDEBUG
        if (Size_ == Capacity_)
            throw TException("Out of bounds PushBack");
        Data_[Size_++] = Data;
#endif
    }

    void PopBack()
    {
#ifndef NDEBUG
        if (Size_ == 0)
            throw TException("Out of bounds PopBack");
#endif
        Size_--;
        Data_[Size_].~TData();
    }

    ~TDynArray()
    {
        if (Data_) {
            clear();
            Allocator_->Deallocate(Data_, Capacity_ * sizeof(TData));
        }
    }

    TDynArray(TDynArray&& a) noexcept
        : Size_(0)
        , Capacity_(0)
        , Data_(nullptr)
        , Allocator_(nullptr)
    {
        std::swap(Size_, a.Size_);
        std::swap(Capacity_, a.Capacity_);
        std::swap(Data_, a.Data_);
        std::swap(Allocator_, a.Allocator_);
    }

    TDynArray& operator = (TDynArray&& a) noexcept
    {
        std::swap(Size_, a.Size_);
        std::swap(Capacity_, a.Capacity_);
        std::swap(Data_, a.Data_);
        std::swap(Allocator_, a.Allocator_);
        return *this;
    }

private:
    void ResizeDown(size_t size)
    {
        if (IS_TRIVIALLY_DESTRUCTIBLE(TData)) {
            if (Size_ > size)
                Size_ = size;
        } else {
            while (Size_ > size)
                PopBack();
        }
    }

    void ResizeUp(size_t size)
    {
        if (IS_TRIVIALLY_CONSTRUCTIBLE(TData)) {
            if (Size_ < size)
                Size_ = size;
        } else {
            while (Size_ < size)
                PushBack(TData());
        }
    }

    size_t Size_;
    size_t Capacity_;
    TData *Data_;
    TAllocator *Allocator_;

    GEO_BASE_DISALLOW_EVIL_CONSTRUCTORS(TDynArray);
};

} // namespace geo_base
