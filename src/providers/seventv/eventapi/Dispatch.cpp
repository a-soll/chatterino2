#include "providers/seventv/eventapi/Dispatch.hpp"

#include <utility>

namespace chatterino::seventv::eventapi {

Dispatch::Dispatch(QJsonObject obj)
    : type(magic_enum::enum_cast<SubscriptionType>(
               obj["type"].toString().toStdString())
               .value_or(SubscriptionType::INVALID))
    , body(obj["body"].toObject())
    , id(this->body["id"].toString())
    , actorName(this->body["actor"].toObject()["display_name"].toString())
{
}

EmoteAddDispatch::EmoteAddDispatch(const Dispatch &dispatch, QJsonObject emote)
    : emoteSetID(dispatch.id)
    , actorName(dispatch.actorName)
    , emoteJson(std::move(emote))
    , emoteID(this->emoteJson["id"].toString())
{
}

bool EmoteAddDispatch::validate() const
{
    bool validValues =
        !this->emoteSetID.isEmpty() && !this->emoteJson.isEmpty();
    if (!validValues)
    {
        return false;
    }
    bool validActiveEmote = this->emoteJson.contains("id") &&
                            this->emoteJson.contains("name") &&
                            this->emoteJson.contains("data");
    if (!validActiveEmote)
    {
        return false;
    }
    auto emoteData = this->emoteJson["data"].toObject();
    return emoteData.contains("name") && emoteData.contains("host") &&
           emoteData.contains("owner");
}

EmoteRemoveDispatch::EmoteRemoveDispatch(const Dispatch &dispatch,
                                         QJsonObject emote)
    : emoteSetID(dispatch.id)
    , actorName(dispatch.actorName)
    , emoteName(emote["name"].toString())
    , emoteID(emote["id"].toString())
{
}

bool EmoteRemoveDispatch::validate() const
{
    return !this->emoteSetID.isEmpty() && !this->emoteName.isEmpty() &&
           !this->emoteID.isEmpty();
}

EmoteUpdateDispatch::EmoteUpdateDispatch(const Dispatch &dispatch,
                                         QJsonObject oldValue,
                                         QJsonObject value)
    : emoteSetID(dispatch.id)
    , actorName(dispatch.actorName)
    , emoteID(value["id"].toString())
    , oldEmoteName(oldValue["name"].toString())
    , emoteName(value["name"].toString())
{
}

bool EmoteUpdateDispatch::validate() const
{
    return !this->emoteSetID.isEmpty() && !this->emoteID.isEmpty() &&
           !this->oldEmoteName.isEmpty() && !this->emoteName.isEmpty() &&
           this->oldEmoteName != this->emoteName;
}

UserConnectionUpdateDispatch::UserConnectionUpdateDispatch(
    const Dispatch &dispatch, const QJsonObject &update, size_t connectionIndex)
    : userID(dispatch.id)
    , actorName(dispatch.actorName)
    , oldEmoteSetID(update["old_value"].toObject()["id"].toString())
    , emoteSetID(update["value"].toObject()["id"].toString())
    , connectionIndex(connectionIndex)
{
}

bool UserConnectionUpdateDispatch::validate() const
{
    return !this->userID.isEmpty() && !this->oldEmoteSetID.isEmpty() &&
           !this->emoteSetID.isEmpty();
}

}  // namespace chatterino::seventv::eventapi
