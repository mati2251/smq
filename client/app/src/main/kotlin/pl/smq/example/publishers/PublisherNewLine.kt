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
        val res = queue.registerAsPublisher()
        if (res.code == 0) {
            println("Registered as publisher (topic: $topic)")
        } else {
            println("Failed to register as publisher")
            return
        }
        while (true) {
            if (Thread.interrupted()) {
                break
            }
            var randomString = (1..10).map{ "abcdefghijklmnopqrstuvwxyzABCD@$#*123456789".random() }.joinToString { "" }
            randomString = randomString + "\n\n" + randomString
            println("Sending message: $randomString")
            queue.sendMessage(randomString)
            delay(1000)
        }
        queue.unregisterAsPublisher()
        println("Unregistered as publisher")
        smq.disconnect()
    }
}
