#pragma once

namespace Util {

class SyncObj {
public:

	enum {

		Infinte = INFINITE
	};

	virtual ~SyncObj()
	{

	}

	virtual void lock() const = 0;
	virtual void unlock() const = 0;

	virtual bool lockEx(unsigned long timeo)
	{
		assert(false);
		return false;
	}
};

class Autolock {
public:
	Autolock(const SyncObj& sync): _sync(sync)
	{
		sync.lock();
	}

	~Autolock()
	{
		_sync.unlock();
	}

	const SyncObj&	_sync;
};

class LightLock: public SyncObj {
public:
	LightLock()
	{
        InitializeCriticalSection(&_cs);
	}

	virtual ~LightLock()
	{
        DeleteCriticalSection(&_cs);
	}

	virtual void lock() const
	{
		EnterCriticalSection(&_cs);
	}

	virtual void unlock() const
	{
		LeaveCriticalSection(&_cs);
	}

	virtual bool lockEx(unsigned long timeo)
	{
		if (timeo == Infinte) {
			EnterCriticalSection(&_cs);
			return true;

		} else {
			return TryEnterCriticalSection(&_cs) == TRUE;
		}
	}

protected:

	mutable CRITICAL_SECTION	_cs;
};

/*
class RWLock: protected LightLock {
public:

	class RLock: public SyncObj {
	public:
		RLock(const RWLock& rwlock): _rwlock(rwlock)
		{

		}

		virtual void lock() const
		{
			_rwlock.lockread();
		}

		virtual void unlock() const
		{
			_rwlock.unlockread();
		}

	protected:
		const RWLock&		_rwlock;
	};

	class WLock: public SyncObj {
	public:
		WLock(const RWLock& rwlock): _rwlock(rwlock)
		{

		}

		virtual void lock() const
		{
            _rwlock.lockwrite();
		}

		virtual void unlock() const
		{
			_rwlock.unlockwrite();
		}

	protected:
		const RWLock&		_rwlock;
	};

	RWLock(): _rlock(*this), _wlock(*this)
	{
		_datalock = CreateSemaphore(NULL, 1, 1, NULL);
		_rlockCount = NULL;
	}

	virtual ~RWLock()
	{
		CloseHandle(_datalock);
	}

	const WLock& wlock() const
	{
		return _wlock;
	}

	const RLock& rlock()  const
	{
		return _rlock;
	}

	void lockread() const
	{
		lock();
        if (_rlockCount == 0)
			WaitForSingleObject(_datalock, INFINITE);        

		_rlockCount ++;
		unlock();
	}

	void unlockread() const
	{
		lock();
		if (_rlockCount > 0) {
			_rlockCount --;
			if (_rlockCount == 0) {
                BOOL r = ReleaseSemaphore(_datalock, 1, NULL);
				assert(r);
			}
		}

		unlock();
	}

	void lockwrite() const
	{
        WaitForSingleObject(_datalock, INFINITE);
	}

	void unlockwrite() const
	{
		BOOL r = ReleaseSemaphore(_datalock, 1, NULL);
		assert(r);
	}

	friend class RWLock::RLock;
	friend class RWLock::WLock;

protected:
	mutable RLock	_rlock;
	mutable WLock	_wlock;

	mutable long	_rlockCount;
	mutable HANDLE	_datalock;
};
*/

} // napespace GsUtil {
