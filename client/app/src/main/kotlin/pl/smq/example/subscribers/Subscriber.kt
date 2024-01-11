package pl.smq.example.subscribers

import kotlinx.coroutines.DelicateCoroutinesApi
import kotlinx.coroutines.channels.BufferOverflow
import pl.smq.example.Action
import pl.smq.lib.SMQ

class Subscriber() : Action {
    @OptIn(DelicateCoroutinesApi::class)
    override suspend fun execute(host: String, port: Int, topic: String) {
        val smq = SMQ(host, port)
        smq.connect()
        val queue = smq.messageQueue(topic, 10, BufferOverflow.DROP_OLDEST)
        try {
            queue.registerAsSubscriber()
            println("Registered as subscriber (topic: $topic)")
        } catch (e: Exception) {
            println("Failed to register as subscriber")
            return
        }
        while (true) {
            val msg = queue.readMessage()
            println("Received message: $msg")
        }
    }
}
