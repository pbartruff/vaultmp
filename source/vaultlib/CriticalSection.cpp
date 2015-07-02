#include "CriticalSection.hpp"
#include "VaultException.hpp"

#ifdef VAULTMP_DEBUG
#include <sstream>
#endif

using namespace std;

#ifdef VAULTMP_DEBUG
string CriticalSection::thread_id(thread& t)
{
	ostringstream id;

	id << t.get_id();

	return id.str();
}

string CriticalSection::thread_id()
{
	ostringstream id;

	id << this_thread::get_id();

	return id.str();
}
#endif
