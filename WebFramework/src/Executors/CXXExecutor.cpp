#include "CXXExecutor.h"

using namespace std;

namespace framework
{
	CXXExecutor::CXXExecutor(HMODULE module, void* implementation) :
		implementation(implementation),
		getTypeFunction(utility::load<GetTypeSignature>(module, "getType")),
		destroyFunction(utility::load<DestroySignature>(module, "destroy"))
	{

	}

	CXXExecutor::ExecutorType CXXExecutor::getType() const
	{
		return getTypeFunction(implementation);
	}

	void CXXExecutor::destroy()
	{
		destroyFunction(implementation);
	}
}
