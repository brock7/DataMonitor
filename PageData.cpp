#include "StdAfx.h"
#include ".\pagedata.h"

PageData::PageData(void)
{
}

PageData::~PageData(void)
{
}

size_t PageData::UpdateBytes(int offset, BYTE data[], size_t len, ULONG now)
{
	int n = offset;
	size_t i = 0;

	if (now == 0) {

		for (; i < len && n < PAGE_SIZE; i ++, n ++ ) {
			_datas[n].data = data[i];
			_datas[n].hit = 0;
			_datas[n].hitTimes ++;
		}

	} else {

		for (; i < len && n < PAGE_SIZE; i ++, n ++ ) {

			if (_datas[n].data != data[i]) {
				_datas[n].data = data[i];
				_datas[n].hit = now;
				_datas[n].hitTimes ++;
			} else // added at last
				_datas[n].hit = 0;
		}
	}

	return i;
}

//////////////////////////////////////////////////////////////////////////

bool PageMgr::InsertPage(ULONG_PTR pageBound, ULONG bp)
{
	if (FindPage(pageBound) != _pages.end())
		return false;
	_pages.push_back(PageAttr(pageBound, bp));
	return true;
}

bool PageMgr::HitPage(ULONG_PTR pageBound, ULONG now, bool hitwrite)
{
	PageList::iterator it = FindPage(pageBound);
	if (it == _pages.end())
		return false;

	it->hit = now;
	it->hitTimes ++;
	it->hitwrite = hitwrite;

	return true;
}
