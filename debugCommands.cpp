#include "debugCommands.h"
#include "debugUtils.h"

#include <cstdio>
#include <string>
#include <vector>

#ifdef _DEBUG

namespace
{
    struct DebugCommand
    {
        Keyboard_Keys key;
        std::string name;
        std::function<void()> action;
        DebugScope scope;
        bool hold;
    };

    std::vector<DebugCommand> s_Commands;
    bool s_DefaultsRegistered = false;

    void Add(Keyboard_Keys key, const char* name, std::function<void()> action,
        DebugScope scope, bool hold)
    {
        s_Commands.push_back({ key, name, std::move(action), scope, hold });
    }

    void RegisterDefaults()
    {
        if (s_DefaultsRegistered)
            return;

        s_DefaultsRegistered = true;
        Add(KK_F1, "コマンド一覧を表示", [] { DebugCommands::PrintList(); },
            DebugScope::Global, false);
    }
}

void DebugCommands::AddTrigger(Keyboard_Keys key, const char* name,
    std::function<void()> action, DebugScope scope)
{
    RegisterDefaults();
    Add(key, name, std::move(action), scope, false);
}

void DebugCommands::AddHold(Keyboard_Keys key, const char* name,
    std::function<void()> action, DebugScope scope)
{
    RegisterDefaults();
    Add(key, name, std::move(action), scope, true);
}

void DebugCommands::ClearScene()
{
    std::erase_if(s_Commands,
        [](const DebugCommand& c) { return c.scope == DebugScope::Scene; });
}

void DebugCommands::Clear()
{
    s_Commands.clear();
    s_DefaultsRegistered = false;
}

void DebugCommands::Update()
{
    RegisterDefaults();

    //コマンド内でシーン変更などが行われても安全なようにコピーして回す
    const std::vector<DebugCommand> commands = s_Commands;

    for (const auto& command : commands)
    {
        const bool pressed = command.hold
            ? Keyboard_IsKeyDown(command.key)
            : Keyboard_IsKeyDownTrigger(command.key);

        if (!pressed)
            continue;

        if (!command.hold)
        {
            DebugLog(("[Debug] " + command.name + "\n").c_str());
        }

        command.action();
    }
}

void DebugCommands::PrintList()
{
    DebugLog("---- Debug Commands ----\n");

    for (const auto& command : s_Commands)
    {
        std::string line = "  key 0x";
        char hex[8];
        sprintf_s(hex, "%02X", static_cast<int>(command.key));
        line += hex;
        line += command.scope == DebugScope::Global ? " [Global] " : " [Scene]  ";
        line += command.hold ? "(hold) " : "";
        line += command.name;
        line += "\n";
        DebugLog(line.c_str());
    }
}

#else //Release: 何もしない

void DebugCommands::AddTrigger(Keyboard_Keys, const char*, std::function<void()>, DebugScope) {}
void DebugCommands::AddHold(Keyboard_Keys, const char*, std::function<void()>, DebugScope) {}
void DebugCommands::ClearScene() {}
void DebugCommands::Clear() {}
void DebugCommands::Update() {}
void DebugCommands::PrintList() {}

#endif
