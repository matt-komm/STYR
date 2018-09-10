#include "styr/Branch.h"

namespace styr
{

template<> const std::string getBranchClass<int>(){return "I";};
template<> const std::string getBranchClass<float>(){return "F";};

}

