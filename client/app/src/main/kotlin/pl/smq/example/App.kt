package pl.smq.example

import pl.smq.lib.SMQ

fun main() {
    val smq = SMQ("localhost", 3000)
    smq.subscribe("test")
    smq.publisher("test")
    smq.unpublisher("test")
    smq.unsubscribe("test")
}
