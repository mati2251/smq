package pl.smq.lib.models

import pl.smq.lib.exceptions.InvalidResponseException

data class Response(
    val requestId: Int,
    val code: Int,
    val message: String,
) {
    companion object {
        fun deserialize(serialized: String): Response {
            val lines = serialized.split("\n")
            if (lines.size != 4) {
                throw InvalidResponseException("Response should have 4 lines but has ${lines.size}")
            }
            if(lines[0] != ExchangeType.RESPONSE.toString()) {
                throw InvalidResponseException("Response should start with RESPONSE at first line")
            }
            return try {
                Response(lines[1].toInt(), lines[2].toInt(), lines[3])
            } catch (e: NumberFormatException) {
                throw InvalidResponseException("Response should have integer code at second line and integer requestId at third line")
            }
        }
    }
}
