package pl.smq.example.publishers

import kotlinx.coroutines.delay
import pl.smq.example.Action
import pl.smq.lib.SMQ

@Suppress("OPT_IN_USAGE")
class PublisherNewLine() : Action {
    override suspend fun execute(host: String, port: Int, topic: String) {
        val smq = SMQ(host, port)
        smq.connect()
        val queue = smq.messageQueue(topic)
        try {
            queue.registerAsPublisher()
        } catch (e: Exception) {
            println("Error while registering as publisher")
            println(e.message)
            return
        }
        while (true) {
            var randomString = (1..10).map { "abcdefghijklmnopqrstuvwxyzABCD@$#*123456789".random() }.jointToString()
            randomString = randomString + "\n\n" + randomString
            println("Sending message: $randomString")
            try {
                queue.sendMessage(randomString)
            } catch (e: Exception) {
                println("Error while sending message")
                println(e.message)
            }
            delay(1000)
        }
    }
}
