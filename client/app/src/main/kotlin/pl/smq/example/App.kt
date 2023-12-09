package pl.smq.example

import pl.smq.lib.SMQ

fun main() {
    val smq = SMQ("localhost", 3000)
    val messageQueue = smq.messageQueue("topic")
    messageQueue.registerAsSubscriber()
    messageQueue.addMessageListener { println(it) }
    messageQueue.registerAsPublisher()
    messageQueue.sendMessage("Hello World!")
    messageQueue.unregisterAsPublisher()
    messageQueue.unregisterAsSubscriber()
}
