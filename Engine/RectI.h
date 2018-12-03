#pragma once

#include "Vei2.h"
#include <algorithm>

class RectI
{
public:
	RectI() = default;
	RectI( int left_in,int right_in,int top_in,int bottom_in );
	RectI( const Vei2& topLeft,const Vei2& bottomRight );
	RectI( const Vei2& topLeft,int width,int height );
	
	static RectI FromCenter( const Vei2& center,int halfWidth,int halfHeight );
	RectI GetExpanded( int offset ) const;
	Vei2 GetCenter() const;
	RectI& ClipTo(const RectI& clip)
	{
		left = std::max(left, clip.left);
		right = std::min(right, clip.right);
		top = std::max(top, clip.top);
		bottom = std::min(bottom, clip.bottom);
		return *this;
	}
	RectI GetClippedTo(const RectI& clip) const
	{
		return RectI(*this).ClipTo(clip);
	}
	bool IsOverlappingWith(const RectI& other) const
	{
		return right > other.left && left < other.right
			&& bottom > other.top && top < other.bottom;
	}
	bool IsContainedBy(const RectI& other) const
	{
		return left >= other.left && right <= other.right &&
			top >= other.top && bottom <= other.bottom;
	}
	bool Contains(const Vei2& point) const
	{
		return point.x >= left && point.x < right && point.y >= top && point.y < bottom;
	}
	// degenerate means top is below or equal bottom / left is right of or equal right
	bool IsDegenerate() const
	{
		return right <= left || bottom <= top;
	}
	int GetWidth() const
	{
		return right - left;
	}
	int GetHeight() const
	{
		return bottom - top;
	}
	Vei2 TopLeft() const
	{
		return{ left,top };
	}
	Vei2 BottomRight() const
	{
		return{ right,bottom };
	}
	RectI& DisplaceBy(const Vei2 d)
	{
		left += d.x;
		right += d.x;
		top += d.y;
		bottom += d.y;
		return *this;
	}
	RectI GetDisplacedBy(const Vei2& d) const
	{
		return RectI(*this).DisplaceBy(d);
	}

public:
	int left;
	int right;
	int top;
	int bottom;
};