@file:OptIn(ExperimentalCli::class)

package pl.smq.example

import kotlinx.cli.*
import pl.smq.lib.SMQ

class Subscriber()  : Subcommand("sub", "Subscriber") {
    val port by option(ArgType.Int, shortName = "p", description = "Port").default(3000)
    val host by option(ArgType.String, shortName = "H", description = "Host").default("localhost")
    val topic by option(ArgType.String, shortName = "t", description = "Topic").default("test")

    override fun execute() {
        val smq = SMQ(host, port)
        val queue = smq.messageQueue(topic)
        queue.registerAsSubscriber()
        queue.addMessageListener { message ->
            println("Received message: $message")
        }
        println("Subscriber")
        while (true) {
            Thread.sleep(1000)
        }
    }
}
