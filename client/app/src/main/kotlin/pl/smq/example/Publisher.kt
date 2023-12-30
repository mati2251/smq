package pl.smq.example

import kotlinx.coroutines.delay
import pl.smq.lib.SMQ

@Suppress("OPT_IN_USAGE")
class Publisher() : Subcommand {
    override suspend fun execute() {
        val smq = SMQ("localhost", 3000)
        val queue = smq.messageQueue("test")
        val res = queue.registerAsPublisher()
        if (res.code == 0) {
            println("Registered as publisher")
        } else {
            println("Failed to register as publisher")
            return
        }
        while (true) {
            queue.sendMessage("Hello World!")
            delay(1000)
        }
    }
}
