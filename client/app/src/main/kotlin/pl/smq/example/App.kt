package pl.smq.example

import pl.smq.lib.SMQ

fun main() {
    val smq = SMQ("localhost", 3000)
    val messageQueue = smq.messageQueue("topic")
//    messageQueue.registerAsPublisher()
    messageQueue.addMessageListener { println(it) }
    messageQueue.registerAsSubscriber()
//    messageQueue.sendMessage("Hello World!")
    while (true) {
    }
}
