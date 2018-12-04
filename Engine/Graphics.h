/******************************************************************************************
*	Chili DirectX Framework Version 16.07.20											  *
*	Graphics.h																			  *
*	Copyright 2016 PlanetChili <http://www.planetchili.net>								  *
*																						  *
*	This file is part of The Chili DirectX Framework.									  *
*																						  *
*	The Chili DirectX Framework is free software: you can redistribute it and/or modify	  *
*	it under the terms of the GNU General Public License as published by				  *
*	the Free Software Foundation, either version 3 of the License, or					  *
*	(at your option) any later version.													  *
*																						  *
*	The Chili DirectX Framework is distributed in the hope that it will be useful,		  *
*	but WITHOUT ANY WARRANTY; without even the implied warranty of						  *
*	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the						  *
*	GNU General Public License for more details.										  *
*																						  *
*	You should have received a copy of the GNU General Public License					  *
*	along with The Chili DirectX Framework.  If not, see <http://www.gnu.org/licenses/>.  *
******************************************************************************************/
#pragma once
#include "ChiliWin.h"
#include <d3d11.h>
#include <wrl.h>
#include "ChiliException.h"
#include "Colors.h"
#include "Surface.h"
#include "RectI.h"
#include <cassert>

class Graphics
{
public:
	class Exception : public ChiliException
	{
	public:
		Exception( HRESULT hr,const std::wstring& note,const wchar_t* file,unsigned int line );
		std::wstring GetErrorName() const;
		std::wstring GetErrorDescription() const;
		virtual std::wstring GetFullMessage() const override;
		virtual std::wstring GetExceptionType() const override;
	private:
		HRESULT hr;
	};
private:
	// vertex format for the framebuffer fullscreen textured quad
	struct FSQVertex
	{
		float x,y,z;		// position
		float u,v;			// texcoords
	};
public:
	Graphics( class HWNDKey& key );
	Graphics( const Graphics& ) = delete;
	Graphics& operator=( const Graphics& ) = delete;
	void EndFrame();
	void BeginFrame( Color bg = Colors::Black );
	Color GetPixel( int x,int y ) const;

	RectI GetRect() const;
	void PutPixel(int x, int y, int r, int g, int b)
	{
		PutPixel(x, y, { unsigned char(r),unsigned char(g),unsigned char(b) });
	}
	void PutPixel(int x, int y, Color c);
	void DrawRect(int x0, int y0, int x1, int y1, Color c);
	void DrawRect(const RectI& rect, Color c)
	{
		DrawRect(rect.left, rect.top, rect.right, rect.bottom, c);
	}
	void DrawRectThin( const RectI& rect,Color color,const RectI& clip = GetScreenRect() )
	{
		// get clipped version of rectangle
		const auto clipped = rect.GetClippedTo( clip );
		// don't bother with degenerate rects
		// (this will also skip rects outside of clip)
		if( clipped.IsDegenerate() )
		{
			return;
		}
		// top line
		if( rect.top >= clip.top )
		{
			for( int x = clipped.left; x < clipped.right; x++ )
			{
				PutPixel( x,rect.top,color );
			}
		}
		// bottom line
		if( rect.bottom <= clip.bottom )
		{
			for( int x = clipped.left; x < clipped.right; x++ )
			{
				PutPixel( x,rect.bottom - 1,color );
			}
		}
		// left line
		if( rect.left >= clip.left )
		{
			for( int y = clipped.top; y < clipped.bottom; y++ )
			{
				PutPixel( rect.left,y,color );
			}
		}
		// right line
		if( rect.right <= clip.right )
		{
			for( int y = clipped.top; y < clipped.bottom; y++ )
			{
				PutPixel( rect.right - 1,y,color );
			}
		}
	}
	
	template<typename E>
	void DrawSprite( int x,int y,const Surface& s,E effect,bool reversed = false )
	{
		DrawSprite( x,y,s.GetRect(),s,effect,reversed );
	}
	template<typename E>
	void DrawSprite( int x,int y,const RectI& srcRect,const Surface& s,E effect,bool reversed = false )
	{
		DrawSprite( x,y,srcRect,GetScreenRect(),s,effect,reversed );
	}
	template<typename E>
	void DrawSprite( int x,int y,RectI srcRect,const RectI& clip,const Surface& s,E effect,bool reversed = false )
	{
		assert( srcRect.left >= 0 );
		assert( srcRect.right <= s.GetWidth() );
		assert( srcRect.top >= 0 );
		assert( srcRect.bottom <= s.GetHeight() );

		// mirror in x depending on reversed bool switch
		if( !reversed )
		{
			// clipping is different depending on mirroring status
			if( x < clip.left )
			{
				srcRect.left += clip.left - x;
				x = clip.left;
			}
			if( y < clip.top )
			{
				srcRect.top += clip.top - y;
				y = clip.top;
			}
			if( x + srcRect.GetWidth() > clip.right )
			{
				srcRect.right -= x + srcRect.GetWidth() - clip.right;
			}
			if( y + srcRect.GetHeight() > clip.bottom )
			{
				srcRect.bottom -= y + srcRect.GetHeight() - clip.bottom;
			}
			for( int sy = srcRect.top; sy < srcRect.bottom; sy++ )
			{
				for( int sx = srcRect.left; sx < srcRect.right; sx++ )
				{
					effect(
						// no mirroring
						s.GetPixel( sx,sy ),
						x + sx - srcRect.left,
						y + sy - srcRect.top,
						*this
					);
				}
			}
		}
		else
		{
			if( x < clip.left )
			{
				srcRect.right -= clip.left - x;
				x = clip.left;
			}
			if( y < clip.top )
			{
				srcRect.top += clip.top - y;
				y = clip.top;
			}
			if( x + srcRect.GetWidth() > clip.right )
			{
				srcRect.left += x + srcRect.GetWidth() - clip.right;
			}
			if( y + srcRect.GetHeight() > clip.bottom )
			{
				srcRect.bottom -= y + srcRect.GetHeight() - clip.bottom;
			}
			const int xOffset = srcRect.left + srcRect.right - 1;
			for( int sy = srcRect.top; sy < srcRect.bottom; sy++ )
			{
				for( int sx = srcRect.left; sx < srcRect.right; sx++ )
				{
					effect(
						// mirror in x
						s.GetPixel( xOffset - sx,sy ),
						x + sx - srcRect.left,
						y + sy - srcRect.top,
						*this
					);
				}
			}
		}
	}

	~Graphics();
private:
	Microsoft::WRL::ComPtr<IDXGISwapChain>				pSwapChain;
	Microsoft::WRL::ComPtr<ID3D11Device>				pDevice;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext>			pImmediateContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView>		pRenderTargetView;
	Microsoft::WRL::ComPtr<ID3D11Texture2D>				pSysBufferTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>	pSysBufferTextureView;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>			pPixelShader;
	Microsoft::WRL::ComPtr<ID3D11VertexShader>			pVertexShader;
	Microsoft::WRL::ComPtr<ID3D11Buffer>				pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>			pInputLayout;
	Microsoft::WRL::ComPtr<ID3D11SamplerState>			pSamplerState;
	D3D11_MAPPED_SUBRESOURCE							mappedSysBufferTexture;
	Color*                                              pSysBuffer = nullptr;
public:
	static constexpr int ScreenWidth = 1600;
	static constexpr int ScreenHeight = 1400;
	static RectI GetScreenRect();
};