package pl.smq.example

import pl.smq.lib.SMQ

class Subscriber()  : Subcommand {
    override fun execute() {
        val smq = SMQ("localhost", 3000)
        val queue = smq.messageQueue("test")
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
