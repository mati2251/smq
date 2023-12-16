@file:OptIn(ExperimentalCli::class)
package pl.smq.example

import kotlinx.cli.ArgType
import kotlinx.cli.ExperimentalCli
import kotlinx.cli.Subcommand
import kotlinx.cli.default
import pl.smq.lib.SMQ

class Publisher (): Subcommand("pub", "Publisher") {
    val port by option(ArgType.Int, shortName = "p", description = "Port").default(3000)
    val host by option(ArgType.String, shortName = "H", description = "Host").default("localhost")
    val topic by option(ArgType.String, shortName = "t", description = "Topic").default("test")

    override fun execute() {
        val smq = SMQ(host, port)
        val queue = smq.messageQueue(topic)
        queue.registerAsPublisher()
        while (true) {
            val randomString = ('a'..'z').map { it }.shuffled().subList(0, 10).joinToString("")
            Thread.sleep(1000)
            queue.sendMessage(randomString)
        }
    }
}
