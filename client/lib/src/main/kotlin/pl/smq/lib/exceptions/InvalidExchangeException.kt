package pl.smq.lib.exceptions

class InvalidExchangeException(what: String) : RuntimeException("Invalid exchange: ${what}")
