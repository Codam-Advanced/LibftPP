#pragma once

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <memory>
#include <stdexcept>
#include <utility>
#include <vector>
#include <stack>

template<typename TType> class Pool {

    template<typename UType> class Object  {

        public:
            Object(TType *ptr, Pool& pool) : _ptr(ptr), _pool(pool) {}

            // once 
            virtual ~Object()
            {
                if (_ptr) _pool.release(_ptr);
            }

            TType& operator*()
            {
                return *_ptr;
            }

            TType* operator->()
            {
                return _ptr;
            }

        
        private:
            TType* _ptr;
            Pool& _pool;
    };

    public:
        Pool() = default;

        ~Pool() 
        {
            for (auto& _usedPointer : _usedObjects)
            {
                // destruct the element
                traits::destroy(_allocator, _usedPointer);
            }

            for (int i = 0; i < _poolChunks.size(); i++)
            {
                // deallocate each chunk.
                traits::deallocate(_allocator, _poolChunks[i], _poolSizes[i]); 
            }
        }

        /**
         * @brief Resize the buffer by adding the amount of object which can be stored
         * 
         * @param numberOfObjectStored
         */
        void resize(const size_t& numberOfObjectStored)
        {   
            // tell the allocator to allocate amount of objects as raw memory
            TType* pool = traits::allocate(_allocator, numberOfObjectStored);

            // add each object to the free stack
            for (int i = 0; i < numberOfObjectStored; i++)
                _freeObjects.push(pool + i);

            // add the pool chuck to the rest of the pools
            _poolChunks.push_back(pool);

            // save the size
            _poolSizes.push_back(numberOfObjectStored);
        }

        template<typename ...TArgs> Pool::Object<TType> acquire(TArgs&&... p_args)
        {
            // if there is no room left throw an error
            if (_freeObjects.empty())
                throw std::runtime_error("No space left in Pool, tip: increase size or return objects to the pool");
    
            // construct the object which will populate the pointer with the actual object
            traits::construct(_allocator, _freeObjects.top(), std::forward<TArgs>(p_args)...);

            // create an object wrapper
            Object<TType> object_wrapper(_freeObjects.top(), *this);
 
            // add it to the used objects
            _usedObjects.push_back(_freeObjects.top());

            // remove the element
            _freeObjects.pop();
            
            // return an Object wrapper
            return object_wrapper;
        }

        void release(TType* object)
        {
            // destroy the object by calling the deconstructor but not freeing the object
            traits::destroy(_allocator, object);

            // add it to the free object stack
            _freeObjects.push(object);

            // Remove the object from the used vector
            _usedObjects.erase(std::find(_usedObjects.begin(), _usedObjects.end(), object));
        }

    private:

        // we want the standard allocator to allocate memory and construct it at two separate points
        std::allocator<TType> _allocator;

        // with c++11 allocator traits were introduces to predefine some static methods and typedefs for the allocator all calls
        // for the allocator are going through the allocator traits which has some default defines method for ease of use.
        using traits = std::allocator_traits<decltype(_allocator)>;

        // size of the pool
        std::vector<size_t> _poolSizes;
        
        // Memory pool chunks
        std::vector<TType*> _poolChunks; 
        
        // free stack
        std::stack<TType*> _freeObjects;   

        // used stack
        std::vector<TType*> _usedObjects; 
};