#pragma once
#include <sdkddkver.h>

// Disable portions of the Windows API that will not be used
#define WIN32_LEAN_AND_MEAN
#define NOGDICAPMASKS
#define NOMENUS
#define NOICONS
#define NOSYSCOMMANDS
#define OEMRESOURCE
#define NOSYSMETRICS
#define NOATOM
#define NOCLIPBOARD
#define NOCOLOR
#define NOCTLMGR
#define NODRAWTEXT
#define NOKERNEL
#define NONLS
#define NOMEMMGR
#define NOMETAFILE
#define NOOPENFILE
#define NOSCROLL
#define NOSERVICE
#define NOSOUND
#define NOTEXTMETRIC
#define NOWH
#define NOCOMM
#define NOKANJI
#define NOHELP
#define NOPROFILER
#define NODEFERWINDOWPOS
#define NOMCX
#define NORPC
#define NOPROXYSTUB
#define NOIMAGE
#define NOTAPE
#define NOMINMAX

// The following should be phased out after transitioning into Direct3D
// #define NORASTEROPS

#define STRICT

#include <Windows.h>