#include "stdafx.h"
#include "HtmlLocalStorage.h"
IMPLEMENT_CUSTOM_CLASS(HtmlLocalStorage, HtmlSessionStorage)

HtmlLocalStorage::HtmlLocalStorage()
{
}


HtmlLocalStorage::~HtmlLocalStorage()
{
}

bool HtmlLocalStorage::InitClass(const HandleContext&context)
{
	if (HtmlSessionStorage::GetPrototype_s().IsNull())
	{
		HtmlSessionStorage::InitClass(context);
	}
	return InitClass_s(context, "HTMLLocalStorage", HtmlSessionStorage::GetPrototype_s(), true);
}

bool HtmlLocalStorage::getItem(IFunctionCallbackArgs&args)
{
	return HtmlSessionStorage::getItem(args);
// 	CHECK_ARGS_COUNT(1, "HtmlLocalStorage::getItem");
// 	std::string szkey = args[0].GetString(args.getContext());
// 	std::unordered_map<std::string, std::string>::iterator it = m_keymap.find(szkey);
// 	if (it == m_keymap.end())args.setReturnValue(ValueBase());
// 	else args.setReturnValue(LocalValue(args.getContext()).SetString(args.getContext(), it->second.c_str()));
// 
// 	return true;
}

bool HtmlLocalStorage::setItem(IFunctionCallbackArgs&args)
{
	HtmlSessionStorage::setItem(args);
// 	CHECK_ARGS_COUNT(2, "HtmlLocalStorage::setItem");
// 	std::string szkey = args[0].GetString(args.getContext());
// 	std::string szval = args[1].GetString(args.getContext());
// 	m_keymap[szkey] = szval;
	SavaToFile();
	return true;
}

bool HtmlLocalStorage::removeItem(IFunctionCallbackArgs&args)
{
	HtmlSessionStorage::removeItem(args);
// 	CHECK_ARGS_COUNT(1, "HtmlLocalStorage::removeItem");
// 	std::string szkey = args[0].GetString(args.getContext());
// 	m_keymap.erase(szkey);
	SavaToFile();
	return true;
}

bool HtmlLocalStorage::clear(IFunctionCallbackArgs&args)
{
	HtmlSessionStorage::clear(args);
//	m_keymap.clear();
	SavaToFile();
	return true;
}

bool HtmlLocalStorage::OnConstructor(IFunctionCallbackArgs&args)
{
	HtmlSessionStorage::OnConstructor(args);
	LoadFromFile();
	return true;
}

bool HtmlLocalStorage::LoadFromFile()
{//文件是二进制格式，格式为key.length()+key+value.length()+value+key.length()...
	m_keymap.clear();
	FileIOInterface*pFileIO = JSVM::GetInstance()->GetFileIOInterface();
	std::string szFile = JSVM::GetInstance()->GetLocalStoragePath();
	if (szFile.empty())return false;
	if (!pFileIO->IsFileExist(szFile.c_str()))return false;
	FileHandle *file = pFileIO->Open(szFile.c_str(), FileIOInterface::OP_READ);
	if (!file)return false;
	int nFileLen = pFileIO->GetLength(file);
	char*pbuf = new char[nFileLen + 1];
	pFileIO->Read(file, pbuf, nFileLen);
	pbuf[nFileLen] = 0;
	char*p = pbuf;
	while (p < pbuf+nFileLen)
	{
		uint32_t nlen = *(uint32_t*)p;
		if (nlen > 100)break;
		p += 4;
		char*szkey = new char[nlen + 1];
		memcpy(szkey, p, nlen);
		szkey[nlen] = 0;
		p += nlen;
		nlen = *(uint32_t*)p;
		p += 4;
		char*szval = new char[nlen + 1];
		memcpy(szval, p, nlen);
		szval[nlen] = 0;
		p += nlen;
		m_keymap[szkey] = szval;

		delete[]szkey;
		delete[]szval;
	}

	delete[]pbuf;
	pFileIO->Close(file);
	return true;
}

bool HtmlLocalStorage::SavaToFile()
{
	FileIOInterface*pFileIO = JSVM::GetInstance()->GetFileIOInterface();
	std::string szFile = JSVM::GetInstance()->GetLocalStoragePath();
	FileHandle *file = pFileIO->Open(szFile.c_str(), FileIOInterface::OP_READWRITE);
	if (!file)return false;
	for (std::unordered_map<std::string, std::string>::iterator it = m_keymap.begin(); it != m_keymap.end(); ++it)
	{
		
		uint32_t nlen = it->first.length();
		pFileIO->Write(file, &nlen, 4);
		pFileIO->Write(file, it->first.c_str(), nlen);
		nlen = it->second.length();
		pFileIO->Write(file, &nlen, 4);
		pFileIO->Write(file, it->second.c_str(), nlen);

	}
	pFileIO->Close(file);
	return true;
}

bool HtmlLocalStorage::OnGetProperty(GetPropertyArgs&args)
{
	return HtmlSessionStorage::OnGetProperty(args);
// 	const std::string&name = args.GetName();
// 	std::unordered_map<std::string, std::string>::iterator it = m_keymap.find(name);
// 	if (it == m_keymap.end())
// 	{
// 		return CustomClassBase::OnGetProperty(args);
// 	}
// 	args.SetRetValue(LocalValue(args.GetContext()).SetString(args.GetContext(), it->second.c_str()));
// 	return true;
}

bool HtmlLocalStorage::OnSetProperty(SetPropertyArgs&args)
{
	HtmlSessionStorage::OnSetProperty(args);
	SavaToFile();
	return true;
// 	const std::string&name = args.GetName();
// 	if (args.GetValue().IsString())
// 	{
// 		m_keymap[name] = args.GetValue().GetString(args.GetContext());
// 		SavaToFile();
// 		return true;
// 	}
// 	return CustomClassBase::OnSetProperty(args);
}
