package pl.smq.example

import pl.smq.lib.SMQ

fun main() {
    val smq = SMQ("localhost", 3000)
    val messageQueue = smq.messageQueue("topic")
    messageQueue.registerAsPublisher()
    messageQueue.registerAsSubscriber()
    messageQueue.addMessageListener { println(it) }
    messageQueue.sendMessage("Hello world")
    while (true) {
    }
}
