#define GOD_GRAPHICS
#include "Graphics.h"

void Graphics::BeginFrame()
{
	// clear the sysbuffer
	memset( pSysBuffer,0u,sizeof( Color ) * Graphics::ScreenHeight * Graphics::ScreenWidth );
}

void Graphics::PutPixel( int x,int y,Color c )
{
	assert( x >= 0 );
	assert( x < int( Graphics::ScreenWidth ) );
	assert( y >= 0 );
	assert( y < int( Graphics::ScreenHeight ) );
	pSysBuffer[Graphics::ScreenWidth * y + x] = c;
}

void Graphics::DrawStright(Vei2 p1, Vei2 p2, Color c){
	if(p1.x != p2.x){
		if(p1.x > p2.x)
			std::swap(p1, p2);

		for(int x = p1.x; x < p2.x; x++)
			if(p1.y >= 0 && p1.y < ScreenHeight && x >= 0 && x < ScreenWidth)
				PutPixel(x, p1.y, c);
	}
	else
	{
		if(p1.y > p2.y)
			std::swap(p1, p2);

		for(int y = p1.y; y < p2.y; y++)
			if(y >= 0 && y < ScreenHeight && p1.x >= 0 && p1.x < ScreenWidth)
				PutPixel(p1.x, y, c);
	}
};

void Graphics::DrawFilledRect(Vei2 pos, Vei2 dims, Color c){
	for(int k = pos.y; k < pos.y + dims.y; k++)
		for(int i = pos.x; i < pos.x + dims.x; i++){
			if(k >= 0 && k < ScreenHeight && i >= 0 && i < ScreenWidth)
				PutPixel(i, k, c);
		}
}

void Graphics::DrawFilledRectBorder(Vei2 pos, Vei2 dims, Color c, Color borderC){
	DrawFilledRect(pos, dims, c);
	DrawStright(pos, {pos.x + dims.x, pos.y}, borderC);
	DrawStright(pos, {pos.x, pos.y + dims.y}, borderC);
	DrawStright(pos + dims, {pos.x + dims.x, pos.y}, borderC);
	DrawStright(pos + dims, {pos.x, pos.y + dims.y}, borderC);

}

Color Graphics::GetPixel( int x,int y ) const
{
	assert( x >= 0 );
	assert( x < int( Graphics::ScreenWidth ) );
	assert( y >= 0 );
	assert( y < int( Graphics::ScreenHeight ) );
	return pSysBuffer[Graphics::ScreenWidth * y + x];
}

void dummy( Graphics& gfx )
{
	gfx.DrawSprite( 0,0,RectI{ 0,0,0,0 },RectI{ 0,0,0,0 },Surface{},SpriteEffect::Copy{} );
	gfx.DrawSprite( 0,0,RectI{ 0,0,0,0 },RectI{ 0,0,0,0 },Surface{},SpriteEffect::Chroma{ Colors::Black } );
	gfx.DrawSprite( 0,0,RectI{ 0,0,0,0 },RectI{ 0,0,0,0 },Surface{},SpriteEffect::Substitution{ Colors::Black,Colors::Black } );
}
