#pragma once
#include "keyboard.h"
#include <functional>

//--------------------------------------------------------------
// デバッグコマンド
//
// キー入力で実行するデバッグ処理・テスト処理をここに登録する。
// Debug ビルドでのみ動作し、Release ビルドでは何もしない。
//
// Global : アプリ全体で常に有効（ManagerのInit登録）
// Scene  : シーン切り替え時に自動で解除される（各シーンのInitで登録）
//
// 例: DebugCommands::AddTrigger(KK_V, "敵を生成", [this]{ ... });
//
// F1 で登録中のコマンド一覧を出力ウィンドウに表示する。
//--------------------------------------------------------------

enum class DebugScope
{
    Global,
    Scene,
};

class DebugCommands
{
public:

    // 押した瞬間に1回実行
    static void AddTrigger(Keyboard_Keys key, const char* name,
        std::function<void()> action, DebugScope scope = DebugScope::Scene);

    // 押している間、毎フレーム実行
    static void AddHold(Keyboard_Keys key, const char* name,
        std::function<void()> action, DebugScope scope = DebugScope::Scene);

    // Scene スコープのコマンドを全て解除（SceneManager が呼ぶ）
    static void ClearScene();

    // 全コマンドを解除
    static void Clear();

    // キー入力を確認してコマンドを実行（Manager::Update から呼ぶ）
    static void Update();

    // 登録中のコマンド一覧を出力ウィンドウに表示
    static void PrintList();
};
