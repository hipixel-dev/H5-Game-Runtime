#pragma once
#include "JSVMInterface.h"
class HtmlConsole :
	public CustomClassBase
{
	DEFINE_CUSTOM_CLASS(HtmlConsole, CustomClassBase)
public:
	BEGIN_CUSTOM_FUNCTION
		DEFINE_CUSTOM_FUNCTION("profile", &HtmlConsole::_profile,1)
		DEFINE_CUSTOM_FUNCTION("groupEnd", &HtmlConsole::_groupEnd, 0)
		DEFINE_CUSTOM_FUNCTION("assert", &HtmlConsole::_assert, 3)
		DEFINE_CUSTOM_FUNCTION("timeEnd", &HtmlConsole::_timeEnd, 1)
		DEFINE_CUSTOM_FUNCTION("time", &HtmlConsole::_time, 1)
		DEFINE_CUSTOM_FUNCTION("clear", &HtmlConsole::_clear, 0)
		DEFINE_CUSTOM_FUNCTION("trace", &HtmlConsole::_trace, 0)
		DEFINE_CUSTOM_FUNCTION("group", &HtmlConsole::_group, 1)
		DEFINE_CUSTOM_FUNCTION("warn", &HtmlConsole::_warn, 1)
		DEFINE_CUSTOM_FUNCTION("dirxml", &HtmlConsole::_dirxml, 1)
		DEFINE_CUSTOM_FUNCTION("debug", &HtmlConsole::_debug, 1)
		DEFINE_CUSTOM_FUNCTION("error", &HtmlConsole::_error, 1)
		DEFINE_CUSTOM_FUNCTION("log", &HtmlConsole::_log, 1)
		DEFINE_CUSTOM_FUNCTION("select", &HtmlConsole::_select, 1)
		DEFINE_CUSTOM_FUNCTION("profileEnd", &HtmlConsole::_profileEnd, 0)
		DEFINE_CUSTOM_FUNCTION("info", &HtmlConsole::_info, 1)
		DEFINE_CUSTOM_FUNCTION("count", &HtmlConsole::_count, 1)
	

		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY
		
		END_CUSTOM_PROPERTY

	HtmlConsole();
	virtual ~HtmlConsole();

	static bool InitClass(const HandleContext&context);

	bool _profile(IFunctionCallbackArgs&args);
	bool _groupEnd(IFunctionCallbackArgs&args);
	bool _assert(IFunctionCallbackArgs&args);
	bool _timeEnd(IFunctionCallbackArgs&args);
	bool _time(IFunctionCallbackArgs&args);
	bool _clear(IFunctionCallbackArgs&args);
	bool _trace(IFunctionCallbackArgs&args);
	bool _group(IFunctionCallbackArgs&args);
	bool _warn(IFunctionCallbackArgs&args);
	bool _dirxml(IFunctionCallbackArgs&args);
	bool _debug(IFunctionCallbackArgs&args);
	bool _error(IFunctionCallbackArgs&args);
	bool _log(IFunctionCallbackArgs&args);
	bool _select(IFunctionCallbackArgs&args);
	bool _profileEnd(IFunctionCallbackArgs&args);
	bool _info(IFunctionCallbackArgs&args);
	bool _count(IFunctionCallbackArgs&args);


};

