package pl.smq.example.publishers

import pl.smq.example.Action
import pl.smq.lib.SMQ

@Suppress("OPT_IN_USAGE")
class PublisherOnce() : Action {
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
        val randomString = (1..10).map { "abcdefghijklmnopqrstuvwxyzABCD@$#*123456789".random() }.joinToString("", "", "")
        try {
            queue.sendMessage(randomString)
            println("Sent message: $randomString")
        } catch (e: Exception) {
            println("Error while sending message")
            println(e.message)
        }
        queue.unregisterAsPublisher()
        smq.disconnect()
    }
}
