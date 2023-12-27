package pl.smq.example

import pl.smq.lib.SMQ

class Publisher() : Subcommand {
    override fun execute() {
        val smq = SMQ("localhost", 3000)
        val queue = smq.messageQueue("test")
        queue.addMessageListener { message ->
            println("Received message: $message")
        }
        queue.registerAsPublisher()
        while (true) {
            val randomString = ('a'..'z').map { it }.shuffled().subList(0, 10).joinToString("")
            queue.sendMessage(randomString)
            Thread.sleep(3000)
        }
    }
}
