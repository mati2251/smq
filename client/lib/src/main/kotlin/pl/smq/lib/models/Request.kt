package pl.smq.lib.models

import pl.smq.lib.exceptions.InvalidRequestException

data class Request(
    val type: RequestType,
    val id: Int,
    val topic: String,
    var body: String,
) {
    fun serialize(): String {
        body = body.replace("\n", "\r")
        return "$type\n$id\n$topic\n$body\n"
    }

    companion object {
        fun deserialize(req: String): Request {
            val lines = req.split("\n")
            if (lines.size != 4) {
                throw InvalidRequestException("Request should have 4 lines but has ${lines.size}")
            }
            val body = lines[3].replace("\u0001", "\n")
            return Request(
                RequestType.valueOf(lines[0]),
                lines[1].toInt(),
                lines[2],
                body
            )
        }
    }
}
