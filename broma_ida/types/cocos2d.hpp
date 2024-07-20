/****************************************************************************
	Copyright (c) 2010-2012 cocos2d-x.org
	Copyright (c) 2008-2010 Ricardo Quesada
	Copyright (c) 2009      Valentin Milea
	Copyright (c) 2011      Zynga Inc.

	http://www.cocos2d-x.org

	Permission is hereby granted, free of charge, to any person obtaining a copy
	of this software and associated documentation files (the "Software"), to deal
	in the Software without restriction, including without limitation the rights
	to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
	copies of the Software, and to permit persons to whom the Software is
	furnished to do so, subject to the following conditions:

	The above copyright notice and this permission notice shall be included in
	all copies or substantial portions of the Software.

	THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
	IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
	FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
	AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
	LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
	OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
	THE SOFTWARE.

	Modified by SpaghettDev
 ****************************************************************************/

#include <cstdint>
#include <climits>
#include <cctype>

#if !defined(BROMAIDA_PLATFORM_ANDROID32) && !defined(BROMAIDA_PLATFORM_ANDROID64)

#include <string>
#include <vector>
#include <set>

#else

#include "gnustl.hpp"

#endif


#ifndef PAD
#define STR_CONCAT_WRAPPER(a, b) a ## b
#define STR_CONCAT(a, b) STR_CONCAT_WRAPPER(a, b)
#define PAD(size) unsigned char STR_CONCAT(__pad, __LINE__)[size]
#endif
#define RT_ADD(...) __VA_ARGS__
#define RT_REMOVE(...)

// cocos macros
#define CC_PROPERTY_READONLY(varType, varName, funName) \
protected: varType varName;                             \
public: virtual varType get##funName(void);

#define CC_PROPERTY_READONLY_PASS_BY_REF(varType, varName, funName) \
protected: varType varName;                                         \
public: virtual const varType& get##funName(void);

#define CC_PROPERTY(varType, varName, funName) \
protected: varType varName;                    \
public: virtual varType get##funName(void);    \
public: virtual void set##funName(varType var);

#define CC_PROPERTY_PASS_BY_REF(varType, varName, funName) \
protected: varType varName;                                \
public: virtual const varType& get##funName(void);         \
public: virtual void set##funName(const varType& var);

#define CC_SYNTHESIZE_READONLY(varType, varName, funName) \
protected: varType varName;                               \
public: virtual varType get##funName(void) const { return varName; }

#define CC_SYNTHESIZE_READONLY_NC(varType, varName, funName) \
protected: varType varName;                                  \
public: virtual varType get##funName(void) { return varName; }

#define CC_SYNTHESIZE_READONLY_PASS_BY_REF(varType, varName, funName) \
protected: varType varName;                                           \
public: virtual const varType& get##funName(void) const { return varName; }

#define CC_SYNTHESIZE(varType, varName, funName)                     \
protected:varType varName;                                           \
public: virtual varType get##funName(void) const { return varName; } \
public: virtual void set##funName(varType var) { varName = var; }

#define CC_SYNTHESIZE_PASS_BY_REF(varType, varName, funName)                \
protected: varType varName;                                                 \
public: virtual const varType& get##funName(void) const { return varName; } \
public: virtual void set##funName(const varType& var) { varName = var; }

#define CC_SYNTHESIZE_RETAIN(varType, varName, funName)               \
private: varType varName;                                             \
public: virtual varType get##funName(void) const { return varName; }  \
public:                                                               \
	virtual void set##funName(varType var) {                          \
	    if (varName != var) {                                         \
	        CC_SAFE_RETAIN(var);                                      \
	        CC_SAFE_RELEASE(varName);                                 \
	        varName = var;                                            \
	    }                                                             \
	}

#define CC_SYNTHESIZE_NV(varType, varName, funName)          \
protected: varType varName;                                  \
public: varType get##funName(void) const { return varName; } \
public: void set##funName(varType var) { varName = var; }

#define CC_SYNTHESIZE_READONLY_NV(varType, varName, funName) \
protected: varType varName;                                  \
public: varType get##funName(void) const { return varName; }

#define CC_SYNTHESIZE_READONLY_NV_NC(varType, varName, funName) \
protected: varType varName;                                     \
public: varType get##funName(void) { return varName; }

#define CC_PROPERTY(varType, varName, funName) \
protected: varType varName;                    \
public: virtual varType get##funName(void);    \
public: virtual void set##funName(varType var);

#define CREATE_FUNC(__TYPE__)            \
	static __TYPE__* create() {          \
	    __TYPE__* pRet = new __TYPE__(); \
	    if (pRet && pRet->init()) {      \
	        pRet->autorelease();         \
	        return pRet;                 \
	    }                                \
	    else {                           \
	        delete pRet;                 \
	        pRet = NULL;                 \
	        return NULL;                 \
	    }                                \
	}

#define NODE_FUNC(__TYPE__)                           \
	CC_DEPRECATED_ATTRIBUTE static __TYPE__* node() { \
	    __TYPE__* pRet = new __TYPE__();              \
	    if (pRet && pRet->init()) {                   \
	        pRet->autorelease();                      \
	        return pRet;                              \
	    }                                             \
	    else {                                        \
	        delete pRet;                              \
	        pRet = NULL;                              \
	        return NULL;                              \
	    }                                             \
	}

#define CC_SAFE_DELETE(p) \
	do {                  \
	    if (p) {          \
	        delete (p);   \
	        (p) = 0;      \
	    }                 \
	} while (0)

#define CC_SAFE_DELETE_ARRAY(p) \
	do {                        \
	    if (p) {                \
	        delete[] (p);       \
	        (p) = 0;            \
	    }                       \
	} while (0)

#define CC_SAFE_FREE(p) \
	do {                \
	    if (p) {        \
	        free(p);    \
	        (p) = 0;    \
	    }               \
	} while (0)

#define CC_SAFE_RELEASE(p)  \
	do {                    \
	    if (p) {            \
	        (p)->release(); \
	    }                   \
	} while (0)

#define CC_SAFE_RELEASE_NULL(p) \
	do {                        \
	    if (p) {                \
	        (p)->release();     \
	        (p) = 0;            \
	    }                       \
	} while (0)

#define CC_SAFE_RETAIN(p)  \
	do {                   \
	    if (p) {           \
	        (p)->retain(); \
	    }                  \
	} while (0)

#define CC_UNUSED_PARAM(unusedparam) (void)unusedparam
#define CCAssert(cond, msg) ((void)(cond))

#if defined(__GNUC__) && (__GNUC__ >= 4)
	#define CC_FORMAT_PRINTF(formatPos, argPos) \
	    __attribute__((__format__(printf, formatPos, argPos)))
#elif defined(__has_attribute) && !defined(_MSC_VER)
	#if __has_attribute(format)
	    #define CC_FORMAT_PRINTF(formatPos, argPos) \
	        __attribute__((__format__(printf, formatPos, argPos)))
	#endif
#else
	#define CC_FORMAT_PRINTF(formatPos, argPos)
#endif

#if defined(__GNUC__) && ((__GNUC__ >= 4) || ((__GNUC__ == 3) && (__GNUC_MINOR__ >= 1)))
	#define CC_DEPRECATED_ATTRIBUTE __attribute__((deprecated))
#elif _MSC_VER >= 1400 // vs 2005 or higher
	#define CC_DEPRECATED_ATTRIBUTE __declspec(deprecated)
#else
	#define CC_DEPRECATED_ATTRIBUTE
#endif


// kazmath
#define kmScalar float
typedef struct kmMat4 {
	kmScalar mat[16];
} kmMat4;


// gl
typedef unsigned int GLenum;
typedef unsigned int GLbitfield;
typedef unsigned int GLuint;
typedef int GLint;
typedef int GLsizei;
typedef unsigned char GLboolean;
typedef signed char GLbyte;
typedef short GLshort;
typedef unsigned char GLubyte;
typedef unsigned short GLushort;
typedef unsigned long GLulong;
typedef float GLfloat;
typedef float GLclampf;
typedef double GLdouble;
typedef double GLclampd;
typedef void GLvoid;



// pugixml
namespace pugi
{
#ifdef PUGIXML_WCHAR_MODE
#	define PUGIXML_TEXT(t) L ## t
#	define PUGIXML_CHAR wchar_t
#else
#	define PUGIXML_TEXT(t) t
#	define PUGIXML_CHAR char
#endif

	typedef PUGIXML_CHAR char_t;
	class xml_node;
	class xml_attribute_iterator;
	struct xml_attribute_struct;
	struct xml_node_struct;

	class xml_node
	{
		friend class xml_attribute_iterator;
		friend class xml_node_iterator;
		friend class xml_named_node_iterator;

	protected:
		xml_node_struct* _root;

		typedef void (*unspecified_bool_type)(xml_node***);

	};

	class xml_attribute
	{
		friend class xml_attribute_iterator;
		friend class xml_node;

	private:
		xml_attribute_struct* _attr;
	
		typedef void (*unspecified_bool_type)(xml_attribute***);
	};

	class xml_attribute_iterator
	{
		friend class xml_node;

	private:
		mutable xml_attribute _wrap;
		xml_node _parent;
	};

	class xml_node_iterator
	{
		friend class xml_node;

	private:
		mutable xml_node _wrap;
		xml_node _parent;
	};

	class xml_named_node_iterator
	{
	private:
		mutable xml_node _node;
		const char_t* _name;
	};

	class xml_document: public xml_node
	{
	private:
		char_t* _buffer;

		char _memory[192];
	};
}


// cocos2d
class DS_Dictionary
{
public:
	pugi::xml_document doc;
	std::vector<pugi::xml_node> dictTree;
	bool compatible;
};

// uthash
struct UT_hash_handle;
typedef struct UT_hash_bucket {
	struct UT_hash_handle *hh_head;
	unsigned count;
	unsigned expand_mult;

} UT_hash_bucket;
typedef struct UT_hash_table {
	UT_hash_bucket *buckets;
	unsigned num_buckets, log2_num_buckets;
	unsigned num_items;
	struct UT_hash_handle *tail; /* tail hh in app order, for fast append    */
	ptrdiff_t hho; /* hash handle offset (byte pos of hash handle in element */

	/* in an ideal situation (all buckets used equally), no bucket would have
	* more than ceil(#items/#buckets) items. that's the ideal chain length. */
	unsigned ideal_chain_maxlen;

	/* nonideal_items is the number of items in the hash whose chain position
	* exceeds the ideal chain maxlen. these items pay the penalty for an uneven
	* hash distribution; reaching them in a chain traversal takes >ideal steps */
	unsigned nonideal_items;

	unsigned ineff_expands, noexpand;

	uint32_t signature; /* used only to find hash tables in external analysis */
#ifdef HASH_BLOOM
	uint32_t bloom_sig; /* used only to test bloom exists in external analysis */
	uint8_t *bloom_bv;
	char bloom_nbits;
#endif

} UT_hash_table;

typedef struct UT_hash_handle {
   struct UT_hash_table *tbl;
   void *prev;                       /* prev element in app order      */
   void *next;                       /* next element in app order      */
   struct UT_hash_handle *hh_prev;   /* previous hh in bucket order    */
   struct UT_hash_handle *hh_next;   /* next hh in bucket order        */
   void *key;                        /* ptr to enclosing struct's key  */
   unsigned keylen;                  /* enclosing struct's key len     */
   unsigned hashv;                   /* result of hash-fcn(key)        */
} UT_hash_handle;

// not in the other cocos2d namespace because they are used extensively
// and i dont want to mix sections
namespace cocos2d
{
	// CCGeometry
	class CCPoint
	{
	public:
		float x;
		float y;

		CCPoint()
			: x(.0f), y(.0f)
		{}

		CCPoint(const CCPoint& p)
			: x(p.x), y(p.y)
		{};

		CCPoint(float x, float y)
			: x(x), y(y)
		{};
	};

	class CCSize
	{
	public:
		float width;
		float height;

		inline CCSize() : width(0), height(0) {}
		inline CCSize(float width, float height) : width(width), height(height) {}
	};

	class CCRect
	{
	public:
		CCPoint origin;
		CCSize size;
	};

	typedef struct _ccColor3B
	{
		GLubyte r;
		GLubyte g;
		GLubyte b;
	} ccColor3B;

	static const ccColor3B ccWHITE = {255,255,255};
	#define CCPointZero CCPointMake(0,0)

	#define CCPointMake(x, y) cocos2d::CCPoint((float)(x), (float)(y))
	#define CCSizeMake(width, height) cocos2d::CCSize((float)(width), (float)(height))
	#define CCRectMake(x, y, width, height) cocos2d::CCRect((float)(x), (float)(y), (float)(width), (float)(height))
}

namespace cocos2d
{
	// not in cocos2d, blame geode
	// 2.2 additions
	struct ParticleStruct
	{
		int TotalParticles;
		float Duration;
		float Life;
		float LifeVar;
		int EmissionRate;
		int Angle;
		int AngleVar;
		int Speed;
		int SpeedVar;
		int PosVarX;
		int PosVarY;
		int GravityX;
		int GravityY;
		int RadialAccel;
		int RadialAccelVar;
		int TangentialAccel;
		int TangentialAccelVar;

		int StartSize;
		int StartSizeVar;
		int StartSpin;
		int StartSpinVar;
		float StartColorR;
		float StartColorVarR;
		float StartColorG;
		float StartColorVarG;
		float StartColorB;
		float StartColorVarB;
		float StartColorA;
		float StartColorVarA;

		int EndSize;
		int EndSizeVar;
		int EndSpin;
		int EndSpinVar;
		float EndColorR;
		float EndColorVarR;
		float EndColorG;
		float EndColorVarG;
		float EndColorB;
		float EndColorVarB;
		float EndColorA;
		float EndColorVarA;

		float FadeInTime;
		float FadeInTimeVar;
		float FadeOutTime;
		float FadeOutTimeVar;

		int StartRadius;
		int StartRadiusVar;
		int EndRadius;
		int EndRadiusVar;
		int RotatePerSecond;
		int RotatePerSecondVar;

		int EmitterMode;
		int PositionType;
		bool isBlendAdditive;
		bool startSpinEqualToEndSpin;
		bool rotationIsDir;
		bool dynamicRotationIsDir;
		int customParticleIdx;
		bool uniformColorMode;

		float frictionPos;
		float frictionPosVar;

		float respawn;
		float respawnVar;

		bool orderSensitive;
		bool startSizeEqualToEndSize;
		bool startRadiusEqualToEndRadius;

		bool startRGBVarSync;
		bool endRGBVarSync;

		float frictionSize;
		float frictionSizeVar;

		float frictionRot;
		float frictionRotVar;

		std::string sFrame;
	};

	// enums
	enum class CCObjectType {
		PlayLayerObjectType = 5,
		LevelEditorLayerObjectType = 6,
		GameObjectObjectType = 13,
		MenuLayerObjectType = 15,
	};

	typedef enum {
		//! 32-bit texture: RGBA8888
		kCCTexture2DPixelFormat_RGBA8888,
		//! 24-bit texture: RGBA888
		kCCTexture2DPixelFormat_RGB888,
		//! 16-bit texture without Alpha channel
		kCCTexture2DPixelFormat_RGB565,
		//! 8-bit textures used as masks
		kCCTexture2DPixelFormat_A8,
		//! 8-bit intensity texture
		kCCTexture2DPixelFormat_I8,
		//! 16-bit textures used as masks
		kCCTexture2DPixelFormat_AI88,
		//! 16-bit textures: RGBA4444
		kCCTexture2DPixelFormat_RGBA4444,
		//! 16-bit textures: RGB5A1
		kCCTexture2DPixelFormat_RGB5A1,    
		//! 4-bit PVRTC-compressed texture: PVRTC4
		kCCTexture2DPixelFormat_PVRTC4,
		//! 2-bit PVRTC-compressed texture: PVRTC2
		kCCTexture2DPixelFormat_PVRTC2,


		//! Default texture format: RGBA8888
		kCCTexture2DPixelFormat_Default = kCCTexture2DPixelFormat_RGBA8888,

		// backward compatibility stuff
		kTexture2DPixelFormat_RGBA8888 = kCCTexture2DPixelFormat_RGBA8888,
		kTexture2DPixelFormat_RGB888 = kCCTexture2DPixelFormat_RGB888,
		kTexture2DPixelFormat_RGB565 = kCCTexture2DPixelFormat_RGB565,
		kTexture2DPixelFormat_A8 = kCCTexture2DPixelFormat_A8,
		kTexture2DPixelFormat_RGBA4444 = kCCTexture2DPixelFormat_RGBA4444,
		kTexture2DPixelFormat_RGB5A1 = kCCTexture2DPixelFormat_RGB5A1,
		kTexture2DPixelFormat_Default = kCCTexture2DPixelFormat_Default

	} CCTexture2DPixelFormat;

	enum {
		kCCUniformPMatrix,
		kCCUniformMVMatrix,
		kCCUniformMVPMatrix,
		kCCUniformTime,
		kCCUniformSinTime,
		kCCUniformCosTime,
		kCCUniformRandom01,
		kCCUniformSampler,

		kCCUniform_MAX,
	};

	typedef enum {
		/// sets a 2D projection (orthogonal projection)
		kCCDirectorProjection2D,
		
		/// sets a 3D projection with a fovy=60, znear=0.5f and zfar=1500.
		kCCDirectorProjection3D,
		
		/// it calls "updateProjection" on the projection delegate.
		kCCDirectorProjectionCustom,
		
		/// Default projection is 3D projection
		kCCDirectorProjectionDefault = kCCDirectorProjection3D,
	} ccDirectorProjection;

	typedef enum {
	//    CC_GL_SCISSOR_TEST = 1 << 0,
	//    CC_GL_STENCIL_TEST = 1 << 1,
	//    CC_GL_DEPTH_TEST = 1 << 2,
	//    CC_GL_BLEND = 1 << 3,
	//    CC_GL_DITHER = 1 << 4,

	//    CC_GL_ALL = ( CC_GL_SCISSOR_TEST | CC_GL_STENCIL_TEST | CC_GL_DEPTH_TEST | CC_GL_BLEND | CC_GL_DITHER ),
	//    CC_GL_ALL = ( CC_GL_BLEND ),
		CC_GL_ALL = 0,

	} ccGLServerState;

	enum ccScriptType {
		kScriptTypeNone = 0,
		kScriptTypeLua,
		kScriptTypeJavascript
	};

	typedef enum
	{
	    // this one might not actually exist in gd itself
	    KEY_Unknown = -0x01,
	    KEY_None = 0x00,
	    KEY_Backspace = 0x08,
	    KEY_Tab = 0x09,
	    KEY_Clear = 0x0C,
	    KEY_Enter = 0x0D,
	    KEY_Shift = 0x10,
	    KEY_Control = 0x11,
	    KEY_Alt = 0x12,
	    KEY_Pause = 0x13,
	    KEY_CapsLock = 0x14,
	    KEY_Escape = 0x1B,
	    KEY_Space = 0x20,
	    KEY_PageUp = 0x21,
	    KEY_PageDown = 0x22,
	    KEY_End = 0x23,
	    KEY_Home = 0x24,
	    KEY_Left = 0x25,
	    KEY_Up = 0x26,
	    KEY_Right = 0x27,
	    KEY_Down = 0x28,
	    KEY_Select = 0x29,
	    KEY_Print = 0x2A,
	    KEY_Execute = 0x2B,
	    KEY_PrintScreen = 0x2C,
	    KEY_Insert = 0x2D,
	    KEY_Delete = 0x2E,
	    KEY_Help = 0x2F,
	    KEY_Zero = 0x30,
	    KEY_One = 0x31,
	    KEY_Two = 0x32,
	    KEY_Three = 0x33,
	    KEY_Four = 0x34,
	    KEY_Five = 0x35,
	    KEY_Six = 0x36,
	    KEY_Seven = 0x37,
	    KEY_Eight = 0x38,
	    KEY_Nine = 0x39,
	    KEY_A = 0x41,
	    KEY_B = 0x42,
	    KEY_C = 0x43,
	    KEY_D = 0x44,
	    KEY_E = 0x45,
	    KEY_F = 0x46,
	    KEY_G = 0x47,
	    KEY_H = 0x48,
	    KEY_I = 0x49,
	    KEY_J = 0x4A,
	    KEY_K = 0x4B,
	    KEY_L = 0x4C,
	    KEY_M = 0x4D,
	    KEY_N = 0x4E,
	    KEY_O = 0x4F,
	    KEY_P = 0x50,
	    KEY_Q = 0x51,
	    KEY_R = 0x52,
	    KEY_S = 0x53,
	    KEY_T = 0x54,
	    KEY_U = 0x55,
	    KEY_V = 0x56,
	    KEY_W = 0x57,
	    KEY_X = 0x58,
	    KEY_Y = 0x59,
	    KEY_Z = 0x5A,
	    KEY_LeftWindowsKey = 0x5B,
	    KEY_RightWindowsKey = 0x5C,
	    KEY_ApplicationsKey = 0x5D,
	    KEY_Sleep = 0x5F,
	    KEY_NumPad0 = 0x60,
	    KEY_NumPad1 = 0x61,
	    KEY_NumPad2 = 0x62,
	    KEY_NumPad3 = 0x63,
	    KEY_NumPad4 = 0x64,
	    KEY_NumPad5 = 0x65,
	    KEY_NumPad6 = 0x66,
	    KEY_NumPad7 = 0x67,
	    KEY_NumPad8 = 0x68,
	    KEY_NumPad9 = 0x69,
	    KEY_Multiply = 0x6A,
	    KEY_Add = 0x6B,
	    KEY_Seperator = 0x6C,
	    KEY_Subtract = 0x6D,
	    KEY_Decimal = 0x6E,
	    KEY_Divide = 0x6F,
	    KEY_F1 = 0x70,
	    KEY_F2 = 0x71,
	    KEY_F3 = 0x72,
	    KEY_F4 = 0x73,
	    KEY_F5 = 0x74,
	    KEY_F6 = 0x75,
	    KEY_F7 = 0x76,
	    KEY_F8 = 0x77,
	    KEY_F9 = 0x78,
	    KEY_F10 = 0x79,
	    KEY_F11 = 0x7A,
	    KEY_F12 = 0x7B,
	    KEY_F13 = 0x7C,
	    KEY_F14 = 0x7D,
	    KEY_F15 = 0x7E,
	    KEY_F16 = 0x7F,
	    KEY_F17 = 0x80,
	    KEY_F18 = 0x81,
	    KEY_F19 = 0x82,
	    KEY_F20 = 0x83,
	    KEY_F21 = 0x84,
	    KEY_F22 = 0x85,
	    KEY_F23 = 0x86,
	    KEY_F24 = 0x87,
	    KEY_Numlock = 0x90,
	    KEY_ScrollLock = 0x91,
	    KEY_LeftShift = 0xA0,
	    KEY_RightShift = 0xA1,
	    KEY_LeftControl = 0xA2,
	    KEY_RightContol = 0xA3,
	    KEY_LeftMenu = 0xA4,
	    KEY_RightMenu = 0xA5,
	    KEY_BrowserBack = 0xA6,
	    KEY_BrowserForward = 0xA7,
	    KEY_BrowserRefresh = 0xA8,
	    KEY_BrowserStop = 0xA9,
	    KEY_BrowserSearch = 0xAA,
	    KEY_BrowserFavorites = 0xAB,
	    KEY_BrowserHome = 0xAC,
	    KEY_VolumeMute = 0xAD,
	    KEY_VolumeDown = 0xAE,
	    KEY_VolumeUp = 0xAF,
	    KEY_NextTrack = 0xB0,
	    KEY_PreviousTrack = 0xB1,
	    KEY_StopMedia = 0xB2,
	    KEY_PlayPause = 0xB3,
	    KEY_LaunchMail = 0xB4,
	    KEY_SelectMedia = 0xB5,
	    KEY_LaunchApp1 = 0xB6,
	    KEY_LaunchApp2 = 0xB7,
	    KEY_OEM1 = 0xBA,
	    KEY_OEMPlus = 0xB8,
	    KEY_OEMComma = 0xBC,
	    KEY_OEMMinus = 0xBD,
	    KEY_OEMPeriod = 0xBE,
	    KEY_OEM2 = 0xBF,
	    KEY_OEM3 = 0xC0,
	    KEY_OEM4 = 0xDB,
	    KEY_OEM5 = 0xDC,
	    KEY_OEM6 = 0xDD,
	    KEY_OEM7 = 0xDE,
	    KEY_OEM8 = 0xDF,
	    KEY_OEM102 = 0xE2,
	    KEY_Process = 0xE5,
	    KEY_Packet = 0xE7,
	    KEY_Attn = 0xF6,
	    KEY_CrSel = 0xF7,
	    KEY_ExSel = 0xF8,
	    KEY_EraseEOF = 0xF9,
	    KEY_Play = 0xFA,
	    KEY_Zoom = 0xFB,
	    KEY_PA1 = 0xFD,
	    KEY_OEMClear = 0xFE,
	    KEY_ArrowUp = 0x11B,
	    KEY_ArrowDown = 0x11C,
	    KEY_ArrowLeft = 0x11D,
	    KEY_ArrowRight = 0x11E,
	    CONTROLLER_A = 0x3E9,
	    CONTROLLER_B = 0x3EB,
	    CONTROLLER_Y = 0x3ED,
	    CONTROLLER_X = 0x3EF,
	    CONTROLLER_Start = 0x3F1,
	    CONTROLLER_Back = 0x3F3,
	    CONTROLLER_RB = 0x3F5,
	    CONTROLLER_LB = 0x3F7,
	    CONTROLLER_RT = 0x3F9,
	    CONTROLLER_LT = 0x3FB,
	    CONTROLLER_Up = 0x3FD,
	    CONTROLLER_Down = 0x3FF,
	    CONTROLLER_Left = 0x401,
	    CONTROLLER_Right = 0x403,
	    CONTROLLER_LTHUMBSTICK_UP = 0x405,
	    CONTROLLER_LTHUMBSTICK_DOWN = 0x407,
	    CONTROLLER_LTHUMBSTICK_LEFT = 0x409,
	    CONTROLLER_LTHUMBSTICK_RIGHT = 0x40B,
	    CONTROLLER_RTHUMBSTICK_UP = 0x40D,
	    CONTROLLER_RTHUMBSTICK_DOWN = 0x40F,
	    CONTROLLER_RTHUMBSTICK_LEFT = 0x411,
	    CONTROLLER_RTHUMBSTICK_RIGHT = 0x413,
	} enumKeyCodes;

	typedef enum {
		kCCTouchesAllAtOnce,
		kCCTouchesOneByOne,
	} ccTouchesMode;

	typedef enum
	{
		kCCTextAlignmentLeft,
		kCCTextAlignmentCenter,
		kCCTextAlignmentRight,
	} CCTextAlignment;

	typedef enum
	{
		kCCVerticalTextAlignmentTop,
		kCCVerticalTextAlignmentCenter,
		kCCVerticalTextAlignmentBottom,
	} CCVerticalTextAlignment;

	typedef enum eImageFormat
	{
		kCCImageFormatJPEG      = 0,
		kCCImageFormatPNG       = 1,
	} tCCImageFormat;

	typedef enum {
		/** Living particles are attached to the world and are unaffected by emitter repositioning. */
		kCCPositionTypeFree,

		/** Living particles are attached to the world but will follow the emitter repositioning.
		Use case: Attach an emitter to an sprite, and you want that the emitter follows the sprite.
		*/
		kCCPositionTypeRelative,

		/** Living particles are attached to the emitter and are translated along with it. */
		kCCPositionTypeGrouped,
	} tCCPositionType;

	typedef enum  
	{
		kCCMenuStateWaiting,
		kCCMenuStateTrackingTouch
	} tCCMenuState;

	RT_ADD(
		typedef enum {
			kTextureQualityLow = 1,
			kTextureQualityMedium,
			kTextureQualityHigh
		} TextureQuality;

		typedef enum {
			kPopTransitionFade,
			kPopTransitionMoveInT
		} PopTransition;
	)

	typedef enum {
		/// Radial Counter-Clockwise
		kCCProgressTimerTypeRadial,
		/// Bar
		kCCProgressTimerTypeBar,
	} CCProgressTimerType;


	// structs
	struct _listEntry;
	struct _hashUniformEntry;
	struct _hashUpdateEntry;
	struct _hashSelectorEntry;
	struct _hashElement;

	typedef struct _ccBlendFunc
	{
		//! source blend function
		GLenum src;
		//! destination blend function
		GLenum dst;
	} ccBlendFunc;

	typedef struct _ccFontShadow
	{
	public:
		
		// shadow is not enabled by default
		_ccFontShadow(): m_shadowEnabled(false) {}
		
		// true if shadow enabled
		bool   m_shadowEnabled;
		// shadow x and y offset
		cocos2d::CCSize m_shadowOffset;
		// shadow blurrines
		float  m_shadowBlur;
		// shadow opacity
		float  m_shadowOpacity;
	} ccFontShadow;

	typedef struct _ccFontStroke
	{
	public:
		
		// stroke is disabled by default
		_ccFontStroke(): m_strokeEnabled(false) {}
		
		// true if stroke enabled
		bool        m_strokeEnabled;
		// stroke color
		cocos2d::ccColor3B   m_strokeColor;
		// stroke size
		float       m_strokeSize;
	} ccFontStroke;

	typedef struct _ccFontDefinition
	{
	public:
		_ccFontDefinition():
			m_alignment(cocos2d::kCCTextAlignmentCenter),
			m_vertAlignment(cocos2d::kCCVerticalTextAlignmentTop),
			m_fontFillColor(cocos2d::ccWHITE),
			m_dimensions(CCSizeMake(0, 0))
		{}
		
		// font name
		std::string             m_fontName;
		// font size
		int                     m_fontSize;
		// horizontal alignment
		cocos2d::CCTextAlignment         m_alignment;
		// vertical alignment
		cocos2d::CCVerticalTextAlignment m_vertAlignment;
		// renering box
		cocos2d::CCSize                  m_dimensions;
		// font color
		cocos2d::ccColor3B               m_fontFillColor;
		// font shadow
		ccFontShadow            m_shadow;
		// font stroke
		ccFontStroke            m_stroke;
		
	} ccFontDefinition;

	typedef struct _ccTexParams {
		GLuint    minFilter;
		GLuint    magFilter;
		GLuint    wrapS;
		GLuint    wrapT;
	} ccTexParams;

	typedef struct _ccTex2F {
		GLfloat u;
		GLfloat v;
	} ccTex2F;

	typedef struct _ccVertex2F
	{
		GLfloat x;
		GLfloat y;
	} ccVertex2F;

	typedef struct _ccVertex3F
	{
		GLfloat x;
		GLfloat y;
		GLfloat z;
	} ccVertex3F;

	typedef struct _ccColor4F {
		GLfloat r;
		GLfloat g;
		GLfloat b;
		GLfloat a;
	} ccColor4F;

	typedef struct _ccColor4B
	{
		GLubyte r;
		GLubyte g;
		GLubyte b;
		GLubyte a;
	} ccColor4B;

	typedef struct _ccHSVValue
	{
		float h, s, v;
		bool absoluteSaturation;
		bool absoluteBrightness;
		private: unsigned char __pad[2];
	} ccHSVValue;

	typedef struct sCCParticle {
		CCPoint     pos;
		CCPoint     startPos;

		ccColor4F    color;
		ccColor4F    deltaColor;

		float        size;
		float        deltaSize;

		float        rotation;
		float        deltaRotation;

		float        timeToLive;

		unsigned int    atlasIndex;

		//! Mode A: gravity, direction, radial accel, tangential accel
		struct {
			CCPoint        dir;
			float        radialAccel;
			float        tangentialAccel;
		} modeA;

		//! Mode B: radius mode
		struct {
			float        angle;
			float        degreesPerSecond;
			float        radius;
			float        deltaRadius;
		} modeB;

	} tCCParticle;

	typedef struct _ccV2F_C4B_T2F
	{
		//! vertices (2F)
		ccVertex2F        vertices;
		//! colors (4B)
		ccColor4B        colors;
		//! tex coords (2F)
		ccTex2F            texCoords;
	} ccV2F_C4B_T2F;

	typedef struct _ccV3F_C4B_T2F
	{
		//! vertices (3F)
		ccVertex3F        vertices;            // 12 bytes
	//    char __padding__[4];

		//! colors (4B)
		ccColor4B        colors;                // 4 bytes
	//    char __padding2__[4];

		// tex coords (2F)
		ccTex2F            texCoords;            // 8 bytes
	} ccV3F_C4B_T2F;

	typedef struct _ccV3F_C4B_T2F_Quad
	{
		//! top left
		ccV3F_C4B_T2F    tl;
		//! bottom left
		ccV3F_C4B_T2F    bl;
		//! top right
		ccV3F_C4B_T2F    tr;
		//! bottom right
		ccV3F_C4B_T2F    br;
	} ccV3F_C4B_T2F_Quad;

	typedef enum LanguageType
	{
		kLanguageEnglish = 0,
		kLanguageChinese,
		kLanguageFrench,
		kLanguageItalian,
		kLanguageGerman,
		kLanguageSpanish,
		kLanguageDutch,
		kLanguageRussian,
		kLanguageKorean,
		kLanguageJapanese,
		kLanguageHungarian,
		kLanguagePortuguese,
		kLanguageArabic
	} ccLanguageType;

	enum TargetPlatform
	{
		kTargetWindows,
		kTargetLinux,
		kTargetMacOS,
		kTargetAndroid,
		kTargetIphone,
		kTargetIpad,
		kTargetBlackBerry,
		kTargetNaCl,
		kTargetEmscripten,
		kTargetTizen,
		kTargetWinRT,
		kTargetWP8
	};

	enum ccTouchType {
		CCTOUCHBEGAN = 0,
		CCTOUCHMOVED = 1,
		CCTOUCHENDED = 2,
		CCTOUCHCANCELLED = 3,
		
		ccTouchMax = 4,
	};

	enum ResolutionPolicy
	{
		// The entire application is visible in the specified area without trying to preserve the original aspect ratio.
		// Distortion can occur, and the application may appear stretched or compressed.
		kResolutionExactFit,
		// The entire application fills the specified area, without distortion but possibly with some cropping,
		// while maintaining the original aspect ratio of the application.
		kResolutionNoBorder,
		// The entire application is visible in the specified area without distortion while maintaining the original
		// aspect ratio of the application. Borders can appear on two sides of the application.
		kResolutionShowAll,
		// The application takes the height of the design resolution size and modifies the width of the internal
		// canvas so that it fits the aspect ratio of the device
		// no distortion will occur however you must make sure your application works on different
		// aspect ratios
		kResolutionFixedHeight,
		// The application takes the width of the design resolution size and modifies the height of the internal
		// canvas so that it fits the aspect ratio of the device
		// no distortion will occur however you must make sure your application works on different
		// aspect ratios
		kResolutionFixedWidth,

		kResolutionUnKnown,
	};

	typedef enum {
		// the back key clicked msg
		kTypeBackClicked = 1,
		kTypeMenuClicked,
	} ccKeypadMSGType;


	struct  cc_timeval
	{
	#ifdef __native_client__
		time_t    tv_sec;        // seconds
	#else
		long    tv_sec;        // seconds
	#endif
		int tv_usec;    // microSeconds
	};

	typedef struct _BMFontPadding {
		/// padding left
		int    left;
		/// padding top
		int top;
		/// padding right
		int right;
		/// padding bottom
		int bottom;
	} ccBMFontPadding;

	typedef struct _BMFontDef {
		//! ID of the character
		unsigned int charID;
		//! origin and size of the font
		CCRect rect;
		//! The X amount the image should be offset when drawing the image (in pixels)
		short xOffset;
		//! The Y amount the image should be offset when drawing the image (in pixels)
		short yOffset;
		//! The amount to move the current position after drawing the character (in pixels)
		short xAdvance;
	} ccBMFontDef;

	typedef struct _FontDefHashElement
	{
		unsigned int	key;		// key. Font Unicode value
		ccBMFontDef		fontDef;	// font definition
		UT_hash_handle	hh;
	} tCCFontDefHashElement;

	typedef struct _KerningHashElement
	{
		int				key;		// key for the hash. 16-bit for 1st element, 16-bit for 2nd element
		int				amount;
		UT_hash_handle	hh;
	} tCCKerningHashElement;

	typedef struct
	{
		CCRect  begin;              // the soft keyboard rectangle when animation begins
		CCRect  end;                // the soft keyboard rectangle when animation ends
		float     duration;           // the soft keyboard animation duration
	} CCIMEKeyboardNotificationInfo;

	typedef struct GLFWvidmode
	{
		/*! The width, in screen coordinates, of the video mode.
		*/
		int width;
		/*! The height, in screen coordinates, of the video mode.
		*/
		int height;
		/*! The bit depth of the red channel of the video mode.
		*/
		int redBits;
		/*! The bit depth of the green channel of the video mode.
		*/
		int greenBits;
		/*! The bit depth of the blue channel of the video mode.
		*/
		int blueBits;
		/*! The refresh rate, in Hz, of the video mode.
		*/
		int refreshRate;
	} GLFWvidmode;

	struct ccTouchHandlerHelperData {
		// we only use the type
	//    void (StandardTouchDelegate::*touchesSel)(CCSet*, CCEvent*);
	//    void (TargetedTouchDelegate::*touchSel)(NSTouch*, CCEvent*);
		int  m_type;
	};


	// CCObject
	class CCObject;
	class CCZone;
	class CCAutoreleasePool;
	class CCArray;
	class CCNode;
	class CCEvent;
	typedef struct _ccArray {
		unsigned int num, max;
		// 2.2 additions
		unsigned int unknown;
		CCObject** arr;
	} ccArray;
	typedef struct _ccCArray {
		unsigned int num, max;
		void** arr;
	} ccCArray;

	/* typedefs */
	typedef void (CCObject::*SEL_SCHEDULE)(float);
	typedef void (CCObject::*SEL_CallFunc)();
	typedef void (CCObject::*SEL_CallFuncN)(CCNode*);
	typedef void (CCObject::*SEL_CallFuncND)(CCNode*, void*);
	typedef void (CCObject::*SEL_CallFuncO)(CCObject*);
	typedef void (CCObject::*SEL_MenuHandler)(CCObject*);
	typedef void (CCObject::*SEL_EventHandler)(CCEvent*);
	typedef int (CCObject::*SEL_Compare)(CCObject*);

	typedef long long (*CUSTOM_WND_PROC)(unsigned int message, unsigned long long wParam, long long lParam, int* pProcessed);


	class CCCopying
	{
	public:
		virtual CCObject* copyWithZone(CCZone* pZone) { return 0; }
	};

	class CCBool;
	class CCInteger;
	class CCFloat;
	class CCDouble;
	class CCString;
	class CCDictionary;
	class CCSet;
	class CCDataVisitor
	{
	public:
		virtual ~CCDataVisitor() {}

		/** default method, called from non-overloaded methods and for unrecognized objects */
		virtual void visitObject(const CCObject *p) = 0;

		virtual void visit(const CCBool *p);
		virtual void visit(const CCInteger *p);
		virtual void visit(const CCFloat *p);
		virtual void visit(const CCDouble *p);
		virtual void visit(const CCString *p);
		virtual void visit(const CCArray *p);
		virtual void visit(const CCDictionary *p);
		virtual void visit(const CCSet *p);
	};

	class CCObject : public CCCopying
	{
	public:
		unsigned int m_uID;
		int m_nLuaID;
	protected:
		int m_nTag;
		unsigned int        m_uReference;
		// count of autorelease
		unsigned int        m_uAutoReleaseCount;

		CCObjectType m_eObjType;

		int m_uIndexInArray;

		// 2.2 additions

		int m_uUnknown;
		int m_unknown2;
		int m_nZOrder;
		int m_uOrderOfArrival;
		int m_unknown5;

	public:
		CCObject(void);
		virtual ~CCObject(void);

		void release(void);
		void retain(void);
		CCObject* autorelease(void);
		CCObject* copy(void);
		bool isSingleReference(void) const;
		inline unsigned int retainCount(void) const {
			return m_uReference;
		}
		virtual bool isEqual(const CCObject* pObject);

		virtual void acceptVisitor(CCDataVisitor &visitor);

		virtual void update(float dt) {CC_UNUSED_PARAM(dt);};
		
		virtual void encodeWithCoder(DS_Dictionary*);

		static CCObject* createWithCoder(DS_Dictionary*);
		
		virtual bool canEncode();

		inline CCObjectType getObjType() const {
			return m_eObjType;
		}
	
		virtual int getTag() const;

		virtual void setTag(int nTag);
		
		inline void setObjType(CCObjectType type) {
			m_eObjType = type;
		}

		friend class CCAutoreleasePool;
	};

	class CCBool : public CCObject
	{
	public:
		CCBool(bool v)
			: m_bValue(v) {}
		bool getValue() const {return m_bValue;}
		bool setValue(bool value) { m_bValue = value; return value; }

		static CCBool* create(bool v)
		{
			CCBool* pRet = new CCBool(v);
			if (pRet)
			{
				pRet->autorelease();
			}
			return pRet;
		}

		virtual void acceptVisitor(CCDataVisitor &visitor) { visitor.visit(this); }

	private:
		bool m_bValue;
	};
	class CCInteger : public CCObject
	{
	public:
		CCInteger(int v)
			: m_nValue(v) {}
		int getValue() const {return m_nValue;}
		void setValue(int v) { m_nValue = v; };

		static CCInteger* create(int v)
		{
			CCInteger* pRet = new CCInteger(v);
			pRet->autorelease();
			return pRet;
		}

		virtual void acceptVisitor(CCDataVisitor &visitor) { visitor.visit(this); }

	private:
		int m_nValue;
	};
	class CCFloat : public CCObject
	{
	public:
		CCFloat(float v)
			: m_fValue(v) {}
		float getValue() const {return m_fValue;}

		static CCFloat* create(float v)
		{
			CCFloat* pRet = new CCFloat(v);
			if (pRet)
			{
				pRet->autorelease();
			}
			return pRet;
		}

		virtual void acceptVisitor(CCDataVisitor &visitor) { visitor.visit(this); }

	private:
		float m_fValue;
	};
	class CCDouble : public CCObject
	{
	public:
		CCDouble(double v)
			: m_dValue(v) {}
		double getValue() const {return m_dValue;}

		static CCDouble* create(double v)
		{
			CCDouble* pRet = new CCDouble(v);
			if (pRet)
			{
				pRet->autorelease();
			}
			return pRet;
		}

		virtual void acceptVisitor(CCDataVisitor &visitor) { visitor.visit(this); }

	private:
		double m_dValue;
	};
	#define kMaxStringLen (1024*100)
	class CCString : public CCObject
	{
	public:
		inline CCString(const char* str) : m_sString(str) {}
		inline CCString(const std::string& str) : m_sString(str.c_str()) {}
		inline CCString(const CCString& str) {}
		virtual inline ~CCString() {}
		
		CCString& operator= (const CCString& other);

		bool initWithFormat(const char* format, ...) CC_FORMAT_PRINTF(2, 3);

		int intValue() const;

		unsigned int uintValue() const;

		float floatValue() const;

		double doubleValue() const;

		bool boolValue() const;

		const char* getCString() const;

		unsigned int length() const;

		int compare(const char*) const;

		virtual CCObject* copyWithZone(CCZone* pZone);
		virtual bool isEqual(const CCObject* pObject);

		static CCString* createWithData(const unsigned char* pData, unsigned long nLen);

		static CCString* createWithContentsOfFile(const char* pszFileName);
		virtual void acceptVisitor(CCDataVisitor &visitor);

	private:
		bool initWithFormatAndValist(const char* format, va_list ap);

	public:
		std::string m_sString;
	};
	typedef std::set<CCObject*>::iterator CCSetIterator;
	class CCSet : public CCObject
	{
	public:
		CCSet(void);
		// CCSet(const CCSet &rSetObject);
		virtual ~CCSet(void);

		static CCSet * create();

		CCSet* copy();
		CCSet* mutableCopy();
		int count();
		void addObject(CCObject *pObject);
		void removeObject(CCObject *pObject);
		void removeAllObjects();
		bool containsObject(CCObject *pObject);
		CCSetIterator begin();
		CCSetIterator end();
		CCObject* anyObject();
		virtual void acceptVisitor(CCDataVisitor &visitor);

	private:
		std::set<CCObject *> *m_pSet;
	};


	// CCArray
	class CCAutoreleasePool : public CCObject
	{
	private:
		CCArray* m_pManagedObjectArray;
	};

	class CCPoolManager
	{
	private:
		CCArray* m_pReleasePoolStack;    
		CCAutoreleasePool* m_pCurReleasePool;

	private:
		friend class CCAutoreleasePool;
	};

	class CCArray : public CCObject
	{
	public:
		~CCArray();

		static CCArray* create();
		static CCArray* create(CCObject* pObject, ...);
		static CCArray* createWithObject(CCObject* pObject);
		static CCArray* createWithCapacity(unsigned int capacity);
		static CCArray* createWithArray(CCArray* otherArray);
		static CCArray* createWithContentsOfFile(const char* pFileName);
		
		static CCArray* createWithContentsOfFileThreadSafe(const char* pFileName);

		bool init();
		bool initWithObject(CCObject* pObject);
		bool initWithObjects(CCObject* pObject, ...);
		bool initWithCapacity(unsigned int capacity);
		bool initWithArray(CCArray* otherArray);

		unsigned int count() const;
		unsigned int capacity() const;
		unsigned int indexOfObject(CCObject* object) const;
		CCObject* objectAtIndex(unsigned int index);
		CCString* stringAtIndex(unsigned int index);

		CCObject* firstObject();
		CCObject* lastObject();
		CCObject* randomObject();
		bool containsObject(CCObject* object) const;
		bool isEqualToArray(CCArray* pOtherArray);

		void addObject(CCObject* object);

		void addObjectNew(CCObject* object);
		void addObjectsFromArray(CCArray* otherArray);
		void insertObject(CCObject* object, unsigned int index);

		void removeFirstObject(bool bReleaseObj = true);
		void removeLastObject(bool bReleaseObj = true);
		void removeObject(CCObject* object, bool bReleaseObj = true);
		void removeObjectAtIndex(unsigned int index, bool bReleaseObj = true);
		void removeObjectsInArray(CCArray* otherArray);
		void removeAllObjects();
		void fastRemoveObject(CCObject* object);
		void fastRemoveObjectAtIndex(unsigned int index);
		void fastRemoveObjectAtIndexNew(unsigned int index);

		void fastRemoveObjectAtIndexChild(unsigned int);

		void recreateNewIndexes();
		void removeObjectAtIndexChild(unsigned int, bool);

		void exchangeObject(CCObject* object1, CCObject* object2);
		void exchangeObjectAtIndex(unsigned int index1, unsigned int index2);

		void replaceObjectAtIndex(unsigned int uIndex, CCObject* pObject, bool bReleaseObject = true);

		void reverseObjects();
		void reduceMemoryFootprint();
	
		virtual CCObject* copyWithZone(CCZone* pZone);

		virtual void acceptVisitor(CCDataVisitor &visitor);

	public:
		ccArray* data;
	};


	// CCDictionary
	class CCDictElement
	{
	private:
		CCDictElement(const char* pszKey, CCObject* pObject);
		CCDictElement(intptr_t iKey, CCObject* pObject);

	public:
		~CCDictElement();

		inline const char* getStrKey() const
		{
			CCAssert(m_szKey[0] != '\0', "Should not call this function for integer dictionary");
			return m_szKey;
		}

		inline intptr_t getIntKey() const
		{
			CCAssert(m_szKey[0] == '\0', "Should not call this function for string dictionary");
			return m_iKey;
		}

		inline CCObject* getObject() const { return m_pObject; }

	private:
		// The max length of string key.
		#define   MAX_KEY_LEN   256
		// char array is needed for HASH_ADD_STR in UT_HASH.
		// So it's a pain that all elements will allocate 256 bytes for this array.
		char      m_szKey[MAX_KEY_LEN];     // hash key of string type
		intptr_t  m_iKey;       // hash key of integer type
		CCObject* m_pObject;    // hash value
	public:
		UT_hash_handle hh;      // makes this class hashable
		friend class CCDictionary; // declare CCDictionary as friend class
	};

	class CCDictionary : public CCObject
	{
	public:
		CCDictionary();
		~CCDictionary();

		unsigned int count();

		CCArray* allKeys();

		CCArray* allKeysForObject(CCObject* object);

		CCObject* objectForKey(const std::string& key);

		CCObject* objectForKey(intptr_t key);
		
		const CCString* valueForKey(const std::string& key);
		
		const CCString* valueForKey(intptr_t key);

		void setObject(CCObject* pObject, const std::string& key);
		
		void setObject(CCObject* pObject, intptr_t key);

		void removeObjectForKey(const std::string& key);

		void removeObjectForKey(intptr_t key);
		
		void removeObjectsForKeys(CCArray* pKeyArray);
		
		void removeObjectForElememt(CCDictElement* pElement);
		
		void removeAllObjects();

		virtual CCObject* copyWithZone(CCZone* pZone);

		CCObject* randomObject();

		static CCDictionary* create();

		static CCDictionary* createWithDictionary(CCDictionary* srcDict);
		
		static CCDictionary* createWithContentsOfFile(const char *pFileName);
		
		bool writeToFile(const char *fullPath);
		
		static CCDictionary* createWithContentsOfFileThreadSafe(const char *pFileName);

		virtual void acceptVisitor(CCDataVisitor &visitor);

		char const* charForKey(std::string const&);
		std::string getFirstKey();

	private:
		void setObjectUnSafe(CCObject* pObject, const std::string& key);
		void setObjectUnSafe(CCObject* pObject, const intptr_t key);
		
	public:
		CCDictElement* m_pElements;

		enum CCDictType
		{
			kCCDictUnknown = 0,
			kCCDictStr,
			kCCDictInt
		};

		CCDictType m_eDictType;
	};


	// CCNode
	struct CCAffineTransform {
		float a, b, c, d;
		float tx, ty;
	};

	class CCCamera : public CCObject
	{
	protected:
		float m_fEyeX;
		float m_fEyeY;
		float m_fEyeZ;

		float m_fCenterX;
		float m_fCenterY;
		float m_fCenterZ;

		float m_fUpX;
		float m_fUpY;
		float m_fUpZ;

		bool m_bDirty;
		kmMat4    m_lookupMatrix;
	public:
		CCCamera(void);
		~CCCamera(void);

		void init(void);
		const char* description(void);

		inline void setDirty(bool bValue) { m_bDirty = bValue; }
		inline bool isDirty(void) { return m_bDirty; }

		void restore(void);
		void locate(void);
		void setEyeXYZ(float fEyeX, float fEyeY, float fEyeZ);
		void setCenterXYZ(float fCenterX, float fCenterY, float fCenterZ);
		void setUpXYZ(float fUpX, float fUpY, float fUpZ);

		void getEyeXYZ(float *pEyeX, float *pEyeY, float *pEyeZ);
		void getCenterXYZ(float *pCenterX, float *pCenterY, float *pCenterZ);
		void getUpXYZ(float *pUpX, float *pUpY, float *pUpZ);
	public:
		static float getZEye();
	};

	class CCGLProgram : CCObject
	{
	public:
		CCGLProgram();
		virtual ~CCGLProgram();

	protected:
		GLuint            m_uProgram;
		GLuint            m_uVertShader;
		GLuint            m_uFragShader;
		GLint             m_uUniforms[kCCUniform_MAX];
		struct _hashUniformEntry* m_pHashForUniforms;
		bool              m_bUsesTime;
		bool              m_hasShaderCompiler;

		#if 0 // windows phone my beloved
		#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
			std::string       m_shaderId;
		#endif
		#endif
	};

	class CCImage : public CCObject
	{
	public:
		CC_SYNTHESIZE_READONLY_NV(unsigned short,   m_nWidth,       Width);
		CC_SYNTHESIZE_READONLY_NV(unsigned short,   m_nHeight,      Height);
		CC_SYNTHESIZE_READONLY_NV(int,     m_nBitsPerComponent,   BitsPerComponent);

	protected:
		unsigned char *m_pData;
		bool m_bHasAlpha;
		bool m_bPreMulti;

	#if 0
	#if (CC_TARGET_PLATFORM == CC_PLATFORM_WINRT) || (CC_TARGET_PLATFORM == CC_PLATFORM_WP8)
		CCFreeTypeFont* m_ft;
	#endif
	#endif
	};

	class CCTexture2D : public CCObject
	{
	public:
		CCTexture2D();
		virtual ~CCTexture2D();
		const char* description(void);

		void releaseData(void *data);
		void* keepData(void *data, unsigned int length);

		bool initWithData(const void* data, CCTexture2DPixelFormat pixelFormat, unsigned int pixelsWide, unsigned int pixelsHigh, const CCSize& contentSize);

		void drawAtPoint(const CCPoint& point);
		void drawInRect(const CCRect& rect);

		bool initWithString(const char *text,  const char *fontName, float fontSize, const CCSize& dimensions, CCTextAlignment hAlignment, CCVerticalTextAlignment vAlignment);
		bool initWithString(const char *text, const char *fontName, float fontSize);
		bool initWithString(const char *text, ccFontDefinition *textDefinition);
		
		bool initWithPVRFile(const char* file);
		
		bool initWithETCFile(const char* file);

		void setTexParameters(ccTexParams* texParams);

		void setAntiAliasTexParameters();

		void setAliasTexParameters();


		void generateMipmap();

		const char* stringForFormat();

		unsigned int bitsPerPixelForFormat();  

		unsigned int bitsPerPixelForFormat(CCTexture2DPixelFormat format);

		static void setDefaultAlphaPixelFormat(CCTexture2DPixelFormat format);

		static CCTexture2DPixelFormat defaultAlphaPixelFormat();

		static void PVRImagesHavePremultipliedAlpha(bool haveAlphaPremultiplied);

		const CCSize& getContentSizeInPixels();
		
		bool hasPremultipliedAlpha();
		bool hasMipmaps();

		void releaseGLTexture();

	private:
		bool initPremultipliedATextureWithImage(CCImage * image, unsigned int pixelsWide, unsigned int pixelsHigh);

		// By default PVR images are treated as if they don't have the alpha channel premultiplied
		bool m_bPVRHaveAlphaPremultiplied;

		/** pixel format of the texture */
		CC_PROPERTY_READONLY(CCTexture2DPixelFormat, m_ePixelFormat, PixelFormat)
		/** width in pixels */
		CC_PROPERTY_READONLY(unsigned int, m_uPixelsWide, PixelsWide)
		/** height in pixels */
		CC_PROPERTY_READONLY(unsigned int, m_uPixelsHigh, PixelsHigh)

		/** texture name */
		CC_PROPERTY_READONLY(GLuint, m_uName, Name)

		/** texture max S */
		CC_PROPERTY(GLfloat, m_fMaxS, MaxS)
		/** texture max T */
		CC_PROPERTY(GLfloat, m_fMaxT, MaxT)
		/** content size */
		CC_PROPERTY_READONLY(CCSize, m_tContentSize, ContentSize)

		/** whether or not the texture has their Alpha premultiplied */
		bool m_bHasPremultipliedAlpha;

		bool m_bHasMipmaps;

		/** shader program used by drawAtPoint and drawInRect */
		CC_PROPERTY(CCGLProgram*, m_pShaderProgram, ShaderProgram);
	};

	class CCGrabber : public CCObject
	{
	protected:
		GLuint m_FBO;
		GLint m_oldFBO;
		GLfloat m_oldClearColor[4];
	};

	class CCGridBase : public CCObject
	{
	public:
		virtual ~CCGridBase(void);

		inline bool isActive(void) { return m_bActive; }
		void setActive(bool bActive);

		inline int getReuseGrid(void) { return m_nReuseGrid; }
		inline void setReuseGrid(int nReuseGrid) { m_nReuseGrid = nReuseGrid; }

		inline const CCSize& getGridSize(void) { return m_sGridSize; }
		inline void setGridSize(const CCSize& gridSize) { m_sGridSize = gridSize; }

		inline const CCPoint& getStep(void) { return m_obStep; }
		inline void setStep(const CCPoint& step) { m_obStep = step; }

		inline bool isTextureFlipped(void) { return m_bIsTextureFlipped; }
		void setTextureFlipped(bool bFlipped);
		bool initWithSize(const CCSize& gridSize, CCTexture2D *pTexture, bool bFlipped);
		bool initWithSize(const CCSize& gridSize);
		void beforeDraw(void);
		void afterDraw(CCNode *pTarget);
		virtual void blit(void);
		virtual void reuse(void);
		virtual void calculateVertexPoints(void);

	public:
		static CCGridBase* create(const CCSize& gridSize, CCTexture2D *texture, bool flipped);
		static CCGridBase* create(const CCSize& gridSize);
		void set2DProjection(void);

	protected:
		bool m_bActive;
		int  m_nReuseGrid;
		CCSize m_sGridSize;
		CCTexture2D *m_pTexture;
		CCPoint m_obStep;
		CCGrabber *m_pGrabber;
		bool m_bIsTextureFlipped;
		CCGLProgram* m_pShaderProgram;
		ccDirectorProjection m_directorProjection;
	};

	class CCScheduler : public CCObject
	{
	protected:
		float m_fTimeScale;

		//
		// "updates with priority" stuff
		//
		struct _listEntry *m_pUpdatesNegList;        // list of priority < 0
		struct _listEntry *m_pUpdates0List;            // list priority == 0
		struct _listEntry *m_pUpdatesPosList;        // list priority > 0
		struct _hashUpdateEntry *m_pHashForUpdates; // hash used to fetch quickly the list entries for pause,delete,etc

		// Used for "selectors with interval"
		struct _hashSelectorEntry *m_pHashForTimers;
		struct _hashSelectorEntry *m_pCurrentTarget;
		bool m_bCurrentTargetSalvaged;
		// If true unschedule will not remove anything from a hash. Elements will only be marked for deletion.
		bool m_bUpdateHashLocked;
		CCArray* m_pScriptHandlerEntries;
	};

	class CCActionManager : public CCObject
	{
	protected:
		struct _hashElement    *m_pTargets;
		struct _hashElement    *m_pCurrentTarget;
		bool            m_bCurrentTargetSalvaged;
	};

	class CCComponent : public CCObject
	{
	protected:
		CCComponent(void);

	public:
		virtual ~CCComponent(void);
		virtual bool init();
		virtual void onEnter();
		virtual void onExit();
		virtual void update(float delta);
		virtual bool serialize(void* r);
		virtual bool isEnabled() const;
		virtual void setEnabled(bool b);
		static CCComponent* create(void);
		
		const char* getName() const;
		void setName(const char *pName);
		
		void setOwner(CCNode *pOwner);
		CCNode* getOwner() const;
		
	protected:
		CCNode *m_pOwner;
		std::string m_strName;
		bool m_bEnabled;
	};

	class CCComponentContainer
	{
	protected:
		CCComponentContainer(CCNode *pNode);
		
	public:
		virtual ~CCComponentContainer(void);
		virtual CCComponent* get(const char *pName) const;
		virtual bool add(CCComponent *pCom);
		virtual bool remove(const char *pName);
		virtual bool remove(CCComponent *pCom);
		virtual void removeAll();
		virtual void visit(float fDelta);
	public:
		bool isEmpty() const;
		
	private:
		void alloc(void);

	private:
		CCDictionary *m_pComponents;        ///< Dictionary of components
		CCNode *m_pOwner;
		
		friend class CCNode;
	};

	class CCAction : public CCObject 
	{
	public:
		CCAction(void);
		virtual ~CCAction(void);
		const char* description();
		virtual CCObject* copyWithZone(CCZone *pZone);

		virtual bool isDone(void);

		virtual void startWithTarget(CCNode *pTarget);

		virtual void stop(void);

		virtual void step(float dt);

		virtual void update(float time);
		
		inline CCNode* getTarget(void) { return m_pTarget; }
		inline void setTarget(CCNode *pTarget) { m_pTarget = pTarget; }
		
		inline CCNode* getOriginalTarget(void) { return m_pOriginalTarget; } 
		inline void setOriginalTarget(CCNode *pOriginalTarget) { m_pOriginalTarget = pOriginalTarget; }

		inline int getTag(void) { return m_nTag; }
		inline void setTag(int nTag) { m_nTag = nTag; }
		
		RT_ADD(void setSpeedMod(float mod);)

		// 2.2 addition
		float getSpeedMod();

	public:
		static CCAction* create();
	protected:
		CCNode    *m_pOriginalTarget;
		CCNode    *m_pTarget;
		int     m_nTag;
		RT_ADD(float m_fSpeedMod;)
	};

	class CCTouch : public CCObject
	{
	public:
		int m_nId;
		bool m_startPointCaptured;
		CCPoint m_startPoint;
		CCPoint m_point;
		CCPoint m_prevPoint;
	};

	class CCEvent : public CCObject
	{};

	class CCNode : public CCObject
	{
	public:
		CCNode(void);
		virtual ~CCNode(void);

		virtual bool init();
		static CCNode * create(void);
		const char* description(void);

		virtual void setZOrder(int zOrder);
		virtual void _setZOrder(int z);
		virtual int getZOrder();

		virtual void setVertexZ(float vertexZ);
		virtual float getVertexZ();

		virtual void setScaleX(float fScaleX);
		virtual float getScaleX();

		virtual void setScaleY(float fScaleY);
		virtual float getScaleY();

		virtual void setScale(float scale);
		virtual float getScale();
		virtual void setScale(float fScaleX, float fScaleY);

		virtual void setPosition(const CCPoint &position);
		virtual const CCPoint& getPosition();
		virtual void setPosition(float x, float y);
		virtual void getPosition(float* x, float* y);
		virtual void  setPositionX(float x);
		virtual float getPositionX(void);
		virtual void  setPositionY(float y);
		virtual float getPositionY(void);

		virtual void setSkewX(float fSkewX);
		virtual float getSkewX();

		virtual void setSkewY(float fSkewY);
		virtual float getSkewY();

		virtual void setAnchorPoint(const CCPoint& anchorPoint);
		virtual const CCPoint& getAnchorPoint();
		virtual const CCPoint& getAnchorPointInPoints();

		virtual void setContentSize(const CCSize& contentSize);
		virtual const CCSize& getContentSize() const;

		// @note RobTop Addition
		virtual CCSize getScaledContentSize(void);

		virtual void setVisible(bool visible);
		virtual bool isVisible();

		virtual void setRotation(float fRotation);
		virtual float getRotation();

		virtual void setRotationX(float fRotaionX);
		virtual float getRotationX();

		virtual void setRotationY(float fRotationY);
		virtual float getRotationY();

		virtual void setOrderOfArrival(unsigned int uOrderOfArrival);
		virtual unsigned int getOrderOfArrival();

		virtual void setGLServerState(ccGLServerState glServerState);
		virtual ccGLServerState getGLServerState();

		virtual void ignoreAnchorPointForPosition(bool ignore);
		virtual bool isIgnoreAnchorPointForPosition();

		virtual void addChild(CCNode* child);
		virtual void addChild(CCNode* child, int zOrder);
		virtual void addChild(CCNode* child, int zOrder, int tag);

		virtual CCNode* getChildByTag(int tag);
		virtual CCArray* getChildren();
		virtual unsigned int getChildrenCount(void) const;

		virtual void setParent(CCNode* parent);
		virtual CCNode* getParent();

		virtual void removeFromParent();
		virtual void removeFromParentAndCleanup(bool cleanup);

		// @note RobTop Addition
		virtual void removeMeAndCleanup(void);

		virtual void removeChild(CCNode* child);
		virtual void removeChild(CCNode* child, bool cleanup);
		virtual void removeChildByTag(int tag);
		virtual void removeChildByTag(int tag, bool cleanup);
		virtual void removeAllChildren();
		virtual void removeAllChildrenWithCleanup(bool cleanup);

		virtual void reorderChild(CCNode* child, int zOrder);

		virtual void sortAllChildren();

		virtual CCGridBase* getGrid();
		virtual void setGrid(CCGridBase *pGrid);

		// Robtop Removal
		// virtual int getTag() const;

		// Robtop Removal
		// virtual void setTag(int nTag);

		virtual void* getUserData();
		virtual void setUserData(void* pUserData);

		virtual CCObject* getUserObject();
		virtual void setUserObject(CCObject *pUserObject);
		
	private:
		virtual CCGLProgram* getShaderProgram();
		virtual void setShaderProgram(CCGLProgram *pShaderProgram);

		virtual CCCamera* getCamera();

		virtual bool isRunning();

		virtual void registerScriptHandler(int handler);
		virtual void unregisterScriptHandler(void);
		inline int getScriptHandler() { return m_nScriptHandler; };

		void scheduleUpdateWithPriorityLua(int nHandler, int priority);

		virtual void onEnter();
		virtual void onEnterTransitionDidFinish();
		virtual void onExit();
		virtual void onExitTransitionDidStart();

		virtual void cleanup(void);

		virtual void draw(void);
		virtual void visit(void);

		CCRect boundingBox(void);

		virtual void setActionManager(CCActionManager* actionManager);
		virtual CCActionManager* getActionManager();

		CCAction* runAction(CCAction* action);
		void stopAllActions(void);
		void stopAction(CCAction* action);
		void stopActionByTag(int tag);

		CCAction* getActionByTag(int tag);
		unsigned int numberOfRunningActions(void);

		virtual void setScheduler(CCScheduler* scheduler);
		virtual CCScheduler* getScheduler();

		bool isScheduled(SEL_SCHEDULE selector);

		void scheduleUpdate(void);
		void scheduleUpdateWithPriority(int priority);
		void unscheduleUpdate(void);

		void schedule(SEL_SCHEDULE selector, float interval, unsigned int repeat, float delay);
		void schedule(SEL_SCHEDULE selector, float interval);
		void scheduleOnce(SEL_SCHEDULE selector, float delay);
		void schedule(SEL_SCHEDULE selector);

		void unschedule(SEL_SCHEDULE selector);
		void unscheduleAllSelectors(void);

		void resumeSchedulerAndActions(void);
		void pauseSchedulerAndActions(void);

		virtual void update(float delta);

		void transform(void);
		void transformAncestors(void);
		virtual void updateTransform(void);
		
		// RobTop addition
		virtual const CCAffineTransform nodeToParentTransform(void);
		// 2.2 additions
		virtual const CCAffineTransform nodeToParentTransformFast();
		// Robtop Addition
		virtual const CCAffineTransform parentToNodeTransform(void);
		virtual CCAffineTransform nodeToWorldTransform(void);
		// 2.2 additions
		virtual CCAffineTransform nodeToWorldTransformFast();
		virtual CCAffineTransform worldToNodeTransform(void);

		CCPoint convertToNodeSpace(const CCPoint& worldPoint);
		CCPoint convertToWorldSpace(const CCPoint& nodePoint);
		CCPoint convertToNodeSpaceAR(const CCPoint& worldPoint);
		CCPoint convertToWorldSpaceAR(const CCPoint& nodePoint);
		CCPoint convertTouchToNodeSpace(CCTouch* touch);
		CCPoint convertTouchToNodeSpaceAR(CCTouch* touch);
		
		void setAdditionalTransform(const CCAffineTransform& additionalTransform);

		CCComponent* getComponent(const char *pName) const;

		virtual bool addComponent(CCComponent* pComponent);
		virtual bool removeComponent(const char* pName);
		virtual bool removeComponent(CCComponent* pComponent);

		virtual void removeAllComponents();
		
		// @note RobTop Addition
		virtual void updateTweenAction(float, const char*);
		// @note RobTop Addition
		CCNode& operator=(const CCNode&);
		// 2.2 additions
		virtual void updateTweenActionInt(float, int);

		cocos2d::CCAffineTransform getTransformTemp();

		bool getUseChildIndex();
		void setUseChildIndex(bool);
		void qsortAllChildrenWithIndex();

	protected:
		static void resetGlobalOrderOfArrival();
		
	public:
		void sortAllChildrenNoIndex();
		void sortAllChildrenWithIndex();
		void updateChildIndexes();

	private:
		void childrenAlloc(void);
		void insertChild(CCNode* child, int z);
		void detachChild(CCNode* child, bool doCleanup);

		CCPoint convertToWindowSpace(const CCPoint& nodePoint);

	protected:
		float m_fRotationX;                 ///< rotation angle on x-axis
		float m_fRotationY;                 ///< rotation angle on y-axis
		
		float m_fScaleX;                    ///< scaling factor on x-axis
		float m_fScaleY;                    ///< scaling factor on y-axis
		
		float m_fVertexZ;                   ///< OpenGL real Z vertex
		
		CCPoint m_obPosition;               ///< position of the node
		
		float m_fSkewX;                     ///< skew angle on x-axis
		float m_fSkewY;                     ///< skew angle on y-axis
		
		CCPoint m_obAnchorPointInPoints;    ///< anchor point in points
		CCPoint m_obAnchorPoint;            ///< anchor point normalized (NOT in points)
		
		CCSize m_obContentSize;             ///< untransformed size of the node
		
		
		CCAffineTransform m_sAdditionalTransform; ///< transform
		CCAffineTransform m_sTransform;     ///< transform
		CCAffineTransform m_sInverse;       ///< transform
		
		CCCamera* m_pCamera;                ///< a camera
		
		CCGridBase* m_pGrid;                ///< a grid
		
		// 2.2 additions
		// Robtop Removal
		// int m_nZOrder;                     ///< z-order value that affects the draw order
		
		CCArray* m_pChildren;               ///< array of children nodes
		CCNode* m_pParent;                  ///< weak reference to parent node
		
		// Robtop Removal
		// int m_nTag;                         ///< a tag. Can be any number you assigned just to identify this node
		
		void* m_pUserData;                  ///< A user assingned void pointer, Can be point to any cpp object
		CCObject* m_pUserObject;            ///< A user assigned CCObject
		
		CCGLProgram* m_pShaderProgram;      ///< OpenGL shader
		
		ccGLServerState m_eGLServerState;   ///< OpenGL servier side state
		
		// 2.2 additions
		// Robtop Removal
		// unsigned int m_uOrderOfArrival;     ///< used to preserve sequence while sorting children with the same zOrder
		
		CCScheduler* m_pScheduler;          ///< scheduler used to schedule timers and updates
		
		CCActionManager* m_pActionManager;  ///< a pointer to ActionManager singleton, which is used to handle all the actions
		
		bool m_bRunning;                    ///< is running
		
		bool m_bTransformDirty;             ///< transform dirty flag
		bool m_bInverseDirty;               ///< transform dirty flag
		bool m_bAdditionalTransformDirty;   ///< The flag to check whether the additional transform is dirty

		// 2.2 additions
		PAD(10); // i dont know if this is related to transform at all, but its here
		
		bool m_bVisible;                    ///< is this node visible
		
		bool m_bIgnoreAnchorPointForPosition; ///< true if the Anchor Point will be (0,0) when you position the CCNode, false otherwise.
											///< Used by CCLayer and CCScene.
		
		bool m_bReorderChildDirty;          ///< children order dirty flag
		
		int m_nScriptHandler;               ///< script handler for onEnter() & onExit(), used in Javascript binding and Lua binding.
		int m_nUpdateScriptHandler;         ///< script handler for update() callback per frame, which is invoked from lua & javascript.
		ccScriptType m_eScriptType;         ///< type of script binding, lua or javascript
		
		CCComponentContainer* m_pComponentContainer;        ///< Dictionary of components

		// 2.2 additions
		bool m_bUnkBool1;
		bool m_bUnkBool2;
	};


	// CCLayer
	class CCTouchDelegate
	{
	public:

		CCTouchDelegate() {}

		virtual ~CCTouchDelegate() {}

		virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent) {CC_UNUSED_PARAM(pTouch); CC_UNUSED_PARAM(pEvent); return false;};

		virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent) {CC_UNUSED_PARAM(pTouch); CC_UNUSED_PARAM(pEvent);}
		virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent) {CC_UNUSED_PARAM(pTouch); CC_UNUSED_PARAM(pEvent);}
		virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent) {CC_UNUSED_PARAM(pTouch); CC_UNUSED_PARAM(pEvent);}

		virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent) {CC_UNUSED_PARAM(pTouches); CC_UNUSED_PARAM(pEvent);}
		virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent) {CC_UNUSED_PARAM(pTouches); CC_UNUSED_PARAM(pEvent);}
		virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent) {CC_UNUSED_PARAM(pTouches); CC_UNUSED_PARAM(pEvent);}
		virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent) {CC_UNUSED_PARAM(pTouches); CC_UNUSED_PARAM(pEvent);}

		virtual void setPreviousPriority(int);
		virtual int getPreviousPriority();
	};

	class CCAcceleration
	{
	public:
		double x;
		double y;
		double z;

		double timestamp;
	};

	class CCAccelerometerDelegate
	{
	public:
		CCAccelerometerDelegate() {}

		virtual void didAccelerate(CCAcceleration* pAccelerationValue) {CC_UNUSED_PARAM(pAccelerationValue);}
	};

	class CCKeypadDelegate
	{
	public:
		virtual void keyBackClicked() {}

		virtual void keyMenuClicked() {};
	};

	class CCKeyboardDelegate
	{
	public:
		virtual void keyDown(enumKeyCodes key) {}

		virtual void keyUp(enumKeyCodes key) {}
	};

	RT_ADD(
		class CCMouseDelegate 
		{
		public:
			virtual void rightKeyDown() {}

			virtual void rightKeyUp() {}

			virtual void scrollWheel(float x, float y) {}
		};
	)

	class CCScriptHandlerEntry : public CCObject
	{
	protected:
		
		int m_nHandler;
		int m_nEntryId;
	};

	class CCTouchScriptHandlerEntry : public CCScriptHandlerEntry
	{
	private:
		bool m_bIsMultiTouches;
		int m_nPriority;
		bool m_bSwallowsTouches;
	};

	class CCLayer : public CCNode, public CCTouchDelegate, public CCAccelerometerDelegate, public CCKeypadDelegate
		RT_ADD(, public CCKeyboardDelegate, public CCMouseDelegate)
	{
	public:
		CCLayer();
		virtual ~CCLayer();
		virtual bool init();

		static CCLayer *create(void);
		virtual void onEnter();
		virtual void onExit();
		virtual void onEnterTransitionDidFinish();
		
		virtual bool ccTouchBegan(CCTouch *pTouch, CCEvent *pEvent);
		virtual void ccTouchMoved(CCTouch *pTouch, CCEvent *pEvent);
		virtual void ccTouchEnded(CCTouch *pTouch, CCEvent *pEvent);
		virtual void ccTouchCancelled(CCTouch *pTouch, CCEvent *pEvent);

		virtual void ccTouchesBegan(CCSet *pTouches, CCEvent *pEvent);
		virtual void ccTouchesMoved(CCSet *pTouches, CCEvent *pEvent);
		virtual void ccTouchesEnded(CCSet *pTouches, CCEvent *pEvent);
		virtual void ccTouchesCancelled(CCSet *pTouches, CCEvent *pEvent);
		virtual void didAccelerate(CCAcceleration* pAccelerationValue);
		void registerScriptAccelerateHandler(int nHandler);
		void unregisterScriptAccelerateHandler(void);

		virtual void registerWithTouchDispatcher(void);
		
		virtual void registerScriptTouchHandler(int nHandler, bool bIsMultiTouches = false, int nPriority = INT_MIN, bool bSwallowsTouches = false);
		virtual void unregisterScriptTouchHandler(void);

		virtual bool isTouchEnabled();
		virtual void setTouchEnabled(bool value);
		
		virtual void setTouchMode(ccTouchesMode mode);
		virtual int getTouchMode();
		
		virtual void setTouchPriority(int priority);
		virtual int getTouchPriority();

		virtual bool isAccelerometerEnabled();
		virtual void setAccelerometerEnabled(bool value);
		virtual void setAccelerometerInterval(double interval);

		virtual bool isKeypadEnabled();
		virtual void setKeypadEnabled(bool value);

		RT_ADD(
			virtual bool isKeyboardEnabled();
			virtual void setKeyboardEnabled(bool value);

			virtual bool isMouseEnabled();
			virtual void setMouseEnabled(bool value);
		)

		void registerScriptKeypadHandler(int nHandler);
		void unregisterScriptKeypadHandler(void);

		virtual void keyBackClicked(void);
		virtual void keyMenuClicked(void);
		
		RT_ADD(
			void keyDown(enumKeyCodes);
		)

		// 2.2 additions
		virtual void setPreviousPriority(int);
		virtual int getPreviousPriority();

		inline CCTouchScriptHandlerEntry* getScriptTouchHandlerEntry() { return m_pScriptTouchHandlerEntry; };
		inline CCScriptHandlerEntry* getScriptKeypadHandlerEntry() { return m_pScriptKeypadHandlerEntry; };
		inline CCScriptHandlerEntry* getScriptAccelerateHandlerEntry() { return m_pScriptAccelerateHandlerEntry; };
	protected:   
		bool m_bTouchEnabled;
		bool m_bAccelerometerEnabled;
		bool m_bKeypadEnabled;
		RT_ADD(
			bool m_bKeyboardEnabled;
			bool m_bMouseEnabled;
		)

	private:
		// Script touch events handler
		CCTouchScriptHandlerEntry* m_pScriptTouchHandlerEntry;
		CCScriptHandlerEntry* m_pScriptKeypadHandlerEntry;
		CCScriptHandlerEntry* m_pScriptAccelerateHandlerEntry;

		int m_nTouchPriority;
		ccTouchesMode m_eTouchMode;

		// 2.2 additions
		int m_uPreviousPriority; // no idea

		int excuteScriptTouchHandler(int nEventType, CCTouch *pTouch);
		int excuteScriptTouchHandler(int nEventType, CCSet *pTouches);
	};


	// CCNodeRGBA
	class CCRGBAProtocol
	{
	public:
		virtual void setColor(const ccColor3B& color) = 0;
		virtual const ccColor3B& getColor(void) = 0;
		virtual const ccColor3B& getDisplayedColor(void) = 0;

		virtual GLubyte getDisplayedOpacity(void) = 0;
		virtual GLubyte getOpacity(void) = 0;
		virtual void setOpacity(GLubyte opacity) = 0;

		virtual void setOpacityModifyRGB(bool bValue) = 0;

		virtual bool isOpacityModifyRGB(void) = 0;

		virtual bool isCascadeColorEnabled(void) = 0;
		virtual void setCascadeColorEnabled(bool cascadeColorEnabled) = 0;

		virtual void updateDisplayedColor(const ccColor3B& color) = 0;

		virtual bool isCascadeOpacityEnabled(void) = 0;
		virtual void setCascadeOpacityEnabled(bool cascadeOpacityEnabled) = 0;

		virtual void updateDisplayedOpacity(GLubyte opacity) = 0;
	};

	class CCNodeRGBA : public CCNode, public CCRGBAProtocol
	{
	public:
		CCNodeRGBA();
		virtual ~CCNodeRGBA();
		
		virtual bool init();

		static CCNodeRGBA * create(void);

		virtual GLubyte getOpacity();
		virtual GLubyte getDisplayedOpacity();
		virtual void setOpacity(GLubyte opacity);
		virtual void updateDisplayedOpacity(GLubyte parentOpacity);
		virtual bool isCascadeOpacityEnabled();
		virtual void setCascadeOpacityEnabled(bool cascadeOpacityEnabled);

		virtual const ccColor3B& getColor(void);
		virtual const ccColor3B& getDisplayedColor();
		virtual void setColor(const ccColor3B& color);
		virtual void updateDisplayedColor(const ccColor3B& parentColor);
		virtual bool isCascadeColorEnabled();
		virtual void setCascadeColorEnabled(bool cascadeColorEnabled);

		virtual void setOpacityModifyRGB(bool bValue) {CC_UNUSED_PARAM(bValue);};
		virtual bool isOpacityModifyRGB() { return false; };

	protected:
		GLubyte		_displayedOpacity;
		GLubyte     _realOpacity;
		ccColor3B	_displayedColor;
		ccColor3B   _realColor;
		bool		_cascadeColorEnabled;
		bool        _cascadeOpacityEnabled;
	};

	// CCLayerColor
	class CCBlendProtocol
	{
	public:
		virtual void setBlendFunc(ccBlendFunc blendFunc) = 0;

		virtual ccBlendFunc getBlendFunc(void) = 0;
	};

	class CCLayerRGBA : public CCLayer, public CCRGBAProtocol
	{
	public:
		CREATE_FUNC(CCLayerRGBA);
		CCLayerRGBA();
		virtual ~CCLayerRGBA();

		virtual bool init();

		virtual GLubyte getOpacity();
		virtual GLubyte getDisplayedOpacity();
		virtual void setOpacity(GLubyte opacity);
		virtual void updateDisplayedOpacity(GLubyte parentOpacity);
		virtual bool isCascadeOpacityEnabled();
		virtual void setCascadeOpacityEnabled(bool cascadeOpacityEnabled);
		
		virtual const ccColor3B& getColor();
		virtual const ccColor3B& getDisplayedColor();
		virtual void setColor(const ccColor3B& color);
		virtual void updateDisplayedColor(const ccColor3B& parentColor);
		virtual bool isCascadeColorEnabled();
		virtual void setCascadeColorEnabled(bool cascadeColorEnabled);
		
		virtual void setOpacityModifyRGB(bool bValue) {CC_UNUSED_PARAM(bValue);}
		virtual bool isOpacityModifyRGB() { return false; }
	protected:
		GLubyte		_displayedOpacity, _realOpacity;
		ccColor3B	_displayedColor, _realColor;
		bool		_cascadeOpacityEnabled, _cascadeColorEnabled;
	};

	class CCLayerColor : public CCLayerRGBA, public CCBlendProtocol
	{
	protected:
		ccVertex2F m_pSquareVertices[4];
		ccColor4F  m_pSquareColors[4];

	public:
		CCLayerColor();
		virtual ~CCLayerColor();

		virtual void draw();
		virtual void setContentSize(const CCSize & var);

		static CCLayerColor* create();

		static CCLayerColor * create(const ccColor4B& color, GLfloat width, GLfloat height);
		static CCLayerColor * create(const ccColor4B& color);

		virtual bool init();
		virtual bool initWithColor(const ccColor4B& color, GLfloat width, GLfloat height);
		virtual bool initWithColor(const ccColor4B& color);

		void changeWidth(GLfloat w);
		void changeHeight(GLfloat h);
		void changeWidthAndHeight(GLfloat w ,GLfloat h);

		CC_PROPERTY(ccBlendFunc, m_tBlendFunc, BlendFunc)

		virtual void setColor(const ccColor3B &color);
		virtual void setOpacity(GLubyte opacity);

		void addToVertices(cocos2d::CCPoint, cocos2d::CCPoint, cocos2d::CCPoint);
		void setVertices(cocos2d::CCPoint, cocos2d::CCPoint, cocos2d::CCPoint);

	protected:
		virtual void updateColor();
	};


	// CCLayerGradient
	class CCLayerGradient : public CCLayerColor
	{
	public:
		CCLayerGradient() {}

		static CCLayerGradient* create(const ccColor4B& start, const ccColor4B& end);

		static CCLayerGradient* create(const ccColor4B& start, const ccColor4B& end, const CCPoint& v);

		virtual bool init();
		virtual bool initWithColor(const ccColor4B& start, const ccColor4B& end);

		virtual bool initWithColor(const ccColor4B& start, const ccColor4B& end, const CCPoint& v);

		CC_PROPERTY_PASS_BY_REF(ccColor3B, m_startColor, StartColor)
		CC_PROPERTY_PASS_BY_REF(ccColor3B, m_endColor, EndColor)
		CC_PROPERTY(GLubyte, m_cStartOpacity, StartOpacity)
		CC_PROPERTY(GLubyte, m_cEndOpacity, EndOpacity)
		CC_PROPERTY_PASS_BY_REF(CCPoint, m_AlongVector, Vector)

		bool getShouldPremultiply() const;
		void setShouldPremultiply(bool);
		void setValues(cocos2d::_ccColor3B const&, unsigned char, cocos2d::_ccColor3B const&, unsigned char, cocos2d::CCPoint const&);

	protected:
		bool m_bCompressedInterpolation;

	public:
		virtual void setCompressedInterpolation(bool bCompressedInterpolation);
		virtual bool isCompressedInterpolation();

		static CCLayerGradient* create();

	protected:
		virtual void updateColor();
	};


	// CCMenu
	enum {
		//* priority used by the menu for the event handler
		kCCMenuHandlerPriority = -128,
	};

	class CCMenuItem : public CCNodeRGBA
	{
	public:
		bool m_bSelected;
		bool m_bEnabled;

	public:
		CCMenuItem()
			: m_bSelected(false)
			, m_bEnabled(false)            
			, m_pListener(NULL)            
			, m_pfnSelector(NULL)
			, m_nScriptTapHandler(0)
			, m_fSizeMult(0.f)
			{}
		virtual ~CCMenuItem();

		static CCMenuItem* create();
		static CCMenuItem* create(CCObject *rec, SEL_MenuHandler selector);
		bool initWithTarget(CCObject *rec, SEL_MenuHandler selector);
		CCRect rect();
		virtual void activate();
		virtual void selected();
		virtual void unselected();

		virtual void registerScriptTapHandler(int nHandler);
		virtual void unregisterScriptTapHandler(void);
		int getScriptTapHandler() { return m_nScriptTapHandler; };

		virtual bool isEnabled();
		virtual void setEnabled(bool value);
		virtual bool isSelected();      

		void setTarget(CCObject *rec, SEL_MenuHandler selector);

	public:
		CCObject*       m_pListener;
		SEL_MenuHandler m_pfnSelector;
		int             m_nScriptTapHandler;

		// 2.2 additions
		RT_ADD(float m_fSizeMult = 0.f;)
	};

	class CCMenu : public CCLayerRGBA
	{
	private:
		bool m_bEnabled;

	public:
		CCMenu() : m_pSelectedItem(NULL) {}
		virtual ~CCMenu(){}

		static CCMenu* create();

		static CCMenu* create(CCMenuItem* item, ...);

		static CCMenu* createWithArray(CCArray* pArrayOfItems);

		static CCMenu* createWithItem(CCMenuItem* item);

		static CCMenu* createWithItems(CCMenuItem *firstItem, va_list args);

		bool init();

		bool initWithArray(CCArray* pArrayOfItems);

		void alignItemsVertically();
		void alignItemsVerticallyWithPadding(float padding);

		void alignItemsHorizontally();
		void alignItemsHorizontallyWithPadding(float padding);

		void alignItemsInColumns(unsigned int columns, ...);
		void alignItemsInColumns(unsigned int columns, va_list args);
		void alignItemsInColumnsWithArray(CCArray* rows);
		void alignItemsInRows(unsigned int rows, ...);
		void alignItemsInRows(unsigned int rows, va_list args);
		void alignItemsInRowsWithArray(CCArray* columns);

		void setHandlerPriority(int newPriority);

		virtual void addChild(CCNode * child);
		virtual void addChild(CCNode * child, int zOrder);
		virtual void addChild(CCNode * child, int zOrder, int tag);
		virtual void registerWithTouchDispatcher();
		virtual void removeChild(CCNode* child, bool cleanup);

		virtual bool ccTouchBegan(CCTouch* touch, CCEvent* event);
		virtual void ccTouchEnded(CCTouch* touch, CCEvent* event);
		virtual void ccTouchCancelled(CCTouch *touch, CCEvent* event);
		virtual void ccTouchMoved(CCTouch* touch, CCEvent* event);

		virtual void onExit();

		virtual void setOpacityModifyRGB(bool bValue) {CC_UNUSED_PARAM(bValue);}
		virtual bool isOpacityModifyRGB(void) { return false;}
		
		virtual bool isEnabled() { return m_bEnabled; }
		virtual void setEnabled(bool value) { m_bEnabled = value; };

	protected:
		CCMenuItem* itemForTouch(CCTouch * touch);
		CCMenuItem* itemForTouch(CCTouch * touch, bool);
		tCCMenuState m_eState;
		CCMenuItem *m_pSelectedItem;
	};


	// CCMenuItemSprite
	class CCMenuItemSprite : public CCMenuItem
	{
		CC_PROPERTY(CCNode*, m_pNormalImage, NormalImage);
		CC_PROPERTY(CCNode*, m_pSelectedImage, SelectedImage);
		CC_PROPERTY(CCNode*, m_pDisabledImage, DisabledImage);
	public:
		CCMenuItemSprite()
			:m_pNormalImage(NULL)
			,m_pSelectedImage(NULL)
			,m_pDisabledImage(NULL)
		{}

		static CCMenuItemSprite * create(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite = NULL);
		static CCMenuItemSprite * create(CCNode* normalSprite, CCNode* selectedSprite, CCObject* target, SEL_MenuHandler selector);
		static CCMenuItemSprite * create(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite, CCObject* target, SEL_MenuHandler selector);

		bool initWithNormalSprite(CCNode* normalSprite, CCNode* selectedSprite, CCNode* disabledSprite, CCObject* target, SEL_MenuHandler selector);

		virtual void selected();
		virtual void unselected();
		virtual void setEnabled(bool bEnabled);

	protected:
		virtual void updateImagesVisibility();
	};


	// CCMenuItemImage
	class CCSpriteFrame : public CCObject
	{
	public:
		// attributes

		inline const CCRect& getRectInPixels(void) { return m_obRectInPixels; }
		void setRectInPixels(const CCRect& rectInPixels);

		inline bool isRotated(void) { return m_bRotated; }
		inline void setRotated(bool bRotated) { m_bRotated = bRotated; }

		inline const CCRect& getRect(void) { return m_obRect; }
		void setRect(const CCRect& rect);

		const CCPoint& getOffsetInPixels(void);
		void setOffsetInPixels(const CCPoint& offsetInPixels);

		inline const CCSize& getOriginalSizeInPixels(void) { return m_obOriginalSizeInPixels; }
		inline void setOriginalSizeInPixels(const CCSize& sizeInPixels) { m_obOriginalSizeInPixels = sizeInPixels; }

		inline const CCSize& getOriginalSize(void) { return m_obOriginalSize; }
		inline void setOriginalSize(const CCSize& sizeInPixels) { m_obOriginalSize = sizeInPixels; }

		CCTexture2D* getTexture(void);
		void setTexture(CCTexture2D* pobTexture);

		const CCPoint& getOffset(void);
		void setOffset(const CCPoint& offsets);

	public:
		~CCSpriteFrame(void);
		virtual CCObject* copyWithZone(CCZone *pZone);

		static CCSpriteFrame* create(const char* filename, const CCRect& rect);
		
		static CCSpriteFrame* create(const char* filename, const CCRect& rect, bool rotated, const CCPoint& offset, const CCSize& originalSize);
		
		static CCSpriteFrame* createWithTexture(CCTexture2D* pobTexture, const CCRect& rect);

		static CCSpriteFrame* createWithTexture(CCTexture2D* pobTexture, const CCRect& rect, bool rotated, const CCPoint& offset, const CCSize& originalSize);

	public:
		bool initWithTexture(CCTexture2D* pobTexture, const CCRect& rect);

		bool initWithTextureFilename(const char* filename, const CCRect& rect);

		bool initWithTexture(CCTexture2D* pobTexture, const CCRect& rect, bool rotated, const CCPoint& offset, const CCSize& originalSize);

		bool initWithTextureFilename(const char* filename, const CCRect& rect, bool rotated, const CCPoint& offset, const CCSize& originalSize);

		std::string getFrameName() const;
		void setFrameName(std::string);

	protected:
		CCPoint m_obOffset;
		CCSize m_obOriginalSize;
		CCRect m_obRectInPixels;
		bool   m_bRotated;
		CCRect m_obRect;
		CCPoint m_obOffsetInPixels;
		CCSize m_obOriginalSizeInPixels;
		CCTexture2D *m_pobTexture;
		std::string  m_strTextureFilename;
		RT_ADD( std::string  m_strFrameName; )
	};

	class CCMenuItemImage : public CCMenuItemSprite
	{
	public:
		CCMenuItemImage(){}
		virtual ~CCMenuItemImage(){}
		
		static CCMenuItemImage* create(const char *normalImage, const char *selectedImage);
		static CCMenuItemImage* create(const char *normalImage, const char *selectedImage, const char *disabledImage);
		static CCMenuItemImage* create(const char *normalImage, const char *selectedImage, CCObject* target, SEL_MenuHandler selector);
		static CCMenuItemImage* create(const char *normalImage, const char *selectedImage, const char *disabledImage, CCObject* target, SEL_MenuHandler selector);
		
		bool init();
		bool initWithNormalImage(const char *normalImage, const char *selectedImage, const char *disabledImage, CCObject* target, SEL_MenuHandler selector);
		void setNormalSpriteFrame(CCSpriteFrame* frame);
		void setSelectedSpriteFrame(CCSpriteFrame* frame);
		void setDisabledSpriteFrame(CCSpriteFrame* frame);

		static CCMenuItemImage* create();
	};


	// CCScene
	class CCScene;
	class CCSceneDelegate {
	public: 
		virtual void willSwitchToScene(CCScene* scene) {}
	};

	class CCScene : public CCNode
	{
	public:
		CCScene();
		virtual ~CCScene();
		bool init();

		static CCScene* create(void);

		CCScene(const CCScene&);
		CCScene& operator=(const CCScene&);

		int getHighestChildZ(void);

		CCSceneDelegate* m_pDelegate;
	};


	// CCSceneTransitionDelegate
	RT_ADD(
		class CCSceneTransitionDelegate {
		public:
			virtual void sceneWillResume(void) {}
		};
	)

	// CCSprite
	class CCTextureProtocol : public CCBlendProtocol
	{
	public:
		virtual CCTexture2D* getTexture(void) = 0;
		virtual void setTexture(CCTexture2D *texture) = 0;
	};

	class CCTextureAtlas : public CCObject 
	{
	protected:
		GLushort*           m_pIndices;
	#if CC_TEXTURE_ATLAS_USE_VAO
		GLuint              m_uVAOname;
	#endif
		GLuint              m_pBuffersVBO[2]; //0: vertex  1: indices
		bool                m_bDirty; //indicates whether or not the array buffer of the VBO needs to be updated


		/** quantity of quads that are going to be drawn */
		CC_PROPERTY_READONLY(unsigned int, m_uTotalQuads, TotalQuads)
		/** quantity of quads that can be stored with the current texture atlas size */
		CC_PROPERTY_READONLY(unsigned int, m_uCapacity, Capacity)
		/** Texture of the texture atlas */
		CC_PROPERTY(CCTexture2D *, m_pTexture, Texture)
		/** Quads that are going to be rendered */
		CC_PROPERTY(ccV3F_C4B_T2F_Quad *, m_pQuads, Quads)

	public:
		CCTextureAtlas();
		virtual ~CCTextureAtlas();
		const char* description();

		static CCTextureAtlas* create(const char* file , unsigned int capacity);

		bool initWithFile(const char* file, unsigned int capacity);

		static CCTextureAtlas* createWithTexture(CCTexture2D *texture, unsigned int capacity);


		bool initWithTexture(CCTexture2D *texture, unsigned int capacity);

		void updateQuad(ccV3F_C4B_T2F_Quad* quad, unsigned int index);
		void insertQuad(ccV3F_C4B_T2F_Quad* quad, unsigned int index);
		void insertQuads(ccV3F_C4B_T2F_Quad* quads, unsigned int index, unsigned int amount);
		void insertQuadFromIndex(unsigned int fromIndex, unsigned int newIndex);
		void removeQuadAtIndex(unsigned int index);
		void removeQuadsAtIndex(unsigned int index, unsigned int amount);
		void removeAllQuads();


		bool resizeCapacity(unsigned int n);

		void increaseTotalQuadsWith(unsigned int amount);

		void moveQuadsFromIndex(unsigned int oldIndex, unsigned int amount, unsigned int newIndex);

		void moveQuadsFromIndex(unsigned int index, unsigned int newIndex);

		void fillWithEmptyQuadsFromIndex(unsigned int index, unsigned int amount);

		void drawNumberOfQuads(unsigned int n);

		void drawNumberOfQuads(unsigned int n, unsigned int start);

		void drawQuads();
		void listenBackToForeground(CCObject *obj);

		inline bool isDirty(void) { return m_bDirty; }
		inline void setDirty(bool bDirty) { m_bDirty = bDirty; }

	private:
		void setupIndices();
		void mapBuffers();
	#if CC_TEXTURE_ATLAS_USE_VAO
		void setupVBOandVAO();
	#else
		void setupVBO();
	#endif
	};

	#define kDefaultSpriteBatchCapacity   29
	class CCSprite;
	class CCSpriteBatchNode : public CCNode, public CCTextureProtocol
	{
	public:
		CCSpriteBatchNode();
		~CCSpriteBatchNode();

		inline CCTextureAtlas* getTextureAtlas(void) { return m_pobTextureAtlas; }
		inline void setTextureAtlas(CCTextureAtlas* textureAtlas) 
		{ 
			if (textureAtlas != m_pobTextureAtlas)
			{
				CC_SAFE_RETAIN(textureAtlas);
				CC_SAFE_RELEASE(m_pobTextureAtlas);
				m_pobTextureAtlas = textureAtlas;
			}
		}

		inline CCArray* getDescendants(void) { return m_pobDescendants; }

		static CCSpriteBatchNode* createWithTexture(CCTexture2D* tex, unsigned int capacity);
		static CCSpriteBatchNode* createWithTexture(CCTexture2D* tex) {
			return CCSpriteBatchNode::createWithTexture(tex, kDefaultSpriteBatchCapacity);
		}

		static CCSpriteBatchNode* create(const char* fileImage, unsigned int capacity);
		static CCSpriteBatchNode* create(const char* fileImage) {
			return CCSpriteBatchNode::create(fileImage, kDefaultSpriteBatchCapacity);
		}

		bool initWithTexture(CCTexture2D *tex, unsigned int capacity);
		bool initWithFile(const char* fileImage, unsigned int capacity);
		bool init();

		void removeChildAtIndex(unsigned int index, bool doCleanup);

		void insertChild(CCSprite *child, unsigned int index);
		void appendChild(CCSprite* sprite);
		void removeSpriteFromAtlas(CCSprite *sprite);

		unsigned int rebuildIndexInOrder(CCSprite *parent, unsigned int index);
		unsigned int highestAtlasIndexInChild(CCSprite *sprite);
		unsigned int lowestAtlasIndexInChild(CCSprite *sprite);
		unsigned int atlasIndexForChild(CCSprite *sprite, int z);
		void reorderBatch(bool reorder);
		// CCTextureProtocol
		virtual CCTexture2D* getTexture(void);
		virtual void setTexture(CCTexture2D *texture);
		virtual void setBlendFunc(ccBlendFunc blendFunc);
		virtual ccBlendFunc getBlendFunc(void);

		virtual void visit(void);
		virtual void addChild(CCNode * child);
		virtual void addChild(CCNode * child, int zOrder);
		virtual void addChild(CCNode * child, int zOrder, int tag);
		virtual void reorderChild(CCNode * child, int zOrder);
			
		virtual void removeChild(CCNode* child, bool cleanup);
		virtual void removeAllChildrenWithCleanup(bool cleanup);
		virtual void sortAllChildren();
		virtual void draw(void);

	protected:
		void insertQuadFromSprite(CCSprite *sprite, unsigned int index);
		void updateQuadFromSprite(CCSprite *sprite, unsigned int index);
		CCSpriteBatchNode * addSpriteWithoutQuad(CCSprite*child, unsigned int z, int aTag);

		RT_ADD(
			bool getManualSortChildren(void)const;
			int getAtlasCapacity(void);
		public:
			int getUsedAtlasCapacity(void);
			void increaseAtlasCapacity(unsigned int);
			void increaseAtlasCapacity();
		protected:
			void manualSortAllChildren(void);
			void setManualSortChildren(bool);
		);

	private:
		void updateAtlasIndex(CCSprite* sprite, int* curIndex);
		void swap(int oldIndex, int newIndex);
		void updateBlendFunc();

	protected:
		CCTextureAtlas *m_pobTextureAtlas;
		ccBlendFunc m_blendFunc;

		// all descendants: children, gran children, etc...
		CCArray* m_pobDescendants;

		RT_ADD(
			bool m_bManualSortChildren;
			bool m_bManualSortAllChildrenDirty;
		)
	};

	class CCSprite : public CCNodeRGBA, public CCTextureProtocol
	{
	public:
		static CCSprite* create();
		static CCSprite* create(const char *pszFileName);
		static CCSprite* create(const char *pszFileName, const CCRect& rect);
		static CCSprite* createWithTexture(CCTexture2D *pTexture);
		static CCSprite* createWithTexture(CCTexture2D *pTexture, const CCRect& rect);
		static CCSprite* createWithSpriteFrame(CCSpriteFrame *pSpriteFrame);
		static CCSprite* createWithSpriteFrameName(const char *pszSpriteFrameName);

		CCSprite(void);

		virtual ~CCSprite(void);
		virtual bool init(void);
		virtual bool initWithTexture(CCTexture2D *pTexture);
		virtual bool initWithTexture(CCTexture2D *pTexture, const CCRect& rect);
		virtual bool initWithTexture(CCTexture2D *pTexture, const CCRect& rect, bool rotated);
		virtual bool initWithSpriteFrame(CCSpriteFrame *pSpriteFrame);
		virtual bool initWithSpriteFrameName(const char *pszSpriteFrameName);
		virtual bool initWithFile(const char *pszFilename);
		virtual bool initWithFile(const char *pszFilename, const CCRect& rect);

		virtual void setTexture(CCTexture2D *texture);
		virtual CCTexture2D* getTexture(void);
		inline void setBlendFunc(ccBlendFunc blendFunc) { m_sBlendFunc = blendFunc; }
		inline ccBlendFunc getBlendFunc(void) { return m_sBlendFunc; }

		RT_ADD(
			virtual void setChildColor(const ccColor3B&);
			virtual void setChildOpacity(GLubyte);
		)

		virtual void setScaleX(float fScaleX);
		virtual void setScaleY(float fScaleY);
		virtual void setPosition(const CCPoint& pos);
		virtual void setRotation(float fRotation);
		virtual void setRotationX(float fRotationX);
		virtual void setRotationY(float fRotationY);
		virtual void setSkewX(float sx);
		virtual void setSkewY(float sy);
		virtual void removeChild(CCNode* pChild, bool bCleanup);
		virtual void removeAllChildrenWithCleanup(bool bCleanup);
		virtual void reorderChild(CCNode *pChild, int zOrder);
		virtual void addChild(CCNode *pChild);
		virtual void addChild(CCNode *pChild, int zOrder);
		virtual void addChild(CCNode *pChild, int zOrder, int tag);
		virtual void sortAllChildren();
		virtual void setScale(float fScale);
		virtual void setVertexZ(float fVertexZ);
		virtual void setAnchorPoint(const CCPoint& anchor);
		virtual void ignoreAnchorPointForPosition(bool value);
		virtual void setVisible(bool bVisible);
		virtual void draw(void);

		virtual void setColor(const ccColor3B& color3);
		virtual void updateDisplayedColor(const ccColor3B& parentColor);
		virtual void setOpacity(GLubyte opacity);
		virtual void setOpacityModifyRGB(bool modify);
		virtual bool isOpacityModifyRGB(void);
		virtual void updateDisplayedOpacity(GLubyte parentOpacity);

		virtual void updateTransform(void);

		virtual CCSpriteBatchNode* getBatchNode(void);
		virtual void setBatchNode(CCSpriteBatchNode *pobSpriteBatchNode);

		RT_ADD(
			virtual void refreshTextureRect(void);
		)

		virtual void setTextureRect(const CCRect& rect);
		virtual void setTextureRect(const CCRect& rect, bool rotated, const CCSize& untrimmedSize);
		virtual void setVertexRect(const CCRect& rect);

		virtual void setDisplayFrame(CCSpriteFrame *pNewFrame);

		virtual bool isFrameDisplayed(CCSpriteFrame *pFrame);

		virtual CCSpriteFrame* displayFrame(void);

		inline virtual bool isDirty(void) { return m_bDirty; }
		
		inline virtual void setDirty(bool bDirty) { m_bDirty = bDirty; }
		
		inline ccV3F_C4B_T2F_Quad getQuad(void) { return m_sQuad; }

		inline bool isTextureRectRotated(void) { return m_bRectRotated; }
		
		inline unsigned int getAtlasIndex(void) { return m_uAtlasIndex; }
		
		inline void setAtlasIndex(unsigned int uAtlasIndex) { m_uAtlasIndex = uAtlasIndex; }

		inline const CCRect& getTextureRect(void) { return m_obRect; }

		inline CCTextureAtlas* getTextureAtlas(void) { return m_pobTextureAtlas; }
		
		inline void setTextureAtlas(CCTextureAtlas *pobTextureAtlas) { m_pobTextureAtlas = pobTextureAtlas; }

		inline const CCPoint& getOffsetPosition(void) { return m_obOffsetPosition; }


		bool isFlipX(void);
		void setFlipX(bool bFlipX);

		bool isFlipY(void);
		void setFlipY(bool bFlipY);

		cocos2d::CCPoint const& getUnflippedOffsetPosition();
		bool getUseVertexMod() const;
		void setUseVertexMod(bool);

	protected:
		void updateColor(void);
		RT_REMOVE(  virtual void setTextureCoords(CCRect rect);         )
		RT_ADD(     virtual void setTextureCoords(const CCRect& rect);  )
		virtual void updateBlendFunc(void);
		virtual void setReorderChildDirtyRecursively(void);
		virtual void setDirtyRecursively(bool bValue);

		CCTextureAtlas*     m_pobTextureAtlas;      /// CCSpriteBatchNode texture atlas (weak reference)
		unsigned int        m_uAtlasIndex;          /// Absolute (real) Index on the SpriteSheet
		CCSpriteBatchNode*  m_pobBatchNode;         /// Used batch node (weak reference)

		bool                m_bDirty;               /// Whether the sprite needs to be updated
		bool                m_bRecursiveDirty;      /// Whether all of the sprite's children needs to be updated
		bool                m_bHasChildren;         /// Whether the sprite contains children
		bool                m_bShouldBeHidden;      /// should not be drawn because one of the ancestors is not visible
		CCAffineTransform   m_transformToBatch;

		ccBlendFunc        m_sBlendFunc;            /// It's required for CCTextureProtocol inheritance
		CCTexture2D*       m_pobTexture;            /// CCTexture2D object that is used to render the sprite

		CCRect m_obRect;                            /// Retangle of CCTexture2D
		bool   m_bRectRotated;                      /// Whether the texture is rotated

		CCPoint m_obOffsetPosition;
		CCPoint m_obUnflippedOffsetPositionFromCenter;

		ccV3F_C4B_T2F_Quad m_sQuad;

		bool m_bOpacityModifyRGB;

		bool m_bFlipX;                              /// Whether the sprite is flipped horizaontally or not.
		bool m_bFlipY;                              /// Whether the sprite is flipped vertically or not.

		RT_ADD(
			CC_SYNTHESIZE_NV(bool, m_bDontDraw, DontDraw);
			CC_SYNTHESIZE_NV(float, m_fTlVertexMod, TlVertexMod);
			CC_SYNTHESIZE_NV(float, m_fTrVertexMod, TrVertexMod);
			CC_SYNTHESIZE_NV(float, m_fBlVertexMod, BlVertexMod);
			CC_SYNTHESIZE_NV(float, m_fBrVertexMod, BrVertexMod);
			PAD(16);
			bool m_bUnknown;
			// 2.2 additions
			// int m_nUnknown;
		)
	};


	// CCProgressTimer
	class CCProgressTimer : public CCNodeRGBA
	{
	public:
		CCProgressTimer();
		~CCProgressTimer(void);

		inline CCProgressTimerType getType(void) { return m_eType; }
		inline float getPercentage(void) {return m_fPercentage; }
		inline CCSprite* getSprite(void) { return m_pSprite; }

		bool initWithSprite(CCSprite* sp);

		void setPercentage(float fPercentage);
		void setSprite(CCSprite *pSprite);
		void setType(CCProgressTimerType type);
		void setReverseProgress(bool reverse);

		virtual void draw(void);
		void setAnchorPoint(CCPoint anchorPoint);

		virtual void setColor(const ccColor3B& color);
		virtual const ccColor3B& getColor() const;
		virtual GLubyte getOpacity() const;
		virtual void setOpacity(GLubyte opacity);
		
		inline bool isReverseDirection() { return m_bReverseDirection; };
		inline void setReverseDirection(bool value) { m_bReverseDirection = value; };

	public:
		static CCProgressTimer* create(CCSprite* sp);

	protected:
		ccTex2F textureCoordFromAlphaPoint(CCPoint alpha);
		ccVertex2F vertexFromAlphaPoint(CCPoint alpha);
		void updateProgress(void);
		void updateBar(void);
		void updateRadial(void);
		void updateColor(void);
		CCPoint boundaryTexCoord(char index);

	protected:
		CCProgressTimerType m_eType;
		float m_fPercentage;
		CCSprite* m_pSprite;
		int m_nVertexDataCount;
		ccV2F_C4B_T2F *m_pVertexData;

		CC_PROPERTY(CCPoint, m_tMidpoint, Midpoint);
		CC_SYNTHESIZE(CCPoint, m_tBarChangeRate, BarChangeRate);

		bool m_bReverseDirection;
	};


	// CCAnimation
	class CCAnimation : public CCObject
	{
	public:
		CCAnimation();
		~CCAnimation(void);
	public:
		static CCAnimation* create(void);

		static CCAnimation* createWithSpriteFrames(CCArray* arrayOfSpriteFrameNames, float delay = 0.0f);

		static CCAnimation* create(CCArray *arrayOfAnimationFrameNames, float delayPerUnit, unsigned int loops);
		static CCAnimation* create(CCArray *arrayOfAnimationFrameNames, float delayPerUnit) {
			return CCAnimation::create(arrayOfAnimationFrameNames, delayPerUnit, 1);
		}

		void addSpriteFrame(CCSpriteFrame *pFrame);
		void addSpriteFrameWithFileName(const char *pszFileName);
		void addSpriteFrameWithTexture(CCTexture2D* pobTexture, const CCRect& rect);

		bool init();
		bool initWithSpriteFrames(CCArray *pFrames, float delay = 0.0f);
		bool initWithAnimationFrames(CCArray* arrayOfAnimationFrames, float delayPerUnit, unsigned int loops);

		virtual CCObject* copyWithZone(CCZone* pZone);

		CC_SYNTHESIZE_READONLY(float, m_fTotalDelayUnits, TotalDelayUnits)

		CC_SYNTHESIZE(float, m_fDelayPerUnit, DelayPerUnit)

		CC_PROPERTY_READONLY(float, m_fDuration, Duration)

		CC_SYNTHESIZE_RETAIN(CCArray*, m_pFrames, Frames)

		CC_SYNTHESIZE(bool, m_bRestoreOriginalFrame, RestoreOriginalFrame)

		CC_SYNTHESIZE(unsigned int, m_uLoops, Loops)
	};

	// CCActionInterval
	class CCFiniteTimeAction : public CCAction
	{
	public:
		CCFiniteTimeAction()
			: m_fDuration(0)
		{}
		virtual ~CCFiniteTimeAction(){}
		inline float getDuration(void) { return m_fDuration; }
		inline void setDuration(float duration) { m_fDuration = duration; }

		virtual CCFiniteTimeAction* reverse(void);
	protected:
		//! duration in seconds
		float m_fDuration;
	};

	class CCActionInterval : public CCFiniteTimeAction
	{
	public:
		CCActionInterval() {}

		inline float getElapsed(void) { return m_elapsed; }

		bool initWithDuration(float d);

		virtual bool isDone(void);
		virtual CCObject* copyWithZone(CCZone* pZone);
		virtual void step(float dt);
		virtual void startWithTarget(CCNode *pTarget);
		virtual CCActionInterval* reverse(void);

	public:

		static CCActionInterval* create(float d);

	public:
		void setAmplitudeRate(float amp);
		float getAmplitudeRate(void);

		// 2.2 addition
		bool getM_bFirstTick(); // rob were you like high on something when you wrote this

	protected:
		float m_elapsed;
		bool   m_bFirstTick;
	};


	// CCAnimate
	class CCAnimate : public CCActionInterval
	{
	public:
		CCAnimate();
		~CCAnimate();

		/** initializes the action with an Animation and will restore the original frame when the animation is over */
		bool initWithAnimation(CCAnimation *pAnimation);

		/**
		*  @js NA
		*  @lua NA
		*/
		virtual CCObject* copyWithZone(CCZone* pZone);
		virtual void startWithTarget(CCNode *pTarget);
		virtual void stop(void);
		virtual void update(float t);
		virtual CCActionInterval* reverse(void);

		// 2.2 addition
		bool getRecenterChildren() const;
		bool getRecenterFrames() const;
		void setRecenterChildren(bool recenter);
		void setRecenterFrames(bool recenter);

	public:
		/** creates the action with an Animation and will restore the original frame when the animation is over */
		static CCAnimate* create(CCAnimation *pAnimation);
		CC_SYNTHESIZE_RETAIN(CCAnimation*, m_pAnimation, Animation)
	protected:
		std::vector<float>* m_pSplitTimes;
		int                m_nNextFrame;
		CCSpriteFrame*  m_pOrigFrame;
		unsigned int    m_uExecutedLoops;
	};


	// CCScaleTo
	class CCScaleTo : public CCActionInterval
	{
	public:
		bool initWithDuration(float duration, float s);

		bool initWithDuration(float duration, float sx, float sy);
		virtual CCObject* copyWithZone(CCZone* pZone);
		virtual void startWithTarget(CCNode *pTarget);
		virtual void update(float time);

	public:
		static CCScaleTo* create(float duration, float s);

		static CCScaleTo* create(float duration, float sx, float sy);
	protected:
		float m_fScaleX;
		float m_fScaleY;
		float m_fStartScaleX;
		float m_fStartScaleY;
		float m_fEndScaleX;
		float m_fEndScaleY;
		float m_fDeltaX;
		float m_fDeltaY;
	};


	// CCRenderTexture
	class CCRenderTexture : public CCNode 
	{
		CC_PROPERTY(CCSprite*, m_pSprite, Sprite)
	public:
		CCRenderTexture();
		virtual ~CCRenderTexture();

		virtual void visit();
		virtual void draw();

		static CCRenderTexture * create(int w ,int h, CCTexture2DPixelFormat eFormat, GLuint uDepthStencilFormat);

		static CCRenderTexture * create(int w, int h, CCTexture2DPixelFormat eFormat);

		static CCRenderTexture * create(int w, int h);

		bool initWithWidthAndHeight(int w, int h, CCTexture2DPixelFormat eFormat);

		bool initWithWidthAndHeight(int w, int h, CCTexture2DPixelFormat eFormat, GLuint uDepthStencilFormat);

		void begin();

		void beginWithClear(float r, float g, float b, float a);

		void beginWithClear(float r, float g, float b, float a, float depthValue);

		void beginWithClear(float r, float g, float b, float a, float depthValue, int stencilValue);

		inline void endToLua(){ end();};

		void end();

		void clear(float r, float g, float b, float a);

		void clearDepth(float depthValue);

		void clearStencil(int stencilValue);
		CCImage* newCCImage(bool flipImage = true);

		bool saveToFile(const char *szFilePath);

		bool saveToFile(const char *name, tCCImageFormat format);

		void listenToBackground(CCObject *obj);

		void listenToForeground(CCObject *obj);
		
		unsigned int getClearFlags() const;
		void setClearFlags(unsigned int uClearFlags);
		
		const ccColor4F& getClearColor() const;
		void setClearColor(const ccColor4F &clearColor);
		
		float getClearDepth() const;
		void setClearDepth(float fClearDepth);
		
		int getClearStencil() const;
		void setClearStencil(float fClearStencil);
		
		bool isAutoDraw() const;
		void setAutoDraw(bool bAutoDraw);

		void updateInternalScale(float, float);

	private:
		void beginWithClear(float r, float g, float b, float a, float depthValue, int stencilValue, GLbitfield flags);

	protected:
		GLuint       m_uFBO;
		GLuint       m_uDepthRenderBufffer;
		GLint        m_nOldFBO;
		CCTexture2D* m_pTexture;
		CCTexture2D* m_pTextureCopy;    // a copy of m_pTexture
		CCImage*     m_pUITextureImage;
		GLenum       m_ePixelFormat;
		
		// code for "auto" update
		GLbitfield   m_uClearFlags;
		ccColor4F    m_sClearColor;
		GLclampf     m_fClearDepth;
		GLint        m_nClearStencil;
		bool         m_bAutoDraw;
	};


	// CCDrawNode
	class CCDrawNode : public CCNodeRGBA
	{
	protected:

		GLuint      m_uVao;
		GLuint      m_uVbo;
		
		unsigned int    m_uBufferCapacity;
		GLsizei         m_nBufferCount;
		ccV2F_C4B_T2F   *m_pBuffer;
		
		ccBlendFunc     m_sBlendFunc;
		
		bool            m_bDirty;
		
	public:
		static CCDrawNode* create();
		virtual ~CCDrawNode();

		virtual bool init();
		virtual void draw();

		/** draw a dot at a position, with a given radius and color */
		bool drawDot(const CCPoint &pos, float radius, const ccColor4F &color);
		
		/** draw a segment with a radius and color */
		bool drawSegment(const CCPoint &from, const CCPoint &to, float radius, const ccColor4F &color);
		
		/** draw a polygon with a fill color and line color 
		* @code
		* when this funciton bound to js,the input params are changed
		* js:var drawPolygon(var verts, var fillColor,var borderWidth,var borderColor)
		* @endcode
		*/
		bool drawPolygon(CCPoint *verts, unsigned int count, const ccColor4F &fillColor, float borderWidth, const ccColor4F &borderColor);

		bool drawCircle(cocos2d::CCPoint const&, float, cocos2d::_ccColor4F const&, float, cocos2d::_ccColor4F const&, unsigned int);
		void drawCubicBezier(cocos2d::CCPoint const&, cocos2d::CCPoint const&, cocos2d::CCPoint const&, cocos2d::CCPoint const&, unsigned int, cocos2d::_ccColor4F const&);
		void drawPreciseCubicBezier(cocos2d::CCPoint const&, cocos2d::CCPoint const&, cocos2d::CCPoint const&, cocos2d::CCPoint const&, unsigned int, cocos2d::_ccColor4F const&);
		bool drawLines(cocos2d::CCPoint*, unsigned int, float, cocos2d::_ccColor4F const&);
		bool drawRect(cocos2d::CCPoint const&, cocos2d::CCPoint const&, cocos2d::_ccColor4F const&, float, cocos2d::_ccColor4F const&);

		void clear();
		ccBlendFunc getBlendFunc() const;
		void setBlendFunc(const ccBlendFunc &blendFunc);
		
		CCDrawNode();

		void listenBackToForeground(CCObject *obj);
	private:
		void ensureCapacity(unsigned int count);
		void render();
	};


	// CCParticleSystemQuad
	class CCParticleSystem;
	#define kCCParticleDefaultCapacity 500
	class CCParticleBatchNode : public CCNode, public CCTextureProtocol
	{
	public:
		CCParticleBatchNode();
		virtual ~CCParticleBatchNode();

		static CCParticleBatchNode* createWithTexture(CCTexture2D *tex, unsigned int capacity = kCCParticleDefaultCapacity);

		static CCParticleBatchNode* create(const char* fileImage, unsigned int capacity = kCCParticleDefaultCapacity);

		bool initWithTexture(CCTexture2D *tex, unsigned int capacity);

		bool initWithFile(const char* fileImage, unsigned int capacity);

		virtual void addChild(CCNode * child);
		virtual void addChild(CCNode * child, int zOrder);
		virtual void addChild(CCNode * child, int zOrder, int tag);

		void insertChild(CCParticleSystem* pSystem, unsigned int index);

		virtual void removeChild(CCNode* child, bool cleanup);
		virtual void reorderChild(CCNode * child, int zOrder);
		void removeChildAtIndex(unsigned int index, bool doCleanup);
		void removeAllChildrenWithCleanup(bool doCleanup);
		void disableParticle(unsigned int particleIndex);
		virtual void draw(void);
		virtual CCTexture2D* getTexture(void);
		virtual void setTexture(CCTexture2D *texture);
		virtual void setBlendFunc(ccBlendFunc blendFunc);
		virtual ccBlendFunc getBlendFunc(void);

		void visit();

	private:
		void updateAllAtlasIndexes();
		void increaseAtlasCapacityTo(unsigned int quantity);
		unsigned int searchNewPositionInChildrenForZ(int z);
		void getCurrentIndex(unsigned int* oldIndex, unsigned int* newIndex, CCNode* child, int z);
		unsigned int addChildHelper(CCParticleSystem* child, int z, int aTag);
		void updateBlendFunc(void);
		CC_SYNTHESIZE(CCTextureAtlas*, m_pTextureAtlas, TextureAtlas);
	private:
		ccBlendFunc m_tBlendFunc;
	};

	class CCParticleSystem : public CCNode, public CCTextureProtocol
	{
	protected:
		std::string m_sPlistFile;
		//! time elapsed since the start of the system (in seconds)
		float m_fElapsed;

		// Different modes
		//! Mode A:Gravity + Tangential Accel + Radial Accel
		struct {
			/** Gravity value. Only available in 'Gravity' mode. */
			CCPoint gravity;
			/** speed of each particle. Only available in 'Gravity' mode.  */
			float speed;
			/** speed variance of each particle. Only available in 'Gravity' mode. */
			float speedVar;
			/** tangential acceleration of each particle. Only available in 'Gravity' mode. */
			float tangentialAccel;
			/** tangential acceleration variance of each particle. Only available in 'Gravity' mode. */
			float tangentialAccelVar;
			/** radial acceleration of each particle. Only available in 'Gravity' mode. */
			float radialAccel;
			/** radial acceleration variance of each particle. Only available in 'Gravity' mode. */
			float radialAccelVar;
			/** set the rotation of each particle to its direction Only available in 'Gravity' mode. */
			bool rotationIsDir;
		} modeA;

		//! Mode B: circular movement (gravity, radial accel and tangential accel don't are not used in this mode)
		struct {
			/** The starting radius of the particles. Only available in 'Radius' mode. */
			float startRadius;
			/** The starting radius variance of the particles. Only available in 'Radius' mode. */
			float startRadiusVar;
			/** The ending radius of the particles. Only available in 'Radius' mode. */
			float endRadius;
			/** The ending radius variance of the particles. Only available in 'Radius' mode. */
			float endRadiusVar;
			/** Number of degrees to rotate a particle around the source pos per second. Only available in 'Radius' mode. */
			float rotatePerSecond;
			/** Variance in degrees for rotatePerSecond. Only available in 'Radius' mode. */
			float rotatePerSecondVar;
		} modeB;

		//! Array of particles
		tCCParticle *m_pParticles;

		// color modulate
		//    BOOL colorModulate;

		//! How many particles can be emitted per second
		float m_fEmitCounter;

		//!  particle idx
		unsigned int m_uParticleIdx;

		// Optimization
		//CC_UPDATE_PARTICLE_IMP    updateParticleImp;
		//SEL                        updateParticleSel;

		/** weak reference to the CCSpriteBatchNode that renders the CCSprite */
		CC_PROPERTY(CCParticleBatchNode*, m_pBatchNode, BatchNode);

		RT_REMOVE(
			CC_SYNTHESIZE(unsigned int, m_uAtlasIndex, AtlasIndex);
		)
		RT_ADD(
			CC_SYNTHESIZE_NV(unsigned int, m_uAtlasIndex, AtlasIndex);
		)

		//true if scaled or rotated
		bool m_bTransformSystemDirty;
		// Number of allocated particles
		unsigned int m_uAllocatedParticles;

		/** Is the emitter active */
		bool m_bIsActive;
		/** Quantity of particles that are being simulated at the moment */
		CC_PROPERTY_READONLY(unsigned int, m_uParticleCount, ParticleCount)
		/** How many seconds the emitter will run. -1 means 'forever' */
		CC_PROPERTY(float, m_fDuration, Duration)
		/** sourcePosition of the emitter */
		CC_PROPERTY_PASS_BY_REF(CCPoint, m_tSourcePosition, SourcePosition)
		/** Position variance of the emitter */
		CC_PROPERTY_PASS_BY_REF(CCPoint, m_tPosVar, PosVar)
		/** life, and life variation of each particle */
		CC_PROPERTY(float, m_fLife, Life)
		/** life variance of each particle */
		CC_PROPERTY(float, m_fLifeVar, LifeVar)
		/** angle and angle variation of each particle */
		CC_PROPERTY(float, m_fAngle, Angle)
		/** angle variance of each particle */
		CC_PROPERTY(float, m_fAngleVar, AngleVar)

	//////////////////////////////////////////////////////////////////////////
	public:
		// mode A
		virtual const CCPoint& getGravity();
		virtual void setGravity(const CCPoint& g);
		virtual float getSpeed();
		virtual void setSpeed(float speed);
		virtual float getSpeedVar();
		virtual void setSpeedVar(float speed);
		virtual float getTangentialAccel();
		virtual void setTangentialAccel(float t);
		virtual float getTangentialAccelVar();
		virtual void setTangentialAccelVar(float t);
		virtual float getRadialAccel();
		virtual void setRadialAccel(float t);
		virtual float getRadialAccelVar();
		virtual void setRadialAccelVar(float t);
		virtual bool getRotationIsDir();
		virtual void setRotationIsDir(bool t);
		// mode B
		virtual float getStartRadius();
		virtual void setStartRadius(float startRadius);
		virtual float getStartRadiusVar();
		virtual void setStartRadiusVar(float startRadiusVar);
		virtual float getEndRadius();
		virtual void setEndRadius(float endRadius);
		virtual float getEndRadiusVar();
		virtual void setEndRadiusVar(float endRadiusVar);
		virtual float getRotatePerSecond();
		virtual void setRotatePerSecond(float degrees);
		virtual float getRotatePerSecondVar();
		virtual void setRotatePerSecondVar(float degrees);

		RT_ADD (
			virtual void setVisible(bool visible);
		)

		virtual void setScale(float s);
		virtual void setRotation(float newRotation);
		virtual void setScaleX(float newScaleX);
		virtual void setScaleY(float newScaleY);
		
		virtual bool isActive();
		virtual bool isBlendAdditive();
		virtual void setBlendAdditive(bool value);
	//////////////////////////////////////////////////////////////////////////
		
		/** start size in pixels of each particle */
		CC_PROPERTY(float, m_fStartSize, StartSize)
		/** size variance in pixels of each particle */
		CC_PROPERTY(float, m_fStartSizeVar, StartSizeVar)
		/** end size in pixels of each particle */
		CC_PROPERTY(float, m_fEndSize, EndSize)
		/** end size variance in pixels of each particle */
		CC_PROPERTY(float, m_fEndSizeVar, EndSizeVar)
		/** start color of each particle */
		CC_PROPERTY_PASS_BY_REF(ccColor4F, m_tStartColor, StartColor)
		/** start color variance of each particle */
		CC_PROPERTY_PASS_BY_REF(ccColor4F, m_tStartColorVar, StartColorVar)
		/** end color and end color variation of each particle */
		CC_PROPERTY_PASS_BY_REF(ccColor4F, m_tEndColor, EndColor)
		/** end color variance of each particle */
		CC_PROPERTY_PASS_BY_REF(ccColor4F, m_tEndColorVar, EndColorVar)
		//* initial angle of each particle
		CC_PROPERTY(float, m_fStartSpin, StartSpin)
		//* initial angle of each particle
		CC_PROPERTY(float, m_fStartSpinVar, StartSpinVar)
		//* initial angle of each particle
		CC_PROPERTY(float, m_fEndSpin, EndSpin)
		//* initial angle of each particle
		CC_PROPERTY(float, m_fEndSpinVar, EndSpinVar)
		/** emission rate of the particles */
		CC_PROPERTY(float, m_fEmissionRate, EmissionRate)
		/** maximum particles of the system */
		CC_PROPERTY(unsigned int, m_uTotalParticles, TotalParticles)
		/** conforms to CocosNodeTexture protocol */
		CC_PROPERTY(CCTexture2D*, m_pTexture, Texture)
		/** conforms to CocosNodeTexture protocol */
		CC_PROPERTY(ccBlendFunc, m_tBlendFunc, BlendFunc)
		/** does the alpha value modify color */
		CC_PROPERTY(bool, m_bOpacityModifyRGB, OpacityModifyRGB)

		/** whether or not the particles are using blend additive.
		If enabled, the following blending function will be used.
		@code
		source blend function = GL_SRC_ALPHA;
		dest blend function = GL_ONE;
		@endcode
		*/
		bool m_bIsBlendAdditive;
		/** particles movement type: Free or Grouped
		@since v0.8
		*/
		CC_PROPERTY(tCCPositionType, m_ePositionType, PositionType)
		/** whether or not the node will be auto-removed when it has no particles left.
		By default it is false.
		@since v0.8
		*/
	protected:
		bool m_bIsAutoRemoveOnFinish;
	public:
		virtual bool isAutoRemoveOnFinish();
		virtual void setAutoRemoveOnFinish(bool var);

		/** Switch between different kind of emitter modes:
		- kCCParticleModeGravity: uses gravity, speed, radial and tangential acceleration
		- kCCParticleModeRadius: uses radius movement + rotation
		*/
		CC_PROPERTY(int, m_nEmitterMode, EmitterMode)

	public:
		/**
		* @js ctor
		*/
		CCParticleSystem();
		/**
		* @js NA
		* @lua NA
		*/
		virtual ~CCParticleSystem();

		/** creates an initializes a CCParticleSystem from a plist file.
		This plist files can be created manually or with Particle Designer:
		http://particledesigner.71squared.com/
		@since v2.0
		*/
		static CCParticleSystem * create(const char *plistFile);

		//! create a system with a fixed number of particles
		static CCParticleSystem* createWithTotalParticles(unsigned int numberOfParticles);

		/** initializes a CCParticleSystem*/
		bool init();
		/** initializes a CCParticleSystem from a plist file.
		This plist files can be created manually or with Particle Designer:
		http://particledesigner.71squared.com/
		@since v0.99.3
		*/
		bool initWithFile(const char *plistFile, bool);

		/** initializes a CCQuadParticleSystem from a CCDictionary.
		@since v0.99.3
		*/
		bool initWithDictionary(CCDictionary *dictionary, bool);
		
		/** initializes a particle system from a NSDictionary and the path from where to load the png
		@since v2.1
		*/
		bool initWithDictionary(CCDictionary *dictionary, const char *dirname, bool);

		//! Initializes a system with a fixed number of particles
		virtual bool initWithTotalParticles(unsigned int numberOfParticles, bool);
		//! Add a particle to the emitter
		bool addParticle();
		//! Initializes a particle
		void initParticle(tCCParticle* particle);
		//! stop emitting particles. Running particles will continue to run until they die
		void stopSystem();
		//! Kill all living particles.
		void resetSystem();
		RT_ADD(
			void resumeSystem();
		)
		//! whether or not the system is full
		bool isFull();

		//! should be overridden by subclasses
		virtual void updateQuadWithParticle(tCCParticle* particle, const CCPoint& newPosition);
		//! should be overridden by subclasses
		virtual void postStep();

		virtual void update(float dt);
		virtual void updateWithNoTime(void);

	protected:
		virtual void updateBlendFunc();

		RT_ADD(
			// saved/loaded in loadDefaults, loadScaledDefaults and saveDefaults

			float m_fDefaultStartSize;
			float m_fDefaultStartSizeVar;
			// saved as m_fEndSize but not loaded,
			// probably was supposed to be m_fDefaultEndSizeVar and saved and loaded as m_fEndSizeVar but was scrapped?
			float m_fDefaultEndSize2;
			float m_fDefaultEndSize;
			float m_fDefaultModeASpeed;
			float m_fDefaultModeASpeedVar;
			CCPoint m_tDefaultPosVar;
		)
	public:
		RT_ADD(
			void saveDefaults(void);
			void loadDefaults(void);
			void loadScaledDefaults(float);

			void calculateWorldSpace();

		bool getDontCleanupOnFinish() const;
		void setDontCleanupOnFinish(bool);

		bool getDynamicRotationIsDir() const;
		void setDynamicRotationIsDir(bool);

		bool getEndRGBVarSync() const;
		void setEndRGBVarSync(bool);

		float getFadeInTime() const;
		float getFadeInTimeVar() const;
		float getFadeOutTime() const;
		float getFadeOutTimeVar() const;
		float getFrictionPos() const;
		float getFrictionPosVar() const;
		float getFrictionRot() const;
		float getFrictionRotVar() const;
		float getFrictionSize() const;
		float getFrictionSizeVar() const;

		bool getOrderSensitive() const;

		float getRespawn() const;
		float getRespawnVar() const;

		bool getStartRGBVarSync() const;
		bool getStartRadiusEqualToEnd() const;
		bool getStartSizeEqualToEnd() const;
		bool getStartSpinEqualToEnd() const;

		float getTimeElapsed();

		bool getUseUniformColorMode() const;

		bool getWasRemoved() const;

		bool getUsingSchedule() const;

		
		void setEndAlpha(float);
		void setFadeInTime(float);
		void setFadeInTimeVar(float);
		void setFadeOutTime(float);
		void setFadeOutTimeVar(float);
		void setFrictionPos(float);
		void setFrictionPosVar(float);
		void setFrictionRot(float);
		void setFrictionRotVar(float);
		void setFrictionSize(float);
		void setFrictionSizeVar(float);

		void setOrderSensitive(bool);

		void setRespawn(float);
		void setRespawnVar(float);

		void setStartAlpha(float);
		void setStartRGBVarSync(bool);
		void setStartRadiusEqualToEnd(bool);
		void setStartSizeEqualToEnd(bool);
		void setStartSpinEqualToEnd(bool);

		void setUsingSchedule(bool);

		void setWasRemoved(bool);

		void toggleUniformColorMode(bool);
		void updateVisible();

		virtual void updateEmissionRate();


		)
	};
	class CCParticleSystemQuad : public CCParticleSystem
	{
	protected:
		ccV3F_C4B_T2F_Quad    *m_pQuads;        // quads to be rendered
		GLushort            *m_pIndices;    // indices

	#if CC_TEXTURE_ATLAS_USE_VAO
		GLuint                m_uVAOname;
	#endif

		GLuint                m_pBuffersVBO[2]; //0: vertex  1: indices

	public:
		CCParticleSystemQuad();
		virtual ~CCParticleSystemQuad();

		static CCParticleSystemQuad * create(const char *plistFile);

		void initIndices();

		void initTexCoordsWithRect(const CCRect& rect);

		void setDisplayFrame(CCSpriteFrame *spriteFrame);

		void setTextureWithRect(CCTexture2D *texture, const CCRect& rect);
		virtual bool initWithTotalParticles(unsigned int numberOfParticles, bool);
		virtual void setTexture(CCTexture2D* texture);
		virtual void updateQuadWithParticle(tCCParticle* particle, const CCPoint& newPosition);
		virtual void postStep();
		virtual void draw();
		virtual void setBatchNode(CCParticleBatchNode* batchNode);
		virtual void setTotalParticles(unsigned int tp);
		void listenBackToForeground(CCObject *obj);

		static CCParticleSystemQuad * create();
		static CCParticleSystemQuad * create(const char*, bool);
		static CCParticleSystemQuad * createWithTotalParticles(unsigned int numberOfParticles, bool);

		unsigned char getOpacity();
		void setOpacity(unsigned char);

		void updateTexCoords();

	private:
	#if CC_TEXTURE_ATLAS_USE_VAO
		void setupVBOandVAO();
	#else
		void setupVBO();
	#endif
		bool allocMemory();
	};


	// CCApplication
	class CCApplicationProtocol
	{
	public:
		virtual ~CCApplicationProtocol() {}

		virtual bool applicationDidFinishLaunching() { return true; }

		virtual void applicationDidEnterBackground() {}

		virtual void applicationWillEnterForeground() {}

		RT_ADD(
			virtual void applicationWillBecomeActive() {}
			virtual void applicationWillResignActive() {}
			virtual void trySaveGame(bool) {}
			virtual void gameDidSave() {}
		)

		virtual void setAnimationInterval(double interval) {}

		virtual ccLanguageType getCurrentLanguage() { return kLanguageEnglish; }
		
		virtual TargetPlatform getTargetPlatform() { return kTargetWindows; }

		RT_ADD( virtual void openURL(const char* url) {} )
	};

	class CCApplication : public CCApplicationProtocol
	{
	public:
		CCApplication();
		virtual ~CCApplication();

		virtual void setAnimationInterval(double interval) override;

		int run();

		static CCApplication* sharedApplication();

		virtual ccLanguageType getCurrentLanguage() override;

		virtual TargetPlatform getTargetPlatform() override;

		virtual void openURL(char const* url) override;

	protected:
		static CCApplication * sm_pSharedApplication;
	};


	// CCLabelBMFont
	enum {
		kCCLabelAutomaticWidth = -1,
	};

	class CCBMFontConfiguration : public CCObject
	{
		// XXX: Creating a public interface so that the bitmapFontArray[] is accessible
	public://@public
		// BMFont definitions
		tCCFontDefHashElement *m_pFontDefDictionary;

		//! FNTConfig: Common Height Should be signed (issue #1343)
		int m_nCommonHeight;
		//! Padding
		ccBMFontPadding    m_tPadding;
		//! atlas name
		std::string m_sAtlasName;
		//! values for kerning
		tCCKerningHashElement *m_pKerningDictionary;

		// Character Set defines the letters that actually exist in the font
		std::set<unsigned int> *m_pCharacterSet;
	public:
		CCBMFontConfiguration();
		/**
		*  @js NA
		*  @lua NA
		*/
		virtual ~CCBMFontConfiguration();
		/**
		*  @js NA
		*  @lua NA
		*/
		const char * description();

		/** allocates a CCBMFontConfiguration with a FNT file */
		static CCBMFontConfiguration * create(const char *FNTfile);

		/** initializes a BitmapFontConfiguration with a FNT file */
		bool initWithFNTfile(const char *FNTfile);

		inline const char* getAtlasName(){ return m_sAtlasName.c_str(); }
		inline void setAtlasName(const char* atlasName) { m_sAtlasName = atlasName; }

		inline std::set<unsigned int>* getCharacterSet() const { return m_pCharacterSet; }
	private:
		std::set<unsigned int>* parseConfigFile(const char *controlFile);
		void parseCharacterDefinition(std::string line, ccBMFontDef *characterDefinition);
		void parseInfoArguments(std::string line);
		void parseCommonArguments(std::string line);
		void parseImageFileName(std::string line, const char *fntFile);
		void parseKerningEntry(std::string line);
		void purgeKerningDictionary();
		void purgeFontDefDictionary();
	};

	class CCLabelProtocol
	{
	public:
		virtual void setString(const char *label) = 0;

		virtual const char* getString(void) = 0;
	};

	class CCLabelBMFont : public CCSpriteBatchNode, public CCLabelProtocol, public CCRGBAProtocol
	{
	public:
		CCLabelBMFont();
		virtual ~CCLabelBMFont();
		static void purgeCachedData();

		static CCLabelBMFont * create(const char *str, const char *fntFile, float width, CCTextAlignment alignment, CCPoint imageOffset);
		static CCLabelBMFont * create(const char *str, const char *fntFile, float width, CCTextAlignment alignment);
		static CCLabelBMFont * create(const char *str, const char *fntFile, float width);
		static CCLabelBMFont * create(const char *str, const char *fntFile);

		static CCLabelBMFont * create();

		bool init();
		bool initWithString(const char *str, const char *fntFile, float width = kCCLabelAutomaticWidth, CCTextAlignment alignment = kCCTextAlignmentLeft, CCPoint imageOffset = CCPointZero);

		void createFontChars();
		// super method
		virtual void setString(const char *newString);
		virtual void setString(const char *newString, bool needUpdateLabel);

		virtual const char* getString(void);
		virtual void setCString(const char *label);
		virtual void setAnchorPoint(const CCPoint& var);
		virtual void updateLabel();
		virtual void setAlignment(CCTextAlignment alignment);
		virtual void setWidth(float width);
		virtual void setLineBreakWithoutSpace(bool breakWithoutSpace);
		virtual void setScale(float scale);
		virtual void setScaleX(float scaleX);
		virtual void setScaleY(float scaleY);

		// CCRGBAProtocol
		virtual bool isOpacityModifyRGB();
		virtual void setOpacityModifyRGB(bool isOpacityModifyRGB); virtual GLubyte getOpacity();
		virtual GLubyte getDisplayedOpacity();
		virtual void setOpacity(GLubyte opacity);
		virtual void updateDisplayedOpacity(GLubyte parentOpacity);
		virtual bool isCascadeOpacityEnabled();
		virtual void setCascadeOpacityEnabled(bool cascadeOpacityEnabled);
		virtual const ccColor3B& getColor(void);
		virtual const ccColor3B& getDisplayedColor();
		virtual void setColor(const ccColor3B& color);
		virtual void updateDisplayedColor(const ccColor3B& parentColor);
		virtual bool isCascadeColorEnabled();
		virtual void setCascadeColorEnabled(bool cascadeColorEnabled);

		void setFntFile(const char* fntFile);
		const char* getFntFile();
		inline CCBMFontConfiguration* getConfiguration() const {
			return m_pConfiguration;
		}
	#if CC_LABELBMFONT_DEBUG_DRAW
		virtual void draw();
	#endif // CC_LABELBMFONT_DEBUG_DRAW

		RT_ADD(
			static CCLabelBMFont* createBatched(const char* str, const char* fntFile, CCArray*, int);
			void limitLabelWidth(float width, float defaultScale, float minScale);

		int getExtraKerning() const;
		void setExtraKerning(int);

		bool getIsBatched() const;
		void setIsBatched(bool);

		cocos2d::CCArray* getTargetArray() const;
		void setTargetArray(cocos2d::CCArray*);

		)

	private:
		char * atlasNameFromFntFile(const char *fntFile);
		int kerningAmountForFirst(unsigned short first, unsigned short second);
		float getLetterPosXLeft( CCSprite* characterSprite, float, bool);
		float getLetterPosXRight( CCSprite* characterSprite, float, bool);

	protected:
		virtual void setString(unsigned short *newString, bool needUpdateLabel);
		// string to render
		unsigned short* m_sString;

		// name of fntFile
		std::string m_sFntFile;

		// initial string without line breaks
		unsigned short* m_sInitialString;
		std::string m_sInitialStringUTF8;

		// alignment of all lines
		CCTextAlignment m_pAlignment;
		// max width until a line break is added
		float m_fWidth;

		CCBMFontConfiguration *m_pConfiguration;

		bool m_bLineBreakWithoutSpaces;
		// offset of the texture atlas
		CCPoint    m_tImageOffset;

		// reused char
		CCSprite *m_pReusedChar;

		// texture RGBA
		GLubyte m_cDisplayedOpacity;
		GLubyte m_cRealOpacity;
		ccColor3B m_tDisplayedColor;
		ccColor3B m_tRealColor;
		bool m_bCascadeColorEnabled;
		bool m_bCascadeOpacityEnabled;
		/** conforms to CCRGBAProtocol protocol */
		bool        m_bIsOpacityModifyRGB;

		RT_ADD(
			bool m_bIsBatched;
			CCArray* m_pTargetArray;
			CCTexture2D* m_pSomeTexture;
			void* m_pUnknown; // 2.2 addition, might be positioned somewhere else
		)
	};


	// CCTouchDispatcher
	class  CCTouchHandler : public CCObject
	{
	public:
		inline CCTouchHandler() = default;
		virtual ~CCTouchHandler(void);

		CCTouchDelegate* getDelegate();
		void setDelegate(CCTouchDelegate *pDelegate);

		int getPriority(void);
		void setPriority(int nPriority);

		int getEnabledSelectors(void);
		void setEnalbedSelectors(int nValue);

		virtual bool initWithDelegate(CCTouchDelegate *pDelegate, int nPriority);

	public:
		static CCTouchHandler* handlerWithDelegate(CCTouchDelegate *pDelegate, int nPriority);

	public:
		CCTouchDelegate *m_pDelegate;
		int m_nPriority;
		int m_nEnabledSelectors;
	};

	class EGLTouchDelegate
	{
	public:
		virtual void touchesBegan(CCSet* touches, CCEvent* pEvent) = 0;
		virtual void touchesMoved(CCSet* touches, CCEvent* pEvent) = 0;
		virtual void touchesEnded(CCSet* touches, CCEvent* pEvent) = 0;
		virtual void touchesCancelled(CCSet* touches, CCEvent* pEvent) = 0;
		virtual ~EGLTouchDelegate() {}
	};

	class CCTouchDispatcher : public CCObject, public EGLTouchDelegate
	{
	public:
		~CCTouchDispatcher();
		bool init(void);
		CCTouchDispatcher()
			: m_pTargetedHandlers(NULL)
			, m_pStandardHandlers(NULL)
			, m_pHandlersToAdd(NULL)
			, m_pHandlersToRemove(NULL)
		{}

	public:
		bool isDispatchEvents(void);
		void setDispatchEvents(bool bDispatchEvents);

		void addStandardDelegate(CCTouchDelegate *pDelegate, int nPriority);

		void addTargetedDelegate(CCTouchDelegate *pDelegate, int nPriority, bool bSwallowsTouches);

		void removeDelegate(CCTouchDelegate *pDelegate);

		void removeAllDelegates(void);

		void setPriority(int nPriority, CCTouchDelegate *pDelegate);
		void touches(CCSet *pTouches, CCEvent *pEvent, unsigned int uIndex);
		virtual void touchesBegan(CCSet* touches, CCEvent* pEvent);
		virtual void touchesMoved(CCSet* touches, CCEvent* pEvent);
		virtual void touchesEnded(CCSet* touches, CCEvent* pEvent);
		virtual void touchesCancelled(CCSet* touches, CCEvent* pEvent);

	public:
		CCTouchHandler* findHandler(CCTouchDelegate *pDelegate);

		void addPrioTargetedDelegate(cocos2d::CCTouchDelegate*, int, bool);
		bool isUsingForcePrio();
		void registerForcePrio(cocos2d::CCObject*, int);
		void unregisterForcePrio(cocos2d::CCObject*);

	private:
		RT_ADD(
			void incrementForcePrio(int priority);
			void decrementForcePrio(int priority);
		)

	protected:
		void forceRemoveDelegate(CCTouchDelegate *pDelegate);
		void forceAddHandler(CCTouchHandler *pHandler, CCArray* pArray);
		void forceRemoveAllDelegates(void);
		void rearrangeHandlers(CCArray* pArray);
		CCTouchHandler* findHandler(CCArray* pArray, CCTouchDelegate *pDelegate);

	public:
		CCArray* m_pTargetedHandlers;
		CCArray* m_pStandardHandlers;

		bool m_bLocked;
		bool m_bToAdd;
		bool m_bToRemove;
		CCArray* m_pHandlersToAdd;
		struct _ccCArray *m_pHandlersToRemove;
		bool m_bToQuit;
		bool m_bDispatchEvents;

		// 4, 1 for each type of event
		struct ccTouchHandlerHelperData m_sHandlerHelperData[ccTouchMax];

	protected:
		// 2.2 changes

		CC_SYNTHESIZE_NV(int, m_forcePrio, ForcePrio);
		void* m_unknown;
		CC_SYNTHESIZE_NV(int, m_targetPrio, TargetPrio);
	};

	// CCDirector
	class TypeInfo
	{
	public:
		virtual long getClassTypeInfo() = 0;
	};

	static inline unsigned int getHashCodeByString(const char *key)
	{
		unsigned int len = strlen(key);
		const char *end=key+len;
		unsigned int hash;

		for (hash = 0; key < end; key++)
		{
			hash *= 16777619;
			hash ^= (unsigned int)(unsigned char)std::toupper(*key);
		}
		return (hash);
	}

	class CCEGLViewProtocol
	{
	public:
		CCEGLViewProtocol();
		virtual ~CCEGLViewProtocol();

		virtual void    end() {}
		virtual bool    isOpenGLReady() { return false; }
		virtual void    swapBuffers() {}
		virtual void    setIMEKeyboardState(bool bOpen) {}
		virtual const CCSize& getFrameSize() const;
		virtual void setFrameSize(float width, float height);
		virtual CCSize getVisibleSize() const;
		virtual CCPoint getVisibleOrigin() const;
		virtual void setDesignResolutionSize(float width, float height, ResolutionPolicy resolutionPolicy);
		virtual const CCSize&  getDesignResolutionSize() const;
		virtual void setTouchDelegate(EGLTouchDelegate * pDelegate);
		virtual void setViewPortInPoints(float x , float y , float w , float h);
		virtual void setScissorInPoints(float x , float y , float w , float h);
		virtual bool isScissorEnabled();
		virtual CCRect getScissorRect();
		virtual void setViewName(const char* pszViewName);
		const char* getViewName();
		virtual void handleTouchesBegin(int num, int ids[], float xs[], float ys[]);
		virtual void handleTouchesMove(int num, int ids[], float xs[], float ys[]);
		virtual void handleTouchesEnd(int num, int ids[], float xs[], float ys[]);
		virtual void handleTouchesCancel(int num, int ids[], float xs[], float ys[]);
		const CCRect& getViewPortRect() const;
		float getScaleX() const;
		float getScaleY() const;
		virtual void pollInputEvents();
		void updateDesignResolutionSize();

	private:
		void getSetOfTouchesEndOrCancel(CCSet& set, int num, int ids[], float xs[], float ys[]);

	protected:
		EGLTouchDelegate* m_pDelegate;

		// real screen size
		CCSize m_obScreenSize;
		// resolution size, it is the size appropriate for the app resources.
		CCSize m_obDesignResolutionSize;
		// the view port size
		CCRect m_obViewPortRect;
		// the view name
		char   m_szViewName[50];

		float  m_fScaleX;
		float  m_fScaleY;
		ResolutionPolicy m_eResolutionPolicy;
	};

	class CCEGLView : public CCEGLViewProtocol RT_ADD(, public CCObject)
	{
	protected:
		RT_ADD( virtual ~CCEGLView(); )
	public:
		CCEGLView();

		RT_REMOVE(  virtual ~CCEGLView();   )

		virtual bool isOpenGLReady();
		virtual void end();
		virtual void swapBuffers();
		virtual void setFrameSize(float width, float height);
		RT_REMOVE(  virtual void setEditorFrameSize(float width, float height,HWND hWnd);   )
		virtual void setIMEKeyboardState(bool bOpen);
		void updateWindow(int width, int height);
		void pollEvents(void);

		void setMenuResource(const wchar_t* menu);
		void setWndProc(CUSTOM_WND_PROC proc);

	protected:
		RT_REMOVE(  virtual bool Create();  )
		void setupWindow(cocos2d::CCRect rect);
		RT_ADD(bool initGlew();)

	public:
		RT_REMOVE(bool initGL();)
		RT_REMOVE(void destroyGL();)

		RT_REMOVE(  virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam); )

		void setHWnd(void* hWnd);
		// win32 platform function
		RT_REMOVE( void* getHWnd(); )
		RT_REMOVE(  virtual void resize(int width, int height); )
		RT_ADD(     void resizeWindow(int width, int height);   )

		void setFrameZoomFactor(float fZoomFactor);
		float getFrameZoomFactor();
		RT_REMOVE(  virtual void centerWindow();    )
		RT_ADD(     void centerWindow();            )
		RT_ADD(     bool windowShouldClose();       )

		RT_ADD(     void showCursor(bool state);    )
			
		typedef void (*LPFN_ACCELEROMETER_KEYHOOK)( unsigned int message, unsigned long long wParam, long long lParam );
		void setAccelerometerKeyHook( LPFN_ACCELEROMETER_KEYHOOK lpfnAccelerometerKeyHook );

		virtual void setViewPortInPoints(float x , float y , float w , float h);
		virtual void setScissorInPoints(float x , float y , float w , float h);

		static CCEGLView* sharedOpenGLView();

		static CCEGLView* create(const std::string&);

		static cocos2d::CCEGLView* createWithFullScreen(std::string const&, bool);
		static cocos2d::CCEGLView* createWithFullScreen(std::string const&, bool, GLFWvidmode const&, void*);
		static cocos2d::CCEGLView* createWithRect(std::string const&, cocos2d::CCRect, float);

		void toggleFullScreen(bool fullscreen, bool borderless);

		void* getWindow(void) const;

		CCSize getDisplaySize();

		void capture();
		void checkErrorGL(char const*);

		void enableRetina(bool);

		bool getCursorLocked() const;
		bool getGameplayActive() const;
		bool getIsBorderless() const;
		bool getIsFullscreen() const;
		int getRetinaFactor() const;
		bool getShouldHideCursor() const;
		void iconify();

		bool initWithFullScreen(std::string const&, bool);
		bool initWithFullscreen(std::string const&, bool, GLFWvidmode const&, void*);
		bool initWithRect(std::string const&, cocos2d::CCRect, float);

		bool isRetinaEnabled() const;

		void onGLFWWindowCloseFunCallback(void*);
		void releaseCapture();
		void showMessage(std::string);

		void toggleGameplayActive(bool);
		void toggleLockCursor(bool);
		void updateDesignSize(int, int);
		void updateFrameSize();

	protected:
		static CCEGLView* s_pEglView;
		bool m_bCaptured;
		RT_REMOVE(
			HWND m_hWnd;
			HDC  m_hDC;
			HGLRC m_hRC;
			LPFN_ACCELEROMETER_KEYHOOK m_lpfnAccelerometerKeyHook;
		)
		bool m_bSupportTouch;
		RT_ADD(
			bool m_bInRetinaMonitor;
			bool m_bRetinaEnabled;
			int m_nRetinaFactor;
			bool m_bCursorHidden;
		)
		RT_REMOVE(
			LPCWSTR m_menu;
			CUSTOM_WND_PROC m_wndproc;
		)
		float m_fFrameZoomFactor;
		RT_ADD(
			void* m_pMainWindow;
			void* m_pPrimaryMonitor;
		)
	public:
		RT_ADD(
			CC_SYNTHESIZE_NV(CCSize, m_obWindowedSize, WindowedSize);
		)

		RT_ADD(
			float m_fMouseX;
			float m_fMouseY;
			bool m_bIsFullscreen;
			bool m_bIsBorderless;
			bool m_bShouldHideCursor;
			bool m_bCursorLocked;
			bool m_bShouldCallGLFinish;
		)

	protected:
		RT_ADD(
			void onGLFWCharCallback(void* window, unsigned int entered);
			void onGLFWCursorEnterFunCallback(void* window, int entered);
			void onGLFWDeviceChangeFunCallback(void* window);
			void onGLFWError(int code, const char* description);
			void onGLFWframebuffersize(void* window, int width, int height);
			void onGLFWMouseMoveCallBack(void* window, double x, double y);
			void onGLFWMouseCallBack(void* window, int button, int action, int mods);
			void onGLFWKeyCallback(void* window, int key, int scancode, int action, int mods);
			void onGLFWMouseScrollCallback(void* window, double xoffset, double yoffset);
			void onGLFWWindowIconifyFunCallback(void* window, int iconified);
			void onGLFWWindowPosCallback(void* window, int x, int y);
			void onGLFWWindowSizeFunCallback(void* window, int width, int height);
		)
	};

	class CCDirectorDelegate
	{
	public:
		virtual void updateProjection(void) = 0;
	};

	class CCKeypadDispatcher : public CCObject
	{
	public:
		CCKeypadDispatcher();
		~CCKeypadDispatcher();

		void addDelegate(CCKeypadDelegate* pDelegate);
		void removeDelegate(CCKeypadDelegate* pDelegate);

		void forceAddDelegate(CCKeypadDelegate* pDelegate);
		void forceRemoveDelegate(CCKeypadDelegate* pDelegate);

		bool dispatchKeypadMSG(ccKeypadMSGType nMsgType);

	protected:
		CCArray* m_pDelegates;
		bool m_bLocked;
		bool m_bToAdd;
		bool m_bToRemove;

		struct _ccCArray *m_pHandlersToAdd;
		struct _ccCArray *m_pHandlersToRemove;
	};

	RT_ADD(
	class CCKeyboardDispatcher : public CCObject 
	{
	public:
		CCKeyboardDispatcher();
		virtual ~CCKeyboardDispatcher();

		void addDelegate(CCKeyboardDelegate* pDelegate);
		void removeDelegate(CCKeyboardDelegate* pDelegate);

		void forceAddDelegate(CCKeyboardDelegate* pDelegate);
		void forceRemoveDelegate(CCKeyboardDelegate* pDelegate);

		static enumKeyCodes convertKeyCode(enumKeyCodes key);

		bool dispatchKeyboardMSG(enumKeyCodes key, bool isKeyDown, bool isKeyRepeat);

		inline bool getAltKeyPressed() const {
			return m_bAltPressed;
		}
		inline bool getCommandKeyPressed() const {
			return m_bCommandPressed;
		}
		inline bool getControlKeyPressed() const {
			return m_bControlPressed;
		}
		inline bool getShiftKeyPressed() const {
			return m_bShiftPressed;
		}

		const char* keyToString(enumKeyCodes key);

		void updateModifierKeys(bool shft, bool ctrl, bool alt, bool cmd);

		inline bool getBlockRepeat() const {
			return m_bBlockRepeat;
		}

		inline void setBlockRepeat(bool blockRepeat) {
			this->m_bBlockRepeat = blockRepeat;
		}

	protected:
		CCArray* m_pDelegates;	// 0x34
		bool m_bUnknown38;			// 0x38
		bool m_bUnknown39;      // 0x39
		bool m_bUnknown3a;			// 0x3a
		ccCArray* m_pUnknown3c; // 0x3c
		ccCArray* m_pUnknown40;	// 0x40
		bool m_bShiftPressed;		// 0x44
		bool m_bControlPressed;	// 0x45
		bool m_bAltPressed;			// 0x46
		bool m_bCommandPressed;	// 0x47
		bool m_bBlockRepeat;		// 0x48

		// ~~there's more here, check the initializer~~ no there's not??
	};

	class CCMouseDispatcher : public CCObject 
	{
	public:
		CCMouseDispatcher();
		virtual ~CCMouseDispatcher();

		void addDelegate(CCMouseDelegate* pDelegate);
		void removeDelegate(CCMouseDelegate* pDelegate);

		void forceAddDelegate(CCMouseDelegate* pDelegate);
		void forceRemoveDelegate(CCMouseDelegate* pDelegate);

		bool dispatchScrollMSG(float x, float y);

	protected:
		CCArray* m_pMouseHandlers;
		bool m_bLocked;
		bool m_bToAdd;
		bool m_bToRemove;
		ccCArray* m_pHandlersToAdd;
		ccCArray* m_pHandlersToRemove;
	};
)

	class CCAccelerometer
	{
	public:
		CCAccelerometer();
		~CCAccelerometer();

		void setDelegate(CCAccelerometerDelegate* pDelegate);
		void setAccelerometerInterval(float interval);
		void update( double x,double y,double z,double timestamp );
	private:
		CCAcceleration m_obAccelerationValue;
		CCAccelerometerDelegate* m_pAccelDelegate;
	};

	class CCTextureAtlas;
	class CCAtlasNode : public CCNodeRGBA, public CCTextureProtocol
	{
	protected:
		unsigned int m_uItemsPerRow;
		unsigned int m_uItemsPerColumn;

		unsigned int    m_uItemWidth;
		unsigned int    m_uItemHeight;

		ccColor3B    m_tColorUnmodified;

		CC_PROPERTY(CCTextureAtlas*, m_pTextureAtlas, TextureAtlas);

		bool m_bIsOpacityModifyRGB;

		CC_PROPERTY(ccBlendFunc, m_tBlendFunc, BlendFunc);

		CC_PROPERTY(unsigned int, m_uQuadsToDraw, QuadsToDraw);
		GLint    m_nUniformColor;
		bool m_bIgnoreContentScaleFactor;

	public:
		CCAtlasNode();
		virtual ~CCAtlasNode();

		static CCAtlasNode * create(const char* tile,unsigned int tileWidth, unsigned int tileHeight, 
			unsigned int itemsToRender);

		bool initWithTileFile(const char* tile, unsigned int tileWidth, unsigned int tileHeight, unsigned int itemsToRender);

		bool initWithTexture(CCTexture2D* texture, unsigned int tileWidth, unsigned int tileHeight, unsigned int itemsToRender);

		virtual void updateAtlasValues();

		virtual void draw(void);

		// CC Texture protocol
		virtual CCTexture2D* getTexture(void);

		virtual void setTexture(CCTexture2D *texture);

		virtual bool isOpacityModifyRGB();
		virtual void setOpacityModifyRGB(bool isOpacityModifyRGB);
		virtual const ccColor3B& getColor(void);
		virtual void setColor(const ccColor3B& color);
		virtual void setOpacity(GLubyte opacity);

	private :
		void calculateMaxItems();
		void updateBlendFunc();
		void updateOpacityModifyRGB();

		friend class CCDirector;
		void setIgnoreContentScaleFactor(bool bIgnoreContentScaleFactor);
	};

	class CCLabelAtlas : public CCAtlasNode, public CCLabelProtocol
	{
	public:
		CCLabelAtlas()
			: m_sString("")
		{}
		virtual ~CCLabelAtlas()
		{ 
			m_sString = ""; 
		}

		static CCLabelAtlas * create(const char *string, const char *charMapFile, unsigned int itemWidth, unsigned int itemHeight, unsigned int startCharMap);

		static CCLabelAtlas* create(const char *string, const char *fntFile);

		bool initWithString(const char *string, const char *charMapFile, unsigned int itemWidth, unsigned int itemHeight, unsigned int startCharMap);

		bool initWithString(const char *string, const char *fntFile);

		bool initWithString(const char* string, CCTexture2D* texture, unsigned int itemWidth, unsigned int itemHeight, unsigned int startCharMap);

		virtual void updateAtlasValues();
		virtual void setString(const char *label);
		virtual const char* getString(void);

	#if CC_LABELATLAS_DEBUG_DRAW
		virtual void draw();
	#endif

	protected:
		// string to render
		std::string m_sString;
		// the first char in the charmap
		unsigned int m_uMapStartChar;
	};

	class CCDirector : public CCObject, public TypeInfo
	{
	public:
		CCDirector(void);
		virtual ~CCDirector(void);
		virtual bool init(void);

		virtual long getClassTypeInfo() {
			// static const long id = cocos2d::getHashCodeByString(typeid(cocos2d::CCDirector).name());
			static const long id = 0;
			return id;
		}

		inline CCScene* getRunningScene(void) { return m_pRunningScene; }

		inline double getAnimationInterval(void) { return m_dAnimationInterval; }
		virtual void setAnimationInterval(double dValue) {}

		inline bool isDisplayStats(void) { return m_bDisplayStats; }
		inline void setDisplayStats(bool bDisplayStats) { m_bDisplayStats = bDisplayStats; }

		inline float getSecondsPerFrame() { return m_fSecondsPerFrame; }

		inline CCEGLView* getOpenGLView(void) { return m_pobOpenGLView; }
		void setOpenGLView(CCEGLView *pobOpenGLView);

		inline bool isNextDeltaTimeZero(void) { return m_bNextDeltaTimeZero; }
		void setNextDeltaTimeZero(bool bNextDeltaTimeZero);

		inline bool isPaused(void) { return m_bPaused; }

		inline unsigned int getTotalFrames(void) { return m_uTotalFrames; }

		inline ccDirectorProjection getProjection(void) { return m_eProjection; }
		void setProjection(ccDirectorProjection kProjection);
		void reshapeProjection(const CCSize& newWindowSize);

		void setViewport();

		inline bool isSendCleanupToScene(void) { return m_bSendCleanupToScene; }

		CCNode* getNotificationNode();
		void setNotificationNode(CCNode *node);

		CCDirectorDelegate* getDelegate() const;
		void setDelegate(CCDirectorDelegate* pDelegate);

		// window size
		CCSize getWinSize(void);
		CCSize getWinSizeInPixels(void);
		CCSize getVisibleSize();
		CCPoint getVisibleOrigin();

		CCPoint convertToGL(const CCPoint& obPoint);

		CCPoint convertToUI(const CCPoint& obPoint);

		float getZEye(void);

		// Scene Management

		void runWithScene(CCScene *pScene);

		RT_REMOVE(  void pushScene(CCScene *pScene);    )
		RT_ADD(     bool pushScene(CCScene *pScene);    )

		void popScene(void);
		void popToRootScene(void);
		void popToSceneStackLevel(int level);

		RT_REMOVE(  void replaceScene(CCScene *pScene); )
		RT_ADD(     bool replaceScene(CCScene *pScene); )

		void end(void);
		void pause(void);
		void resume(void);

	protected:
		virtual void stopAnimation(void) {}
		virtual void startAnimation(void) {}

	public:
		void drawScene(void);

		// Memory Helper

		void purgeCachedData(void);
		void setDefaultValues(void);

		// OpenGL Helper
		void setGLDefaultValues(void);
		void setAlphaBlending(bool bOn);
		void setDepthTest(bool bOn);

	protected:
		virtual void mainLoop(void) {}

	public:
		void setContentScaleFactor(float scaleFactor);
		inline float getContentScaleFactor(void) { return m_fContentScaleFactor; }

	RT_ADD(
	public:
		void checkSceneReference(void);

		CCScene* getNextScene(void);
		int levelForSceneInStack(CCScene*);
		bool popSceneWithTransition(float, PopTransition);
		void popToSceneInStack(CCScene*);
		int sceneCount(void);
		void willSwitchToScene(CCScene*);

		void removeStatsLabel(void);

		void resetSmoothFixCounter(void);
		void setDeltaTime(float);

		void setupScreenScale(CCSize, CCSize, TextureQuality);
		void updateContentScale(TextureQuality);
		void updateScreenScale(CCSize);

		void applySmoothFix();
		void showFPSLabel();
		void toggleShowFPS(bool, std::string, cocos2d::CCPoint);
	protected:
		void createStatsLabel();

	protected:
		CC_SYNTHESIZE_READONLY_NV(float, m_fScreenScaleFactor, ScreenScaleFactor);
		CC_SYNTHESIZE_READONLY_NV(float, m_fScreenScaleFactorMax, ScreenScaleFactorMax);
		CC_SYNTHESIZE_READONLY_NV(float, m_fScreenScaleFactorW, ScreenScaleFactorW);
		CC_SYNTHESIZE_READONLY_NV(float, m_fScreenScaleFactorH, ScreenScaleFactorH);
		CC_SYNTHESIZE_READONLY_NV(float, m_fScreenTop, ScreenTop);
		CC_SYNTHESIZE_READONLY_NV_NC(float, m_fScreenBottom, ScreenBottom);
		CC_SYNTHESIZE_READONLY_NV_NC(float, m_fScreenLeft, ScreenLeft);
		CC_SYNTHESIZE_READONLY_NV(float, m_fScreenRight, ScreenRight);
		CC_SYNTHESIZE_NV(CCScene*, m_pSceneReference, SceneReference);
	)

	public:
		CC_PROPERTY(CCScheduler*, m_pScheduler, Scheduler);
		CC_PROPERTY(CCActionManager*, m_pActionManager, ActionManager);
		CC_PROPERTY(CCTouchDispatcher*, m_pTouchDispatcher, TouchDispatcher);
		CC_PROPERTY(CCKeypadDispatcher*, m_pKeypadDispatcher, KeypadDispatcher);
		RT_ADD(
			CC_PROPERTY(CCKeyboardDispatcher*, m_pKeyboardDispatcher, KeyboardDispatcher);

			CC_PROPERTY(CCMouseDispatcher*, m_pMouseDispatcher, MouseDispatcher);
		)
		CC_PROPERTY(CCAccelerometer*, m_pAccelerometer, Accelerometer);
		CC_PROPERTY_READONLY(float, m_fDeltaTime, DeltaTime);

		RT_ADD(
			CC_SYNTHESIZE_NV(float, m_fActualDeltaTime, ActualDeltaTime); 
		)

	public:
		static CCDirector* sharedDirector(void);

	protected:
		void purgeDirector();
		RT_ADD(
			CC_SYNTHESIZE_READONLY_NV(bool, m_bIsTransitioning, IsTransitioning);   // if in a CCTransitionScene

			CC_SYNTHESIZE_NV(bool, m_bSmoothFix, SmoothFix);                        // if smooth fix is on
			CC_SYNTHESIZE_NV(bool, m_bSmoothFixCheck, SmoothFixCheck);              // not exactly sure what this is, but the name says something ig
			CC_SYNTHESIZE_NV(bool, m_bForceSmoothFix, ForceSmoothFix);              // if "force smooth fix" is on or not
			CC_SYNTHESIZE_READONLY_NV(int, m_nSmoothFixCounter, SmoothFixCounter);  // not sure about this one either
		)

		bool m_bPurgeDirecotorInNextLoop; // this flag will be set to true in end()

	protected:
		void setNextScene(void);

		void showStats();
		RT_REMOVE(void createStatsLabel();)
		void calculateMPF();
		void getFPSImageData(unsigned char** datapointer, unsigned int* length);

		void calculateDeltaTime();
	public:
		CCEGLView    *m_pobOpenGLView;

		double m_dAnimationInterval;
		double m_dOldAnimationInterval;

		bool m_bLandscape;
		
		bool m_bDisplayStats;

		float m_fFpsAccumDt;

		float m_fAccumDt;
		float m_fFrameRate;

		CCLabelAtlas *m_pFPSLabel;
		CCLabelAtlas *m_pSPFLabel;
		CCLabelAtlas *m_pDrawsLabel;

		bool m_bPaused;

		unsigned int m_uTotalFrames;
		unsigned int m_uFrames;
		float m_fSecondsPerFrame;

		CCScene *m_pRunningScene;

		CCScene *m_pNextScene;

		bool    m_bSendCleanupToScene;

		CCArray* m_pobScenesStack;

		struct cc_timeval *m_pLastUpdate;

		bool m_bNextDeltaTimeZero;

		ccDirectorProjection m_eProjection;

		CCSize m_obWinSizeInPoints;

		float    m_fContentScaleFactor;

		char *m_pszFPS;

		CCNode *m_pNotificationNode;

		CCDirectorDelegate *m_pProjectionDelegate;

		RT_ADD(
			CC_SYNTHESIZE(CCSceneDelegate*, m_pAppDelegate, SceneDelegate);
			bool m_bDisplayFPS;
			CCLabelBMFont* m_pFPSNode;
			CCSize m_obScaleFactor;
			CCSize m_obResolutionInPixels;
			CC_SYNTHESIZE_READONLY_NV(TextureQuality, m_eTextureQuality, LoadedTextureQuality);
			CC_SYNTHESIZE_NV(bool, m_bDontCallWillSwitch, DontCallWillSwitch);
			void* m_unknownPtr2;
			void* m_unknownPtr3;
		)

		friend class CCEGLViewProtocol;
	};


	// CCTextFieldTTF
	class CCLabelTTF : public CCSprite, public CCLabelProtocol
	{
	public:
		CCLabelTTF();
		virtual ~CCLabelTTF();
		const char* description();

		static CCLabelTTF * create(const char *string, const char *fontName, float fontSize);

		static CCLabelTTF * create(const char *string, const char *fontName, float fontSize,
								const CCSize& dimensions, CCTextAlignment hAlignment);

		static CCLabelTTF * create(const char *string, const char *fontName, float fontSize,
								const CCSize& dimensions, CCTextAlignment hAlignment, 
								CCVerticalTextAlignment vAlignment);

		static CCLabelTTF * createWithFontDefinition(const char *string, ccFontDefinition &textDefinition);
		
		bool initWithString(const char *string, const char *fontName, float fontSize);
		
		bool initWithString(const char *string, const char *fontName, float fontSize,
							const CCSize& dimensions, CCTextAlignment hAlignment);

		bool initWithString(const char *string, const char *fontName, float fontSize,
							const CCSize& dimensions, CCTextAlignment hAlignment, 
							CCVerticalTextAlignment vAlignment);
		
		bool initWithStringAndTextDefinition(const char *string, ccFontDefinition &textDefinition);
		
		void setTextDefinition(ccFontDefinition *theDefinition);
		
		ccFontDefinition * getTextDefinition();
		
		
		
		void enableShadow(const CCSize &shadowOffset, float shadowOpacity, float shadowBlur, bool mustUpdateTexture = true);
		
		void disableShadow(bool mustUpdateTexture = true);
		
		void enableStroke(const ccColor3B &strokeColor, float strokeSize, bool mustUpdateTexture = true);
		
		void disableStroke(bool mustUpdateTexture = true);
		
		void setFontFillColor(const ccColor3B &tintColor, bool mustUpdateTexture = true);


		bool init();

		static CCLabelTTF * create();

		virtual void setString(const char *label);
		virtual const char* getString(void);
		
		CCTextAlignment getHorizontalAlignment() {
			return m_hAlignment;
		}
		void setHorizontalAlignment(CCTextAlignment alignment) {
			if (alignment != m_hAlignment)
			{
				m_hAlignment = alignment;
				
				// Force update
				if (std::string(m_string).size() > 0)
					this->updateTexture();
			}
		}
		
		CCVerticalTextAlignment getVerticalAlignment() {
			return m_vAlignment;
		}
		void setVerticalAlignment(CCVerticalTextAlignment verticalAlignment) {
			if (verticalAlignment != m_vAlignment)
			{
				m_vAlignment = verticalAlignment;
				
				// Force update
				if (std::string(m_string).size() > 0)
					this->updateTexture();
			}
		}
		
		CCSize getDimensions() {
			return m_tDimensions;
		}
		void setDimensions(const CCSize &dim) {
			if (dim.width != m_tDimensions.width || dim.height != m_tDimensions.height)
			{
				m_tDimensions = dim;
				
				// Force update
				if (std::string(m_string).size() > 0)
					this->updateTexture();
			}
		}
		
		float getFontSize() {
			return m_fFontSize;
		}
		void setFontSize(float fontSize) {
			if (m_fFontSize != fontSize)
			{
				m_fFontSize = fontSize;
				
				// Force update
				if (std::string(m_string).size() > 0)
					this->updateTexture();
			}
		}
		
		const char* getFontName();
		void setFontName(const char *fontName);

	private:
		bool updateTexture();

	protected:
		void                _updateWithTextDefinition(ccFontDefinition & textDefinition, bool mustUpdateTexture = true);
		ccFontDefinition    _prepareTextDefinition(bool adjustForResolution = false);

		CCSize m_tDimensions;
		CCTextAlignment         m_hAlignment;
		CCVerticalTextAlignment m_vAlignment;
		std::string * m_pFontName;
		float m_fFontSize;
		std::string m_string;

		bool    m_shadowEnabled;
		CCSize  m_shadowOffset;
		float   m_shadowOpacity;
		float   m_shadowBlur;


		bool        m_strokeEnabled;
		ccColor3B   m_strokeColor;
		float       m_strokeSize;

		ccColor3B   m_textFillColor;
	};

	// is this really pimpl?
	class CCIMEDelegate;
	class CCIMEDispatcher
	{
	public:
		~CCIMEDispatcher();

		static CCIMEDispatcher* sharedDispatcher();

	//     /**
	//     @brief Releases all CCIMEDelegates from the shared dispatcher.
	//     */
	//     static void purgeSharedDispatcher();

		void dispatchInsertText(const char * pText, int nLen, cocos2d::enumKeyCodes);

		void dispatchDeleteBackward();

		void dispatchDeleteForward();

		bool hasDelegate();

		const char * getContentText();

		//////////////////////////////////////////////////////////////////////////
		// dispatch keyboard notification
		//////////////////////////////////////////////////////////////////////////
		void dispatchKeyboardWillShow(CCIMEKeyboardNotificationInfo& info);
		void dispatchKeyboardDidShow(CCIMEKeyboardNotificationInfo& info);
		void dispatchKeyboardWillHide(CCIMEKeyboardNotificationInfo& info);
		void dispatchKeyboardWillHide();
		void dispatchKeyboardDidHide(CCIMEKeyboardNotificationInfo& info);

	protected:
		friend class CCIMEDelegate;

		void addDelegate(CCIMEDelegate * pDelegate);

		bool attachDelegateWithIME(CCIMEDelegate * pDelegate);
		bool detachDelegateWithIME(CCIMEDelegate * pDelegate);

		void removeDelegate(CCIMEDelegate * pDelegate);

	private:
		CCIMEDispatcher();

		class Impl;
		Impl * m_pImpl;
	};

	class CCIMEDelegate
	{
	public:
		virtual ~CCIMEDelegate();

		virtual bool attachWithIME();
		virtual bool detachWithIME();

	protected:
		friend class CCIMEDispatcher;

		virtual bool canAttachWithIME() { return false; }
		virtual void didAttachWithIME() {}

		virtual bool canDetachWithIME() { return false; }

		virtual void didDetachWithIME() {}

		virtual void insertText(const char * text, int len, cocos2d::enumKeyCodes) {CC_UNUSED_PARAM(text);CC_UNUSED_PARAM(len);}

		virtual void deleteBackward() {}

		virtual const char * getContentText() { return 0; }

		//////////////////////////////////////////////////////////////////////////
		// keyboard show/hide notification
		//////////////////////////////////////////////////////////////////////////
		virtual void keyboardWillShow(CCIMEKeyboardNotificationInfo& info)   {CC_UNUSED_PARAM(info);}
		virtual void keyboardDidShow(CCIMEKeyboardNotificationInfo& info)    {CC_UNUSED_PARAM(info);}
		virtual void keyboardWillHide(CCIMEKeyboardNotificationInfo& info)   {CC_UNUSED_PARAM(info);}
		virtual void keyboardDidHide(CCIMEKeyboardNotificationInfo& info)    {CC_UNUSED_PARAM(info);}

		virtual void deleteForward();

	protected:
		CCIMEDelegate();
	};

	class CCTextFieldTTF;
	class CCTextFieldDelegate
	{
	public:
		virtual bool onTextFieldAttachWithIME(CCTextFieldTTF * sender)
		{
			CC_UNUSED_PARAM(sender);
			return false;
		}

		virtual bool onTextFieldDetachWithIME(CCTextFieldTTF * sender)
		{
			CC_UNUSED_PARAM(sender);
			return false;
		}

		virtual bool onTextFieldInsertText(CCTextFieldTTF * sender, const char * text, int nLen, cocos2d::enumKeyCodes)
		{
			CC_UNUSED_PARAM(sender);
			CC_UNUSED_PARAM(text);
			CC_UNUSED_PARAM(nLen);
			return false;
		}

		virtual bool onTextFieldDeleteBackward(CCTextFieldTTF * sender, const char * delText, int nLen)
		{
			CC_UNUSED_PARAM(sender);
			CC_UNUSED_PARAM(delText);
			CC_UNUSED_PARAM(nLen);
			return false;
		}

		virtual bool onDraw(CCTextFieldTTF * sender)
		{
			CC_UNUSED_PARAM(sender);
			return false;
		}

		RT_ADD(
			virtual void textChanged() {}
		)
	};

	class CCTextFieldTTF : public CCLabelTTF, public CCIMEDelegate
	{
	public:
		CCTextFieldTTF();
		virtual ~CCTextFieldTTF();

		//char * description();

		static CCTextFieldTTF * textFieldWithPlaceHolder(const char *placeholder, const CCSize& dimensions, CCTextAlignment alignment, const char *fontName, float fontSize);
		static CCTextFieldTTF * textFieldWithPlaceHolder(const char *placeholder, const char *fontName, float fontSize);
		bool initWithPlaceHolder(const char *placeholder, const CCSize& dimensions, CCTextAlignment alignment, const char *fontName, float fontSize);
		bool initWithPlaceHolder(const char *placeholder, const char *fontName, float fontSize);

		virtual bool attachWithIME();

		virtual bool detachWithIME();

		//////////////////////////////////////////////////////////////////////////
		// properties
		//////////////////////////////////////////////////////////////////////////

		CC_SYNTHESIZE_NV(CCTextFieldDelegate *, m_pDelegate, Delegate);
		CC_SYNTHESIZE_READONLY(int, m_nCharCount, CharCount);
		virtual const ccColor3B& getColorSpaceHolder();
		virtual void setColorSpaceHolder(const ccColor3B& color);

		// input text property
	public:
		virtual void setString(const char *text);
		virtual const char* getString(void);
	protected:
		std::string * m_pInputText;

		// place holder text property
		// place holder text displayed when there is no text in the text field.
	public:
		virtual void setPlaceHolder(const char * text);
		virtual const char * getPlaceHolder(void);
	protected:
		std::string * m_pPlaceHolder;
		ccColor3B m_ColorSpaceHolder;
	public:
		virtual void setSecureTextEntry(bool value);
		virtual bool isSecureTextEntry();
	protected:
		bool m_bSecureTextEntry;
	protected:

		virtual void draw();

		//////////////////////////////////////////////////////////////////////////
		// CCIMEDelegate interface
		//////////////////////////////////////////////////////////////////////////

		virtual bool canAttachWithIME();
		virtual bool canDetachWithIME();
		virtual void insertText(const char * text, int len, cocos2d::enumKeyCodes);
		virtual void deleteBackward();
		virtual const char * getContentText();
	private:
		class LengthStack;
		LengthStack * m_pLens;
	public:
		int m_uCursorPos;
	};


	// CCMotionStreak
	class CCMotionStreak : public CCNodeRGBA, public CCTextureProtocol
	{
	public:
		CCMotionStreak();
		virtual ~CCMotionStreak();

		static CCMotionStreak* create(
			float fade, float minSeg, float stroke, ccColor3B const& color, char const* path
		);
		static CCMotionStreak* create(
			float fade, float minSeg, float stroke, ccColor3B const& color, CCTexture2D* texture
		);

		bool initWithFade(
			float fade, float minSeg, float stroke, ccColor3B const& color, char const* path
		);
		bool initWithFade(
			float fade, float minSeg, float stroke, ccColor3B const& color, CCTexture2D* texture
		);

		void tintWithColor(ccColor3B colors);

		void reset();

		virtual void setPosition(CCPoint const& position);
		virtual void draw();
		virtual void update(float delta);

		virtual CCTexture2D* getTexture(void);
		virtual void setTexture(CCTexture2D* texture);
		virtual void setBlendFunc(ccBlendFunc blendFunc);
		virtual ccBlendFunc getBlendFunc(void);
		virtual GLubyte getOpacity(void);
		virtual void setOpacity(GLubyte opacity);
		virtual void setOpacityModifyRGB(bool bValue);
		virtual bool isOpacityModifyRGB(void);

		RT_ADD(
			void resumeStroke();
			void stopStroke();

			void enableRepeatMode(float);

			bool getDontOpacityFade() const;
			void setDontOpacityFade(bool);

			float getM_fMaxSeg() const;
			void setM_fMaxSeg(float);

			void setStroke(float);
			void updateFade(float);
		)

		inline bool isFastMode() {
			return m_bFastMode;
		}

		inline void setFastMode(bool bFastMode) {
			m_bFastMode = bFastMode;
		}

		inline bool isStartingPositionInitialized() {
			return m_bStartingPositionInitialized;
		}

		inline void setStartingPositionInitialized(bool bStartingPositionInitialized) {
			m_bStartingPositionInitialized = bStartingPositionInitialized;
		}

	protected:
		bool m_bFastMode;
		bool m_bStartingPositionInitialized;
		bool m_bStroke;
		CCTexture2D* m_pTexture;
		ccBlendFunc m_tBlendFunc;
		CCPoint m_tPositionR;

		float m_fStroke;
		float m_fFadeDelta;
		float m_fMinSeg;

		unsigned int m_uMaxPoints;
		unsigned int m_uNuPoints;
		unsigned int m_uPreviousNuPoints;

		CCPoint* m_pPointVertexes;
		float* m_pPointState;

		ccVertex2F* m_pVertices;
		GLubyte* m_pColorPointer;
		ccTex2F* m_pTexCoords;

		RT_ADD(
			bool m_bRepeatMode;
			float m_fRepeatSpeed;
			float m_fRepeatTime;
			bool m_idk;
			float m_fMaxSeg;
			bool m_bDontOpacityFade;
		)
	};


	// extension
	namespace extension
	{
		// http stuff
		class CCHttpClient;
		class CCHttpResponse;
		typedef void (CCObject::*SEL_HttpResponse)(CCHttpClient* client, CCHttpResponse* response);

		class CCHttpRequest : public CCObject
		{
		public:
			/** Use this enum type as param in setReqeustType(param) */
			typedef enum
			{
				kHttpGet,
				kHttpPost,
				kHttpPut,
				kHttpDelete,
				kHttpUnkown,
			} HttpRequestType;
			
			CCHttpRequest() : _url(), _requestData(), _tag()
			{
				_requestType = kHttpUnkown;
				_pTarget = NULL;
				_pSelector = NULL;
				_pUserData = NULL;
			};
			
			virtual ~CCHttpRequest()
			{
				if (_pTarget)
				{
					_pTarget->release();
				}
			};
			
			CCObject* autorelease(void)
			{
				CCAssert(false, "HttpResponse is used between network thread and ui thread \
						therefore, autorelease is forbidden here");
				return NULL;
			}
					
			// setter/getters for properties
			
			inline void setRequestType(HttpRequestType type)
			{
				_requestType = type;
			};
			inline HttpRequestType getRequestType()
			{
				return _requestType;
			};
			
			inline void setUrl(const char* url)
			{
				_url = url;
			};
			inline const char* getUrl()
			{
				return _url.c_str();
			};
			
			inline void setRequestData(const char* buffer, unsigned int len)
			{
				auto vec = std::vector<char>(_requestData);
				vec.assign(buffer, buffer + len);
				_requestData = std::vector<char>(vec);
			};
			inline char* getRequestData()
			{
				return &(_requestData.front());
			}
			inline int getRequestDataSize()
			{
				return std::vector<char>(_requestData).size();
			}
			
			inline void setTag(const char* tag)
			{
				_tag = tag;
			};
			inline const char* getTag()
			{
				return _tag.c_str();
			};
			
			inline void setUserData(void* pUserData)
			{
				_pUserData = pUserData;
			};
			inline void* getUserData()
			{
				return _pUserData;
			};
			
			CC_DEPRECATED_ATTRIBUTE inline void setResponseCallback(CCObject* pTarget, SEL_CallFuncND pSelector)
			{
				setResponseCallback(pTarget, (SEL_HttpResponse) pSelector);
			}

			inline void setResponseCallback(CCObject* pTarget, SEL_HttpResponse pSelector)
			{
				_pTarget = pTarget;
				_pSelector = pSelector;
				
				if (_pTarget)
				{
					_pTarget->retain();
				}
			}    
			inline CCObject* getTarget()
			{
				return _pTarget;
			}

			class _prxy
			{
			public:
				_prxy( SEL_HttpResponse cb ) :_cb(cb) {}
				~_prxy(){};
				operator SEL_HttpResponse() const { return _cb; }
				CC_DEPRECATED_ATTRIBUTE operator SEL_CallFuncND()   const { return (SEL_CallFuncND) _cb; }
			protected:
				SEL_HttpResponse _cb;
			};

			inline _prxy getSelector()
			{
				return _prxy(_pSelector);
			}

			inline void setHeaders(std::vector<std::string> pHeaders)
			{
				_headers=pHeaders;
			}

			inline std::vector<std::string> getHeaders()
			{
				return _headers;
			}


		protected:
			// properties
			HttpRequestType             _requestType;    /// kHttpRequestGet, kHttpRequestPost or other enums
			std::string                 _url;            /// target url that this request is sent to
			std::vector<char>           _requestData;    /// used for POST
			std::string                 _tag;            /// user defined tag, to identify different requests in response callback
			CCObject*          _pTarget;        /// callback target of pSelector function
			SEL_HttpResponse            _pSelector;      /// callback function, e.g. MyLayer::onHttpResponse(CCHttpClient *sender, CCHttpResponse * response)
			void*                       _pUserData;      /// You can add your customed data here 
			std::vector<std::string>    _headers;		      /// custom http headers

			RT_ADD(
				int _requestTypeGJ;
				bool _shouldCancel;
				int _downloadProgress;
				int _readTimeout;
			)
		};

		class CCHttpResponse : public CCObject
		{
		public:
			CCHttpResponse(CCHttpRequest* request) : _responseData(), _errorBuffer()
			{
				_pHttpRequest = request;
				if (_pHttpRequest)
				{
					_pHttpRequest->retain();
				}
				
				_succeed = false;
			}
			
			virtual ~CCHttpResponse()
			{
				if (_pHttpRequest)
				{
					_pHttpRequest->release();
				}
			}
			
			CCObject* autorelease(void)
			{
				CCAssert(false, "HttpResponse is used between network thread and ui thread \
								therefore, autorelease is forbidden here");
				return NULL;
			}
			
			// getters, will be called by users
			
			inline CCHttpRequest* getHttpRequest()
			{
				return _pHttpRequest;
			}
				
			inline bool isSucceed()
			{
				return _succeed;
			};
			
			inline std::vector<char>* getResponseData()
			{
				return &_responseData;
			}
			
			inline std::vector<char>* getResponseHeader()
			{
				return &_responseHeader;
			}

			inline int getResponseCode()
			{
				return _responseCode;
			}

			inline const char* getErrorBuffer()
			{
				return _errorBuffer.c_str();
			}
			
			// setters, will be called by CCHttpClient
			// users should avoid invoking these methods
			
			
			inline void setSucceed(bool value)
			{
				_succeed = value;
			};
			
			
			inline void setResponseData(std::vector<char>* data)
			{
				_responseData = *data;
			}
			
			inline void setResponseHeader(std::vector<char>* data)
			{
				_responseHeader = *data;
			}
			
			
			inline void setResponseCode(int value)
			{
				_responseCode = value;
			}
			
			
			inline void setErrorBuffer(const char* value)
			{
				_errorBuffer = std::string(value);
			};
			
		protected:
			bool initWithRequest(CCHttpRequest* request);

			// properties
			CCHttpRequest*        _pHttpRequest;  /// the corresponding HttpRequest pointer who leads to this response 
			bool                _succeed;       /// to indecate if the http reqeust is successful simply
			std::vector<char>   _responseData;  /// the returned raw data. You can also dump it as a string
			std::vector<char>   _responseHeader;  /// the returned raw header data. You can also dump it as a string
			int                 _responseCode;    /// the status code returned from libcurl, e.g. 200, 404
			std::string         _errorBuffer;   /// if _responseCode != 200, please read _errorBuffer to find the reason 
		};

		class CCHttpClient : public CCObject
		{
		public:
			static CCHttpClient *getInstance();
			
			static void destroyInstance();
				
			void send(CCHttpRequest* request);
		
			
			inline void setTimeoutForConnect(int value) {_timeoutForConnect = value;};
			
			inline int getTimeoutForConnect() {return _timeoutForConnect;}
			
			
			inline void setTimeoutForRead(int value) {_timeoutForRead = value;};
			

			inline int getTimeoutForRead() {return _timeoutForRead;};

		private:
			CCHttpClient();
			virtual ~CCHttpClient();
			bool init(void);
			
			bool lazyInitThreadSemphore();
			void dispatchResponseCallbacks(float delta);

		private:
			int _timeoutForConnect;
			int _timeoutForRead;

			// std::string reqId;
		};


		// misc
		class CCScale9Sprite : public CCNodeRGBA
		{
		public:
			CCScale9Sprite();
			virtual ~CCScale9Sprite();

		public:
			CC_SYNTHESIZE_READONLY_NV(CCSize, m_originalSize, OriginalSize);

			CC_PROPERTY(CCSize, m_preferredSize, PreferredSize); 
			CC_PROPERTY(CCRect, m_capInsets, CapInsets);
			CC_PROPERTY(float, m_insetLeft, InsetLeft);
			CC_PROPERTY(float, m_insetTop, InsetTop);
			CC_PROPERTY(float, m_insetRight, InsetRight);
			CC_PROPERTY(float, m_insetBottom, InsetBottom);

		protected:
			bool m_bSpritesGenerated;
			CCRect m_spriteRect;
			bool   m_bSpriteFrameRotated;
			CCRect m_capInsetsInternal;
			bool m_positionsAreDirty;
			
			CCSpriteBatchNode* _scale9Image;
			CCSprite* _topLeft;
			CCSprite* _top;
			CCSprite* _topRight;
			CCSprite* _left;
			CCSprite* _centre;
			CCSprite* _right;
			CCSprite* _bottomLeft;
			CCSprite* _bottom;
			CCSprite* _bottomRight;

			bool _opacityModifyRGB;
			GLubyte _opacity;
			ccColor3B _color;
			
			void updateCapInset();
			void updatePositions();

		public:
			virtual void setContentSize(const CCSize & size);
			virtual void visit();
			
			virtual bool init();

			virtual bool initWithBatchNode(CCSpriteBatchNode* batchnode, CCRect rect, bool rotated, CCRect capInsets);
			virtual bool initWithBatchNode(CCSpriteBatchNode* batchnode, CCRect rect, CCRect capInsets);
			virtual bool initWithFile(const char* file, CCRect rect,  CCRect capInsets);
			
			static CCScale9Sprite* create(const char* file, CCRect rect,  CCRect capInsets);

			virtual bool initWithFile(const char* file, CCRect rect);
			
			static CCScale9Sprite* create(const char* file, CCRect rect);

			virtual bool initWithFile(CCRect capInsets, const char* file);

			static CCScale9Sprite* create(CCRect capInsets, const char* file);

			virtual bool initWithFile(const char* file);

			static CCScale9Sprite* create(const char* file);
			
			virtual bool initWithSpriteFrame(CCSpriteFrame* spriteFrame, CCRect capInsets);

			static CCScale9Sprite* createWithSpriteFrame(CCSpriteFrame* spriteFrame, CCRect capInsets); 
			virtual bool initWithSpriteFrame(CCSpriteFrame* spriteFrame);

			static CCScale9Sprite* createWithSpriteFrame(CCSpriteFrame* spriteFrame);  

			virtual bool initWithSpriteFrameName(const char*spriteFrameName, CCRect capInsets);

			static CCScale9Sprite* createWithSpriteFrameName(const char*spriteFrameName, CCRect capInsets); 

			virtual bool initWithSpriteFrameName(const char*spriteFrameName);

			static CCScale9Sprite* createWithSpriteFrameName(const char*spriteFrameName);

			CCScale9Sprite* resizableSpriteWithCapInsets(CCRect capInsets);

			static CCScale9Sprite* create();

			// optional

			virtual void setOpacityModifyRGB(bool bValue);

			virtual bool isOpacityModifyRGB(void);
			virtual void setOpacity(GLubyte opacity);
			virtual GLubyte getOpacity();
			virtual void setColor(const ccColor3B& color);
			virtual const ccColor3B& getColor();

			virtual bool updateWithBatchNode(CCSpriteBatchNode* batchnode, CCRect rect, bool rotated, CCRect capInsets);

			virtual void setSpriteFrame(CCSpriteFrame * spriteFrame);
			
			virtual void updateDisplayedOpacity(GLubyte parentOpacity);
			virtual void updateDisplayedColor(const cocos2d::ccColor3B& parentColor);
		};


		class ColorPickerDelegate
		{
		public:
			virtual void colorValueChanged(ccColor3B) {}
		};
	}
}
