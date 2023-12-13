package pl.smq.lib.models

import kotlinx.serialization.Serializable

@Serializable
data class Message(
    val topic: String,
    val content: String
)
