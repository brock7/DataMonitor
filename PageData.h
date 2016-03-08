#pragma once

#include "SyncUtil.h"
#include "DbgEng.h"

#define DARKLEING_TIME			10000

class PageData {
public:
	PageData(void);
	~PageData(void);

	size_t UpdateBytes(int offset, BYTE data[], size_t len, ULONG now);

	bool GetByte(int pos, ULONG now, BYTE& data)
	{
		data = _datas[pos].data;
		// return now - _datas[pos].hit < DARKLEING_TIME;
		return _datas[pos].hit != 0;
	}

	ULONG GetData(ULONG offset, BYTE data[], ULONG len)
	{
		ULONG i = 0;
		for (; i < len && offset + i < PAGE_SIZE; i ++) {
            data[i] = _datas[offset + i].data;
		}

		return i;
	}

	void Reset()
	{
		for (size_t i = 0; i < PAGE_SIZE; i ++) {
			_datas[i].data = 0;
			_datas[i].hit = 0;
			_datas[i].hitTimes = 0;
		}
	}

protected:

	struct ByteAttr {

		ByteAttr()
		{
			data = 0;
			hit = 0;
			hitTimes = 0;
		}

		BYTE	data;
		DWORD	hitTimes;
		ULONG	hit;

		operator BYTE()
		{
			return data;
		}
	};

	ByteAttr	_datas[PAGE_SIZE];
};

//////////////////////////////////////////////////////////////////////////

class PageMgr {
public:

	bool InsertPage(ULONG_PTR pageBound, ULONG bp);
	bool HitPage(ULONG_PTR pageBound, ULONG now, bool hitwrite);
	
	struct PageAttr {
		PageAttr(ULONG_PTR bound, ULONG bp)
		{
			pageBound = bound;
			hit = 0;
			hitTimes = 0;
			enabled = false;
			bpnum = bp;
			hitwrite = false;
		}

		enum {
			Normal, 
			Flash, 
			Highlight,
		};

		int TestPageHit(ULONG now)
		{
			if (now - hit > DARKLEING_TIME)
				return Normal;
			else {

				if (hitwrite)
					return Highlight;
				else
					return Flash;
			}
		}

		bool operator < (PageAttr& pageAttr)
		{
			return pageBound < pageAttr.pageBound;
		}

		ULONG_PTR		pageBound;
		ULONG			hit;
		ULONG			hitTimes;
		bool			enabled;
		ULONG			bpnum;
		bool			hitwrite;
	};

	typedef std::vector<PageAttr> PageList;

	PageList& GetPages()
	{
		return _pages;
	}

	size_t GetPageCount()
	{
		return _pages.size();
	}

	PageAttr* GetPage(ULONG num)
	{
		if (num >= _pages.size())
			return NULL;

		return &_pages[num];
	}

	bool IsValidPage(ULONG_PTR pageBound)
	{
		return FindPage(pageBound) != _pages.end();
	}

	void Reset()
	{
		_pages.clear();
	}

	void SortPages()
	{
		std::sort(_pages.begin(), _pages.end());
	}

	bool DeletePage(ULONG num)
	{
		if (num >= _pages.size())
			return false;

		_pages.erase(_pages.begin() + num);
		return true;
	}

	ULONG GetPageNum(ULONG_PTR pageBound)
	{
		PageList::iterator it = FindPage(pageBound);
		if (it == _pages.end())
			return -1;
        
		return it - _pages.begin();
	}

protected:
	PageList::iterator FindPage(ULONG_PTR pageBound)
	{
		PageList::iterator it;
		for (it = _pages.begin(); it != _pages.end(); it ++) {
            if (it->pageBound == pageBound)
				return it;
		}

		return _pages.end();
	}

protected:

	PageList		_pages;
};
