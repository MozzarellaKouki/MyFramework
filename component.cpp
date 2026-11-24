#include "component.h"
#include "spatialObject.h"
#include "debugUtils.h"

#include <cassert>
#include <string>

SpatialObject* Component::FindSpatialOwner() const
{
    //GameObject* から SpatialObject* への変換。毎フレームではなく Init で1回だけ使う想定
    return dynamic_cast<SpatialObject*>(m_Owner);
}

SpatialObject* Component::RequireSpatialOwner(const char* componentName) const
{
    SpatialObject* owner = FindSpatialOwner();

    if (owner == nullptr)
    {
        const std::string message = std::string("[") + componentName +
            "] 位置を持たない GameObject に追加されました。SpatialObject に追加してください。\n";
        DebugLog(message.c_str());
        assert(!"位置が必要なコンポーネントが SpatialObject 以外に追加されました（出力ウィンドウ参照）");
    }

    return owner;
}

Transform* Component::RequireTransform(const char* componentName) const
{
    SpatialObject* owner = RequireSpatialOwner(componentName);
    return owner ? owner->GetTransform() : nullptr;
}
