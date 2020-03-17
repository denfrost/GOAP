#include <stdlib.h>

#include "Scheduler.h"

#include "GOAP/GOAP.h"

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

//////////////////////////////////////////////////////////////////////////
class MyAllocator
    : public GOAP::Allocator
{
protected:
    void * allocate( size_t _size ) override
    {
        void * p = ::malloc( _size );

        return p;
    }
    
    void deallocate( void * _ptr ) override
    {
        ::free( _ptr );
    }
};
//////////////////////////////////////////////////////////////////////////
void test( GOAP::Allocator * _allocator )
{
    GOAP::KernelInterfacePtr kernel = GOAP::Helper::makeKernel( _allocator );

    SchedulerPtr sch = SchedulerPtr::from( new Scheduler );

    ::srand( (unsigned int)time( nullptr ) );

    printf( "%f %f %f\n"
        , ::fmod( 0.5, 1.0 )
        , ::fmod( 1.3, 1.0 )
        , ::fmod( 3.0, 1.0 )
    );

    GOAP::SourceInterfacePtr source = kernel->makeSource();

    GOAP::Cook::addTask<TaskPrint>( source, "begin" );
    GOAP::Cook::addTask<TaskDelay>( source, _allocator, 2000.f, sch );

    std::vector<int> v;
    v.push_back( 1 );
    v.push_back( 3 );
    v.push_back( 5 );
    v.push_back( 7 );

    for( auto && [value_source, value] : GOAP::Cook::addParallelZip( source, v ) )
    {
        GOAP::Cook::addTask<TaskPrint>( value_source, "zip [%d]", value );
    }

    auto && [parallel0, parallel1] = GOAP::Cook::addParallel<2>( source );

    GOAP::Cook::addTask<TaskDelay>( parallel0, _allocator, 1000.f, sch );
    GOAP::Cook::addTask<TaskPrint>( parallel1, "process" );

    GOAP::Cook::addTask<TaskPrint>( source, "end" );
    GOAP::Cook::addTask<TaskDelay>( source, _allocator, 1000.f, sch );
    GOAP::Cook::addTask<TaskPrint>( source, "****ROLL*****" );
    GOAP::Cook::addTask<TaskDelay>( source, _allocator, 1000.f, sch );

    auto && [race0, race1, race2] = GOAP::Cook::addRace<3>( source );

    GOAP::Cook::addTask<TaskRoll>( race0, _allocator, 200.f, 1, 6, sch );
    GOAP::Cook::addTask<TaskPrint>( race0, "---1---" );

    GOAP::Cook::addTask<TaskRoll>( race1, _allocator, 100.f, 2, 12, sch );
    GOAP::Cook::addTask<TaskPrint>( race1, "---2---" );

    GOAP::Cook::addTask<TaskRoll>( race2, _allocator, 50.f, 4, 24, sch );
    GOAP::Cook::addTask<TaskPrint>( race2, "---3---" );

    GOAP::Cook::addTask<TaskDelay>( source, _allocator, 1000.f, sch );
    GOAP::Cook::addTask<TaskPrint>( source, "****WIN*****" );
    GOAP::Cook::addTask<TaskDelay>( source, _allocator, 1000.f, sch );

    GOAP::Cook::addCallback( source, []( const GOAP::CallbackObserverPtr & _observer, bool isSkip )
    {
        printf( "HTTP!!!!!\n" );

        GOAP_SLEEP( 100 );

        _observer->onCallback( isSkip );
    } );

    GOAP::Cook::addFunction( source, []()
    {
        printf( "WOW!!\n" );
    } );

    GOAP::Cook::addScope( source, []( const GOAP::SourceInterfacePtr & _scope ) -> bool
    {
        GOAP::Cook::addFunction( _scope, []()
        {
            printf( "SCOPE????? WOW!!!" );
        } );

        return true;
    } );

    GOAP::Cook::addFunction( source, []()
    {
        printf( "Oh\n" );
    } );

    auto [source_true, source_false] = GOAP::Cook::addIf( source, []()
    {
        return ::rand() % 2 ? true : false;
    } );

    GOAP::Cook::addTask<TaskPrint>( source_true, "---TRUE---" );
    GOAP::Cook::addTask<TaskPrint>( source_false, "---FALSE---" );

    const GOAP::ViewSources<GOAP::SourceInterface> & source_switch = GOAP::Cook::addSwitch( source, 3, []()
    {
        return ::rand() % 3;
    } );

    GOAP::Cook::addTask<TaskPrint>( source_switch[0], "---Switch 1---" );
    GOAP::Cook::addTask<TaskPrint>( source_switch[1], "---Switch 2---" );
    GOAP::Cook::addTask<TaskPrint>( source_switch[2], "---Switch 3---" );

    GOAP::Cook::addFor( source, 10, [sch, _allocator]( const GOAP::SourceInterfacePtr & _scope, uint32_t _iterator, uint32_t _count )
    {
        GOAP_UNUSED( _iterator );
        GOAP_UNUSED( _count );

        GOAP::Cook::addTask<TaskDelay>( _scope, _allocator, 500.f, sch );
        GOAP::Cook::addTask<TaskPrint>( _scope, "For!!!!" );

        return true;
    } );

    uint32_t count = 0;
    GOAP::Cook::addWhile( source, [sch, _allocator, &count]( const GOAP::SourceInterfacePtr & _scope )
    {
        GOAP::Cook::addTask<TaskDelay>( _scope, _allocator, 1000.f, sch );
        GOAP::Cook::addTask<TaskPrint>( _scope, "While!!!!" );

        if( ++count == 5 )
        {
            return false;
        }

        return true;
    } );

    GOAP::TimerInterfacePtr timer = kernel->makeTimer();

    GOAP::Cook::addGenerator( source, timer, []( uint32_t _iterator )
    {
        if( _iterator == 10 )
        {
            return -1.f;
        }

        return 100.f;
    }, [_allocator, sch]( const GOAP::SourceInterfacePtr & _source, uint32_t _iterator, float _delay )
    {
        GOAP::Cook::addTask<TaskPrint>( _source, "Gen [%d] time (%f) go!", _iterator, _delay );
        GOAP::Cook::addTask<TaskDelay>( _source, _allocator, 1000.f, sch );
        GOAP::Cook::addTask<TaskPrint>( _source, "Gen [%d] time (%f) end!", _iterator, _delay );
    } );

    GOAP::Cook::addTask<TaskPrint>( source, "Generator [COMPLETE]" );

    GOAP::SourceInterfacePtr source_until = GOAP::Cook::addRepeat( source, [_allocator, sch]( const GOAP::SourceInterfacePtr & _scope )
    {
        GOAP::Cook::addTask<TaskDelay>( _scope, _allocator, 1000.f, sch );
        GOAP::Cook::addTask<TaskPrint>( _scope, "REPEAT!!!!" );

        return true;
    } );

    GOAP::Cook::addTask<TaskDelay>( source_until, _allocator, 10000.f, sch );

    GOAP::ChainInterfacePtr tc = kernel->makeChain( source, __FILE__, __LINE__ );

    tc->run();

    while( tc->isComplete() == false )
    {
        timer->update( 100.f );
        sch->update( 100.f );

        GOAP_SLEEP( 10 );
    }

    printf( "FINALIZE\n" );
}
//////////////////////////////////////////////////////////////////////////
int main()
{
    MyAllocator * allocator = new MyAllocator;

    test( allocator );

    delete allocator;

    return 0;
}