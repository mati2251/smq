package pl.smq.example.publishers

import pl.smq.example.Action
import pl.smq.lib.SMQ

@Suppress("OPT_IN_USAGE")
class PublisherOnce() : Action {
    override suspend fun execute(host: String, port: Int, topic: String) {
        val smq = SMQ(host, port)
        smq.connect()
        val queue = smq.messageQueue(topic)
        var res = queue.registerAsPublisher()
        if (res.code == 0) {
            println("Registered as publisher (topic: $topic)")
        } else {
            println("Failed to register as publisher")
            return
        }
        val randomString = (1..10).map{ "abcdefghijklmnopqrstuvwxyzABCD@$#*123456789".random() }.joinToString()
        res = queue.sendMessage(randomString)
        println(res)
        queue.unregisterAsPublisher()
        smq.disconnect()
    }
}
