package pl.smq.example

import pl.smq.example.subscribers.*

object SubscriberFactory {
    fun get(testCase: Int): Action {
        return when(testCase){
            1 -> Subscriber()
            2 -> SubscriberOnce()
            else -> Subscriber()
        }
    }
}
