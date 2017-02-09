#pragma once
#include "HtmlEventTarget.h"
class HtmlAudio :
	public HtmlEventTarget
{
	DEFINE_CUSTOM_CLASS(HtmlAudio, HtmlEventTarget)
		int m_readyState = 0;
	AudioInterface*m_pAudio = NULL;
	bool m_bAutoPlay = true;
	std::string m_src;
	bool m_bdefaultMuted = false;
	bool m_bloop = false;
	bool m_bmuted = false;
	bool m_bpaused = false;
	std::string m_preload;
	double m_volume = 1;
//	std::string m_szFullPath;
public:
	BEGIN_CUSTOM_FUNCTION
		DEFINE_CUSTOM_FUNCTION("__load",&HtmlAudio::__load,0)


		DEFINE_CUSTOM_FUNCTION("canPlayType", &HtmlAudio::canPlayType,1)
		DEFINE_CUSTOM_FUNCTION("load", &HtmlAudio::load, 0)
		DEFINE_CUSTOM_FUNCTION("play", &HtmlAudio::play, 0)
		DEFINE_CUSTOM_FUNCTION("pause", &HtmlAudio::pause, 0)

		END_CUSTOM_FUNCTION

		BEGIN_CUSTOM_PROPERTY
		DEFINE_CUSTOM_PROPERTY("autoplay")
		DEFINE_CUSTOM_PROPERTY("currentSrc")
		DEFINE_CUSTOM_PROPERTY("defaultMuted")
		DEFINE_CUSTOM_PROPERTY("loop")
		DEFINE_CUSTOM_PROPERTY("muted")
		DEFINE_CUSTOM_PROPERTY("paused")
		DEFINE_CUSTOM_PROPERTY("preload")
		DEFINE_CUSTOM_PROPERTY("readyState")
		DEFINE_CUSTOM_PROPERTY("src")
		DEFINE_CUSTOM_PROPERTY("volume")
		DEFINE_CUSTOM_PROPERTY("currentTime")
		DEFINE_CUSTOM_PROPERTY("ended")

		END_CUSTOM_PROPERTY


	HtmlAudio();
	virtual ~HtmlAudio();



	static bool InitClass(const HandleContext&context);

	bool __load(IFunctionCallbackArgs&args);


	bool canPlayType(IFunctionCallbackArgs&args);
	bool load(IFunctionCallbackArgs&args);
	bool play(IFunctionCallbackArgs&args);
	bool pause(IFunctionCallbackArgs&args);
	virtual bool OnGetProperty(GetPropertyArgs&args);
	virtual bool OnSetProperty(SetPropertyArgs&args);
	virtual bool OnConstructor(IFunctionCallbackArgs&args);
};

