#include "stl_includes.hpp"
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

// exists because IDA cant dynamically make STL types
class holy_shit
{
public:
	std::string m_str;

	std::vector<std::string> m_V_str;
	std::vector<void*> m_V_void_p;
	std::vector<int> m_V_int;
	std::vector<cocos2d::CCObject*> m_V_ccobj_p;
	std::vector<EnterEffectInstance> m_V_eei;
	std::vector<SFXTriggerState> m_V_sfxts;
	std::vector<EffectGameObject*> m_V_ego;
	std::vector<GameObject*> m_V_go_p;
	std::vector<DynamicObjectAction> m_V_doa;
	std::vector<SongTriggerState> m_V_sts;
	std::vector<ChanceObject> m_V_co;
	std::vector<cocos2d::CCArray*> m_V_ccarr_p;
	std::vector<PlayerButtonCommand> m_V_pbc;
	std::vector<std::vector<std::vector<GameObject*>*>*> m_V_v_V_go_p_p_p;
	std::vector<std::vector<int>*> m_V_v_int_p;
	std::vector<EventTriggerInstance> m_V_eti;
	std::vector<AdvancedFollowInstance> m_V_afi;
	std::vector<SmartPrefabResult> m_V_spr;
	std::vector<KeyframeObject*> m_V_ko_p;
	std::vector<bool> m_V_bool;
	std::vector<float> m_V_float;
	std::vector<SavedActiveObjectState> m_V_saos;
	std::vector<SavedSpecialObjectState> m_V_spos;
	std::vector<SavedObjectStateRef> m_V_sosr;

	std::map<std::string, bool> m_M_str_bool;
	std::map<std::pair<GJGameEvent, int>, int> m_M_pair_gjge_int_int;
	std::map<int, std::string> m_M_int_str;
	std::map<IconType, int> m_M_it_int;
	std::map<int, bool> m_M_int_bool;
	std::map<int, UnlockType> m_M_int_ut;
	std::map<std::pair<int, int>, int> m_M_pair_int_int_int;
	std::map<std::string, std::string> m_M_str_str;
	std::map<std::pair<int, int>, SFXTriggerInstance> m_M_pair_int_int_sfxti;
	std::map<int, std::map<int, int>> m_M_int_M_int_int;
	std::map<int, std::vector<cocos2d::CCObject*>> m_M_int_V_ccobj_p;
	std::map<int, int> m_M_int_int;
	std::map<std::pair<GJGameEvent, int>, std::vector<EventTriggerInstance>> m_M_pair_gjge_int_V_eti;

	std::unordered_map<int, GameObject*> m_UM_int_go_p;
	std::unordered_map<int, std::pair<int, int>> m_UM_int_pair_int_int;
	std::unordered_map<int, int> m_UM_int_int;
	std::unordered_map<int, std::pair<double, double>> m_UM_int_pair_double_double;
	std::unordered_map<int, bool> m_UM_int_bool;
	std::unordered_map<int, std::vector<int>> m_UM_int_V_int;
	std::unordered_map<int, std::unordered_set<int>> m_UM_US_int;
	std::unordered_map<int, GameObjectEditorState> m_UM_int_goes;
	std::unordered_map<int, GJValueTween> m_UM_int_gjvt;
	std::unordered_map<int, SongChannelState> m_UM_int_scs;
	std::unordered_map<int, GJPointDouble> m_UM_int_gjpd;
	std::unordered_map<int, float> m_UM_int_float;
	std::unordered_map<int, GameObjectPhysics> m_UM_int_gop;
	std::unordered_map<int, std::vector<EnterEffectInstance>> m_UM_int_V_eei;
	std::unordered_map<int, std::vector<SongTriggerState>> m_UM_int_V_sts;
	std::unordered_map<int, std::vector<GameObject*>> m_UM_int_V_go_p;
	std::unordered_map<int, std::string> m_UM_int_str;
	std::unordered_map<int, EnhancedGameObject*> m_UM_int_ego_p;

	std::set<int> m_S_int;

	std::unordered_set<int> m_US_int;
};
