package pl.smq.example.publishers

import pl.smq.example.Action
import pl.smq.lib.SMQ
import java.io.File

@Suppress("OPT_IN_USAGE")
class PublisherDevRandom() : Action {
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
        println("Sending messages...")
        val file = File("/dev/random")
        while (true) {
            if (Thread.interrupted()) {
                break
            }
            val cbuf = CharArray(10000)
            file.reader().read(cbuf, 0, 10000)
            try {
                queue.sendMessage(cbuf.joinToString(""))
            } catch (e: Exception) {
                println(e.message)
                break
            }
        }
        queue.unregisterAsPublisher()
        println("Unregistered as publisher")
        smq.disconnect()
    }
}
