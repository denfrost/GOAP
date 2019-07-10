#include <stdlib.h>

#include "Scheduler.h"

#include "GOAP/Task.h"
#include "GOAP/Chain.h"
#include "GOAP/Source.h"
#include "GOAP/ChainProvider.h"

#include "TaskDelay.h"
#include "TaskPrint.h"
#include "TaskRoll.h"

#ifdef _WIN32
#   include <Windows.h>
#   define GOAP_SLEEP(Time) (::Sleep(Time))
#else
#   include <unistd.h>
#   define GOAP_SLEEP(Time) (::usleep(Time * 1000))
#endif

#include <time.h>
#include <math.h>

#include <array>
#include <string>
#include <typeinfo>

int main()
{
    Scheduler * sch = new Scheduler;

    srand( (unsigned int)time( NULL ) );

    printf( "%f %f %f\n", fmod( 0.5, 1.0 ), fmod( 1.3, 1.0 ), fmod( 3.0, 1.0 ) );

    GOAP::SourcePtr source = new GOAP::Source();

    source->addTask( new TaskPrint( "begin" ) );
    source->addTask( new TaskDelay( 2000.f, sch ) );

    std::vector<int> v;
    v.push_back( 1 );
    v.push_back( 3 );
    v.push_back( 5 );
    v.push_back( 7 );

    for( auto zip : source->addParallelZip( v ) )
    {
        std::string msg = std::to_string( *zip.value );

        zip.source->addTask( new TaskPrint( msg ) );
    }

    auto [parallel0, parallel1] = source->addParallel<2>();

    parallel0->addTask( new TaskDelay( 1000.f, sch ) );
    parallel1->addTask( new TaskPrint( "process" ) );

    source->addTask( new TaskPrint( "end" ) );
    source->addTask( new TaskDelay( 1000.f, sch ) );
    source->addTask( new TaskPrint( "****ROLL*****" ) );
    source->addTask( new TaskDelay( 1000.f, sch ) );


    auto [race0, race1, race2] = source->addRace<3>();

    race0->addTask( new TaskRoll( 200.f, 1, 6, sch ) );
    race0->addTask( new TaskPrint( "---1---" ) );

    race1->addTask( new TaskRoll( 100.f, 2, 12, sch ) );
    race1->addTask( new TaskPrint( "---2---" ) );

    race2->addTask( new TaskRoll( 50.f, 4, 24, sch ) );
    race2->addTask( new TaskPrint( "---3---" ) );

    source->addTask( new TaskDelay( 1000.f, sch ) );
    source->addTask( new TaskPrint( "****WIN*****" ) );
    source->addTask( new TaskDelay( 1000.f, sch ) );

    source->addCallback( []( const GOAP::CallbackObserverPtr & _observer, bool isSkip )
    {
        printf( "HTTP!!!!!\n" ); 
        
        GOAP_SLEEP( 100 ); 
        
        _observer->onCallback( isSkip );
    } );

    source->addFunction( []()
    {
        printf( "WOW!!\n" );
    } );

    source->addScope( []( const GOAP::SourcePtr & _scope ) -> bool
    {
        _scope->addFunction( []()
        {
            printf( "SCOPE????? WOW!!!" );
        } );
        
        return true;
    } );

    source->addFunction( []()
    {
        printf( "Oh\n" );
    } );

    auto [source_true, source_false] = source->addIf( []()
    {
        return rand() % 2 ? true : false;
    } );

    source_true->addTask( new TaskPrint( "---TRUE---" ) );
    source_false->addTask( new TaskPrint( "---FALSE---" ) );

    const GOAP::VectorSources & source_switch = source->addSwitch( 3, []()
    {
        return rand() % 3;
    } );

    source_switch[0]->addTask( new TaskPrint( "---Switch 1---" ) );
    source_switch[1]->addTask( new TaskPrint( "---Switch 2---" ) );
    source_switch[2]->addTask( new TaskPrint( "---Switch 3---" ) );

    source->addFor( 10, [sch]( const GOAP::SourcePtr & _scope, uint32_t _iterator, uint32_t _count )
    {
        _scope->addTask( new TaskDelay( 500.f, sch ) );
        _scope->addTask( new TaskPrint( "For!!!!" ) );

        return true;
    } );

    uint32_t count = 0;
    source->addWhile( [sch, &count]( const GOAP::SourcePtr & _scope )
    {
        _scope->addTask( new TaskDelay( 1000.f, sch ) );
        _scope->addTask( new TaskPrint( "While!!!!" ) );

        if( ++count == 5 )
        {
            return false;
        }

        return true;
    } );

    GOAP::SourcePtr source_until = source->addRepeat( [sch]( const GOAP::SourcePtr & _scope )
    {
        _scope->addTask( new TaskDelay( 1000.f, sch ) );
        _scope->addTask( new TaskPrint( "REPEAT!!!!" ) );

        return true;
    } );

    source_until->addTask( new TaskDelay( 10000.f, sch ) );

    GOAP::ChainPtr tc = new GOAP::Chain( source );

    tc->run();

    while( tc->isComplete() == false )
    {
        sch->update( 100.f );

        GOAP_SLEEP( 10 );
    }

    printf( "FINALIZE\n" );

    delete sch;

    return 0;
}