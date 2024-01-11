package pl.smq.example.publishers

import kotlinx.coroutines.delay
import pl.smq.example.Action
import pl.smq.lib.SMQ

@Suppress("OPT_IN_USAGE")
class Publisher() : Action {
    override suspend fun execute(host: String, port: Int, topic: String) {
        val smq = SMQ(host, port)
        smq.connect()
        val queue = smq.messageQueue(topic)
        try {
            queue.registerAsPublisher()
            println("Registered as publisher (topic: $topic)")
        } catch (e: Exception) {
            println("Error while registering as publisher")
            println(e.message)
            return
        }
        while (true) {
            if (Thread.interrupted()) {
                break
            }
            val randomString = (1..10).map { "abcdefghijklmnopqrstuvwxyzABCD@$#*123456789".random() }.joinToString()
            println("Sending message: $randomString")
            try {
                queue.sendMessage(randomString)
            } catch (e: Exception) {
                println("Error while sending message")
                println(e.message)
            }
            delay(1000)
        }
        queue.unregisterAsPublisher()
        println("Unregistered as publisher")
        smq.disconnect()
    }
}
