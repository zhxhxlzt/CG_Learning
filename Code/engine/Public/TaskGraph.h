#include "STD.h"

class CTaskGraph;
class CTask;


class CTask
{
	friend class CTaskGraph;
public:
	using FuncType = std::function<void()>;
	std::string GetTaskName();
protected:
	class CTaskWaiter;
	CTask(std::string task_name, FuncType func=[](){});
	void Start(CTaskWaiter& waiter);
	void Connect(CTask* next);
	int DecPrevCount();
private:
	std::atomic<int> m_prev_count = 0;
	std::string m_task_name;
	FuncType m_func;
	std::vector<CTask*> m_sub_tasks;
};

class CTaskGraph
{
public:
	CTask* CreateTask(std::string task_name, CTask::FuncType func);
	CTask* GetTask(std::string task_name);
	void Connect(CTask* prev, CTask* next);
	void Run();
protected:
	std::vector<CTask*> GetHeadTasks();
	bool ExistLoop();
private:
	std::map<std::string, std::unique_ptr<CTask>> m_tasks;
};


