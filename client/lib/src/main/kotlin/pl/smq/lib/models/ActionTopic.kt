package pl.smq.lib.models

import kotlinx.serialization.Serializable

@Serializable
data class ActionTopic(
    val action: ActionTopicType,
    val topic: String
)
