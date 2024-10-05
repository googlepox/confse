#include "obse/GameAPI.h"
#include "obse/ParamInfos.h"
#include "obse/PluginAPI.h"

#include "commands.hpp"
#include "definitions.hpp"

extern "C"
{
    bool OBSEPlugin_Query(const OBSEInterface *obse, PluginInfo *info)
    {
        LOGMESSAGE("OBSEPlugin_Query running");

        info->infoVersion = PluginInfo::kInfoVersion;
        info->name = OBSEPLUGIN_NAME;
        info->version = OBSEPLUGIN_VERSION;

        if (obse->isEditor)
        {
            LOGMESSAGE("editor mode, skip other checks)");
        }
        else
        {
            if (obse->obseVersion < OBSE_VERSION_INTEGER)
            {
                LOGMESSAGE("OBSE version is too old, current is %08X, expected >= %08X", obse->obseVersion,
                           OBSE_VERSION_INTEGER);
                return false;
            }
            else if (obse->oblivionVersion != OBLIVION_VERSION)
            {
                LOGMESSAGE("Invalid Oblivion version, current is %08X, plugin needs %08X", obse->oblivionVersion,
                           OBLIVION_VERSION);
                return false;
            }
            else if ((kOBSEScript = (OBSEScriptInterface *)obse->QueryInterface(kInterface_Script)) == NULL)
            {
                LOGMESSAGE("Failed to acquire ScriptInterface");
                return false;
            }
            else if ((kOBSEStringVar = (OBSEStringVarInterface *)obse->QueryInterface(kInterface_StringVar)) == NULL)
            {
                LOGMESSAGE("Failed to acquire StringVarInterface");
                return false;
            }
            else if ((kOBSEArrayVar = (OBSEArrayVarInterface *)obse->QueryInterface(kInterface_ArrayVar)) == NULL)
            {
                LOGMESSAGE("Failed to acquire ArrayVarInterface");
                return false;
            }
        }

        LOGMESSAGE("OBSEPlugin_Query passed");
        return true;
    }

    bool OBSEPlugin_Load(const OBSEInterface *obse)
    {
        LOGMESSAGE("OBSEPlugin_Load running");

        kPluginHandle = obse->GetPluginHandle();

        if (!obse->isEditor)
        {
            kOblivionDirectory = std::filesystem::canonical(std::filesystem::path(obse->GetOblivionDirectory()));
            LOGMESSAGE("Game directory: %s", kOblivionDirectory.string().c_str());
        }

        obse->SetOpcodeBase(OBSEPLUGIN_OPCODE_BASE);

        
        obse->RegisterTypedCommand(&commandinfo_get_string, kRetnType_String);
        obse->RegisterCommand(&commandinfo_set_string);

        obse->RegisterCommand(&commandinfo_get_float);
        obse->RegisterCommand(&commandinfo_set_float);

        obse->RegisterCommand(&commandinfo_get_int);
        obse->RegisterCommand(&commandinfo_set_int);

        obse->RegisterTypedCommand(&commandinfo_get_form, kRetnType_Form);
        obse->RegisterCommand(&commandinfo_set_form);

        obse->RegisterCommand(&commandinfo_erase_key);

        obse->RegisterTypedCommand(&commandinfo_list_keys, kRetnType_Array);

        LOGMESSAGE("OBSEPlugin_Load finished");
        return true;
    }
};
