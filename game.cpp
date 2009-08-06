//==============================================================================

#include <game.hpp>
#include <engine.hpp>
#include <entity.hpp>
#include <entity_manager.hpp>
#include <viewport.hpp>
#include <score.hpp>

#include <hgeresource.h>

#include <algorithm>
#include <set>

namespace
{
    int PIECE[7][8] = {
        { 0,1,1,1,2,1,3,1 },    // I
        { 0,1,0,2,1,2,2,2 },    // J
        { 1,2,2,2,3,2,3,1 },    // L
        { 1,1,1,2,2,1,2,2 },    // O
        { 0,2,1,2,1,1,2,1 },    // S
        { 0,1,1,1,2,1,1,2 },    // T
        { 1,1,2,1,2,2,3,2 }     // Z
    };
}

//==============================================================================
Game::Game()
    :
    Context(),
    m_arena(),
    m_piece(),
    m_index( 0 ),
    m_rotate( 0 )
{
}

//------------------------------------------------------------------------------
Game::~Game()
{
}

//------------------------------------------------------------------------------
// public:
//------------------------------------------------------------------------------
void
Game::init()
{
    HGE * hge( Engine::hge() );
    b2World * b2d( Engine::b2d() );
    hgeResourceManager * rm( Engine::rm() );
    ViewPort * vp( Engine::vp() );

    Engine::em()->init();

    vp->offset().x = 0.0f;
    vp->offset().y = 0.0f;
    vp->centre().x = 0.0f;
    vp->centre().y = 0.0f;
    vp->bounds().x = 1280.0f;
    vp->bounds().y = 720.0f;
    vp->setAngle( 0.0f );
    vp->setScale( 10.0f );
    
    for ( int x = 0; x < 10; ++x )
    {
        for ( int y = 0; y < 20; ++y )
        {
            m_arena[x][y] = false;
        }
    }

    clearPiece();

    m_index = 0;
    m_rotate = 0;
}

//------------------------------------------------------------------------------
void
Game::fini()
{
    Engine::em()->fini();
}

//------------------------------------------------------------------------------
bool
Game::update( float dt )
{
    const Controller & pad( Engine::instance()->getController() );
    HGE * hge( Engine::hge() );
    ViewPort * vp( Engine::vp() );

    bool paused( Engine::instance()->isPaused() );

    clearPiece();
    for ( int i = 0; i < 8; i += 2 )
    {
        int x( PIECE[m_index][i] ), y( PIECE[m_index][i+1] );
        for ( int j = 0; j < m_rotate; ++j )
        {
            b2Swap( x, y );
            y = 3 - y;
        }
        m_piece[x][y]= true;
    }

    if ( ! paused && pad.isConnected() )
    {
        if ( pad.buttonDown( XPAD_BUTTON_LEFT_SHOULDER ) )
        {
            m_index = ( m_index + 6 ) % 7;
        }
        if ( pad.buttonDown( XPAD_BUTTON_RIGHT_SHOULDER ) )
        {
            m_index = ( m_index + 1 ) % 7;
        }
        if ( pad.buttonDown( XPAD_BUTTON_DPAD_UP ) )
        {
            m_rotate = ( m_rotate + 3 ) % 4;
        }
        if ( pad.buttonDown( XPAD_BUTTON_DPAD_DOWN ) )
        {
            m_rotate = ( m_rotate + 1 ) % 4;
        }
    }

    return false;
}

//------------------------------------------------------------------------------
void
Game::render()
{
    hgeResourceManager * rm( Engine::rm() );
    hgeFont* font = Engine::rm()->GetFont("menu");
    ViewPort * vp( Engine::vp() );
    
    vp->setTransform();

    float scale( 1.0f );
    std::vector< Entity * > entities;
    for ( b2Body * body( Engine::b2d()->GetBodyList() ); body != NULL;
          body = body->GetNext() )
    {
        Entity * entity( static_cast< Entity * >( body->GetUserData() ) );
        if ( entity )
        {
            entity->render( scale );
        }
    }

    Engine::vp()->reset();
    vp->setTransform();

    hgeSprite * black( rm->GetSprite( "empty" ) );
    hgeSprite * white( rm->GetSprite( "tile" ) );

    for ( int x = 0; x < 10; ++x )
    {
        for ( int y = 0; y < 20; ++y )
        {
            hgeSprite * sprite( m_arena[x][y] ? white : black );
            sprite->RenderEx( ( x - 4.5f  ) * 3.2f,
                              ( y - 9.5f ) * 3.2f,
                              0.0f, 0.1f );
        }
    }
    for ( int x = 0; x < 4; ++x )
    {
        for ( int y = 0; y < 4; ++y )
        {
            hgeSprite * sprite( m_piece[x][y] ? white : black );
            sprite->RenderEx( ( x - 1.5f + 12.0f ) * 3.2f,
                              ( y - 1.5f ) * 3.2f,
                              0.0f, 0.1f );
        }
    }
}

//------------------------------------------------------------------------------
//private:
//------------------------------------------------------------------------------
void
Game::clearPiece()
{
    for ( int x = 0; x < 4; ++x )
    {
        for ( int y = 0; y < 4; ++y )
        {
            m_piece[x][y] = false;
        }
    }
}

//==============================================================================
