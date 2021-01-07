#include "TaskGraph.h"

class CTask::CTaskWaiter
{
	friend class CTaskGraph;
	using FutureType = std::future<void>;
public:
	void AddFuture(FutureType& fut)
	{
		std::lock_guard<std::mutex> lock_guard(m_lock);
		m_futures.push(Move(fut));
	}

	void Wait()
	{
		FutureType fut;
		while(FetchFuture(fut))
		{
			fut.wait();
		}
	}
protected:
	CTaskWaiter() = default;
	bool FetchFuture(FutureType& fut)
	{
		std::lock_guard<std::mutex> lock_guard(m_lock);
		if (!m_futures.empty())
		{
			fut = move(m_futures.front());
			m_futures.pop();
			return true;
		}
		return false;
	}
	
private:
	std::mutex m_lock;
	std::queue<FutureType> m_futures;
};


std::string CTask::GetTaskName()
{
	return m_task_name;
}

CTask::CTask(std::string task_name,  FuncType func): m_task_name(task_name), m_func(func)
{
}

void CTask::Start(CTaskWaiter& waiter)
{
	auto f = [&]()
	{
		Print("run task: %s\n", m_task_name.data());
		m_func();
		for (auto& sub_task : m_sub_tasks)
		{
			if (sub_task->DecPrevCount() == 1)
			{
				sub_task->Start(waiter);
			}
		}
	};
	auto fut = std::async(std::launch::async, f);
	waiter.AddFuture(Move(fut));
}

void CTask::Connect(CTask* next)
{
	next->m_prev_count += 1;
	m_sub_tasks.push_back(next);
}

int CTask::DecPrevCount()
{
	return m_prev_count.fetch_sub(1);
}

CTask* CTaskGraph::CreateTask(std::string task_name, CTask::FuncType func)
{
	Assert(m_tasks.find(task_name) == m_tasks.end(), "");
	m_tasks[task_name] = std::unique_ptr<CTask>(new CTask(task_name, func));
	return m_tasks[task_name].get();
}

CTask* CTaskGraph::GetTask(std::string task_name)
{
	return m_tasks[task_name].get();
}

void CTaskGraph::Connect(CTask* prev, CTask* next)
{
	Assert(prev != next, "");
	Assert(GetTask(prev->m_task_name) == prev, "");
	Assert(GetTask(next->m_task_name) == next, "");
	prev->Connect(next);
	Assert(!ExistLoop(), "Exist Loop %s ==> %s", prev->GetTaskName().data(), next->GetTaskName().data());
}

void CTaskGraph::Run()
{
	CTask::CTaskWaiter waiter;
	Print("begin run task\n");
	for (auto& task : GetHeadTasks())
	{
		task->Start(waiter);
	}
	waiter.Wait();
	Print("finish run task\n");
}

std::vector<CTask*> CTaskGraph::GetHeadTasks()
{
	std::vector<CTask*> head_tasks;
	for (auto& task_pair : m_tasks)
	{
		if (task_pair.second->m_prev_count == 0)
		{
			head_tasks.push_back(task_pair.second.get());
		}
	}
	return head_tasks;
}

bool CTaskGraph::ExistLoop()
{
	enum class TaskState { Undiscovered, Discovered, Visited };
	struct TaskInfo
	{
		CTask* task;
		size_t sub_idx;
		bool GetNextSubTask(CTask*& sub_task)
		{
			if (sub_idx < task->m_sub_tasks.size())
			{
				sub_task = task->m_sub_tasks[sub_idx++];
				return true;
			}
			return false;
		}
	};

	auto head_tasks = GetHeadTasks();
	if (head_tasks.empty() && !m_tasks.empty())
	{
		return true;
	}
	
	std::map<CTask*, TaskState> task_states;
	for (auto& task_pair : m_tasks)
	{
		task_states[task_pair.second.get()] = TaskState::Undiscovered;
	}

	// 深度遍历找环
	for (auto& head_task : head_tasks)
	{
		std::stack<TaskInfo> trace;

		auto MakeDiscovered = [&](CTask* task)
		{
			task_states[task] = TaskState::Discovered;
			trace.push({ task, 0 });
		};

		auto MakeVisited = [&](CTask* task)
		{
			task_states[task] = TaskState::Visited;
			trace.pop();
		};

		MakeDiscovered(head_task);
		while (!trace.empty())
		{
			auto& cur_task_info = trace.top();
			bool end_task = true;
			CTask* sub_task;
			while (cur_task_info.GetNextSubTask(sub_task))
			{
				if (task_states[sub_task] == TaskState::Undiscovered)
				{
					end_task = false;
					MakeDiscovered(sub_task);
					break;
				}
				if (task_states[sub_task] == TaskState::Discovered)
				{
					return true;
				}
			}
			if (end_task)
			{
				MakeVisited(cur_task_info.task);
			}
		}
	}
	
	return false;
}
