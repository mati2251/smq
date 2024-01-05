package pl.smq.example

import kotlinx.coroutines.DelicateCoroutinesApi
import kotlinx.coroutines.channels.BufferOverflow
import pl.smq.lib.SMQ

class Subscriber() : Subcommand {
    @OptIn(DelicateCoroutinesApi::class)
    override suspend fun execute() {
        val smq = SMQ("localhost", 3000)
        val queue = smq.messageQueue("test", 10, BufferOverflow.DROP_OLDEST)
        val res = queue.registerAsSubscriber()
        if (res.code == 0) {
            println("Registered as subscriber")
        } else {
            println("Failed to register as subscriber")
            return
        }
        while (true) {
            val msg = queue.readMessages()
            println("Received message: $msg")
        }
    }
}
