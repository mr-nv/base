#include "sdk.hpp"

#include "../Helpers/Utils.hpp"

IVEngineClient*       g_pEngine   = nullptr;
IBaseClientDLL*       g_pClient      = nullptr;
IClientEntityList*    g_pEntityList     = nullptr;
CGlobalVarsBase*      g_pGlobals     = nullptr;
IEngineTrace*         g_pEngineTrace    = nullptr;
ICvar*                g_pCVar           = nullptr;
IPanel*               g_pPanel      = nullptr;
IClientMode*          g_pClientMode     = nullptr;
IVDebugOverlay*       g_pDebugOverlay   = nullptr;
ISurface*             g_pSurface    = nullptr;
CInput*               g_pInput          = nullptr;
IVModelInfoClient*    g_pMDLInfo        = nullptr;
IVModelRender*        g_pMDLRender      = nullptr;
IVRenderView*         g_pRenderView     = nullptr;
IMaterialSystem*      g_pMaterialSystem      = nullptr;
IGameEventManager2*   g_pGameEventManager     = nullptr;
IMoveHelper*          g_pMoveHelper     = nullptr;
IMDLCache*            g_pMDLCache       = nullptr;
IPrediction*          g_pPrediction     = nullptr;
CGameMovement*        g_pGameMovement   = nullptr;
IEngineSound*         g_pEngineSound    = nullptr;
CGlowObjectManager*   g_pGlowObjectManager = nullptr;
IViewRender*          g_pViewRender     = nullptr;
IDirect3DDevice9*     g_pD3DDevice9     = nullptr;
CClientState*         g_pClientState    = nullptr;
IPhysicsSurfaceProps* g_pPhysicsSurface    = nullptr;
C_LocalPlayer         pLocal;

namespace Interfaces
{
    CreateInterfaceFn get_module_factory(HMODULE module)
    {
        return reinterpret_cast<CreateInterfaceFn>(GetProcAddress(module, "CreateInterface"));
    }

    template<typename T>
    T* get_interface(CreateInterfaceFn f, const char* szInterfaceVersion)
    {
        auto result = reinterpret_cast<T*>(f(szInterfaceVersion, nullptr));

        if(!result) {
            throw std::runtime_error(std::string("[get_interface] Failed to GetOffset interface: ") + szInterfaceVersion);
        }

        return result;
    }

    void Initialize()
    {
        auto engineFactory    = get_module_factory(GetModuleHandleW(L"engine.dll"));
        auto clientFactory    = get_module_factory(GetModuleHandleW(L"client.dll"));
        auto valveStdFactory  = get_module_factory(GetModuleHandleW(L"vstdlib.dll"));
        auto vguiFactory      = get_module_factory(GetModuleHandleW(L"vguimatsurface.dll"));
        auto vgui2Factory     = get_module_factory(GetModuleHandleW(L"vgui2.dll"));
        auto matSysFactory    = get_module_factory(GetModuleHandleW(L"materialsystem.dll"));
        auto dataCacheFactory = get_module_factory(GetModuleHandleW(L"datacache.dll"));
        auto vphysicsFactory  = get_module_factory(GetModuleHandleW(L"vphysics.dll"));
        
        g_pClient           = get_interface<IBaseClientDLL>      (clientFactory   , "VClient018");
        g_pEntityList          = get_interface<IClientEntityList>   (clientFactory   , "VClientEntityList003");
        g_pPrediction          = get_interface<IPrediction>         (clientFactory   , "VClientPrediction001");
        g_pGameMovement        = get_interface<CGameMovement>       (clientFactory   , "GameMovement001");
        g_pMDLCache            = get_interface<IMDLCache>           (dataCacheFactory, "MDLCache004");
        g_pEngine        = get_interface<IVEngineClient>      (engineFactory   , "VEngineClient014");
        g_pMDLInfo             = get_interface<IVModelInfoClient>   (engineFactory   , "VModelInfoClient004");
        g_pMDLRender           = get_interface<IVModelRender>       (engineFactory   , "VEngineModel016");
        g_pRenderView          = get_interface<IVRenderView>        (engineFactory   , "VEngineRenderView014");
        g_pEngineTrace         = get_interface<IEngineTrace>        (engineFactory   , "EngineTraceClient004");
        g_pDebugOverlay        = get_interface<IVDebugOverlay>      (engineFactory   , "VDebugOverlay004");
        g_pGameEventManager          = get_interface<IGameEventManager2>  (engineFactory   , "GAMEEVENTSMANAGER002");
        g_pEngineSound         = get_interface<IEngineSound>        (engineFactory   , "IEngineSoundClient003");
        g_pMaterialSystem           = get_interface<IMaterialSystem>     (matSysFactory   , "VMaterialSystem080");
        g_pCVar                = get_interface<ICvar>               (valveStdFactory , "VEngineCvar007");
        g_pPanel           = get_interface<IPanel>              (vgui2Factory    , "VGUI_Panel009");
        g_pSurface         = get_interface<ISurface>            (vguiFactory     , "VGUI_Surface031");
        g_pPhysicsSurface         = get_interface<IPhysicsSurfaceProps>(vphysicsFactory , "VPhysicsSurfaceProps001");

        auto client = "client.dll";
        auto engine = "engine.dll";
        auto dx9api = "shaderapidx9.dll";

        g_pGlobals      =  **(CGlobalVarsBase***)(Utils::PatternScan(client, "A1 ? ? ? ? 5E 8B 40 10") + 1);
        g_pClientMode      =        *(IClientMode**)(Utils::PatternScan(client, "A1 ? ? ? ? 8B 80 ? ? ? ? 5D") + 1);
        g_pInput           =             *(CInput**)(Utils::PatternScan(client, "B9 ? ? ? ? 8B 40 38 FF D0 84 C0 0F 85") + 1);
        g_pMoveHelper      =      **(IMoveHelper***)(Utils::PatternScan(client, "8B 0D ? ? ? ? 8B 45 ? 51 8B D4 89 02 8B 01") + 2);
        g_pGlowObjectManager  = *(CGlowObjectManager**)(Utils::PatternScan(client, "0F 11 05 ? ? ? ? 83 C8 01") + 3);
        g_pViewRender      =        *(IViewRender**)(Utils::PatternScan(client, "A1 ? ? ? ? B9 ? ? ? ? C7 05 ? ? ? ? ? ? ? ? FF 10") + 1);
        g_pD3DDevice9      = **(IDirect3DDevice9***)(Utils::PatternScan(dx9api, "A1 ? ? ? ? 50 8B 08 FF 51 0C") + 1);
        g_pClientState     =     **(CClientState***)(Utils::PatternScan(engine, "A1 ? ? ? ? 8B 80 ? ? ? ? C3") + 1);

        pLocal     = *(C_LocalPlayer*)(Utils::PatternScan(client, "8B 0D ? ? ? ? 83 FF FF 74 07") + 2);
    }

    void Dump()
    {
        // Ugly macros ugh
        #define STRINGIFY_IMPL(s) #s
        #define STRINGIFY(s)      STRINGIFY_IMPL(s)
        #define PRINT_INTERFACE(name) Utils::ConsolePrint("%-20s: [0x%p]\n", STRINGIFY(name), name)

        PRINT_INTERFACE(g_pClient			);
        PRINT_INTERFACE(g_pEntityList		);
        PRINT_INTERFACE(g_pPrediction		);
        PRINT_INTERFACE(g_pGameMovement		);
        PRINT_INTERFACE(g_pMDLCache			);
        PRINT_INTERFACE(g_pEngine			);
        PRINT_INTERFACE(g_pMDLInfo			);
        PRINT_INTERFACE(g_pMDLRender		);
        PRINT_INTERFACE(g_pRenderView		);
        PRINT_INTERFACE(g_pEngineTrace		);
        PRINT_INTERFACE(g_pDebugOverlay		);
        PRINT_INTERFACE(g_pGameEventManager	);
        PRINT_INTERFACE(g_pEngineSound		);
        PRINT_INTERFACE(g_pMaterialSystem	);
        PRINT_INTERFACE(g_pCVar				);
        PRINT_INTERFACE(g_pPanel			);
        PRINT_INTERFACE(g_pSurface			);
        PRINT_INTERFACE(g_pPhysicsSurface	);
		PRINT_INTERFACE(g_pViewRender		);
    }
}