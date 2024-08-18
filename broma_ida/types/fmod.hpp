/* ======================================================================================== */
/* FMOD Core API - C++ header file.                                                         */
/* Copyright (c), Firelight Technologies Pty, Ltd. 2004-2020.                               */
/*                                                                                          */
/* Use this header in conjunction with fmod_common.h (which contains all the constants /    */
/* callbacks) to develop using the C++ language.                                            */
/*                                                                                          */
/* For more detail visit:                                                                   */
/* https://fmod.com/resources/documentation-api?version=2.0&page=core-api.html              */
/*                                                                                          */
/* Modified by SpaghettDev                                                                  */
/* ======================================================================================== */

// TODO: add the other structs, instead of just keeping them as a typedef

#define F_API __stdcall
#define F_CALL F_API
#define F_CALLBACK F_CALL

// enums
typedef enum FMOD_RESULT
{
	FMOD_OK,
	FMOD_ERR_BADCOMMAND,
	FMOD_ERR_CHANNEL_ALLOC,
	FMOD_ERR_CHANNEL_STOLEN,
	FMOD_ERR_DMA,
	FMOD_ERR_DSP_CONNECTION,
	FMOD_ERR_DSP_DONTPROCESS,
	FMOD_ERR_DSP_FORMAT,
	FMOD_ERR_DSP_INUSE,
	FMOD_ERR_DSP_NOTFOUND,
	FMOD_ERR_DSP_RESERVED,
	FMOD_ERR_DSP_SILENCE,
	FMOD_ERR_DSP_TYPE,
	FMOD_ERR_FILE_BAD,
	FMOD_ERR_FILE_COULDNOTSEEK,
	FMOD_ERR_FILE_DISKEJECTED,
	FMOD_ERR_FILE_EOF,
	FMOD_ERR_FILE_ENDOFDATA,
	FMOD_ERR_FILE_NOTFOUND,
	FMOD_ERR_FORMAT,
	FMOD_ERR_HEADER_MISMATCH,
	FMOD_ERR_HTTP,
	FMOD_ERR_HTTP_ACCESS,
	FMOD_ERR_HTTP_PROXY_AUTH,
	FMOD_ERR_HTTP_SERVER_ERROR,
	FMOD_ERR_HTTP_TIMEOUT,
	FMOD_ERR_INITIALIZATION,
	FMOD_ERR_INITIALIZED,
	FMOD_ERR_INTERNAL,
	FMOD_ERR_INVALID_FLOAT,
	FMOD_ERR_INVALID_HANDLE,
	FMOD_ERR_INVALID_PARAM,
	FMOD_ERR_INVALID_POSITION,
	FMOD_ERR_INVALID_SPEAKER,
	FMOD_ERR_INVALID_SYNCPOINT,
	FMOD_ERR_INVALID_THREAD,
	FMOD_ERR_INVALID_VECTOR,
	FMOD_ERR_MAXAUDIBLE,
	FMOD_ERR_MEMORY,
	FMOD_ERR_MEMORY_CANTPOINT,
	FMOD_ERR_NEEDS3D,
	FMOD_ERR_NEEDSHARDWARE,
	FMOD_ERR_NET_CONNECT,
	FMOD_ERR_NET_SOCKET_ERROR,
	FMOD_ERR_NET_URL,
	FMOD_ERR_NET_WOULD_BLOCK,
	FMOD_ERR_NOTREADY,
	FMOD_ERR_OUTPUT_ALLOCATED,
	FMOD_ERR_OUTPUT_CREATEBUFFER,
	FMOD_ERR_OUTPUT_DRIVERCALL,
	FMOD_ERR_OUTPUT_FORMAT,
	FMOD_ERR_OUTPUT_INIT,
	FMOD_ERR_OUTPUT_NODRIVERS,
	FMOD_ERR_PLUGIN,
	FMOD_ERR_PLUGIN_MISSING,
	FMOD_ERR_PLUGIN_RESOURCE,
	FMOD_ERR_PLUGIN_VERSION,
	FMOD_ERR_RECORD,
	FMOD_ERR_REVERB_CHANNELGROUP,
	FMOD_ERR_REVERB_INSTANCE,
	FMOD_ERR_SUBSOUNDS,
	FMOD_ERR_SUBSOUND_ALLOCATED,
	FMOD_ERR_SUBSOUND_CANTMOVE,
	FMOD_ERR_TAGNOTFOUND,
	FMOD_ERR_TOOMANYCHANNELS,
	FMOD_ERR_TRUNCATED,
	FMOD_ERR_UNIMPLEMENTED,
	FMOD_ERR_UNINITIALIZED,
	FMOD_ERR_UNSUPPORTED,
	FMOD_ERR_VERSION,
	FMOD_ERR_EVENT_ALREADY_LOADED,
	FMOD_ERR_EVENT_LIVEUPDATE_BUSY,
	FMOD_ERR_EVENT_LIVEUPDATE_MISMATCH,
	FMOD_ERR_EVENT_LIVEUPDATE_TIMEOUT,
	FMOD_ERR_EVENT_NOTFOUND,
	FMOD_ERR_STUDIO_UNINITIALIZED,
	FMOD_ERR_STUDIO_NOT_LOADED,
	FMOD_ERR_INVALID_STRING,
	FMOD_ERR_ALREADY_LOCKED,
	FMOD_ERR_NOT_LOCKED,
	FMOD_ERR_RECORD_DISCONNECTED,
	FMOD_ERR_TOOMANYSAMPLES,

	FMOD_RESULT_FORCEINT = 65536
} FMOD_RESULT;

typedef enum FMOD_OUTPUTTYPE
{
	FMOD_OUTPUTTYPE_AUTODETECT,
	FMOD_OUTPUTTYPE_UNKNOWN,
	FMOD_OUTPUTTYPE_NOSOUND,
	FMOD_OUTPUTTYPE_WAVWRITER,
	FMOD_OUTPUTTYPE_NOSOUND_NRT,
	FMOD_OUTPUTTYPE_WAVWRITER_NRT,
	FMOD_OUTPUTTYPE_WASAPI,
	FMOD_OUTPUTTYPE_ASIO,
	FMOD_OUTPUTTYPE_PULSEAUDIO,
	FMOD_OUTPUTTYPE_ALSA,
	FMOD_OUTPUTTYPE_COREAUDIO,
	FMOD_OUTPUTTYPE_AUDIOTRACK,
	FMOD_OUTPUTTYPE_OPENSL,
	FMOD_OUTPUTTYPE_AUDIOOUT,
	FMOD_OUTPUTTYPE_AUDIO3D,
	FMOD_OUTPUTTYPE_WEBAUDIO,
	FMOD_OUTPUTTYPE_NNAUDIO,
	FMOD_OUTPUTTYPE_WINSONIC,
	FMOD_OUTPUTTYPE_AAUDIO,

	FMOD_OUTPUTTYPE_MAX,
	FMOD_OUTPUTTYPE_FORCEINT = 65536
} FMOD_OUTPUTTYPE;

typedef enum FMOD_SPEAKERMODE
{
	FMOD_SPEAKERMODE_DEFAULT,
	FMOD_SPEAKERMODE_RAW,
	FMOD_SPEAKERMODE_MONO,
	FMOD_SPEAKERMODE_STEREO,
	FMOD_SPEAKERMODE_QUAD,
	FMOD_SPEAKERMODE_SURROUND,
	FMOD_SPEAKERMODE_5POINT1,
	FMOD_SPEAKERMODE_7POINT1,
	FMOD_SPEAKERMODE_7POINT1POINT4,

	FMOD_SPEAKERMODE_MAX,
	FMOD_SPEAKERMODE_FORCEINT = 65536
} FMOD_SPEAKERMODE;

typedef enum FMOD_CHANNELCONTROL_TYPE
{
	FMOD_CHANNELCONTROL_CHANNEL,
	FMOD_CHANNELCONTROL_CHANNELGROUP,

	FMOD_CHANNELCONTROL_MAX,
	FMOD_CHANNELCONTROL_FORCEINT = 65536
} FMOD_CHANNELCONTROL_TYPE;

typedef enum FMOD_CHANNELCONTROL_CALLBACK_TYPE
{
	FMOD_CHANNELCONTROL_CALLBACK_END,
	FMOD_CHANNELCONTROL_CALLBACK_VIRTUALVOICE,
	FMOD_CHANNELCONTROL_CALLBACK_SYNCPOINT,
	FMOD_CHANNELCONTROL_CALLBACK_OCCLUSION,

	FMOD_CHANNELCONTROL_CALLBACK_MAX,
	FMOD_CHANNELCONTROL_CALLBACK_FORCEINT = 65536
} FMOD_CHANNELCONTROL_CALLBACK_TYPE;

typedef enum FMOD_SPEAKER
{
	FMOD_SPEAKER_NONE = -1,
	FMOD_SPEAKER_FRONT_LEFT = 0,
	FMOD_SPEAKER_FRONT_RIGHT,
	FMOD_SPEAKER_FRONT_CENTER,
	FMOD_SPEAKER_LOW_FREQUENCY,
	FMOD_SPEAKER_SURROUND_LEFT,
	FMOD_SPEAKER_SURROUND_RIGHT,
	FMOD_SPEAKER_BACK_LEFT,
	FMOD_SPEAKER_BACK_RIGHT,
	FMOD_SPEAKER_TOP_FRONT_LEFT,
	FMOD_SPEAKER_TOP_FRONT_RIGHT,
	FMOD_SPEAKER_TOP_BACK_LEFT,
	FMOD_SPEAKER_TOP_BACK_RIGHT,

	FMOD_SPEAKER_MAX,
	FMOD_SPEAKER_FORCEINT = 65536
} FMOD_SPEAKER;

typedef enum FMOD_DSP_RESAMPLER
{
	FMOD_DSP_RESAMPLER_DEFAULT,
	FMOD_DSP_RESAMPLER_NOINTERP,
	FMOD_DSP_RESAMPLER_LINEAR,
	FMOD_DSP_RESAMPLER_CUBIC,
	FMOD_DSP_RESAMPLER_SPLINE,

	FMOD_DSP_RESAMPLER_MAX,
	FMOD_DSP_RESAMPLER_FORCEINT = 65536
} FMOD_DSP_RESAMPLER;

typedef enum FMOD_PLUGINTYPE
{
	FMOD_PLUGINTYPE_OUTPUT,
	FMOD_PLUGINTYPE_CODEC,
	FMOD_PLUGINTYPE_DSP,

	FMOD_PLUGINTYPE_MAX,
	FMOD_PLUGINTYPE_FORCEINT = 65536
} FMOD_PLUGINTYPE;

typedef enum FMOD_DSPCONNECTION_TYPE
{
	FMOD_DSPCONNECTION_TYPE_STANDARD,
	FMOD_DSPCONNECTION_TYPE_SIDECHAIN,
	FMOD_DSPCONNECTION_TYPE_SEND,
	FMOD_DSPCONNECTION_TYPE_SEND_SIDECHAIN,

	FMOD_DSPCONNECTION_TYPE_MAX,
	FMOD_DSPCONNECTION_TYPE_FORCEINT = 65536
} FMOD_DSPCONNECTION_TYPE;

typedef enum
{
	FMOD_DSP_PARAMETER_TYPE_FLOAT,
	FMOD_DSP_PARAMETER_TYPE_INT,
	FMOD_DSP_PARAMETER_TYPE_BOOL,
	FMOD_DSP_PARAMETER_TYPE_DATA,

	FMOD_DSP_PARAMETER_TYPE_MAX,
	FMOD_DSP_PARAMETER_TYPE_FORCEINT = 65536
} FMOD_DSP_PARAMETER_TYPE;

typedef enum
{
	FMOD_DSP_PARAMETER_FLOAT_MAPPING_TYPE_LINEAR,
	FMOD_DSP_PARAMETER_FLOAT_MAPPING_TYPE_AUTO,
	FMOD_DSP_PARAMETER_FLOAT_MAPPING_TYPE_PIECEWISE_LINEAR,

	FMOD_DSP_PARAMETER_FLOAT_MAPPING_TYPE_FORCEINT = 65536
} FMOD_DSP_PARAMETER_FLOAT_MAPPING_TYPE;

typedef enum
{
	FMOD_DSP_TYPE_UNKNOWN,
	FMOD_DSP_TYPE_MIXER,
	FMOD_DSP_TYPE_OSCILLATOR,
	FMOD_DSP_TYPE_LOWPASS,
	FMOD_DSP_TYPE_ITLOWPASS,
	FMOD_DSP_TYPE_HIGHPASS,
	FMOD_DSP_TYPE_ECHO,
	FMOD_DSP_TYPE_FADER,
	FMOD_DSP_TYPE_FLANGE,
	FMOD_DSP_TYPE_DISTORTION,
	FMOD_DSP_TYPE_NORMALIZE,
	FMOD_DSP_TYPE_LIMITER,
	FMOD_DSP_TYPE_PARAMEQ,
	FMOD_DSP_TYPE_PITCHSHIFT,
	FMOD_DSP_TYPE_CHORUS,
	FMOD_DSP_TYPE_VSTPLUGIN,
	FMOD_DSP_TYPE_WINAMPPLUGIN,
	FMOD_DSP_TYPE_ITECHO,
	FMOD_DSP_TYPE_COMPRESSOR,
	FMOD_DSP_TYPE_SFXREVERB,
	FMOD_DSP_TYPE_LOWPASS_SIMPLE,
	FMOD_DSP_TYPE_DELAY,
	FMOD_DSP_TYPE_TREMOLO,
	FMOD_DSP_TYPE_LADSPAPLUGIN,
	FMOD_DSP_TYPE_SEND,
	FMOD_DSP_TYPE_RETURN,
	FMOD_DSP_TYPE_HIGHPASS_SIMPLE,
	FMOD_DSP_TYPE_PAN,
	FMOD_DSP_TYPE_THREE_EQ,
	FMOD_DSP_TYPE_FFT,
	FMOD_DSP_TYPE_LOUDNESS_METER,
	FMOD_DSP_TYPE_ENVELOPEFOLLOWER,
	FMOD_DSP_TYPE_CONVOLUTIONREVERB,
	FMOD_DSP_TYPE_CHANNELMIX,
	FMOD_DSP_TYPE_TRANSCEIVER,
	FMOD_DSP_TYPE_OBJECTPAN,
	FMOD_DSP_TYPE_MULTIBAND_EQ,

	FMOD_DSP_TYPE_MAX,
	FMOD_DSP_TYPE_FORCEINT = 65536    /* Makes sure this enum is signed 32bit. */
} FMOD_DSP_TYPE;

typedef enum
{
	FMOD_DSP_PROCESS_PERFORM,
	FMOD_DSP_PROCESS_QUERY
} FMOD_DSP_PROCESS_OPERATION;

typedef enum FMOD_TAGTYPE
{
	FMOD_TAGTYPE_UNKNOWN,
	FMOD_TAGTYPE_ID3V1,
	FMOD_TAGTYPE_ID3V2,
	FMOD_TAGTYPE_VORBISCOMMENT,
	FMOD_TAGTYPE_SHOUTCAST,
	FMOD_TAGTYPE_ICECAST,
	FMOD_TAGTYPE_ASF,
	FMOD_TAGTYPE_MIDI,
	FMOD_TAGTYPE_PLAYLIST,
	FMOD_TAGTYPE_FMOD,
	FMOD_TAGTYPE_USER,

	FMOD_TAGTYPE_MAX,
	FMOD_TAGTYPE_FORCEINT = 65536
} FMOD_TAGTYPE;

typedef enum FMOD_TAGDATATYPE
{
	FMOD_TAGDATATYPE_BINARY,
	FMOD_TAGDATATYPE_INT,
	FMOD_TAGDATATYPE_FLOAT,
	FMOD_TAGDATATYPE_STRING,
	FMOD_TAGDATATYPE_STRING_UTF16,
	FMOD_TAGDATATYPE_STRING_UTF16BE,
	FMOD_TAGDATATYPE_STRING_UTF8,

	FMOD_TAGDATATYPE_MAX,
	FMOD_TAGDATATYPE_FORCEINT = 65536
} FMOD_TAGDATATYPE;

typedef enum FMOD_SOUND_FORMAT
{
	FMOD_SOUND_FORMAT_NONE,
	FMOD_SOUND_FORMAT_PCM8,
	FMOD_SOUND_FORMAT_PCM16,
	FMOD_SOUND_FORMAT_PCM24,
	FMOD_SOUND_FORMAT_PCM32,
	FMOD_SOUND_FORMAT_PCMFLOAT,
	FMOD_SOUND_FORMAT_BITSTREAM,

	FMOD_SOUND_FORMAT_MAX,
	FMOD_SOUND_FORMAT_FORCEINT = 65536
} FMOD_SOUND_FORMAT;

typedef enum FMOD_CHANNELORDER
{
	FMOD_CHANNELORDER_DEFAULT,
	FMOD_CHANNELORDER_WAVEFORMAT,
	FMOD_CHANNELORDER_PROTOOLS,
	FMOD_CHANNELORDER_ALLMONO,
	FMOD_CHANNELORDER_ALLSTEREO,
	FMOD_CHANNELORDER_ALSA,

	FMOD_CHANNELORDER_MAX,
	FMOD_CHANNELORDER_FORCEINT = 65536
} FMOD_CHANNELORDER;

typedef enum FMOD_SOUND_TYPE
{
	FMOD_SOUND_TYPE_UNKNOWN,
	FMOD_SOUND_TYPE_AIFF,
	FMOD_SOUND_TYPE_ASF,
	FMOD_SOUND_TYPE_DLS,
	FMOD_SOUND_TYPE_FLAC,
	FMOD_SOUND_TYPE_FSB,
	FMOD_SOUND_TYPE_IT,
	FMOD_SOUND_TYPE_MIDI,
	FMOD_SOUND_TYPE_MOD,
	FMOD_SOUND_TYPE_MPEG,
	FMOD_SOUND_TYPE_OGGVORBIS,
	FMOD_SOUND_TYPE_PLAYLIST,
	FMOD_SOUND_TYPE_RAW,
	FMOD_SOUND_TYPE_S3M,
	FMOD_SOUND_TYPE_USER,
	FMOD_SOUND_TYPE_WAV,
	FMOD_SOUND_TYPE_XM,
	FMOD_SOUND_TYPE_XMA,
	FMOD_SOUND_TYPE_AUDIOQUEUE,
	FMOD_SOUND_TYPE_AT9,
	FMOD_SOUND_TYPE_VORBIS,
	FMOD_SOUND_TYPE_MEDIA_FOUNDATION,
	FMOD_SOUND_TYPE_MEDIACODEC,
	FMOD_SOUND_TYPE_FADPCM,
	FMOD_SOUND_TYPE_OPUS,

	FMOD_SOUND_TYPE_MAX,
	FMOD_SOUND_TYPE_FORCEINT = 65536
} FMOD_SOUND_TYPE;

typedef enum FMOD_OPENSTATE
{
	FMOD_OPENSTATE_READY,
	FMOD_OPENSTATE_LOADING,
	FMOD_OPENSTATE_ERROR,
	FMOD_OPENSTATE_CONNECTING,
	FMOD_OPENSTATE_BUFFERING,
	FMOD_OPENSTATE_SEEKING,
	FMOD_OPENSTATE_PLAYING,
	FMOD_OPENSTATE_SETPOSITION,

	FMOD_OPENSTATE_MAX,
	FMOD_OPENSTATE_FORCEINT = 65536
} FMOD_OPENSTATE;

typedef enum FMOD_SOUNDGROUP_BEHAVIOR
{
	FMOD_SOUNDGROUP_BEHAVIOR_FAIL,
	FMOD_SOUNDGROUP_BEHAVIOR_MUTE,
	FMOD_SOUNDGROUP_BEHAVIOR_STEALLOWEST,

	FMOD_SOUNDGROUP_BEHAVIOR_MAX,
	FMOD_SOUNDGROUP_BEHAVIOR_FORCEINT = 65536
} FMOD_SOUNDGROUP_BEHAVIOR;


// typedefs
/*
	FMOD core types
*/
typedef int                        FMOD_BOOL;
typedef struct FMOD_SYSTEM         FMOD_SYSTEM;
typedef struct FMOD_SOUND          FMOD_SOUND;
typedef struct FMOD_CHANNELCONTROL FMOD_CHANNELCONTROL;
typedef struct FMOD_CHANNEL        FMOD_CHANNEL;
typedef struct FMOD_CHANNELGROUP   FMOD_CHANNELGROUP;
typedef struct FMOD_SOUNDGROUP     FMOD_SOUNDGROUP;
typedef struct FMOD_REVERB3D       FMOD_REVERB3D;
typedef struct FMOD_DSP            FMOD_DSP;
typedef struct FMOD_DSPCONNECTION  FMOD_DSPCONNECTION;
typedef struct FMOD_POLYGON        FMOD_POLYGON;
typedef struct FMOD_GEOMETRY       FMOD_GEOMETRY;
typedef struct FMOD_SYNCPOINT      FMOD_SYNCPOINT;
typedef struct FMOD_ASYNCREADINFO  FMOD_ASYNCREADINFO;
typedef unsigned int               FMOD_PORT_TYPE;
typedef unsigned long long         FMOD_PORT_INDEX;

typedef unsigned int FMOD_TIMEUNIT;
typedef unsigned int FMOD_DEBUG_FLAGS;
typedef unsigned int FMOD_CHANNELMASK;

typedef unsigned int FMOD_SYSTEM_CALLBACK_TYPE;
typedef unsigned int FMOD_MEMORY_TYPE;

typedef FMOD_RESULT (F_CALL *FMOD_DEBUG_CALLBACK)           (FMOD_DEBUG_FLAGS flags, const char *file, int line, const char* func, const char* message);
typedef FMOD_RESULT (F_CALL *FMOD_SYSTEM_CALLBACK)          (FMOD_SYSTEM *system, FMOD_SYSTEM_CALLBACK_TYPE type, void *commanddata1, void* commanddata2, void *userdata);
typedef FMOD_RESULT (F_CALL *FMOD_CHANNELCONTROL_CALLBACK)  (FMOD_CHANNELCONTROL *channelcontrol, FMOD_CHANNELCONTROL_TYPE controltype, FMOD_CHANNELCONTROL_CALLBACK_TYPE callbacktype, void *commanddata1, void *commanddata2);
typedef FMOD_RESULT (F_CALL *FMOD_SOUND_NONBLOCK_CALLBACK)  (FMOD_SOUND *sound, FMOD_RESULT result);
typedef FMOD_RESULT (F_CALL *FMOD_SOUND_PCMREAD_CALLBACK)   (FMOD_SOUND *sound, void *data, unsigned int datalen);
typedef FMOD_RESULT (F_CALL *FMOD_SOUND_PCMSETPOS_CALLBACK) (FMOD_SOUND *sound, int subsound, unsigned int position, FMOD_TIMEUNIT postype);
typedef FMOD_RESULT (F_CALL *FMOD_FILE_OPEN_CALLBACK)       (const char *name, unsigned int *filesize, void **handle, void *userdata);
typedef FMOD_RESULT (F_CALL *FMOD_FILE_CLOSE_CALLBACK)      (void *handle, void *userdata);
typedef FMOD_RESULT (F_CALL *FMOD_FILE_READ_CALLBACK)       (void *handle, void *buffer, unsigned int sizebytes, unsigned int *bytesread, void *userdata);
typedef FMOD_RESULT (F_CALL *FMOD_FILE_SEEK_CALLBACK)       (void *handle, unsigned int pos, void *userdata);
typedef FMOD_RESULT (F_CALL *FMOD_FILE_ASYNCREAD_CALLBACK)  (FMOD_ASYNCREADINFO *info, void *userdata);
typedef FMOD_RESULT (F_CALL *FMOD_FILE_ASYNCCANCEL_CALLBACK)(FMOD_ASYNCREADINFO *info, void *userdata);
typedef void        (F_CALL *FMOD_FILE_ASYNCDONE_FUNC)      (FMOD_ASYNCREADINFO *info, FMOD_RESULT result);
typedef void*       (F_CALL *FMOD_MEMORY_ALLOC_CALLBACK)    (unsigned int size, FMOD_MEMORY_TYPE type, const char *sourcestr);
typedef void*       (F_CALL *FMOD_MEMORY_REALLOC_CALLBACK)  (void *ptr, unsigned int size, FMOD_MEMORY_TYPE type, const char *sourcestr);
typedef void        (F_CALL *FMOD_MEMORY_FREE_CALLBACK)     (void *ptr, FMOD_MEMORY_TYPE type, const char *sourcestr);
typedef float       (F_CALL *FMOD_3D_ROLLOFF_CALLBACK)      (FMOD_CHANNELCONTROL *channelcontrol, float distance);

/*
	DSP Callbacks
*/
typedef struct FMOD_DSP_STATE        FMOD_DSP_STATE;
typedef struct FMOD_DSP_BUFFER_ARRAY FMOD_DSP_BUFFER_ARRAY;
typedef FMOD_RESULT (F_CALL *FMOD_DSP_CREATE_CALLBACK)                    (FMOD_DSP_STATE *dsp_state);
typedef FMOD_RESULT (F_CALL *FMOD_DSP_RELEASE_CALLBACK)                   (FMOD_DSP_STATE *dsp_state);
typedef FMOD_RESULT (F_CALL *FMOD_DSP_RESET_CALLBACK)                     (FMOD_DSP_STATE *dsp_state);
typedef FMOD_RESULT (F_CALL *FMOD_DSP_READ_CALLBACK)                      (FMOD_DSP_STATE *dsp_state, float *inbuffer, float *outbuffer, unsigned int length, int inchannels, int *outchannels);
typedef FMOD_RESULT (F_CALL *FMOD_DSP_PROCESS_CALLBACK)                   (FMOD_DSP_STATE *dsp_state, unsigned int length, const FMOD_DSP_BUFFER_ARRAY *inbufferarray, FMOD_DSP_BUFFER_ARRAY *outbufferarray, FMOD_BOOL inputsidle, FMOD_DSP_PROCESS_OPERATION op);
typedef FMOD_RESULT (F_CALL *FMOD_DSP_SETPOSITION_CALLBACK)               (FMOD_DSP_STATE *dsp_state, unsigned int pos);
typedef FMOD_RESULT (F_CALL *FMOD_DSP_SHOULDIPROCESS_CALLBACK)            (FMOD_DSP_STATE *dsp_state, FMOD_BOOL inputsidle, unsigned int length, FMOD_CHANNELMASK inmask, int inchannels, FMOD_SPEAKERMODE speakermode);
typedef FMOD_RESULT (F_CALL *FMOD_DSP_SETPARAM_FLOAT_CALLBACK)            (FMOD_DSP_STATE *dsp_state, int index, float value);
typedef FMOD_RESULT (F_CALL *FMOD_DSP_SETPARAM_INT_CALLBACK)              (FMOD_DSP_STATE *dsp_state, int index, int value);
typedef FMOD_RESULT (F_CALL *FMOD_DSP_SETPARAM_BOOL_CALLBACK)             (FMOD_DSP_STATE *dsp_state, int index, FMOD_BOOL value);
typedef FMOD_RESULT (F_CALL *FMOD_DSP_SETPARAM_DATA_CALLBACK)             (FMOD_DSP_STATE *dsp_state, int index, void *data, unsigned int length);
typedef FMOD_RESULT (F_CALL *FMOD_DSP_GETPARAM_FLOAT_CALLBACK)            (FMOD_DSP_STATE *dsp_state, int index, float *value, char *valuestr);
typedef FMOD_RESULT (F_CALL *FMOD_DSP_GETPARAM_INT_CALLBACK)              (FMOD_DSP_STATE *dsp_state, int index, int *value, char *valuestr);
typedef FMOD_RESULT (F_CALL *FMOD_DSP_GETPARAM_BOOL_CALLBACK)             (FMOD_DSP_STATE *dsp_state, int index, FMOD_BOOL *value, char *valuestr);
typedef FMOD_RESULT (F_CALL *FMOD_DSP_GETPARAM_DATA_CALLBACK)             (FMOD_DSP_STATE *dsp_state, int index, void **data, unsigned int *length, char *valuestr);
typedef FMOD_RESULT (F_CALL *FMOD_DSP_SYSTEM_REGISTER_CALLBACK)           (FMOD_DSP_STATE *dsp_state);
typedef FMOD_RESULT (F_CALL *FMOD_DSP_SYSTEM_DEREGISTER_CALLBACK)         (FMOD_DSP_STATE *dsp_state);
typedef FMOD_RESULT (F_CALL *FMOD_DSP_SYSTEM_MIX_CALLBACK)                (FMOD_DSP_STATE *dsp_state, int stage);

/*
	Codec callbacks
*/
struct FMOD_CODEC_STATE;
struct FMOD_CODEC_WAVEFORMAT;
struct FMOD_CREATESOUNDEXINFO;
typedef unsigned int FMOD_MODE;
typedef FMOD_RESULT (F_CALLBACK *FMOD_CODEC_OPEN_CALLBACK)         (FMOD_CODEC_STATE *codec_state, FMOD_MODE usermode, FMOD_CREATESOUNDEXINFO *userexinfo);
typedef FMOD_RESULT (F_CALLBACK *FMOD_CODEC_CLOSE_CALLBACK)        (FMOD_CODEC_STATE *codec_state);
typedef FMOD_RESULT (F_CALLBACK *FMOD_CODEC_READ_CALLBACK)         (FMOD_CODEC_STATE *codec_state, void *buffer, unsigned int samples_in, unsigned int *samples_out);
typedef FMOD_RESULT (F_CALLBACK *FMOD_CODEC_GETLENGTH_CALLBACK)    (FMOD_CODEC_STATE *codec_state, unsigned int *length, FMOD_TIMEUNIT lengthtype);
typedef FMOD_RESULT (F_CALLBACK *FMOD_CODEC_SETPOSITION_CALLBACK)  (FMOD_CODEC_STATE *codec_state, int subsound, unsigned int position, FMOD_TIMEUNIT postype);
typedef FMOD_RESULT (F_CALLBACK *FMOD_CODEC_GETPOSITION_CALLBACK)  (FMOD_CODEC_STATE *codec_state, unsigned int *position, FMOD_TIMEUNIT postype);
typedef FMOD_RESULT (F_CALLBACK *FMOD_CODEC_SOUNDCREATE_CALLBACK)  (FMOD_CODEC_STATE *codec_state, int subsound, FMOD_SOUND *sound);
typedef FMOD_RESULT (F_CALLBACK *FMOD_CODEC_METADATA_CALLBACK)     (FMOD_CODEC_STATE *codec_state, FMOD_TAGTYPE tagtype, char *name, void *data, unsigned int datalen, FMOD_TAGDATATYPE datatype, int unique);
typedef FMOD_RESULT (F_CALLBACK *FMOD_CODEC_GETWAVEFORMAT_CALLBACK)(FMOD_CODEC_STATE *codec_state, int index, FMOD_CODEC_WAVEFORMAT *waveformat);

struct FMOD_CODEC_STATE
{
	int                          numsubsounds;
	FMOD_CODEC_WAVEFORMAT       *waveformat;
	void                        *plugindata;

	void                        *filehandle;
	unsigned int                 filesize;
	FMOD_FILE_READ_CALLBACK      fileread;
	FMOD_FILE_SEEK_CALLBACK      fileseek;
	FMOD_CODEC_METADATA_CALLBACK metadata;

	int                          waveformatversion;
};

struct FMOD_CODEC_WAVEFORMAT
{
	const char*        name;
	FMOD_SOUND_FORMAT  format;
	int                channels;
	int                frequency;
	unsigned int       lengthbytes;
	unsigned int       lengthpcm;
	unsigned int       pcmblocksize;
	int                loopstart;
	int                loopend;
	FMOD_MODE          mode;
	FMOD_CHANNELMASK   channelmask;
	FMOD_CHANNELORDER  channelorder;
	float              peakvolume;
};

typedef struct FMOD_GUID
{
	unsigned int   Data1;
	unsigned short Data2;
	unsigned short Data3;
	unsigned char  Data4[8];
} FMOD_GUID;

typedef struct FMOD_CREATESOUNDEXINFO
{
	int                            cbsize;
	unsigned int                   length;
	unsigned int                   fileoffset;
	int                            numchannels;
	int                            defaultfrequency;
	FMOD_SOUND_FORMAT              format;
	unsigned int                   decodebuffersize;
	int                            initialsubsound;
	int                            numsubsounds;
	int                           *inclusionlist;
	int                            inclusionlistnum;
	FMOD_SOUND_PCMREAD_CALLBACK    pcmreadcallback;
	FMOD_SOUND_PCMSETPOS_CALLBACK  pcmsetposcallback;
	FMOD_SOUND_NONBLOCK_CALLBACK   nonblockcallback;
	const char                    *dlsname;
	const char                    *encryptionkey;
	int                            maxpolyphony;
	void                          *userdata;
	FMOD_SOUND_TYPE                suggestedsoundtype;
	FMOD_FILE_OPEN_CALLBACK        fileuseropen;
	FMOD_FILE_CLOSE_CALLBACK       fileuserclose;
	FMOD_FILE_READ_CALLBACK        fileuserread;
	FMOD_FILE_SEEK_CALLBACK        fileuserseek;
	FMOD_FILE_ASYNCREAD_CALLBACK   fileuserasyncread;
	FMOD_FILE_ASYNCCANCEL_CALLBACK fileuserasynccancel;
	void                          *fileuserdata;
	int                            filebuffersize;
	FMOD_CHANNELORDER              channelorder;
	FMOD_SOUNDGROUP               *initialsoundgroup;
	unsigned int                   initialseekposition;
	FMOD_TIMEUNIT                  initialseekpostype;
	int                            ignoresetfilesystem;
	unsigned int                   audioqueuepolicy;
	unsigned int                   minmidigranularity;
	int                            nonblockthreadid;
	FMOD_GUID                     *fsbguid;
} FMOD_CREATESOUNDEXINFO;

/*
	Output callbacks
*/
/*
	Output functions
*/
struct FMOD_OUTPUT_STATE;
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_READFROMMIXER_FUNC)        (FMOD_OUTPUT_STATE *output_state, void *buffer, unsigned int length);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_COPYPORT_FUNC)             (FMOD_OUTPUT_STATE *output_state, int portId, void *buffer, unsigned int length);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_REQUESTRESET_FUNC)         (FMOD_OUTPUT_STATE *output_state);
typedef void *      (F_CALL *FMOD_OUTPUT_ALLOC_FUNC)                (unsigned int size, unsigned int align, const char *file, int line);
typedef void        (F_CALL *FMOD_OUTPUT_FREE_FUNC)                 (void *ptr, const char *file, int line);
typedef void        (F_CALL *FMOD_OUTPUT_LOG_FUNC)                  (FMOD_DEBUG_FLAGS level, const char *file, int line, const char *function, const char *string);

struct FMOD_OUTPUT_STATE
{
	void                            *plugindata;
	FMOD_OUTPUT_READFROMMIXER_FUNC   readfrommixer;
	FMOD_OUTPUT_ALLOC_FUNC           alloc;
	FMOD_OUTPUT_FREE_FUNC            free;
	FMOD_OUTPUT_LOG_FUNC             log;
	FMOD_OUTPUT_COPYPORT_FUNC        copyport;
	FMOD_OUTPUT_REQUESTRESET_FUNC    requestreset;
};

struct FMOD_OUTPUT_OBJECT3DINFO;
typedef unsigned int FMOD_INITFLAGS;
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_GETNUMDRIVERS_CALLBACK)    (FMOD_OUTPUT_STATE *output_state, int *numdrivers);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_GETDRIVERINFO_CALLBACK)    (FMOD_OUTPUT_STATE *output_state, int id, char *name, int namelen, FMOD_GUID *guid, int *systemrate, FMOD_SPEAKERMODE *speakermode, int *speakermodechannels);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_INIT_CALLBACK)             (FMOD_OUTPUT_STATE *output_state, int selecteddriver, FMOD_INITFLAGS flags, int *outputrate, FMOD_SPEAKERMODE *speakermode, int *speakermodechannels, FMOD_SOUND_FORMAT *outputformat, int dspbufferlength, int dspnumbuffers, void *extradriverdata);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_START_CALLBACK)            (FMOD_OUTPUT_STATE *output_state);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_STOP_CALLBACK)             (FMOD_OUTPUT_STATE *output_state);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_CLOSE_CALLBACK)            (FMOD_OUTPUT_STATE *output_state);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_UPDATE_CALLBACK)           (FMOD_OUTPUT_STATE *output_state);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_GETHANDLE_CALLBACK)        (FMOD_OUTPUT_STATE *output_state, void **handle);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_GETPOSITION_CALLBACK)      (FMOD_OUTPUT_STATE *output_state, unsigned int *pcm);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_LOCK_CALLBACK)             (FMOD_OUTPUT_STATE *output_state, unsigned int offset, unsigned int length, void **ptr1, void **ptr2, unsigned int *len1, unsigned int *len2);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_UNLOCK_CALLBACK)           (FMOD_OUTPUT_STATE *output_state, void *ptr1, void *ptr2, unsigned int len1, unsigned int len2);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_MIXER_CALLBACK)            (FMOD_OUTPUT_STATE *output_state);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_OBJECT3DGETINFO_CALLBACK)  (FMOD_OUTPUT_STATE *output_state, int *maxhardwareobjects);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_OBJECT3DALLOC_CALLBACK)    (FMOD_OUTPUT_STATE *output_state, void **object3d);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_OBJECT3DFREE_CALLBACK)     (FMOD_OUTPUT_STATE *output_state, void *object3d);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_OBJECT3DUPDATE_CALLBACK)   (FMOD_OUTPUT_STATE *output_state, void *object3d, const FMOD_OUTPUT_OBJECT3DINFO *info);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_OPENPORT_CALLBACK)         (FMOD_OUTPUT_STATE *output_state, FMOD_PORT_TYPE portType, FMOD_PORT_INDEX portIndex, int *portId, int *portRate, int *portChannels, FMOD_SOUND_FORMAT *portFormat);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_CLOSEPORT_CALLBACK)        (FMOD_OUTPUT_STATE *output_state, int portId);
typedef FMOD_RESULT (F_CALL *FMOD_OUTPUT_DEVICELISTCHANGED_CALLBACK)(FMOD_OUTPUT_STATE *output_state);

typedef struct FMOD_VECTOR
{
	float x;
	float y;
	float z;
} FMOD_VECTOR;

struct FMOD_OUTPUT_OBJECT3DINFO
{
	float          *buffer;
	unsigned int    bufferlength;
	FMOD_VECTOR     position;
	float           gain;
	float           spread;
	float           priority;
};

typedef unsigned int FMOD_DRIVER_STATE;


// structs
typedef struct FMOD_ADVANCEDSETTINGS
{
	int                 cbSize;
	int                 maxMPEGCodecs;
	int                 maxADPCMCodecs;
	int                 maxXMACodecs;
	int                 maxVorbisCodecs;
	int                 maxAT9Codecs;
	int                 maxFADPCMCodecs;
	int                 maxPCMCodecs;
	int                 ASIONumChannels;
	char              **ASIOChannelList;
	FMOD_SPEAKER       *ASIOSpeakerList;
	float               vol0virtualvol;
	unsigned int        defaultDecodeBufferSize;
	unsigned short      profilePort;
	unsigned int        geometryMaxFadeTime;
	float               distanceFilterCenterFreq;
	int                 reverb3Dinstance;
	int                 DSPBufferPoolSize;
	unsigned int        stackSizeStream;
	unsigned int        stackSizeNonBlocking;
	unsigned int        stackSizeMixer;
	FMOD_DSP_RESAMPLER  resamplerMethod;
	unsigned int        commandQueueSize;
	unsigned int        randomSeed;
} FMOD_ADVANCEDSETTINGS;

typedef struct FMOD_PLUGINLIST
{
	FMOD_PLUGINTYPE  type;
	void            *description;
} FMOD_PLUGINLIST;

typedef struct FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR
{
	int     numpoints;
	float  *pointparamvalues;
	float  *pointpositions;
} FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR;

typedef struct FMOD_DSP_PARAMETER_FLOAT_MAPPING
{
	FMOD_DSP_PARAMETER_FLOAT_MAPPING_TYPE               type;
	FMOD_DSP_PARAMETER_FLOAT_MAPPING_PIECEWISE_LINEAR   piecewiselinearmapping;
} FMOD_DSP_PARAMETER_FLOAT_MAPPING;

typedef struct FMOD_DSP_PARAMETER_DESC_FLOAT
{
	float                               min;
	float                               max;
	float                               defaultval;
	FMOD_DSP_PARAMETER_FLOAT_MAPPING    mapping;
} FMOD_DSP_PARAMETER_DESC_FLOAT;

typedef struct FMOD_DSP_PARAMETER_DESC_INT
{
	int                 min;
	int                 max;
	int                 defaultval;
	FMOD_BOOL           goestoinf;
	const char* const*  valuenames;
} FMOD_DSP_PARAMETER_DESC_INT;

typedef struct FMOD_DSP_PARAMETER_DESC_BOOL
{
	FMOD_BOOL           defaultval;
	const char* const*  valuenames;
} FMOD_DSP_PARAMETER_DESC_BOOL;

typedef struct FMOD_DSP_PARAMETER_DESC_DATA
{
	int datatype;
} FMOD_DSP_PARAMETER_DESC_DATA;

typedef struct FMOD_DSP_PARAMETER_DESC
{
	FMOD_DSP_PARAMETER_TYPE type;
	char                    name[16];
	char                    label[16];
	const char             *description;

	union
	{
		FMOD_DSP_PARAMETER_DESC_FLOAT   floatdesc;
		FMOD_DSP_PARAMETER_DESC_INT     intdesc;
		FMOD_DSP_PARAMETER_DESC_BOOL    booldesc;
		FMOD_DSP_PARAMETER_DESC_DATA    datadesc;
	};
} FMOD_DSP_PARAMETER_DESC;

typedef struct FMOD_DSP_METERING_INFO
{
	int   numsamples;
	float peaklevel[32];
	float rmslevel[32];
	short numchannels;
} FMOD_DSP_METERING_INFO;

typedef struct FMOD_DSP_DESCRIPTION
{
	unsigned int                        pluginsdkversion;
	char                                name[32];
	unsigned int                        version;
	int                                 numinputbuffers;
	int                                 numoutputbuffers;
	FMOD_DSP_CREATE_CALLBACK            create;
	FMOD_DSP_RELEASE_CALLBACK           release;
	FMOD_DSP_RESET_CALLBACK             reset;
	FMOD_DSP_READ_CALLBACK              read;
	FMOD_DSP_PROCESS_CALLBACK           process;
	FMOD_DSP_SETPOSITION_CALLBACK       setposition;

	int                                 numparameters;
	FMOD_DSP_PARAMETER_DESC           **paramdesc;
	FMOD_DSP_SETPARAM_FLOAT_CALLBACK    setparameterfloat;
	FMOD_DSP_SETPARAM_INT_CALLBACK      setparameterint;
	FMOD_DSP_SETPARAM_BOOL_CALLBACK     setparameterbool;
	FMOD_DSP_SETPARAM_DATA_CALLBACK     setparameterdata;
	FMOD_DSP_GETPARAM_FLOAT_CALLBACK    getparameterfloat;
	FMOD_DSP_GETPARAM_INT_CALLBACK      getparameterint;
	FMOD_DSP_GETPARAM_BOOL_CALLBACK     getparameterbool;
	FMOD_DSP_GETPARAM_DATA_CALLBACK     getparameterdata;
	FMOD_DSP_SHOULDIPROCESS_CALLBACK    shouldiprocess;
	void                               *userdata;

	FMOD_DSP_SYSTEM_REGISTER_CALLBACK   sys_register;
	FMOD_DSP_SYSTEM_DEREGISTER_CALLBACK sys_deregister;
	FMOD_DSP_SYSTEM_MIX_CALLBACK        sys_mix;

} FMOD_DSP_DESCRIPTION;

typedef struct FMOD_CODEC_DESCRIPTION
{
	const char                       *name;
	unsigned int                      version;
	int                               defaultasstream;
	FMOD_TIMEUNIT                     timeunits;
	FMOD_CODEC_OPEN_CALLBACK          open;
	FMOD_CODEC_CLOSE_CALLBACK         close;
	FMOD_CODEC_READ_CALLBACK          read;
	FMOD_CODEC_GETLENGTH_CALLBACK     getlength;
	FMOD_CODEC_SETPOSITION_CALLBACK   setposition;
	FMOD_CODEC_GETPOSITION_CALLBACK   getposition;
	FMOD_CODEC_SOUNDCREATE_CALLBACK   soundcreate;
	FMOD_CODEC_GETWAVEFORMAT_CALLBACK getwaveformat;
} FMOD_CODEC_DESCRIPTION;

typedef unsigned int FMOD_OUTPUT_METHOD;
typedef struct FMOD_OUTPUT_DESCRIPTION
{
	unsigned int                            apiversion;
	const char                             *name;
	unsigned int                            version;
	FMOD_OUTPUT_METHOD                      polling; /* This will become "method" on the next major version */
	FMOD_OUTPUT_GETNUMDRIVERS_CALLBACK      getnumdrivers;
	FMOD_OUTPUT_GETDRIVERINFO_CALLBACK      getdriverinfo;
	FMOD_OUTPUT_INIT_CALLBACK               init;
	FMOD_OUTPUT_START_CALLBACK              start;
	FMOD_OUTPUT_STOP_CALLBACK               stop;
	FMOD_OUTPUT_CLOSE_CALLBACK              close;
	FMOD_OUTPUT_UPDATE_CALLBACK             update;
	FMOD_OUTPUT_GETHANDLE_CALLBACK          gethandle;
	FMOD_OUTPUT_GETPOSITION_CALLBACK        getposition;
	FMOD_OUTPUT_LOCK_CALLBACK               lock;
	FMOD_OUTPUT_UNLOCK_CALLBACK             unlock;
	FMOD_OUTPUT_MIXER_CALLBACK              mixer;
	FMOD_OUTPUT_OBJECT3DGETINFO_CALLBACK    object3dgetinfo;
	FMOD_OUTPUT_OBJECT3DALLOC_CALLBACK      object3dalloc;
	FMOD_OUTPUT_OBJECT3DFREE_CALLBACK       object3dfree;
	FMOD_OUTPUT_OBJECT3DUPDATE_CALLBACK     object3dupdate;
	FMOD_OUTPUT_OPENPORT_CALLBACK           openport;
	FMOD_OUTPUT_CLOSEPORT_CALLBACK          closeport;
	FMOD_OUTPUT_DEVICELISTCHANGED_CALLBACK  devicelistchanged;
} FMOD_OUTPUT_DESCRIPTION;

typedef struct FMOD_TAG
{
	FMOD_TAGTYPE      type;
	FMOD_TAGDATATYPE  datatype;
	char             *name;
	void             *data;
	unsigned int      datalen;
	FMOD_BOOL         updated;
} FMOD_TAG;

typedef struct FMOD_REVERB_PROPERTIES
{
	float DecayTime;
	float EarlyDelay;
	float LateDelay;
	float HFReference;
	float HFDecayRatio;
	float Diffusion;
	float Density;
	float LowShelfFrequency;
	float LowShelfGain;
	float HighCut;
	float EarlyLateMix;
	float WetLevel;
} FMOD_REVERB_PROPERTIES;


namespace FMOD
{
	// Channel
	class System;
	class DSP;
	class Sound;
	class ChannelGroup;
	class Channel;

	class DSPConnection
	{
	private:
		// Constructor made private so user cannot statically instance a DSPConnection class.  Appropriate DSPConnection creation or retrieval function must be used.
		DSPConnection();
		DSPConnection(const DSPConnection &);

	public:
		FMOD_RESULT F_API getInput              (DSP **input);
		FMOD_RESULT F_API getOutput             (DSP **output);
		FMOD_RESULT F_API setMix                (float volume);
		FMOD_RESULT F_API getMix                (float *volume);
		FMOD_RESULT F_API setMixMatrix          (float *matrix, int outchannels, int inchannels, int inchannel_hop = 0);
		FMOD_RESULT F_API getMixMatrix          (float *matrix, int *outchannels, int *inchannels, int inchannel_hop = 0);
		FMOD_RESULT F_API getType               (FMOD_DSPCONNECTION_TYPE *type);

		// Userdata set/get.
		FMOD_RESULT F_API setUserData           (void *userdata);
		FMOD_RESULT F_API getUserData           (void **userdata);
	};

	class DSP
	{
	private:
		// Constructor made private so user cannot statically instance a DSP class.  Appropriate DSP creation or retrieval function must be used.
		DSP();
		DSP(const DSP &);

	public:
		FMOD_RESULT F_API release                ();
		FMOD_RESULT F_API getSystemObject        (System **system);

		// Connection / disconnection / input and output enumeration.
		FMOD_RESULT F_API addInput               (DSP *input, DSPConnection **connection = 0, FMOD_DSPCONNECTION_TYPE type = FMOD_DSPCONNECTION_TYPE_STANDARD);
		FMOD_RESULT F_API disconnectFrom         (DSP *target, DSPConnection *connection = 0);
		FMOD_RESULT F_API disconnectAll          (bool inputs, bool outputs);
		FMOD_RESULT F_API getNumInputs           (int *numinputs);
		FMOD_RESULT F_API getNumOutputs          (int *numoutputs);
		FMOD_RESULT F_API getInput               (int index, DSP **input, DSPConnection **inputconnection);
		FMOD_RESULT F_API getOutput              (int index, DSP **output, DSPConnection **outputconnection);

		// DSP unit control.
		FMOD_RESULT F_API setActive              (bool active);
		FMOD_RESULT F_API getActive              (bool *active);
		FMOD_RESULT F_API setBypass              (bool bypass);
		FMOD_RESULT F_API getBypass              (bool *bypass);
		FMOD_RESULT F_API setWetDryMix           (float prewet, float postwet, float dry);
		FMOD_RESULT F_API getWetDryMix           (float *prewet, float *postwet, float *dry);
		FMOD_RESULT F_API setChannelFormat       (FMOD_CHANNELMASK channelmask, int numchannels, FMOD_SPEAKERMODE source_speakermode);
		FMOD_RESULT F_API getChannelFormat       (FMOD_CHANNELMASK *channelmask, int *numchannels, FMOD_SPEAKERMODE *source_speakermode);
		FMOD_RESULT F_API getOutputChannelFormat (FMOD_CHANNELMASK inmask, int inchannels, FMOD_SPEAKERMODE inspeakermode, FMOD_CHANNELMASK *outmask, int *outchannels, FMOD_SPEAKERMODE *outspeakermode);
		FMOD_RESULT F_API reset                  ();

		// DSP parameter control.
		FMOD_RESULT F_API setParameterFloat      (int index, float value);
		FMOD_RESULT F_API setParameterInt        (int index, int value);
		FMOD_RESULT F_API setParameterBool       (int index, bool value);
		FMOD_RESULT F_API setParameterData       (int index, void *data, unsigned int length);
		FMOD_RESULT F_API getParameterFloat      (int index, float *value, char *valuestr, int valuestrlen);
		FMOD_RESULT F_API getParameterInt        (int index, int *value, char *valuestr, int valuestrlen);
		FMOD_RESULT F_API getParameterBool       (int index, bool *value, char *valuestr, int valuestrlen);
		FMOD_RESULT F_API getParameterData       (int index, void **data, unsigned int *length, char *valuestr, int valuestrlen);
		FMOD_RESULT F_API getNumParameters       (int *numparams);
		FMOD_RESULT F_API getParameterInfo       (int index, FMOD_DSP_PARAMETER_DESC **desc);
		FMOD_RESULT F_API getDataParameterIndex  (int datatype, int *index);
		FMOD_RESULT F_API showConfigDialog       (void *hwnd, bool show);

		// DSP attributes.
		FMOD_RESULT F_API getInfo                (char *name, unsigned int *version, int *channels, int *configwidth, int *configheight);
		FMOD_RESULT F_API getType                (FMOD_DSP_TYPE *type);
		FMOD_RESULT F_API getIdle                (bool *idle);

		// Userdata set/get.
		FMOD_RESULT F_API setUserData            (void *userdata);
		FMOD_RESULT F_API getUserData            (void **userdata);

		// Metering.
		FMOD_RESULT F_API setMeteringEnabled     (bool inputEnabled, bool outputEnabled);
		FMOD_RESULT F_API getMeteringEnabled     (bool *inputEnabled, bool *outputEnabled);
		FMOD_RESULT F_API getMeteringInfo        (FMOD_DSP_METERING_INFO *inputInfo, FMOD_DSP_METERING_INFO *outputInfo);
		FMOD_RESULT F_API getCPUUsage            (unsigned int *exclusive, unsigned int *inclusive);
	};

	class SoundGroup
	{
	private:
		// Constructor made private so user cannot statically instance a SoundGroup class.  Appropriate SoundGroup creation or retrieval function must be used.
		SoundGroup();
		SoundGroup(const SoundGroup &);

	public:
		FMOD_RESULT F_API release                ();
		FMOD_RESULT F_API getSystemObject        (System **system);

		// SoundGroup control functions.
		FMOD_RESULT F_API setMaxAudible          (int maxaudible);
		FMOD_RESULT F_API getMaxAudible          (int *maxaudible);
		FMOD_RESULT F_API setMaxAudibleBehavior  (FMOD_SOUNDGROUP_BEHAVIOR behavior);
		FMOD_RESULT F_API getMaxAudibleBehavior  (FMOD_SOUNDGROUP_BEHAVIOR *behavior);
		FMOD_RESULT F_API setMuteFadeSpeed       (float speed);
		FMOD_RESULT F_API getMuteFadeSpeed       (float *speed);
		FMOD_RESULT F_API setVolume              (float volume);
		FMOD_RESULT F_API getVolume              (float *volume);
		FMOD_RESULT F_API stop                   ();

		// Information only functions.
		FMOD_RESULT F_API getName                (char *name, int namelen);
		FMOD_RESULT F_API getNumSounds           (int *numsounds);
		FMOD_RESULT F_API getSound               (int index, Sound **sound);
		FMOD_RESULT F_API getNumPlaying          (int *numplaying);

		// Userdata set/get.
		FMOD_RESULT F_API setUserData            (void *userdata);
		FMOD_RESULT F_API getUserData            (void **userdata);
	};

	class Sound
	{
	private:
		// Constructor made private so user cannot statically instance a Sound class.  Appropriate Sound creation or retrieval function must be used.
		Sound();
		Sound(const Sound &);

	public:
		FMOD_RESULT F_API release                ();
		FMOD_RESULT F_API getSystemObject        (System **system);

		// Standard sound manipulation functions.
		FMOD_RESULT F_API lock                   (unsigned int offset, unsigned int length, void **ptr1, void **ptr2, unsigned int *len1, unsigned int *len2);
		FMOD_RESULT F_API unlock                 (void *ptr1, void *ptr2, unsigned int len1, unsigned int len2);
		FMOD_RESULT F_API setDefaults            (float frequency, int priority);
		FMOD_RESULT F_API getDefaults            (float *frequency, int *priority);
		FMOD_RESULT F_API set3DMinMaxDistance    (float min, float max);
		FMOD_RESULT F_API get3DMinMaxDistance    (float *min, float *max);
		FMOD_RESULT F_API set3DConeSettings      (float insideconeangle, float outsideconeangle, float outsidevolume);
		FMOD_RESULT F_API get3DConeSettings      (float *insideconeangle, float *outsideconeangle, float *outsidevolume);
		FMOD_RESULT F_API set3DCustomRolloff     (FMOD_VECTOR *points, int numpoints);
		FMOD_RESULT F_API get3DCustomRolloff     (FMOD_VECTOR **points, int *numpoints);
		FMOD_RESULT F_API getSubSound            (int index, Sound **subsound);
		FMOD_RESULT F_API getSubSoundParent      (Sound **parentsound);
		FMOD_RESULT F_API getName                (char *name, int namelen);
		FMOD_RESULT F_API getLength              (unsigned int *length, FMOD_TIMEUNIT lengthtype);
		FMOD_RESULT F_API getFormat              (FMOD_SOUND_TYPE *type, FMOD_SOUND_FORMAT *format, int *channels, int *bits);
		FMOD_RESULT F_API getNumSubSounds        (int *numsubsounds);
		FMOD_RESULT F_API getNumTags             (int *numtags, int *numtagsupdated);
		FMOD_RESULT F_API getTag                 (const char *name, int index, FMOD_TAG *tag);
		FMOD_RESULT F_API getOpenState           (FMOD_OPENSTATE *openstate, unsigned int *percentbuffered, bool *starving, bool *diskbusy);
		FMOD_RESULT F_API readData               (void *buffer, unsigned int length, unsigned int *read);
		FMOD_RESULT F_API seekData               (unsigned int pcm);

		FMOD_RESULT F_API setSoundGroup          (SoundGroup *soundgroup);
		FMOD_RESULT F_API getSoundGroup          (SoundGroup **soundgroup);

		// Synchronization point API.  These points can come from markers embedded in wav files, and can also generate channel callbacks.
		FMOD_RESULT F_API getNumSyncPoints       (int *numsyncpoints);
		FMOD_RESULT F_API getSyncPoint           (int index, FMOD_SYNCPOINT **point);
		FMOD_RESULT F_API getSyncPointInfo       (FMOD_SYNCPOINT *point, char *name, int namelen, unsigned int *offset, FMOD_TIMEUNIT offsettype);
		FMOD_RESULT F_API addSyncPoint           (unsigned int offset, FMOD_TIMEUNIT offsettype, const char *name, FMOD_SYNCPOINT **point);
		FMOD_RESULT F_API deleteSyncPoint        (FMOD_SYNCPOINT *point);

		// Functions also in Channel class but here they are the 'default' to save having to change it in Channel all the time.
		FMOD_RESULT F_API setMode                (FMOD_MODE mode);
		FMOD_RESULT F_API getMode                (FMOD_MODE *mode);
		FMOD_RESULT F_API setLoopCount           (int loopcount);
		FMOD_RESULT F_API getLoopCount           (int *loopcount);
		FMOD_RESULT F_API setLoopPoints          (unsigned int loopstart, FMOD_TIMEUNIT loopstarttype, unsigned int loopend, FMOD_TIMEUNIT loopendtype);
		FMOD_RESULT F_API getLoopPoints          (unsigned int *loopstart, FMOD_TIMEUNIT loopstarttype, unsigned int *loopend, FMOD_TIMEUNIT loopendtype);

		// For MOD/S3M/XM/IT/MID sequenced formats only.
		FMOD_RESULT F_API getMusicNumChannels    (int *numchannels);
		FMOD_RESULT F_API setMusicChannelVolume  (int channel, float volume);
		FMOD_RESULT F_API getMusicChannelVolume  (int channel, float *volume);
		FMOD_RESULT F_API setMusicSpeed          (float speed);
		FMOD_RESULT F_API getMusicSpeed          (float *speed);

		// Userdata set/get.
		FMOD_RESULT F_API setUserData            (void *userdata);
		FMOD_RESULT F_API getUserData            (void **userdata);
	};

	class Reverb3D
	{
	private:
		// Constructor made private so user cannot statically instance a Reverb3D class.  Appropriate Reverb creation or retrieval function must be used.
		Reverb3D();
		Reverb3D(const Reverb3D &);

	public:
		FMOD_RESULT F_API release                ();

		// Reverb manipulation.
		FMOD_RESULT F_API set3DAttributes        (const FMOD_VECTOR *position, float mindistance, float maxdistance);
		FMOD_RESULT F_API get3DAttributes        (FMOD_VECTOR *position, float *mindistance,float *maxdistance);
		FMOD_RESULT F_API setProperties          (const FMOD_REVERB_PROPERTIES *properties);
		FMOD_RESULT F_API getProperties          (FMOD_REVERB_PROPERTIES *properties);
		FMOD_RESULT F_API setActive              (bool active);
		FMOD_RESULT F_API getActive              (bool *active);

		// Userdata set/get.
		FMOD_RESULT F_API setUserData            (void *userdata);
		FMOD_RESULT F_API getUserData            (void **userdata);
	};

	class Geometry
	{
	private:
		// Constructor made private so user cannot statically instance a Geometry class.  Appropriate Geometry creation or retrieval function must be used.
		Geometry();
		Geometry(const Geometry &);

	public:
		FMOD_RESULT F_API release                ();

		// Polygon manipulation.
		FMOD_RESULT F_API addPolygon             (float directocclusion, float reverbocclusion, bool doublesided, int numvertices, const FMOD_VECTOR *vertices, int *polygonindex);
		FMOD_RESULT F_API getNumPolygons         (int *numpolygons);
		FMOD_RESULT F_API getMaxPolygons         (int *maxpolygons, int *maxvertices);
		FMOD_RESULT F_API getPolygonNumVertices  (int index, int *numvertices);
		FMOD_RESULT F_API setPolygonVertex       (int index, int vertexindex, const FMOD_VECTOR *vertex);
		FMOD_RESULT F_API getPolygonVertex       (int index, int vertexindex, FMOD_VECTOR *vertex);
		FMOD_RESULT F_API setPolygonAttributes   (int index, float directocclusion, float reverbocclusion, bool doublesided);
		FMOD_RESULT F_API getPolygonAttributes   (int index, float *directocclusion, float *reverbocclusion, bool *doublesided);

		// Object manipulation.
		FMOD_RESULT F_API setActive              (bool active);
		FMOD_RESULT F_API getActive              (bool *active);
		FMOD_RESULT F_API setRotation            (const FMOD_VECTOR *forward, const FMOD_VECTOR *up);
		FMOD_RESULT F_API getRotation            (FMOD_VECTOR *forward, FMOD_VECTOR *up);
		FMOD_RESULT F_API setPosition            (const FMOD_VECTOR *position);
		FMOD_RESULT F_API getPosition            (FMOD_VECTOR *position);
		FMOD_RESULT F_API setScale               (const FMOD_VECTOR *scale);
		FMOD_RESULT F_API getScale               (FMOD_VECTOR *scale);
		FMOD_RESULT F_API save                   (void *data, int *datasize);

		// Userdata set/get.
		FMOD_RESULT F_API setUserData            (void *userdata);
		FMOD_RESULT F_API getUserData            (void **userdata);
	};

	#define FMOD_SYSTEM_CALLBACK_ALL 0xFFFFFFFF
	class System
	{
	private:
		// Constructor made private so user cannot statically instance a System class.  System_Create must be used.
		System();
		System(const System &);

	public:
		FMOD_RESULT F_API release                 ();

		// Setup functions.
		FMOD_RESULT F_API setOutput               (FMOD_OUTPUTTYPE output);
		FMOD_RESULT F_API getOutput               (FMOD_OUTPUTTYPE *output);
		FMOD_RESULT F_API getNumDrivers           (int *numdrivers);
		FMOD_RESULT F_API getDriverInfo           (int id, char *name, int namelen, FMOD_GUID *guid, int *systemrate, FMOD_SPEAKERMODE *speakermode, int *speakermodechannels);
		FMOD_RESULT F_API setDriver               (int driver);
		FMOD_RESULT F_API getDriver               (int *driver);
		FMOD_RESULT F_API setSoftwareChannels     (int numsoftwarechannels);
		FMOD_RESULT F_API getSoftwareChannels     (int *numsoftwarechannels);
		FMOD_RESULT F_API setSoftwareFormat       (int samplerate, FMOD_SPEAKERMODE speakermode, int numrawspeakers);
		FMOD_RESULT F_API getSoftwareFormat       (int *samplerate, FMOD_SPEAKERMODE *speakermode, int *numrawspeakers);
		FMOD_RESULT F_API setDSPBufferSize        (unsigned int bufferlength, int numbuffers);
		FMOD_RESULT F_API getDSPBufferSize        (unsigned int *bufferlength, int *numbuffers);
		FMOD_RESULT F_API setFileSystem           (FMOD_FILE_OPEN_CALLBACK useropen, FMOD_FILE_CLOSE_CALLBACK userclose, FMOD_FILE_READ_CALLBACK userread, FMOD_FILE_SEEK_CALLBACK userseek, FMOD_FILE_ASYNCREAD_CALLBACK userasyncread, FMOD_FILE_ASYNCCANCEL_CALLBACK userasynccancel, int blockalign);
		FMOD_RESULT F_API attachFileSystem        (FMOD_FILE_OPEN_CALLBACK useropen, FMOD_FILE_CLOSE_CALLBACK userclose, FMOD_FILE_READ_CALLBACK userread, FMOD_FILE_SEEK_CALLBACK userseek);
		FMOD_RESULT F_API setAdvancedSettings     (FMOD_ADVANCEDSETTINGS *settings);
		FMOD_RESULT F_API getAdvancedSettings     (FMOD_ADVANCEDSETTINGS *settings);
		FMOD_RESULT F_API setCallback             (FMOD_SYSTEM_CALLBACK callback, FMOD_SYSTEM_CALLBACK_TYPE callbackmask = FMOD_SYSTEM_CALLBACK_ALL);

		// Plug-in support.
		FMOD_RESULT F_API setPluginPath           (const char *path);
		FMOD_RESULT F_API loadPlugin              (const char *filename, unsigned int *handle, unsigned int priority = 0);
		FMOD_RESULT F_API unloadPlugin            (unsigned int handle);
		FMOD_RESULT F_API getNumNestedPlugins     (unsigned int handle, int *count);
		FMOD_RESULT F_API getNestedPlugin         (unsigned int handle, int index, unsigned int *nestedhandle);
		FMOD_RESULT F_API getNumPlugins           (FMOD_PLUGINTYPE plugintype, int *numplugins);
		FMOD_RESULT F_API getPluginHandle         (FMOD_PLUGINTYPE plugintype, int index, unsigned int *handle);
		FMOD_RESULT F_API getPluginInfo           (unsigned int handle, FMOD_PLUGINTYPE *plugintype, char *name, int namelen, unsigned int *version);
		FMOD_RESULT F_API setOutputByPlugin       (unsigned int handle);
		FMOD_RESULT F_API getOutputByPlugin       (unsigned int *handle);
		FMOD_RESULT F_API createDSPByPlugin       (unsigned int handle, DSP **dsp);
		FMOD_RESULT F_API getDSPInfoByPlugin      (unsigned int handle, const FMOD_DSP_DESCRIPTION **description);
		FMOD_RESULT F_API registerCodec           (FMOD_CODEC_DESCRIPTION *description, unsigned int *handle, unsigned int priority = 0);
		FMOD_RESULT F_API registerDSP             (const FMOD_DSP_DESCRIPTION *description, unsigned int *handle);
		FMOD_RESULT F_API registerOutput          (const FMOD_OUTPUT_DESCRIPTION *description, unsigned int *handle);

		// Init/Close.
		FMOD_RESULT F_API init                    (int maxchannels, FMOD_INITFLAGS flags, void *extradriverdata);
		FMOD_RESULT F_API close                   ();

		// General post-init system functions.
		FMOD_RESULT F_API update                  ();        /* IMPORTANT! CALL THIS ONCE PER FRAME! */

		FMOD_RESULT F_API setSpeakerPosition      (FMOD_SPEAKER speaker, float x, float y, bool active);
		FMOD_RESULT F_API getSpeakerPosition      (FMOD_SPEAKER speaker, float *x, float *y, bool *active);
		FMOD_RESULT F_API setStreamBufferSize     (unsigned int filebuffersize, FMOD_TIMEUNIT filebuffersizetype);
		FMOD_RESULT F_API getStreamBufferSize     (unsigned int *filebuffersize, FMOD_TIMEUNIT *filebuffersizetype);
		FMOD_RESULT F_API set3DSettings           (float dopplerscale, float distancefactor, float rolloffscale);
		FMOD_RESULT F_API get3DSettings           (float *dopplerscale, float *distancefactor, float *rolloffscale);
		FMOD_RESULT F_API set3DNumListeners       (int numlisteners);
		FMOD_RESULT F_API get3DNumListeners       (int *numlisteners);
		FMOD_RESULT F_API set3DListenerAttributes (int listener, const FMOD_VECTOR *pos, const FMOD_VECTOR *vel, const FMOD_VECTOR *forward, const FMOD_VECTOR *up);
		FMOD_RESULT F_API get3DListenerAttributes (int listener, FMOD_VECTOR *pos, FMOD_VECTOR *vel, FMOD_VECTOR *forward, FMOD_VECTOR *up);
		FMOD_RESULT F_API set3DRolloffCallback    (FMOD_3D_ROLLOFF_CALLBACK callback);
		FMOD_RESULT F_API mixerSuspend            ();
		FMOD_RESULT F_API mixerResume             ();
		FMOD_RESULT F_API getDefaultMixMatrix     (FMOD_SPEAKERMODE sourcespeakermode, FMOD_SPEAKERMODE targetspeakermode, float *matrix, int matrixhop);
		FMOD_RESULT F_API getSpeakerModeChannels  (FMOD_SPEAKERMODE mode, int *channels);

		// System information functions.
		FMOD_RESULT F_API getVersion              (unsigned int *version);
		FMOD_RESULT F_API getOutputHandle         (void **handle);
		FMOD_RESULT F_API getChannelsPlaying      (int *channels, int *realchannels = 0);
		FMOD_RESULT F_API getCPUUsage             (float *dsp, float *stream, float *geometry, float *update, float *total);
		FMOD_RESULT F_API getFileUsage            (long long *sampleBytesRead, long long *streamBytesRead, long long *otherBytesRead);

		// Sound/DSP/Channel/FX creation and retrieval.
		FMOD_RESULT F_API createSound             (const char *name_or_data, FMOD_MODE mode, FMOD_CREATESOUNDEXINFO *exinfo, Sound **sound);
		FMOD_RESULT F_API createStream            (const char *name_or_data, FMOD_MODE mode, FMOD_CREATESOUNDEXINFO *exinfo, Sound **sound);
		FMOD_RESULT F_API createDSP               (const FMOD_DSP_DESCRIPTION *description, DSP **dsp);
		FMOD_RESULT F_API createDSPByType         (FMOD_DSP_TYPE type, DSP **dsp);
		FMOD_RESULT F_API createChannelGroup      (const char *name, ChannelGroup **channelgroup);
		FMOD_RESULT F_API createSoundGroup        (const char *name, SoundGroup **soundgroup);
		FMOD_RESULT F_API createReverb3D          (Reverb3D **reverb);

		FMOD_RESULT F_API playSound               (Sound *sound, ChannelGroup *channelgroup, bool paused, Channel **channel);
		FMOD_RESULT F_API playDSP                 (DSP *dsp, ChannelGroup *channelgroup, bool paused, Channel **channel);
		FMOD_RESULT F_API getChannel              (int channelid, Channel **channel);
		FMOD_RESULT F_API getMasterChannelGroup   (ChannelGroup **channelgroup);
		FMOD_RESULT F_API getMasterSoundGroup     (SoundGroup **soundgroup);

		// Routing to ports.
		FMOD_RESULT F_API attachChannelGroupToPort   (FMOD_PORT_TYPE portType, FMOD_PORT_INDEX portIndex, ChannelGroup *channelgroup, bool passThru = false);
		FMOD_RESULT F_API detachChannelGroupFromPort (ChannelGroup *channelgroup);

		// Reverb API.
		FMOD_RESULT F_API setReverbProperties     (int instance, const FMOD_REVERB_PROPERTIES *prop);
		FMOD_RESULT F_API getReverbProperties     (int instance, FMOD_REVERB_PROPERTIES *prop);

		// System level DSP functionality.
		FMOD_RESULT F_API lockDSP                 ();
		FMOD_RESULT F_API unlockDSP               ();

		// Recording API.
		FMOD_RESULT F_API getRecordNumDrivers     (int *numdrivers, int *numconnected);
		FMOD_RESULT F_API getRecordDriverInfo     (int id, char *name, int namelen, FMOD_GUID *guid, int *systemrate, FMOD_SPEAKERMODE *speakermode, int *speakermodechannels, FMOD_DRIVER_STATE *state);
		FMOD_RESULT F_API getRecordPosition       (int id, unsigned int *position);
		FMOD_RESULT F_API recordStart             (int id, Sound *sound, bool loop);
		FMOD_RESULT F_API recordStop              (int id);
		FMOD_RESULT F_API isRecording             (int id, bool *recording);

		// Geometry API.
		FMOD_RESULT F_API createGeometry          (int maxpolygons, int maxvertices, Geometry **geometry);
		FMOD_RESULT F_API setGeometrySettings     (float maxworldsize);
		FMOD_RESULT F_API getGeometrySettings     (float *maxworldsize);
		FMOD_RESULT F_API loadGeometry            (const void *data, int datasize, Geometry **geometry);
		FMOD_RESULT F_API getGeometryOcclusion    (const FMOD_VECTOR *listener, const FMOD_VECTOR *source, float *direct, float *reverb);

		// Network functions.
		FMOD_RESULT F_API setNetworkProxy         (const char *proxy);
		FMOD_RESULT F_API getNetworkProxy         (char *proxy, int proxylen);
		FMOD_RESULT F_API setNetworkTimeout       (int timeout);
		FMOD_RESULT F_API getNetworkTimeout       (int *timeout);

		// Userdata set/get.
		FMOD_RESULT F_API setUserData             (void *userdata);
		FMOD_RESULT F_API getUserData             (void **userdata);
	};

	class ChannelControl
	{
	private:
		// Constructor made private so user cannot statically instance a Control class.
		ChannelControl();
		ChannelControl(const ChannelControl &);

	public:
		FMOD_RESULT F_API getSystemObject        (System **system);

		// General control functionality for Channels and ChannelGroups.
		FMOD_RESULT F_API stop                   ();
		FMOD_RESULT F_API setPaused              (bool paused);
		FMOD_RESULT F_API getPaused              (bool *paused);
		FMOD_RESULT F_API setVolume              (float volume);
		FMOD_RESULT F_API getVolume              (float *volume);
		FMOD_RESULT F_API setVolumeRamp          (bool ramp);
		FMOD_RESULT F_API getVolumeRamp          (bool *ramp);
		FMOD_RESULT F_API getAudibility          (float *audibility);
		FMOD_RESULT F_API setPitch               (float pitch);
		FMOD_RESULT F_API getPitch               (float *pitch);
		FMOD_RESULT F_API setMute                (bool mute);
		FMOD_RESULT F_API getMute                (bool *mute);
		FMOD_RESULT F_API setReverbProperties    (int instance, float wet);
		FMOD_RESULT F_API getReverbProperties    (int instance, float *wet);
		FMOD_RESULT F_API setLowPassGain         (float gain);
		FMOD_RESULT F_API getLowPassGain         (float *gain);
		FMOD_RESULT F_API setMode                (FMOD_MODE mode);
		FMOD_RESULT F_API getMode                (FMOD_MODE *mode);
		FMOD_RESULT F_API setCallback            (FMOD_CHANNELCONTROL_CALLBACK callback);
		FMOD_RESULT F_API isPlaying              (bool *isplaying);

		// Panning and level adjustment.
		// Note all 'set' functions alter a final matrix, this is why the only get function is getMixMatrix, to avoid other get functions returning incorrect/obsolete values.
		FMOD_RESULT F_API setPan                 (float pan);
		FMOD_RESULT F_API setMixLevelsOutput     (float frontleft, float frontright, float center, float lfe, float surroundleft, float surroundright, float backleft, float backright);
		FMOD_RESULT F_API setMixLevelsInput      (float *levels, int numlevels);
		FMOD_RESULT F_API setMixMatrix           (float *matrix, int outchannels, int inchannels, int inchannel_hop = 0);
		FMOD_RESULT F_API getMixMatrix           (float *matrix, int *outchannels, int *inchannels, int inchannel_hop = 0);

		// Clock based functionality.
		FMOD_RESULT F_API getDSPClock            (unsigned long long *dspclock, unsigned long long *parentclock);
		FMOD_RESULT F_API setDelay               (unsigned long long dspclock_start, unsigned long long dspclock_end, bool stopchannels = true);
		FMOD_RESULT F_API getDelay               (unsigned long long *dspclock_start, unsigned long long *dspclock_end, bool *stopchannels = 0);
		FMOD_RESULT F_API addFadePoint           (unsigned long long dspclock, float volume);
		FMOD_RESULT F_API setFadePointRamp       (unsigned long long dspclock, float volume);
		FMOD_RESULT F_API removeFadePoints       (unsigned long long dspclock_start, unsigned long long dspclock_end);
		FMOD_RESULT F_API getFadePoints          (unsigned int *numpoints, unsigned long long *point_dspclock, float *point_volume);

		// DSP effects.
		FMOD_RESULT F_API getDSP                 (int index, DSP **dsp);
		FMOD_RESULT F_API addDSP                 (int index, DSP *dsp);
		FMOD_RESULT F_API removeDSP              (DSP *dsp);
		FMOD_RESULT F_API getNumDSPs             (int *numdsps);
		FMOD_RESULT F_API setDSPIndex            (DSP *dsp, int index);
		FMOD_RESULT F_API getDSPIndex            (DSP *dsp, int *index);

		// 3D functionality.
		FMOD_RESULT F_API set3DAttributes        (const FMOD_VECTOR *pos, const FMOD_VECTOR *vel);
		FMOD_RESULT F_API get3DAttributes        (FMOD_VECTOR *pos, FMOD_VECTOR *vel);
		FMOD_RESULT F_API set3DMinMaxDistance    (float mindistance, float maxdistance);
		FMOD_RESULT F_API get3DMinMaxDistance    (float *mindistance, float *maxdistance);
		FMOD_RESULT F_API set3DConeSettings      (float insideconeangle, float outsideconeangle, float outsidevolume);
		FMOD_RESULT F_API get3DConeSettings      (float *insideconeangle, float *outsideconeangle, float *outsidevolume);
		FMOD_RESULT F_API set3DConeOrientation   (FMOD_VECTOR *orientation);
		FMOD_RESULT F_API get3DConeOrientation   (FMOD_VECTOR *orientation);
		FMOD_RESULT F_API set3DCustomRolloff     (FMOD_VECTOR *points, int numpoints);
		FMOD_RESULT F_API get3DCustomRolloff     (FMOD_VECTOR **points, int *numpoints);
		FMOD_RESULT F_API set3DOcclusion         (float directocclusion, float reverbocclusion);
		FMOD_RESULT F_API get3DOcclusion         (float *directocclusion, float *reverbocclusion);
		FMOD_RESULT F_API set3DSpread            (float angle);
		FMOD_RESULT F_API get3DSpread            (float *angle);
		FMOD_RESULT F_API set3DLevel             (float level);
		FMOD_RESULT F_API get3DLevel             (float *level);
		FMOD_RESULT F_API set3DDopplerLevel      (float level);
		FMOD_RESULT F_API get3DDopplerLevel      (float *level);
		FMOD_RESULT F_API set3DDistanceFilter    (bool custom, float customLevel, float centerFreq);
		FMOD_RESULT F_API get3DDistanceFilter    (bool *custom, float *customLevel, float *centerFreq);

		// Userdata set/get.
		FMOD_RESULT F_API setUserData            (void *userdata);
		FMOD_RESULT F_API getUserData            (void **userdata);
	};

	class Channel : public ChannelControl
	{
	private:
		Channel();
		Channel(const Channel &);

	public:
		// Channel specific control functionality.
		FMOD_RESULT F_API setFrequency(float frequency);
		FMOD_RESULT F_API getFrequency(float *frequency);
		FMOD_RESULT F_API setPriority(int priority);
		FMOD_RESULT F_API getPriority(int *priority);
		FMOD_RESULT F_API setPosition(unsigned int position, FMOD_TIMEUNIT postype);
		FMOD_RESULT F_API getPosition(unsigned int *position, FMOD_TIMEUNIT postype);
		FMOD_RESULT F_API setChannelGroup(ChannelGroup *channelgroup);
		FMOD_RESULT F_API getChannelGroup(ChannelGroup **channelgroup);
		FMOD_RESULT F_API setLoopCount(int loopcount);
		FMOD_RESULT F_API getLoopCount(int *loopcount);
		FMOD_RESULT F_API setLoopPoints(unsigned int loopstart, FMOD_TIMEUNIT loopstarttype, unsigned int loopend, FMOD_TIMEUNIT loopendtype);
		FMOD_RESULT F_API getLoopPoints(unsigned int *loopstart, FMOD_TIMEUNIT loopstarttype, unsigned int *loopend, FMOD_TIMEUNIT loopendtype);

		// Information only functions.
		FMOD_RESULT F_API isVirtual(bool *isvirtual);
		FMOD_RESULT F_API getCurrentSound(Sound ** sound);
		FMOD_RESULT F_API getIndex(int *index);
	};

	class ChannelGroup : public ChannelControl
	{
	private:
		// Constructor made private so user cannot statically instance a ChannelGroup class.  Appropriate ChannelGroup creation or retrieval function must be used.
		ChannelGroup();
		ChannelGroup(const ChannelGroup &);

	public:
		FMOD_RESULT F_API release                 ();

		// Nested channel groups.
		FMOD_RESULT F_API addGroup                (ChannelGroup *group, bool propagatedspclock = true, DSPConnection **connection = 0);
		FMOD_RESULT F_API getNumGroups            (int *numgroups);
		FMOD_RESULT F_API getGroup                (int index, ChannelGroup **group);
		FMOD_RESULT F_API getParentGroup          (ChannelGroup **group);

		// Information only functions.
		FMOD_RESULT F_API getName                 (char *name, int namelen);
		FMOD_RESULT F_API getNumChannels          (int *numchannels);
		FMOD_RESULT F_API getChannel              (int index, Channel **channel);
	};
}
