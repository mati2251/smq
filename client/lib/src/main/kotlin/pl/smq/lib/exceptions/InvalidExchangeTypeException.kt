package pl.smq.lib.exceptions

class InvalidExchangeTypeException(type: String) : RuntimeException("Invalid exchange type: ${type}")
