#pragma once
#include <map>
class CHTML5Color
{
public:
	std::map<std::string, uint32_t> m_ColorMap;

	static CHTML5Color s_instance;
	CHTML5Color();
	~CHTML5Color();
	static uint32_t GetColorValue(std::string name);
};

