#ifndef _COMMAINITIALIZER_H_
#define _COMMAINITIALIZER_H_

#include <assert.h>

template<typename T, typename S>
struct CommaInitializer
{
	CommaInitializer(T& t, const S& s) : mT(t), mIndex(1)
	{
		mT[0] = s;
	}

	~CommaInitializer()
	{
		assert(mIndex == T::Size() && "Too few values passed to operator<<");
	}

	CommaInitializer& operator,(const S& s)
	{
		assert(mIndex < T::Size() && "Too many values passed to operator<<");
		
		if(mIndex < T::Size())
			mT[mIndex++] = s;

		return *this;
	}

	T& mT;
	size_t mIndex;
};

#endif
