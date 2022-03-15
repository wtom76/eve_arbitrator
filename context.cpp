#include <iostream>
#include "context.h"
#include "task_load_regions.h"

//---------------------------------------------------------------------------------------------------------
context& ctx()
{
	static context instance;
	return instance;
}

//---------------------------------------------------------------------------------------------------------
context::context()
	: multi_handle_{make_shared<curl_multi>()}
{}
//---------------------------------------------------------------------------------------------------------
void context::_run()
{
	shared_ptr<task> load_regions{make_shared<task_load_regions>()};
	tasks_.emplace(load_regions->handle(), load_regions);
	load_regions->activate(multi_handle_);

	int msgs_left{-1};
	int still_alive{1};
	//CURLMsg* msg{nullptr};
	while (still_alive > 0)
	{
		still_alive = 0;
		const CURLMcode res{curl_multi_perform(multi_handle_->handle(), &still_alive)};
		cout << "still_alive: " << still_alive << " res: " << res << endl;

		while (CURLMsg* msg{curl_multi_info_read(multi_handle_->handle(), &msgs_left)})
		{
			cout << "msg: " << msg << endl;

			if (msg->msg == CURLMSG_DONE)
			{
				const auto task_i{tasks_.find(msg->easy_handle)};
				if (task_i == tasks_.cend())
				{
					cout << "no tasks found";
				}
				else
				{
					task_i->second->finish();
					tasks_.erase(task_i);
				}
				cout << "done";
			}
			cout << "CURLcode: " << msg->data.result << endl;
		}
        if (still_alive)
        {
            curl_multi_wait(multi_handle_->handle(), nullptr, 0, 1000, nullptr);
        }
    }
}
//---------------------------------------------------------------------------------------------------------
void context::start()
{
	_run();
}
