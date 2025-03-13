#pragma once

#include "miniaudio.h"

namespace Banana
{

  struct SoundHelper
  {
    public:
      SoundHelper()
      {
      	int success = ma_engine_init(NULL, &engine);
		if(success != MA_SUCCESS)
		{
		  LOG("Could not init engine: " + std::to_string(success));
		}

        success = ma_engine_start(&engine);
        if(success != MA_SUCCESS)
        {
          LOG("Could not start engine: " + std::to_string(success));
        }
      }

      ~SoundHelper()
      {
        ma_engine_stop(&engine);
        ma_engine_uninit(&engine);
      }

      ma_engine& GetEngine()
      {
        return engine;
      }

    private:

      ma_engine engine;
  };

  class Sound
  {
    public:
      Sound(const std::string& path_to_sound, bool loop = false);
      void Change(const std::string& path_to_sound, bool loop = false);

      void Start();
      void Stop();

      void SetVolume(float vol);
      float GetVolume();

      ~Sound();

    private:
      void InitSound(const std::string& path_to_sound, bool loop);
      ma_sound current_sound;
      std::string sound_path;
  };
};
