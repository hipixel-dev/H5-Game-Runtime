#include "stdafx.h"
#include "HTML5Color.h"




CHTML5Color CHTML5Color::s_instance;
CHTML5Color::CHTML5Color()
{
	m_ColorMap["lightpink"] = RGB(255, 182, 193);
	m_ColorMap["pink"] = RGB(255, 192, 203);
	m_ColorMap["crimson"] = RGB(220, 20, 60);
	m_ColorMap["lavenderblush"] = RGB(255, 240, 245);
	m_ColorMap["palevioletred"] = RGB(219, 112, 147);
	m_ColorMap["hotpink"] = RGB(255, 105, 180);
	m_ColorMap["deeppink"] = RGB(255, 20, 147);
	m_ColorMap["mediumvioletred"] = RGB(199, 21, 133);
	m_ColorMap["orchid"] = RGB(218, 112, 214);
	m_ColorMap["thistle"] = RGB(216, 191, 216);
	m_ColorMap["plum"] = RGB(221, 160, 221);
	m_ColorMap["violet"] = RGB(238, 130, 238);
	m_ColorMap["magenta"] = RGB(255, 0, 255);
	m_ColorMap["fuchsia"] = RGB(255, 0, 255);
	m_ColorMap["darkmagenta"] = RGB(139, 0, 139);
	m_ColorMap["purple"] = RGB(128, 0, 128);
	m_ColorMap["mediumorchid"] = RGB(186, 85, 211);
	m_ColorMap["darkviolet"] = RGB(148, 0, 211);
	m_ColorMap["darkorchid"] = RGB(153, 50, 204);
	m_ColorMap["indigo"] = RGB(75, 0, 130);
	m_ColorMap["blueviolet"] = RGB(138, 43, 226);
	m_ColorMap["mediumpurple"] = RGB(147, 112, 219);
	m_ColorMap["mediumslateblue"] = RGB(123, 104, 238);
	m_ColorMap["slateblue"] = RGB(106, 90, 205);
	m_ColorMap["darkslateblue"] = RGB(72, 61, 139);
	m_ColorMap["lavender"] = RGB(230, 230, 250);
	m_ColorMap["ghostwhite"] = RGB(248, 248, 255);
	m_ColorMap["blue"] = RGB(0, 0, 255);
	m_ColorMap["mediumblue"] = RGB(0, 0, 205);
	m_ColorMap["midnightblue"] = RGB(25, 25, 112);
	m_ColorMap["darkblue"] = RGB(0, 0, 139);
	m_ColorMap["navy"] = RGB(0, 0, 128);
	m_ColorMap["royalblue"] = RGB(65, 105, 225);
	m_ColorMap["cornflowerblue"] = RGB(100, 149, 237);
	m_ColorMap["lightsteelblue"] = RGB(176, 196, 222);
	m_ColorMap["lightslategray"] = RGB(119, 136, 153);
	m_ColorMap["slategray"] = RGB(112, 128, 144);
	m_ColorMap["dodgerblue"] = RGB(30, 144, 255);
	m_ColorMap["aliceblue"] = RGB(240, 248, 255);
	m_ColorMap["steelblue"] = RGB(70, 130, 180);
	m_ColorMap["lightskyblue"] = RGB(135, 206, 250);
	m_ColorMap["skyblue"] = RGB(135, 206, 235);
	m_ColorMap["deepskyblue"] = RGB(0, 191, 255);
	m_ColorMap["lightblue"] = RGB(173, 216, 230);
	m_ColorMap["powderblue"] = RGB(176, 224, 230);
	m_ColorMap["cadetblue"] = RGB(95, 158, 160);
	m_ColorMap["azure"] = RGB(240, 255, 255);
	m_ColorMap["lightcyan"] = RGB(224, 255, 255);
	m_ColorMap["paleturquoise"] = RGB(175, 238, 238);
	m_ColorMap["cyan"] = RGB(0, 255, 255);
	m_ColorMap["aqua"] = RGB(0, 255, 255);
	m_ColorMap["darkturquoise"] = RGB(0, 206, 209);
	m_ColorMap["darkslategray"] = RGB(47, 79, 79);
	m_ColorMap["darkcyan"] = RGB(0, 139, 139);
	m_ColorMap["teal"] = RGB(0, 128, 128);
	m_ColorMap["mediumturquoise"] = RGB(72, 209, 204);
	m_ColorMap["lightseagreen"] = RGB(32, 178, 170);
	m_ColorMap["turquoise"] = RGB(64, 224, 208);
	m_ColorMap["aquamarine"] = RGB(127, 255, 212);
	m_ColorMap["mediumaquamarine"] = RGB(102, 205, 170);
	m_ColorMap["mediumspringgreen"] = RGB(0, 250, 154);
	m_ColorMap["mintcream"] = RGB(245, 255, 250);
	m_ColorMap["springgreen"] = RGB(0, 255, 127);
	m_ColorMap["mediumseagreen"] = RGB(60, 179, 113);
	m_ColorMap["seagreen"] = RGB(46, 139, 87);
	m_ColorMap["honeydew"] = RGB(240, 255, 240);
	m_ColorMap["lightgreen"] = RGB(144, 238, 144);
	m_ColorMap["palegreen"] = RGB(152, 251, 152);
	m_ColorMap["darkseagreen"] = RGB(143, 188, 143);
	m_ColorMap["limegreen"] = RGB(50, 205, 50);
	m_ColorMap["lime"] = RGB(0, 255, 0);
	m_ColorMap["forestgreen"] = RGB(34, 139, 34);
	m_ColorMap["green"] = RGB(0, 128, 0);
	m_ColorMap["darkgreen"] = RGB(0, 100, 0);
	m_ColorMap["chartreuse"] = RGB(127, 255, 0);
	m_ColorMap["lawngreen"] = RGB(124, 252, 0);
	m_ColorMap["greenyellow"] = RGB(173, 255, 47);
	m_ColorMap["darkolivegreen"] = RGB(85, 107, 47);
	m_ColorMap["yellowgreen"] = RGB(154, 205, 50);
	m_ColorMap["olivedrab"] = RGB(107, 142, 35);
	m_ColorMap["beige"] = RGB(245, 245, 220);
	m_ColorMap["lightgoldenrodyellow"] = RGB(250, 250, 210);
	m_ColorMap["ivory"] = RGB(255, 255, 240);
	m_ColorMap["lightyellow"] = RGB(255, 255, 224);
	m_ColorMap["yellow"] = RGB(255, 255, 0);
	m_ColorMap["olive"] = RGB(128, 128, 0);
	m_ColorMap["darkkhaki"] = RGB(189, 183, 107);
	m_ColorMap["lemonchiffon"] = RGB(255, 250, 205);
	m_ColorMap["palegoldenrod"] = RGB(238, 232, 170);
	m_ColorMap["khaki"] = RGB(240, 230, 140);
	m_ColorMap["gold"] = RGB(255, 215, 0);
	m_ColorMap["cornsilk"] = RGB(255, 248, 220);
	m_ColorMap["goldenrod"] = RGB(218, 165, 32);
	m_ColorMap["darkgoldenrod"] = RGB(184, 134, 11);
	m_ColorMap["floralwhite"] = RGB(255, 250, 240);
	m_ColorMap["oldlace"] = RGB(253, 245, 230);
	m_ColorMap["wheat"] = RGB(245, 222, 179);
	m_ColorMap["moccasin"] = RGB(255, 228, 181);
	m_ColorMap["orange"] = RGB(255, 165, 0);
	m_ColorMap["papayawhip"] = RGB(255, 239, 213);
	m_ColorMap["blanchedalmond"] = RGB(255, 235, 205);
	m_ColorMap["navajowhite"] = RGB(255, 222, 173);
	m_ColorMap["antiquewhite"] = RGB(250, 235, 215);
	m_ColorMap["tan"] = RGB(210, 180, 140);
	m_ColorMap["burlywood"] = RGB(222, 184, 135);
	m_ColorMap["bisque"] = RGB(255, 228, 196);
	m_ColorMap["darkorange"] = RGB(255, 140, 0);
	m_ColorMap["linen"] = RGB(250, 240, 230);
	m_ColorMap["peru"] = RGB(205, 133, 63);
	m_ColorMap["peachpuff"] = RGB(255, 218, 185);
	m_ColorMap["sandybrown"] = RGB(244, 164, 96);
	m_ColorMap["chocolate"] = RGB(210, 105, 30);
	m_ColorMap["saddlebrown"] = RGB(139, 69, 19);
	m_ColorMap["seashell"] = RGB(255, 245, 238);
	m_ColorMap["sienna"] = RGB(160, 82, 45);
	m_ColorMap["lightsalmon"] = RGB(255, 160, 122);
	m_ColorMap["coral"] = RGB(255, 127, 80);
	m_ColorMap["orangered"] = RGB(255, 69, 0);
	m_ColorMap["darksalmon"] = RGB(233, 150, 122);
	m_ColorMap["tomato"] = RGB(255, 99, 71);
	m_ColorMap["mistyrose"] = RGB(255, 228, 225);
	m_ColorMap["salmon"] = RGB(250, 128, 114);
	m_ColorMap["snow"] = RGB(255, 250, 250);
	m_ColorMap["lightcoral"] = RGB(240, 128, 128);
	m_ColorMap["rosybrown"] = RGB(188, 143, 143);
	m_ColorMap["indianred"] = RGB(205, 92, 92);
	m_ColorMap["red"] = RGB(255, 0, 0);
	m_ColorMap["brown"] = RGB(165, 42, 42);
	m_ColorMap["firebrick"] = RGB(178, 34, 34);
	m_ColorMap["darkred"] = RGB(139, 0, 0);
	m_ColorMap["maroon"] = RGB(128, 0, 0);
	m_ColorMap["white"] = RGB(255, 255, 255);
	m_ColorMap["whitesmoke"] = RGB(245, 245, 245);
	m_ColorMap["gainsboro"] = RGB(220, 220, 220);
	m_ColorMap["lightgrey"] = RGB(211, 211, 211);
	m_ColorMap["silver"] = RGB(192, 192, 192);
	m_ColorMap["darkgray"] = RGB(169, 169, 169);
	m_ColorMap["gray"] = RGB(128, 128, 128);
	m_ColorMap["dimgray"] = RGB(105, 105, 105);
	m_ColorMap["black"] = RGB(0, 0, 0);




}


CHTML5Color::~CHTML5Color()
{
}

uint32_t CHTML5Color::GetColorValue(std::string name)
{
	if (name.length() == 0)return 0;
	if (name.data()[0] == '#'&&name.length()>=7)
	{
		uint32_t ret = 0;
		char ch[3] = "";
		const char*p = name.data() + 1;
		ch[0] = *(p++);
		ch[1] = *(p++);
		ret = strtol(ch, 0, 16);
		ch[0] = *(p++);
		ch[1] = *(p++);
		ret |= strtol(ch, 0, 16)<<8;
		ch[0] = *(p++);
		ch[1] = *(p++);
		ret |= strtol(ch, 0, 16)<<16;
		if (name.length() >= 9)
		{//alpha
			ch[0] = *(p++);
			ch[1] = *(p++);
			ret |= strtol(ch, 0, 16) << 24;
		}
		else
		{
			ret |= 0xff000000;
		}
		return ret;
	}
	else if (name.data()[0] == '#'&&name.length() == 4)
	{
		uint32_t ret = 0;
		char ch[3] = "";
		const char*p = name.data() + 1;
		ch[0] = *(p++);
		ch[1] = ch[0];
		ret = strtol(ch, 0, 16);
		ch[0] = *(p++);
		ch[1] = ch[0];
		ret |= strtol(ch, 0, 16) << 8;
		ch[0] = *(p++);
		ch[1] = ch[0];
		ret |= strtol(ch, 0, 16) << 16;
		ret |= 0xff000000;
		return ret;
	}
	else if (!strncmp(name.c_str(), "rgba", 4))
	{
		char*p = (char*)name.c_str();
		p = strstr(p + 4, "(");
		if (!p)return 0xffffffff;
		p++;
		double r = strtod(p, &p);
		p = strstr(p, ",");
		p++;
		double g = strtod(p, &p);
		p = strstr(p, ",");
		p++;
		double b = strtod(p, &p);
		p = strstr(p, ",");
		p++;
		double a = strtod(p, &p);
		return uint32_t(r) | (uint32_t(g) << 8) | (uint32_t(b) << 16) | (uint32_t(a * 255) << 24);
	}
	else if (!strncmp(name.c_str(), "rgb", 3))
	{
		char*p = (char*)name.c_str();
		p = strstr(p + 4, "(");
		if (!p)return 0xffffffff;
		double r = strtod(p, &p);
		p = strstr(p, ",");
		p++;
		double g = strtod(p, &p);
		p = strstr(p, ",");
		p++;
		double b = strtod(p, &p);
		
		return uint32_t(r) | (uint32_t(g) << 8) | (uint32_t(b) << 16) | 0xff000000;

	}
	transform(name.begin(), name.end(), name.begin(), tolower);
	std::map<std::string, uint32_t>::iterator it = s_instance.m_ColorMap.find(name);
	if (it == s_instance.m_ColorMap.end())
	{
		return 0;
	}
	return it->second|0xff000000;
}
