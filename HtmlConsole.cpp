#include "stdafx.h"
#include "HtmlConsole.h"

IMPLEMENT_CUSTOM_CLASS(HtmlConsole, CustomClassBase)
HtmlConsole::HtmlConsole()
{
}


HtmlConsole::~HtmlConsole()
{
}

bool HtmlConsole::InitClass(const HandleContext&context)
{
	return InitClass_s(context, "Console", HandleObject());
}

bool HtmlConsole::_profile(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlConsole::_groupEnd(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlConsole::_assert(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlConsole::_timeEnd(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlConsole::_time(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlConsole::_clear(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlConsole::_trace(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, " HtmlConsole::_trace");
	std::string str = args[0].GetString(args.getContext())+"\n";
	
	JSVM::GetInstance()->GetSystemInterface()->ShowErr(str.c_str());
	PrintDescribeScriptedCaller(args.getContext());
	return true;
}

bool HtmlConsole::_group(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlConsole::_warn(IFunctionCallbackArgs&args)
{

	return true;
}

bool HtmlConsole::_dirxml(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlConsole::_debug(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlConsole::_error(IFunctionCallbackArgs&args)
{
//	assert(0);
	return true;
}

bool HtmlConsole::_log(IFunctionCallbackArgs&args)
{
//	assert(0);
	return true;
}

bool HtmlConsole::_select(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlConsole::_profileEnd(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlConsole::_info(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}

bool HtmlConsole::_count(IFunctionCallbackArgs&args)
{
	assert(0);
	return true;
}
