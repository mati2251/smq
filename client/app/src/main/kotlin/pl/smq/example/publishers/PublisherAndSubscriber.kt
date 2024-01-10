package pl.smq.example.publishers

import kotlinx.coroutines.DelicateCoroutinesApi
import kotlinx.coroutines.delay
import pl.smq.example.Action
import pl.smq.lib.SMQ

class PublisherAndSubscriber: Action {
    @OptIn(DelicateCoroutinesApi::class)
    override suspend fun execute(host: String, port: Int, topic: String) {
        val smq = SMQ(host, port)
        smq.connect()
        val queue = smq.messageQueue(topic)
        val queue2 = smq.messageQueue(topic + "2")
        var res =  queue2.registerAsSubscriber()
        if (res.code == 0) {
            println("Registered as subscriber (topic: $topic)")
        } else {
            println("Failed to register as subscriber")
            return
        }
        res = queue.registerAsPublisher()
        if (res.code == 0) {
            println("Registered as publisher (topic: $topic)")
        } else {
            println("Failed to register as publisher")
            return
        }
        queue2.addMessageListener { println(it) }
        while (true) {
            val randomString = (1..10).map{ "abcdefghijklmnopqrstuvwxyzABCD@$#*123456789".random() }.joinToString()
            println("Sending message: $randomString")
            queue.sendMessage(randomString)
            delay(1000)
        }
    }
}
