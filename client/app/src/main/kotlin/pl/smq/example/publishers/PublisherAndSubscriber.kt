package pl.smq.example.publishers

import kotlinx.coroutines.DelicateCoroutinesApi
import kotlinx.coroutines.delay
import pl.smq.example.Action
import pl.smq.lib.SMQ

class PublisherAndSubscriber : Action {
    @OptIn(DelicateCoroutinesApi::class)
    override suspend fun execute(host: String, port: Int, topic: String) {
        val smq = SMQ(host, port)
        smq.connect()
        val queue = smq.messageQueue(topic)
        val queue2 = smq.messageQueue(topic + "2")
        try {
            queue2.registerAsSubscriber()
            println("Registered as subscriber (topic: $topic)")
            queue.registerAsPublisher()
            println("Registered as publisher (topic: $topic)")
        } catch (e: Exception) {
            println("Error while registering")
            println(e.message)
            return
        }
        queue2.addMessageListener { println(it) }
        while (true) {
            val randomString = (1..10).map { "abcdefghijklmnopqrstuvwxyzABCD@$#*123456789".random() }.joinToString("", "", "")
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
