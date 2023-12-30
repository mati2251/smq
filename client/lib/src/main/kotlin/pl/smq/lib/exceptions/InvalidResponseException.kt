package pl.smq.lib.exceptions

class InvalidResponseException(what: String) : RuntimeException("Invalid response: $what")

