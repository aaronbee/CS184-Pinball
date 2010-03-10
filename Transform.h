// Transform.h: interface for the Transform class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_Transform_H__C38D00E4_6A44_4E5D_9394_36D0650369D0__INCLUDED_)
#define AFX_Transform_H__C38D00E4_6A44_4E5D_9394_36D0650369D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "nv/nv_math.h"
#include "nv/nv_algebra.h"

class Transform  
{
public:
	Transform();
	virtual ~Transform();
	static void left(float degrees, vec3& eye, vec3& up);
	static void up(float degrees, vec3& eye, vec3& up);
};

#endif // !defined(AFX_Transform_H__C38D00E4_6A44_4E5D_9394_36D0650369D0__INCLUDED_)
