#pragma once

#include <FMOD/fmod.hpp>
#include <FMOD/fmod_errors.h>
#include <iostream>
#include <string>
#include <map>


class FModManager
{
public:
	FModManager();
	~FModManager();

	struct CHgroup
	{
		FMOD::ChannelGroup* group_ptr;
		float current_pan;
		float dsp_pitch;
		FMOD::Channel* chn_ptr;

		CHgroup();
	};

	bool Fmod_init(const int num_channel, const int system_flag);
	void shutdown();

	bool create_channel(const std::string& CH_name);
	bool find_channel_group(const std::string& CH_name, CHgroup** channel_group);
	void remove_channel(const std::string& CH_name);
	bool set_channel_parent(const std::string& child_name, const std::string& parent_name);

	bool get_channel_vol(const std::string& CH_name, float* vol);
	bool set_channel_vol(const std::string& CH_name, float vol);

	//bool get_channel_group_pan(const std::string& CH_name, float* pan);
	//bool set_channel_group_pan(const std::string& CH_name, float pan);

	bool get_channel_group_enabled(const std::string& CH_name, bool* enabled);
	bool set_channel_group_enabled(const std::string& CH_name, bool enabled);

	//bool get_channel_pitch(const std::string& CH_name, float* pitch);
	//bool set_channel_pitch(const std::string& CH_name, float pitch);

	//bool get_channel_freq(const std::string& CH_name, float* freq);
	//bool set_channel_freq(const std::string& CH_name, float freq);

	//bool get_playback_pos(const std::string& CH_name, unsigned int* pos);


	bool create_sound(const std::string& Sound_name, const std::string& path, const int mode);

	bool remove_sound(const std::string& Sound_name);
	bool play_sound(const std::string& Sound_name, const std::string& CH_name);
	////play 3d sound
	//bool play_sound(const std::string& sound_name, glm::vec3 position, float max_distance);
	//bool stop_sound(const std::string& CH_name);
	//bool get_speed(const std::string& Sound_name, float* speed);
	//bool set_speed(const std::string& Sound_name, float speed);
	//bool get_sound_name(const std::string& Sound_name, std::string* fname);
	//bool get_sound_format(const std::string& Sound_name, std::string* type, std::string* format,int* chn,int* bit);
	//bool get_sound_defualt(const std::string& Sound_name, float* freq, int* priority);
	//bool get_sound_lengh(const std::string& Sound_name,unsigned int* length);


	////dsp
	//bool add_dsp(const std::string& CH_name, const std::string& fx_name);//todo
	//bool remove_dsp(const std::string& CH_name, const std::string& fx_name);//todo
	////bool create_dsp(const std::string& DSP_name, FMOD_DSP_TYPE DSP_type, const float value);
	//bool create_dsp(const std::string& DSP_name, FMOD_DSP_TYPE DSP_type);
	//bool get_dsp(const std::string& DSP_name, FMOD::DSP** dsp);
	//bool get_dsp_param(const std::string& DSP_name, const int index, float* value, char* valuestr, const int valuestrlen);
	////todo

	////bool get_dsp_param(const std::string& DSP_name, const int index, const float* value);
	//bool set_dsp_param(const std::string& DSP_name, const int index, const float value);

	////3d sound
	//bool update_listener_position(const glm::vec3 position);
	//bool set_speaker_position();
	//bool set_SWFormat(const bool isSurround);

protected:
	FMOD_RESULT last_result_;
	FMOD::System* system_;

	std::map<std::string, CHgroup*> channel_group_;
	std::map<std::string, FMOD::Sound*> sound_;
	std::map<std::string, FMOD::DSP*> dsp_;


	bool is_Fmod_ok(const bool show_error = true) const;
	const char* FmodTypeEnumtoChar(FMOD_SOUND_TYPE t) noexcept;
	const char* FmodFormatEnumtoChar(FMOD_SOUND_FORMAT f) noexcept;


};