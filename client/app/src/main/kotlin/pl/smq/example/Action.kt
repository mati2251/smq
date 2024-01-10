package pl.smq.example

interface Action {
    suspend fun execute(host: String, port: Int, topic: String)
}
