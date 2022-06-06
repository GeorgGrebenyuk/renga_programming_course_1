#include "pch.h"
#include "init_app.h"
#include <Renga/CreateApplication.hpp>
#include <fstream>
#include <ctime>
#include <sstream>
#pragma warning(disable : 4996)

bool init_app::initialize(const wchar_t* pluginPath) {
	auto renga_app_temp = 
		Renga::CreateApplication(CLSCTX_INPROC_SERVER);
	if (!renga_app_temp) return false;
	renga_app = renga_app_temp;

	time_start_app = std::chrono::system_clock::now();
	m_pEventHandler.reset(new app_handler(renga_app));
	return true;
}
void init_app::stop()
{
	m_pEventHandler.reset();
}


app_handler::app_handler(Renga::IApplicationPtr pApplication) :
	Renga::ApplicationEventHandler(pApplication)
{
}

void app_handler::OnApplicationClose(Renga::IApplicationCloseEvent*)
{
	const char* log_path = "E:\\Temp\\renga_test_log.txt";
	std::ofstream out(log_path);
	if (out.is_open())
	{
		out << "Welcome to Renga logging" << std::endl;
		for (std::string one_str : app_log)
		{
			out << one_str << std::endl;
		}
		
	}
	out.close();
	app_log.clear();
}

void app_handler::OnProjectClose(Renga::IProjectCloseEvent* pEvent)
{
	_bstr_t project_path_getting = renga_app->Project->GetFilePath();

	const char* project_path;
	if (project_path_getting.length() == 0) project_path = "no saving";
	else project_path = project_path_getting;
	//Засекаем текущее время и считаем сколькоп рошло с запуска приложения
	std::chrono::time_point<std::chrono::system_clock> time_stop = std::chrono::system_clock::now();
	double time_sec_duration = std::chrono::duration_cast<std::chrono::seconds>(time_stop - time_start_app).count();

	std::time_t time = std::chrono::system_clock::to_time_t(time_stop);
	std::string time_str = std::ctime(&time);
	time_str.resize(time_str.size() - 1);
	
	std::stringstream to_log;
	to_log << time_str << "\t" << project_path << "\t" << time_sec_duration << " s.";
	app_log.push_back(to_log.str());

	
}