package pl.smq.example.subscribers

import kotlinx.coroutines.DelicateCoroutinesApi
import kotlinx.coroutines.channels.BufferOverflow
import pl.smq.example.Action
import pl.smq.lib.SMQ
import java.lang.Thread.sleep

class SubscriberBlocked : Action {
    @OptIn(DelicateCoroutinesApi::class)
    override suspend fun execute(host: String, port: Int, topic: String) {
        val smq = SMQ(host, port)
        smq.connect()
        val queue = smq.messageQueue(topic, 1, BufferOverflow.SUSPEND)
        try {
            queue.registerAsSubscriber()
        } catch (e: Exception) {
            println("Failed to register as subscriber")
            return
        }
        sleep(20000)
        var msg = queue.readMessage()
        println("Received message: $msg")
        msg = queue.readMessage()
        println("Received message: $msg")
        queue.unregisterAsSubscriber()
        smq.disconnect()
    }
}
