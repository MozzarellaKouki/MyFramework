#include "component.h"
#include "gameObject.h"
#include "debugUtils.h"

#include <cassert>
#include <string>

Transform* Component::RequireTransform(const char* componentName) const
{
    Transform* transform = m_Owner ? m_Owner->GetTransform() : nullptr;

    if (transform == nullptr)
    {
        const std::string message = std::string("[") + componentName +
            "] 位置を持たない GameObject に追加されました。SpatialObject に追加してください。\n";
        DebugLog(message.c_str());
        assert(!"位置が必要なコンポーネントが SpatialObject 以外に追加されました（出力ウィンドウ参照）");
    }

    return transform;
}
