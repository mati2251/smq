package pl.smq.lib.models

data class Request(
    val type: RequestType,
    val body: String
)
