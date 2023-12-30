package pl.smq.lib.exceptions

class InvalidRequestException(what: String) : RuntimeException("Invalid request: $what")

