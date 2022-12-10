export module mp_arithmetic;

#include <vector>
#include <concepts>

template<typename T>
concept ValidArgument = std::is_integral_v<T> and 
						std::is_unsigned_v<T> and 
						sizeof(T) > 4;

template<typename T>
class MPInt {
private:
	std::vector<T> mMpNumber;
};