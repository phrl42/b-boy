#pragma once
#include "incs.h"
#include "Log.h"

namespace Banana
{
  template<typename T>
  using Shr = std::shared_ptr<T>;

  template<typename T, typename ...  Args>
  constexpr Shr<T> MakeShr(Args&& ... args)
  {
    return std::make_shared<T>(std::forward<Args>(args) ...);
  }


	template<typename T>
	using Scope = std::unique_ptr<T>;
	template<typename T, typename ... Args>
	constexpr Scope<T> MakeScoped(Args&& ... args)
	{
		return std::make_unique<T>(std::forward<Args>(args)...);
	}
  
	using core_id = uint32_t;
};