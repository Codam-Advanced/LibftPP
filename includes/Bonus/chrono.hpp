#pragma once

// the static class
#include <ratio>
#include <type_traits>

class chrono {

	template <class Rep, class Period = std::ratio<1>>
	class duration
	{
		public:
	
			constexpr explicit duration(const Rep& count): _count(count) {};

			constexpr Rep count() const 
			{ 
				return _count; 
			}

			friend bool operator==(const duration& leftObject, const duration& rightObject)
			{
				std::common_type<Rep>::type;
			}


		private:
			Rep _count;

	};
};
