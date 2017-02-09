#include "stdafx.h"
#include "HtmlSessionStorage.h"
IMPLEMENT_CUSTOM_CLASS(HtmlSessionStorage, CustomClassBase)

HtmlSessionStorage::HtmlSessionStorage()
{
}


HtmlSessionStorage::~HtmlSessionStorage()
{
}

bool HtmlSessionStorage::InitClass(const HandleContext&context)
{
	return InitClass_s(context, "HTMLSessionStorage", HandleObject(), true);
}



bool HtmlSessionStorage::OnGetProperty(GetPropertyArgs&args)
{
	const std::string&name = args.GetName();
	std::unordered_map<std::string, std::string>::iterator it = m_keymap.find(name);
	if (it == m_keymap.end())
	{
		return CustomClassBase::OnGetProperty(args);
	}
	args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), it->second.c_str()));
	return true;
}

bool HtmlSessionStorage::OnSetProperty(SetPropertyArgs&args)
{
	const std::string&name = args.GetName();
	if (args.GetValue().IsString())
	{
		m_keymap[name] = args.GetValue().GetString(args.GetContext());

		return true;
	}
	return CustomClassBase::OnSetProperty(args);
}

bool HtmlSessionStorage::getItem(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "HtmlSessionStorage::getItem");
	std::string &&szkey = args[0].GetString(args.getContext());
	std::unordered_map<std::string, std::string>::iterator it = m_keymap.find(szkey);
	if (it == m_keymap.end())args.setReturnValue(ValueBase());
	else args.setReturnValue(LocalValue(args.getContext()).SetString(args.getContext(), it->second.c_str()));

	return true;
}

bool HtmlSessionStorage::setItem(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(2, "HtmlSessionStorage::setItem");
	std::string &&szkey = args[0].GetString(args.getContext());
	std::string &&szval = args[1].GetString(args.getContext());
	m_keymap[szkey] = szval;

	return true;
}

bool HtmlSessionStorage::removeItem(IFunctionCallbackArgs&args)
{
	CHECK_ARGS_COUNT(1, "HtmlSessionStorage::removeItem");
	std::string&& szkey = args[0].GetString(args.getContext());
	m_keymap.erase(szkey);

	return true;
}

bool HtmlSessionStorage::clear(IFunctionCallbackArgs&args)
{
	m_keymap.clear();
	return true;
}
