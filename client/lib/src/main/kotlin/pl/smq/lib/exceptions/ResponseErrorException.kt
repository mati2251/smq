package pl.smq.lib.exceptions

import pl.smq.lib.models.Response

class ResponseErrorException(val res: Response) : RuntimeException("Response error: ${res.code} ${res.message}")
