#pragma once

#define NOMINMAX
#include <Windows.h>

#include "Misc/Enums.hpp"
#include "Misc/vfunc.hpp"

#include "Math/VMatrix.hpp"
#include "Math/QAngle.hpp"
#include "Math/Vector.hpp"
#include "Misc/Studio.hpp"

#include "Interfaces/IAppSystem.hpp"
#include "Interfaces/IBaseClientDll.hpp"
#include "Interfaces/IClientEntity.hpp"
#include "Interfaces/IClientEntityList.hpp"
#include "Interfaces/IClientMode.hpp"
#include "Interfaces/IConVar.hpp"
#include "Interfaces/ICvar.hpp"
#include "Interfaces/IEngineTrace.hpp"
#include "Interfaces/IVEngineClient.hpp"
#include "Interfaces/IVDebugOverlay.hpp"
#include "Interfaces/ISurface.hpp"
#include "Interfaces/CInput.hpp"
#include "Interfaces/IVModelInfoClient.hpp"
#include "Interfaces/IVModelRender.hpp"
#include "Interfaces/IRenderView.hpp"
#include "Interfaces/IGameEventmanager.hpp"
#include "Interfaces/IMaterialSystem.hpp"
#include "Interfaces/IMoveHelper.hpp"
#include "Interfaces/IMDLCache.hpp"
#include "Interfaces/IPrediction.hpp"
#include "Interfaces/IPanel.hpp"
#include "Interfaces/IEngineSound.hpp"
#include "Interfaces/IViewRender.hpp"
#include "Interfaces/CClientState.hpp"
#include "Interfaces/IPhysics.hpp"

#include "Misc/Convar.hpp"
#include "Misc/CUserCmd.hpp"
#include "Misc/glow_outline_effect.hpp"

#include "netvars.hpp"

struct IDirect3DDevice9;

namespace Interfaces
{
    void Initialize();
    void Dump();
}

extern IVEngineClient*       g_pEngine;
extern IBaseClientDLL*       g_pClient;
extern IClientEntityList*    g_pEntityList;
extern CGlobalVarsBase*      g_pGlobals;
extern IEngineTrace*         g_pEngineTrace;
extern ICvar*                g_pCVar;
extern IPanel*               g_pPanel;
extern IClientMode*          g_pClientMode;
extern IVDebugOverlay*       g_pDebugOverlay;
extern ISurface*             g_pSurface;
extern CInput*               g_pInput;
extern IVModelInfoClient*    g_pMDLInfo;
extern IVModelRender*        g_pMDLRender;
extern IVRenderView*         g_pRenderView;
extern IMaterialSystem*      g_pMaterialSystem;
extern IGameEventManager2*   g_pGameEventManager;
extern IMoveHelper*          g_pMoveHelper;
extern IMDLCache*            g_pMDLCache;
extern IPrediction*          g_pPrediction;
extern CGameMovement*        g_pGameMovement;
extern IEngineSound*         g_pEngineSound;
extern CGlowObjectManager*   g_pGlowObjectManager;
extern IViewRender*          g_pViewRender;
extern IDirect3DDevice9*     g_pD3DDevice9;
extern CClientState*         g_pClientState;
extern IPhysicsSurfaceProps* g_pPhysicsSurface;

template<typename... Args>
void ConMsg(const char* pMsg, Args... args)
{
    static auto import = (void(*)(const char*, ...))GetProcAddress(GetModuleHandleW(L"tier0.dll"), "?ConMsg@@YAXPBDZZ");
    return import(pMsg, args...);
}
template<typename... Args>
void ConColorMsg(const Color& clr, const char* pMsg, Args... args)
{
    static auto import = (void(*)(const Color&, const char*, ...))GetProcAddress(GetModuleHandleW(L"tier0.dll"), "?ConColorMsg@@YAXABVColor@@PBDZZ");
    return import(clr, pMsg, args...);
}

#include "Misc/EHandle.hpp"

class C_LocalPlayer
{
    friend bool operator==(const C_LocalPlayer& lhs, void* rhs);
public:
    C_LocalPlayer() : m_local(nullptr) {}

    operator bool() const { return *m_local != nullptr; }
    operator C_BasePlayer*() const { return *m_local; }

    C_BasePlayer* operator->() { return *m_local; }

private:
    C_BasePlayer** m_local;
};

extern C_LocalPlayer pLocal;

