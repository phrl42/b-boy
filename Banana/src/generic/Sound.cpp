#include "incs.h"

#include "Sound.h"

#define MINIAUDIO_IMPLEMENTATION
#include "miniaudio.h"

#include "generic/Application.hpp"

namespace Banana
{
  Sound::Sound(const std::string& path_to_sound, bool loop)
  :sound_path(path_to_sound)
  {
    LOG("Sorry! Miniaudio is currently deactivated");
    //InitSound(path_to_sound, loop);
  }

  Sound::~Sound()
  {
    //Stop();
  }

  void Sound::Change(const std::string& path_to_sound, bool loop)
  {
    LOG("Sorry! This is not supported by miniaudio. Create a new Sound object");
    return;
    if(ma_sound_is_playing(&current_sound))
    {
      Stop();
    }

    InitSound(path_to_sound, loop);
  }
    
  void Sound::InitSound(const std::string& path_to_sound, bool loop)
  {
    LOG("Sorry! Miniaudio is currently deactivated");
    return;
    if(int success = ma_sound_init_from_file(&Application::GetInstance().soundhelper->GetEngine(), path_to_sound.c_str(), MA_SOUND_FLAG_DECODE, NULL, NULL, &current_sound); success != MA_SUCCESS)
    {
      if(success == MA_DOES_NOT_EXIST)
      {
        LOG("Could not find " + path_to_sound);
        return;
      }
      LOG("Could not init sound with MINIAUDIO error: " + std::to_string(success));
    }
    
    if(loop)
    {
      ma_sound_set_looping(&current_sound, loop);
    }
  }

  void Sound::Start()
  {
    //ma_sound_start(&current_sound);
  }

  void Sound::Stop()
  {
    if(ma_sound_is_playing(&current_sound))
    {
      //ma_sound_stop(&current_sound);
    }

    //ma_sound_uninit(&current_sound);
  }

  void Sound::SetVolume(float vol)
  {
    //ma_sound_set_volume(&current_sound, vol);
  }

  float Sound::GetVolume()
  {
    return 69.42f; //ma_sound_get_volume(&current_sound);
  }

}
