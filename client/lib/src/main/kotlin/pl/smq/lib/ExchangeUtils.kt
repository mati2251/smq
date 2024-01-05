package pl.smq.lib

import pl.smq.lib.exceptions.InvalidExchangeException
import pl.smq.lib.models.ExchangeType
import pl.smq.lib.models.Request
import pl.smq.lib.models.Response

object ExchangeUtils {
    fun isWholeExchange(exchange: String): Boolean {
        return exchange.contains("\n\n")
    }

    fun splitExchange(exchange: String): Pair<String, String> {
        val endOfRequest = exchange.indexOf("\n\n")
        return Pair(exchange.substring(0, endOfRequest), exchange.substring(endOfRequest + 2))
    }

    fun checkType(exchange: String): ExchangeType {
        val firstNewLineIndex = exchange.indexOf("\n")
        if (firstNewLineIndex == -1) {
            throw InvalidExchangeException("No new line in exchange after type")
        }
        val type = exchange.substring(0, exchange.indexOf("\n"))
        if (type == ExchangeType.RESPONSE.toString()) return ExchangeType.RESPONSE
        return ExchangeType.REQUEST
    }

    fun deserializeResponse(exchange: String): Response {
        return Response.deserialize(exchange)
    }

    fun deserializeRequest(exchange: String): Request {
        return Request.deserialize(exchange)
    }
}
