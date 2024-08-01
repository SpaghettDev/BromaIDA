#include <utility>

#if !defined(BROMAIDA_DONT_USE_CUSTOM_GNUSTL) && (defined(BROMAIDA_PLATFORM_ANDROID32) || defined(BROMAIDA_PLATFORM_ANDROID64))

#include "gnustl.hpp"

#else

#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>

#if defined(BROMAIDA_PLATFORM_ANDROID32) || defined(BROMAIDA_PLATFORM_ANDROID64)

// idaclang thinks empty classes occupy 0 bytes of space therefore breaking the
// implementation of std::map/std::set (caused by the empty class std::less)
template <typename T>
struct custom_less : std::less<T>
{
    bool pad;
};

namespace std
{
    // providing custom specializations for standard library templates is
    // technically UB but clang allows it
    template <typename Key, typename T, typename Alloc>
    class map<Key, T, less<Key>, Alloc> : public map<Key, T, custom_less<Key>, Alloc> {};

    template <typename Key, typename Alloc>
    class set<Key, less<Key>, Alloc> : public set<Key, custom_less<Key>, Alloc> {};
}

#endif

#endif

#include "enums.hpp"

namespace cocos2d
{
	class CCObject;
	class CCArray;
}

class EnterEffectInstance;
class EffectGameObject;
class SFXTriggerState;
class GameObject;
class DynamicObjectAction;
class SongTriggerState;
class ChanceObject;
class EnhancedGameObject;
struct PlayerButtonCommand;
class EventTriggerInstance;
class AdvancedFollowInstance;
struct KeyframeObject;
struct SavedActiveObjectState;
struct SavedSpecialObjectState;
struct SavedObjectStateRef;
struct SFXTriggerInstance;
struct GameObjectEditorState;
struct GJValueTween;
struct SongChannelState;
struct GJPointDouble;
struct GameObjectPhysics;

class holy_shit
{
public:
    std::string m_str;

	std::vector<std::string> m_m0;
	std::vector<void*> m_m1;
	std::vector<int> m_m2;
	std::vector<cocos2d::CCObject*> m_m3;
	std::vector<EnterEffectInstance> m_m4;
	std::vector<SFXTriggerState> m_m5;
	std::vector<EffectGameObject*> m_m6;
	std::vector<GameObject*> m_m7;
	std::vector<DynamicObjectAction> m_m8;
	std::vector<SongTriggerState> m_m9;
	std::vector<ChanceObject> m_m10;
	std::vector<cocos2d::CCArray*> m_m11;
	std::vector<PlayerButtonCommand> m_m12;
	std::vector<std::vector<std::vector<GameObject*>*>*> m_m13;
	std::vector<std::vector<int>*> m_m14;
	std::vector<EventTriggerInstance> m_m15;
	std::vector<AdvancedFollowInstance> m_m16;
	std::vector<SmartPrefabResult> m_m17;
	std::vector<KeyframeObject*> m_m18;
	std::vector<bool> m_m19;
	std::vector<float> m_m20;
	std::vector<SavedActiveObjectState> m_m21;
	std::vector<SavedSpecialObjectState> m_m22;
	std::vector<SavedObjectStateRef> m_m23;

	std::map<std::string, bool> m_m24;
	std::map<std::pair<GJGameEvent, int>, int> m_m25;
	std::map<int, std::string> m_m26;
	std::map<IconType, int> m_m27;
	std::map<int, bool> m_m28;
	std::map<int, UnlockType> m_m29;
	std::map<std::pair<int, int>, int> m_m30;
	std::map<std::string, std::string> m_m31;
	std::map<std::pair<int, int>, SFXTriggerInstance> m_m32;
	std::map<int, std::map<int, int>> m_m33;
	std::map<int, std::vector<cocos2d::CCObject*>> m_m34;
	std::map<int, int> m_m35;
	std::map<std::pair<GJGameEvent, int>, std::vector<EventTriggerInstance>> m_m36;

	std::unordered_map<int, GameObject*> m_m37;
	std::unordered_map<int, std::pair<int, int>> m_m38;
	std::unordered_map<int, int> m_m39;
	std::unordered_map<int, std::pair<double, double>> m_m40;
	std::unordered_map<int, bool> m_m41;
	std::unordered_map<int, std::vector<int>> m_m42;
	std::unordered_map<int, std::unordered_set<int>> m_m43;
	std::unordered_map<int, GameObjectEditorState> m_m44;
	std::unordered_map<int, GJValueTween> m_m45;
	std::unordered_map<int, SongChannelState> m_m46;
	std::unordered_map<int, GJPointDouble> m_m47;
	std::unordered_map<int, float> m_m48;
	std::unordered_map<int, GameObjectPhysics> m_m49;
	std::unordered_map<int, std::vector<EnterEffectInstance>> m_m50;
	std::unordered_map<int, std::vector<SongTriggerState>> m_m51;
	std::unordered_map<int, std::vector<GameObject*>> m_m52;
	std::unordered_map<int, std::string> m_m53;
	std::unordered_map<int, EnhancedGameObject*> m_m54;

	std::set<int> m_m55;

	std::unordered_set<int> m_m56;
};
