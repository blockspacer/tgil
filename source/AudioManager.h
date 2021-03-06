#ifndef AUDIO_MANAGER_H
#define AUDIO_MANAGER_H

#include <glm/glm.hpp>

#include <string>
#include <unordered_map>
#include <vector>

namespace FMOD {

class Channel;
class ChannelGroup;
class Sound;
class System;

} // namespace FMOD

typedef std::unordered_map<std::string, FMOD::Sound*> SoundMap;

struct ListenerAttributes {
   glm::vec3 position;
   glm::vec3 velocity;
   glm::vec3 forward;
   glm::vec3 up;
};

namespace {

const float MUSIC_FADE_TIME = 1.0f;

} // namespace

class SoundGroup {
protected:
   const std::vector<std::string> soundFiles;
   const bool stream;
   const bool threeDimensional;
   const float minDistance;

   SoundGroup(const std::vector<std::string> &soundFiles, bool stream, bool threeDimensional, float minDistance);

public:
   // Music
   static const SoundGroup MENU_MUSIC;
   static const SoundGroup GAME_MUSIC;
   static const SoundGroup WIN_MUSIC;
   static const SoundGroup SILENCE;

   // Sound effects
   static const SoundGroup JUMP;
   static const SoundGroup STEP;
   static const SoundGroup DIE;
   static const SoundGroup EXPLOSION;
   static const SoundGroup SHOVE;
   static const SoundGroup THROW;
   static const SoundGroup ENTER;
   static const SoundGroup CLICK;

   virtual ~SoundGroup();

   const std::string& getSoundFile() const;

   const std::vector<std::string>& getSoundFiles() const {
      return soundFiles;
   }

   bool isStream() const {
      return stream;
   }

   bool isThreeDimensional() const {
      return threeDimensional;
   }

   float getMinDistance() const {
      return minDistance;
   }
};

class AudioManager {
protected:
   FMOD::System *system;
   FMOD::ChannelGroup *musicGroup;
   FMOD::ChannelGroup *effectsGroup;
   FMOD::Channel *activeMusicChannel;

   SoundMap musicMap;
   SoundMap effectsMap;

   SoundMap soundMap;

   int numListeners;

   void release();

   void load(const SoundGroup &soundGroup);

public:
   AudioManager();

   virtual ~AudioManager();

   void init();

   void update();

   void updateAttributes(const std::vector<ListenerAttributes> &attributes);

   void play(const SoundGroup &soundGroup, const glm::vec3 &pos = glm::vec3(0.0f), const glm::vec3 &vel = glm::vec3(0.0f));
};

#endif
