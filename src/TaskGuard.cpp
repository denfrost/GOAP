#	include "GOAP/TaskGuard.h"
#	include "GOAP/GuardProvider.h"

namespace GOAP
{
	//////////////////////////////////////////////////////////////////////////
	TaskGuard::TaskGuard(const GuardProviderPtr & _begin, const GuardProviderPtr & _end)
        : Task( TASK_EVENT_RUN | TASK_EVENT_CANCEL )
		, m_begin(_begin)
		, m_end(_end)
	{}
	//////////////////////////////////////////////////////////////////////////
	TaskGuard::~TaskGuard()
	{}
	//////////////////////////////////////////////////////////////////////////
	bool TaskGuard::_onRun()
	{		
		m_begin->onGuard();

		return false;
	}
	//////////////////////////////////////////////////////////////////////////
	void TaskGuard::_onCancel()
	{
		m_end->onGuard();
	}
}
