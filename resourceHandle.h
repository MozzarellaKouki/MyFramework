#pragma once
#include <cstdint>

//--------------------------------------------------------------
// リソースハンドル（リソースの「整理番号」）
//
// index      : プールの何番目のスロットか
// generation : そのスロットの何代目のリソースか
//
// リソースが解放されるとスロットの世代が進むため、
// 古いハンドルで Get すると nullptr が返る（解放済みメモリを触らない）。
//
// ハンドルはただの数値なので、コピーも保存も自由にしてよい。
//--------------------------------------------------------------
template<typename T> struct Handle
{
    static constexpr uint32_t InvalidIndex = 0xFFFFFFFFu;

    uint32_t index = InvalidIndex;
    uint32_t generation = 0;

    //番号を持っているか（中身がまだ生きているかは ResourceManager::Get で確認する）
    bool IsValid() const { return index != InvalidIndex; }

    bool operator==(const Handle&) const = default;
};

//--------------------------------------------------------------
// リソースの寿命（スコープ）
//--------------------------------------------------------------
enum class ResourceScope
{
    Global, //アプリ終了まで残る（フェード、フォント、共通UIなど）
    Scene,  //シーン切り替え時に解放される（ステージのモデルや背景など）
};
