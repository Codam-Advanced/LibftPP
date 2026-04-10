#pragma once
# include <stdexcept>
# include <iostream>
# include <utility>

template<typename TType> class Singleton
{
	public:
		~Singleton()
		{
			if (_object != nullptr)
			delete _object;
		}

		// It doesn't make senses to copy a singleton
		Singleton() = delete;
		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;

		/**
		 * @brief Instance will return the object
		 * 
		 * @return TType*
		 */
		static TType* instance()
		{
			return _object;
		}

		/**
		 * @brief Instantiate the class object
		 * @throws when already instantiated
		 * @tparam Targs 
		 * @param p_args 
		 */
		template<typename ...TArgs>
		static void instantiate(TArgs&& ...p_args)
		{
			// if object is already instantiated throw an error
			if (_object != nullptr)
				throw std::runtime_error("Error: Singleton already instantiated!");

			_object = new TType(std::forward<TArgs>(p_args)...);
		}

	private:
		inline static TType* _object = nullptr;
};

class TestCar 
{
	// declare the singleton as a friend so that it has accesses to construct the object
	friend class Singleton<TestCar>;

	public:
		void vroom()
		{
			std::cout << "The " << _color << " Car is going " << _type << std::endl;
		}

	protected:
		TestCar(const std::string& color, const std::string& type) : _color(color), _type(type) {}

	private:
		std::string _color;
		std::string _type;
};