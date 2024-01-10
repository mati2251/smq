package pl.smq.example.subscribers

import kotlinx.coroutines.DelicateCoroutinesApi
import kotlinx.coroutines.channels.BufferOverflow
import pl.smq.example.Action
import pl.smq.lib.SMQ

class SubscriberOnce() : Action {
    @OptIn(DelicateCoroutinesApi::class)
    override suspend fun execute(host: String, port: Int, topic: String) {
        val smq = SMQ(host, port)
        smq.connect()
        val queue = smq.messageQueue(topic, 10, BufferOverflow.DROP_OLDEST)
        val res = queue.registerAsSubscriber()
        println(res)
        if (res.code == 0) {
            println("Registered as subscriber (topic: $topic)")
        } else {
            println("Failed to register as subscriber")
            return
        }
        val msg = queue.readMessages()
        println("Message recievd: $msg")
        queue.unregisterAsSubscriber()
        smq.disconnect()
    }
}
