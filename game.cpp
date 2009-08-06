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

//==============================================================================
Game::Game()
    :
    Context(),
    m_arena()
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
            sprite->RenderEx( ( x - 4.5  ) * 3.2f, ( y - 9.5 ) * 3.2f,
                              0.0f, 0.1f );
        }
    }
}

//==============================================================================
